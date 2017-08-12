# include <stdio.h>
# include <stdlib.h>
# include <conio.h>
__global__ void myhistogram256Kernel(const unsigned int const * d_hist_data,
                                     unsigned int * const d_bin_data,
                                     unsigned int N)
{
    const unsigned int idx = (blockIdx.x * (blockDim.x * N)) + threadIdx.x;
    const unsigned int idy = (blockIdx.y * blockDim.y) + threadIdx.y;
    const unsigned int tid = idx + idy * (blockDim.x * N) * (gridDim.x);
    d_bin_data_shared[threadIdx.x] = 0;
    __syncthreads();
    for (unsigned int i = 0, tid_offset = 0; i < N ; i++, tid_offset += 256)
    {
        const unsigned int value_u32 = d_hist_data[tid + tid_offset];
        atomicAdd(&(d_bin_data_shared[((value_u32 & 0x000000FF))]), 1);
        atomicAdd(&(d_bin_data_shared[((value_u32 & 0x0000FF00)>>8)]), 1);
        atomicAdd(&(d_bin_data_shared[((value_u32 & 0x00FF0000)>>16)]), 1);
        atomicAdd(&(d_bin_data_shared[((value_u32 & 0xFF000000)>>24)]), 1);
    }
    __syncthreads();
    atomicAdd(&(d_bin_data[threadIdx.x]), d_bin_data_shared[threadIdx.x]);
}