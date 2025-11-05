#include "cpu/pred/lab2_bp.hh"

namespace gem5 {
namespace branch_prediction {
Lab2PerceptronBP::Lab2PerceptronBP(const Lab2PerceptronBPParams &params)
    : BPredUnit(params), 
    numPerceptrons(params.numPerceptrons),
    historyLength(params.historyLength),
    weightBits(params.weightBits),
    threshold(params.threshold),
    initWeight(params.initWeight) {
      weightMax = (1 << (weightBits - 1)) - 1;
      weightMin = -(1 << (weightBits - 1));

      globalHistory.assign(historyLength, false);

      perceptronTable.resize(numPerceptrons);
      for (unsigned i = 0; i < numPerceptrons; ++i) {
          perceptronTable[i].weights.resize(historyLength + 1, initWeight);
      }
    }

inline
int
Lab2PerceptronBP::saturatingAdd(int weight, int value) {
    int result = weight + value;
    if (result > weightMax) {
        return weightMax;
    } else if (result < weightMin) {
        return weightMin;
    } else {
        return result;
    }
}

inline
int
Lab2PerceptronBP::predictPerceptron(const Perceptron &p) {
    int y = p.weights[0]; // bias weight
    for (unsigned i = 0; i < historyLength; ++i) {
        if (globalHistory[i]) {
            y += p.weights[i + 1];
        } else {
            y -= p.weights[i + 1];
        }
    }
    return y;
}

inline
unsigned
Lab2PerceptronBP::getPerceptronIndex(Addr branch_addr) {
    return (branch_addr >> 2) % numPerceptrons;
}

bool Lab2PerceptronBP::lookup(ThreadID tid, Addr branch_addr,
                              void *&bp_history) {
  unsigned p_index = getPerceptronIndex(branch_addr);
  Perceptron &p = perceptronTable[p_index];
  int y = predictPerceptron(p);
  bool prediction = (y >= 0);

  BPhistory *history = new BPhistory;
  history->perceptronIndex = p_index;
  history->globalHistory = globalHistory;
  history->outputY = y;
  history->prediction = prediction;
  bp_history = (void *)history;

  return prediction;

}
void Lab2PerceptronBP::updateHistories(ThreadID tid, Addr pc, bool uncond,
                                       bool taken, Addr target,
                                       void *&bp_history) {
  assert(uncond || bp_history);

  if(uncond) {
      BPhistory *history = new BPhistory;
      history->perceptronIndex = getPerceptronIndex(pc);
      history->globalHistory = globalHistory;
      history->outputY = threshold + 1; // unused
      history->prediction = true; // unused
      bp_history = (void *)history;
  }

  // Update global history
  globalHistory.insert(globalHistory.begin(), taken);
  if (globalHistory.size() > historyLength) {
      globalHistory.pop_back();
  }

}
void Lab2PerceptronBP::update(ThreadID tid, Addr pc, bool taken,
                              void *&bp_history, bool squashed,
                              const StaticInstPtr &inst, Addr target) {
  assert(bp_history);
  BPhistory *history = static_cast<BPhistory *>(bp_history);
  Perceptron &p = perceptronTable[history->perceptronIndex];

  // Train the perceptron if the prediction was incorrect or
  // the output y is within the threshold
  int t = taken ? 1 : -1;

  if (squashed) {
    globalHistory = history->globalHistory;
    // delete history;
    // bp_history = nullptr;
    return;
  }

  if (history->prediction != taken || abs(history->outputY) <= threshold) {
    // Update bias weight
    p.weights[0] = saturatingAdd(p.weights[0], t);
    
    // Update other weights
    for (unsigned i = 0; i < historyLength; ++i) {
      int h = history->globalHistory[i] ? 1 : -1;
      p.weights[i + 1] = saturatingAdd(p.weights[i + 1], t * h);
    }
  }

  delete history;
  bp_history = nullptr;
}
void Lab2PerceptronBP::squash(ThreadID tid, void *&bp_history) {

  assert(bp_history);
  BPhistory *history = static_cast<BPhistory *>(bp_history);
  globalHistory = history->globalHistory;
  delete history;
  bp_history = nullptr;

}
} // namespace branch_prediction
} // namespace gem5
