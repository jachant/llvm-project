#ifndef LLVM_TRANSFORMS_UTILS_TEST2_H
#define LLVM_TRANSFORMS_UTILS_TEST2_H

#include "llvm/IR/PassManager.h"

namespace llvm::test2 {

class InstrumentFunctionsPass
    : public llvm::PassInfoMixin<InstrumentFunctionsPass> {
public:
  llvm::PreservedAnalyses run(llvm::Module &M,
                              llvm::ModuleAnalysisManager &AM);
};

}

#endif