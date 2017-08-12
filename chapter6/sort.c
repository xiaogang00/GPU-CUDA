//基数排序的串行代码
__host__ void cpu_sort(u32 * const data,
                       const u32 num_elements)
{
    static u32 cpu_tmp_0[NUM_ELEM];
    static u32 cou_tmp_1[NUM_ELEM];
    for(u32 bit = 0 ; bit < 32 ; bit++)
    {
        u32 base_cnt_0 = 0;
        u32 base_cnt_1 = 0;
        for(u32 i = 0; i < num_elements; i++)
        {
            const u32 d = data[i];
            const u32 bit_mask = (1<<bit);
            if( (d & bit__mask) > 0 )
            {
                cpu_tmp_1[base_cnt_1] = d;
                base_cnt_1++;
            }
            else
            {
                cpu_tmp_0[base_cnt_0] = d;
                base_cnt_0++;
            }
        }
        for(u32 i = 0; i < base_cnt_0; i++)
        {
            data[i] = cpu_tmp_0[i];
        }
        for(u32 i = 0; i < base_cnt_1; i++)
        {
            data[base_cnt_0 + i] = cpu_tmp_1[i];
        }
    }
}


//GPU版本代码
//num_lists是每个线程块起动的线程数目
__device__ void radix_sort(u32 * const sort_tmp, const u32 num_lists,
                           const u32 num_elements, const u32 tid,
                           u32 * const sort_tmp_0, u32 * const sort_tmp_1)
{
    for(u32 bit = 0 ; bit < 32 ; bit++)
    {
        u32 base_cnt_0 = 0;
        u32 base_cnt_1 = 0;
        for(u32 i = 0 ; i<num_elements; i+=num_lists)
        {
            const u32 elem = sort_tmp[i+tid];
            const u32 bit_mask = (1 << bit);
            if((elem & bit_mask) > 0)
            {
                sort_tmp_1[base_cnt_1+tid] = elem;
                base_cnt_1 += num_lists;
            }
            else
            {
                sort_tmp_0[base_cnt_0+tid] = elem;
                base_cnt_0 += num_lists;
            }
        }
        for(u32 i=0 ; i < base_cnt_0; i+=num_lists)
        {
            sort_tmp[i+tid] = sort_tmp_0[i+tid];
        }
        for(u32 i=0 ; i < base_cnt_1; i+=num_lists)
        {
            sort_tmp[base_cnt_0 + i + tid] = sort_tmp_1[i+tid];
        }
    }
    __syncthreads();
}