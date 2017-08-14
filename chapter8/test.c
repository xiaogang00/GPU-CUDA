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