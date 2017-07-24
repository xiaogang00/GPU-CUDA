# include <stdio.h>
# include <stdlib.h>
# include <conio.h>
__global__ void what_is_my_id_2d_A(
    unsigned int * const block_x,
    unsigned int * const block_y,
    unsigned int * const thread,
    unsigned int * const calc_thread,
    unsigned int * const x_thread,
    unsigned int * const y_thread,
    unsigned int * const grid_dimx,
    unsigned int * const block_dimx,
    unsigned int * const grid_dimy,
    unsigned int * const block_dimy)
{
    const unsigned int idx = (blockIdx.x * blockDim.x) + threadIdx.x;
    const unsigned int idy = (blockIdx.y * blockDim.y) + threadIdx.y;
    const unsigned int thread_idx = ((gridDim.x * blockDim.x) * idy) + idx;
    block_x[thread_idx] = blockIdx.x;
    block_y[thread_idx] = blockIdx.y;
    thread[thread_idx] = threadIdx.x;
    calc_thread[thread_idx] = thread_idx;
    x_thread[thread_idx] = idx;
    y_thread[thread_idx] = idy;
    grid_dimx[thread_idx] = gridDim.x;
    grid_dimy[thread_idx] = gridDim.y;
    block_dimx[thread_idx] = blockDim.x;
    block_dimy[thread_idx] = blockDim.y;
}