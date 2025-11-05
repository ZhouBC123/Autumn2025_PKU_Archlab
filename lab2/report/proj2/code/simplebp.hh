#ifndef __CPU_PRED_SIMPLE_BP_HH__
#define __CPU_PRED_SIMPLE_BP_HH__

#include "base/types.hh"
#include "cpu/pred/bpred_unit.hh"
#include "params/SimpleBP.hh"

namespace gem5 {
namespace branch_prediction {
class SimpleBP : public BPredUnit {
public:

  SimpleBP(const SimpleBPParams &params);

  bool lookup(ThreadID tid, Addr pc, void *&bp_history) override;

  void updateHistories(ThreadID tid, Addr pc, bool uncond, bool taken,
                       Addr target, void *&bp_history) override;

  void update(ThreadID tid, Addr pc, bool taken, void *&bp_history,
              bool squashed, const StaticInstPtr &inst, Addr target) override;
              
  void squash(ThreadID tid, void *&bp_history) override;

  bool pred_result;
};
} // namespace branch_prediction
} // namespace gem5
#endif // __CPU_PRED_SIMPLE_BP_HH__