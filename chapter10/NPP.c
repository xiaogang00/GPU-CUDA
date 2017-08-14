#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include "cuda.h"
#include "cuda_helper.h"
#include "common_types.h"
#include "timer.h"

#include "npp.h"
#include "nppcore.h"
#include "nppdefs.h"
#include "nppi.h"
#include "npps.h"
#include "nppversion.h"

#define NPP_CALL(x) {const NPPStatus a = (x); if (a != NPP_SUCCESS){printf("\nPPError: (err_num=%d)\n", a); cudaDeviceReset(); ASSERT(0);}}

int main(int argc, char * argv[])
{
    const int num_bytes = (1024u * 255u) * sizeof(Npp8u);
    Npp8u * host_src_ptr1 = (u8 * malloc(num_bytes));
    Npp8u * host_src_ptr2 = (u8 * malloc(num_bytes));
    Npp8u * host_src_ptr3 = (u8 * malloc(num_bytes));
    Npp8u * host_src_ptr4 = (u8 * malloc(num_bytes));

    Npp8u * device_src_ptr1;
    Npp8u * device_src_ptr2;
    Npp8u * device_src_ptr3;
    Npp8u * device_src_ptr4;
    CUDA_CALL(cudaMalloc((void **)&device_src_ptr1, num_bytes));
    CUDA_CALL(cudaMalloc((void **)&device_src_ptr2, num_bytes));
    CUDA_CALL(cudaMalloc((void **)&device_src_ptr3, num_bytes));
    CUDA_CALL(cudaMalloc((void **)&device_src_ptr4, num_bytes));

    for(u32 i = 0; i < num_bytes; i++)
    {
        host_src_ptr1[i] = (rand() % 255);
        host_src_ptr2[i] = (rand() % 255);
    }
    CUDA_CALL(cudaMemcpy(device_src_ptr1, host_src_ptr1, num_bytes, cudaMemcpyHostToDevice));
    CUDA_CALL(cudaMemcpy(device_src_ptr2, host_src_ptr2, num_bytes, cudaMemcpyHostToDevice));
    TIMER_T start_time_device = get_time();
    NPP_CALL(nppsXor_8u(device_src_ptr1, device_src_ptr2, device_dst_ptr1, num_bytes));
    NPP_CALL(nppsXor_8u(device_src_ptr1, device_src_ptr1, device_dst_ptr2, num_bytes));
    TIMER_T delta_time_device = get_time() - start_time_device;
}
