#ifndef ONEFLOW_CORE_KERNEL_PRELU_GRAD_KERNEL_H_
#define ONEFLOW_CORE_KERNEL_PRELU_GRAD_KERNEL_H_

#include "oneflow/core/kernel/kernel.h"

namespace oneflow {

template<DeviceType device_type, typename T>
class PReluGradKernel final : public KernelIfWithModel<device_type, T> {
 public:
  OF_DISALLOW_COPY_AND_MOVE(PReluGradKernel);
  PReluGradKernel() = default;
  ~PReluGradKernel() = default;

 private:
  void ForwardDataContent(const KernelCtx&,
                          std::function<Blob*(const std::string&)>) const override;
};

}  // namespace oneflow

#endif  // ONEFLOW_CORE_KERNEL_PRELU_GRAD_KERNEL_H_
