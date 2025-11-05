#include "cpu/pred/simplebp.hh"
namespace gem5 {
namespace branch_prediction {
SimpleBP::SimpleBP(const SimpleBPParams &params) : BPredUnit(params), pred_result(params.pred_result) {}
bool SimpleBP::lookup(ThreadID tid, Addr pc, void *&bp_history) { return true; 
}
void SimpleBP::updateHistories(ThreadID tid, Addr pc, bool uncond, bool taken,
                               Addr target, void *&bp_history) {}
void SimpleBP::update(ThreadID tid, Addr pc, bool taken, void *&bp_history,
                      bool squashed, const StaticInstPtr &inst, Addr target) {}
void SimpleBP::squash(ThreadID tid, void *&bp_history) {}
} // namespace branch_prediction
} // namespace gem5
