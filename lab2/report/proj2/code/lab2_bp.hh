#ifndef __CPU_PRED_LAB2_BP_HH__
#define __CPU_PRED_LAB2_BP_HH__

#include "base/types.hh"
#include "cpu/pred/bpred_unit.hh"
#include "params/Lab2PerceptronBP.hh"

namespace gem5 {
namespace branch_prediction {

struct Perceptron {
    std::vector<int> weights; // include w0..wh
};



class Lab2PerceptronBP : public BPredUnit {

private:
  unsigned historyLength;
  unsigned weightBits;
  unsigned numPerceptrons;
  int threshold;
  int initWeight;
  int weightMax, weightMin;

  std::vector<Perceptron> perceptronTable;
  std::vector<bool> globalHistory; // size = historyLength

  inline int saturatingAdd(int weight, int value);
  inline int predictPerceptron(const Perceptron &p);
  inline unsigned getPerceptronIndex(Addr branch_addr);

  struct BPhistory {
    unsigned perceptronIndex;
    std::vector<bool> globalHistory; // size = historyLength
    int outputY;
    bool prediction;
  };

public:
  Lab2PerceptronBP(const Lab2PerceptronBPParams &params);

  bool lookup(ThreadID tid, Addr branch_addr, void *&bp_history) override;
  void updateHistories(ThreadID tid, Addr pc, bool uncond, bool taken,
                       Addr target, void *&bp_history) override;
  void update(ThreadID tid, Addr pc, bool taken, void *&bp_history,
              bool squashed, const StaticInstPtr &inst, Addr target) override;
  void squash(ThreadID tid, void *&bp_history) override;

  

};
} // namespace branch_prediction
} // namespace gem5

#endif // __CPU_PRED_LAB2_BP_HH__
