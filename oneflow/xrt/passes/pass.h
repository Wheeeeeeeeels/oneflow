#ifndef ONEFLOW_XRT_PASSES_PASS_H_
#define ONEFLOW_XRT_PASSES_PASS_H_

#include "oneflow/xrt/any.h"
#include "oneflow/xrt/graph/graph.h"
#include "oneflow/xrt/utility/registry.h"

namespace oneflow {
namespace xrt {

struct ClusteringOptions {
  // Minimum node number in each cluster after clustering. If the number of
  // nodes contained by a cluster is less than `minimum_nodes` or grater than
  // `maximum_nodes`, then this cluster will be given up and not compiled.
  int32_t minimum_nodes = 0x1;
  int32_t maximum_nodes = 0x7fffffff;

  // Clustering nodes if it's compiled by the engine.
  XrtEngine engine = XrtEngine::XLA;

  // Option to clustering with strict dependencies analysis.
  bool strict_clustering = true;

  // Maximum iteration count for iteratively clustering. You can set it -1 means
  // that it will always iteratively merge nodes as much as possible until no
  // nodes can be merged.
  int32_t max_iteration = 20;

  // Clustering subgraph for train phase.
  bool train_phase = true;

  bool ignore_sbp_policy = false;
  bool ignore_time_shape = false;
};

struct XrtPassOptions {
  ClusteringOptions clustering_options;
};

class XrtPass {
 public:
  XrtPass() = default;
  virtual ~XrtPass() = default;

  virtual void Run(XrtGraph *graph, const XrtPassOptions &options) {
    LOG(FATAL) << "Should not call this function.";
  }

  virtual void Run(XrtGraph *graph, const XrtPassOptions &options,
                   const std::vector<Any> &params) {
    LOG(FATAL) << "Should not call this function.";
  }

  static auto Registry()
      -> util::Registry<std::string, std::function<XrtPass *()>> * {
    return util::Registry<std::string, std::function<XrtPass *()>>::Global();
  }
};

// typedef XrtPass *(*XrtPassCreator)();

#define REGISTER_XRT_PASS(PassName, PassType)                     \
  namespace PassName {                                            \
  struct _XrtPassRegistrar {                                      \
    _XrtPassRegistrar() {                                         \
      XrtPass::Registry()->Register(                              \
          #PassName, []() -> XrtPass * { return new PassType; }); \
    }                                                             \
  };                                                              \
  _XrtPassRegistrar _xrt_pass_registrar_ __attribute__((unused)); \
  }  // namespace // PassName

inline void RunPassImpl(const std::string &pass, XrtGraph *graph,
                        const XrtPassOptions &options) {
  auto optimize_pass =
      std::shared_ptr<XrtPass>(XrtPass::Registry()->Lookup(pass)());
  optimize_pass->Run(graph, options);
}

template <typename... Args>
inline void RunPassImpl(const std::string &pass, XrtGraph *graph,
                        const XrtPassOptions &options, Args &&... args) {
  std::vector<Any> params{std::forward<Args>(args)...};
  auto optimize_pass =
      std::shared_ptr<XrtPass>(XrtPass::Registry()->Lookup(pass)());
  optimize_pass->Run(graph, options, params);
}

}  // namespace xrt
}  // namespace oneflow

#endif  // ONEFLOW_XRT_PASSES_PASS_H_