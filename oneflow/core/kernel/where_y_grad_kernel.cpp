#include "oneflow/core/kernel/where_kernel.h"
#include "oneflow/core/kernel/where_y_grad_kernel.h"

namespace oneflow {

template<DeviceType device_type, typename T>
void WhereYGradKernel<device_type, T>::ForwardDataContent(
    const KernelCtx& ctx, std::function<Blob*(const std::string&)> BnInOp2Blob) const {
  const Blob* condition_blob = BnInOp2Blob("condition");
  int64_t elem_cnt = condition_blob->shape().elem_cnt();

  if (BnInOp2Blob("y_diff") != nullptr) {
    WhereKernelUtil<device_type, T>::CmptYDiff(ctx.device_ctx, elem_cnt, condition_blob->dptr<T>(),
                                               BnInOp2Blob("out_diff")->dptr<T>(),
                                               BnInOp2Blob("y_diff")->mut_dptr<T>());
  }
}

ADD_DEFAULT_KERNEL_CREATOR(OperatorConf::kWhereYGradConf, WhereYGradKernel, ARITHMETIC_DATA_TYPE_SEQ);

}  // namespace oneflow
