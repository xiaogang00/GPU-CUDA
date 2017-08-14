#### 多CPU系统

在这里我们遇到的最多的多CPU系统是数据中心的服务器。任何多处理器系统的主要问题之一是内存的一致性。对于GPU，也就是每个GPU卡上的全局内存。

而且还存在缓存一致性的问题。用的卡槽越多，保持同步性就越难。

#### 多GPU系统

一般来说允许PC至少有两个GPU的显卡，CUDA不使用也不需要用连接器。

**多GPU算法**

CUDA本身是不支持多GPU模式的合作。我们所要做的就是要尽可能消除系统中的数据移动。可以使用SDK4.0版本中的GPU对等通信模型。或者在CPU层使用CPU级别的语言进行合作。

**按需选择GPU**

我们通过调用以下的代码来设置一个设备：

```c
cudaError_t cudaSetDevice(int device_num);
```

device_num是一个从零开始到系统设备数量的一个数字。如果要查询设备数量，我们可以调用下面的语句：

```c
cudeError_t cudaGetDeviceCount(int * device_count);
CUDA_CALL(cudaGetDeviceCount(&num_devices));
```

单节点系统是唯一支持多GPU模型的系统，一个基于单CPU的任务将与单GPU上下文相关联。

