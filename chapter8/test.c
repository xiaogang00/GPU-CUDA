void full_array(u32 * data, const u32 num_elements)
{
    for(u32 i=0; i < num_elements; i++)
    {
        data[i] = i;
    }
}

void check_array(char * device_prefix, u32 * data, const u32 num_elements)
{
    bool error_found = false;
    for(u32 i = 0; i < num_elements; i++)
    {
        if(data[i] != (i*2))
        {
            printf("%s Error: %u %u", device_prefix, i, data[i]);
            error_found = true;
        }
    }
    if(error_found == false)
       printf("%s array check passed", device_prefix);
}

__global__ void gpu_test_kernel(u32 * data)
{
    const int tid = (blockIdx.x * blockDim.x) + threadIdx.x;
    data[tid] *= 2;
}

#define MAX_NUM_DEVICES (4)
#define NUM_ELEM(1024 * 1024 * 8)

cudaStream stream[MAX_NUM_DEVICES];
char device_prefix[MAX_NUM_DEVICES][300];
u32 * gpu_data[MAX_NUM_DEVICES];
u32 * cpu_src_data[MAX_NUM_DEVICES];
u32 * cpu_dest_data[MAX_NUM_DEVICES];

__host__ void gpu_kernel(void)
{
    const int shared_memory_usage = 0;
    const size_t single_gpu_chunk_size = (sizeof(u32) * NUM_ELEM);
    const int num_threads = 256;
    const int num_blocks = ((NUM_ELEM + (num_threads - 1))/num_threads);
    int num_devices;
    CUDA_CALL(cudaGetDeviceCount(&num_devices));
    if(num_devices > MAX_NUM_DEVICES)
      num_devices = MAX_NUM_DEVICES;
    for(int device_num = 0 ; device_num < num_devices; device_num++)
    {
        CUDA_CALL(cudeSetDevice(device_num));
        struct cudaDeviceProp device_prop;
        CUDA_CALL(cudeGetDeviceProperties(&device_prop, device_num));
        sprintf(&device_prefix[device_num][0], "\nID: %d %s", device_num, device_prop.name);
        //新建一个流在这个device上
        CUDA_CALL(cudaStreamCreate(&stream[device_num]));

        //在GPU上设置空间，用流可以copy过去
        CUDA_CALL(cudaMalloc((void **)&gpu_data[device_num], single_gpu_chunk_size));
        CUDA_CALL(cudaMallocHost((void **)& cpu_src_data[device_num], single_gpu_chunk_size));
        CUDA_CALL(cudaMallocHost((void **)& cpu_dest_data[device_num], single_gpu_chunk_size));

        fill_array(cpu_src_data[device_num], NUM_ELEM);
        //copy from CPU to GPU
        CUDA_CALL(cudaMemcpyAsync(gpu_data[device_num], cpu_src_data[device_num],
                                  single_gpu_chunk_size, cudeMemcpyHostToDevice,
                                  stream[device_num]));
        gpu_test_kernel<<<num_blocks, num_threads, shared_memory_usage, stream[device_num]>>>(gpu_data[device_num]);
        //这些blocks, num_threads都是必须的，表示在调用GPU时候的设置
        cuda_error_check(device_prefix[device_num], "Failed to invoke gpu_test_kernel");
        CUDA_CALL(cudaMemcpyAsync(cpu_dest_data[device_num], gpu_data[device_num],
                                  single_gpu_chunk_size, cudeMemcpyHostToDevice,
                                  stream[device_num]));
    }
    for(int device_num = 0; device_num < num_devices; device_num++)
    {
        CUDA_CALL(cudaSetDevice(device_num));
        CUDA_CALL(cudaStreamSynchronize(stream[device_num]));
        CUDA_CALL(cudaStreamDestroy(stream[device_num]));
        CUDA_CALL(cudaFree(gpu_data[device_num]));

        check_array(device_prefix[device_num], cpu_dest_data[device_num], NUM_ELEM);
        CUDA_CALL(cudaFreeHost(cpu_src_data[device_num]));
        CUDA_CALL(cudaFreeHost(cpu_dest_data[device_num]));
        CUDA_CALL(cudaDeviceReset());
    }
}