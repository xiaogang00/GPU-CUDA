__host__ float add_test_non_interleaved_cpu(NON_INTERLEAVED_T * const host_dest_ptr,
                                            const NON_INTERLEAVED_T * const host_src_ptr,
                                            const u32 iter, const u32 num_elements)
{
    float start_time = get_time();
    for(u32 tid = 0; tid < num_elements; tid++)
    {
        for(u32 i = 0; i < iter; i++)
        {
            host_dest_ptr->a[tid] = host_src_ptr->a[tid];
            host_dest_ptr->b[tid] = host_src_ptr->b[tid];
            host_dest_ptr->c[tid] = host_src_ptr->c[tid];
            host_dest_ptr->d[tid] = host_src_ptr->d[tid];
        }
    }
    const float delta = get_time() - start_time;
    return delta;
}
