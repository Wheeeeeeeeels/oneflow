#ifndef ONEFLOW_CORE_DEVICE_CUDA_UTIL_H_
#define ONEFLOW_CORE_DEVICE_CUDA_UTIL_H_

#include "oneflow/core/common/util.h"
#include "oneflow/core/common/preprocessor.h"

#ifdef WITH_CUDA

#include <cublas_v2.h>
#include <cuda.h>
#include <cuda_runtime.h>
#include <cudnn.h>
#include <curand.h>
#include <nccl.h>

namespace oneflow {

template<typename T>
void CudaCheck(T error);

// CUDA: grid stride looping
#define CUDA_1D_KERNEL_LOOP(i, n) \
  for (int32_t i = blockIdx.x * blockDim.x + threadIdx.x; i < (n); i += blockDim.x * gridDim.x)

const int32_t kCudaThreadsNumPerBlock = 512;
const int32_t kCudaMaxBlocksNum = 4096;

inline int32_t BlocksNum4ThreadsNum(const int32_t n) {
  return std::min((n + kCudaThreadsNumPerBlock - 1) / kCudaThreadsNumPerBlock, kCudaMaxBlocksNum);
}

size_t GetAvailableGpuMemSize(int dev_id);

#define CUDA_WORK_TYPE_SEQ           \
  OF_PP_MAKE_TUPLE_SEQ(kCompute)     \
  OF_PP_MAKE_TUPLE_SEQ(kCopyH2D)     \
  OF_PP_MAKE_TUPLE_SEQ(kCopyD2H)     \
  OF_PP_MAKE_TUPLE_SEQ(kNcclScatter) \
  OF_PP_MAKE_TUPLE_SEQ(kNcclGather)  \
  OF_PP_MAKE_TUPLE_SEQ(kMix)         \
  OF_PP_MAKE_TUPLE_SEQ(kMdUpdt)

enum class CudaWorkType {
#define DECLARE_CUDA_WORK_TYPE(type) type,
  OF_PP_FOR_EACH_TUPLE(DECLARE_CUDA_WORK_TYPE, CUDA_WORK_TYPE_SEQ)
};
inline size_t GetCudaWorkTypeSize() { return OF_PP_SEQ_SIZE(CUDA_WORK_TYPE_SEQ); }

}  // namespace oneflow

#endif  // WITH_CUDA

#endif  // ONEFLOW_CORE_DEVICE_CUDA_UTIL_H_
