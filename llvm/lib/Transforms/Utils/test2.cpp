#include "llvm/Transforms/Utils/test2.h"

#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace llvm::test2 {

PreservedAnalyses InstrumentFunctionsPass::run(Module &M,
                                               ModuleAnalysisManager &) {

  bool Changed = false;
  LLVMContext &Ctx = M.getContext();

  // create instrument_start if missing
  Function *StartFn = M.getFunction("instrument_start");
  if (!StartFn) {
    FunctionType *FT = FunctionType::get(Type::getVoidTy(Ctx), false);
    StartFn =
        Function::Create(FT, Function::ExternalLinkage, "instrument_start", M);
  }

  // create instrument_end if missing
  Function *EndFn = M.getFunction("instrument_end");
  if (!EndFn) {
    FunctionType *FT = FunctionType::get(Type::getVoidTy(Ctx), false);
    EndFn =
        Function::Create(FT, Function::ExternalLinkage, "instrument_end", M);
  }

  for (Function &F : M) {

    if (F.isDeclaration())
      continue;

    outs() << "Instrumenting: " << F.getName() << "\n";

    // insert instrument_start
    BasicBlock &Entry = F.getEntryBlock();
    Instruction *InsertPt = &*Entry.getFirstInsertionPt();

    IRBuilder<> Builder(InsertPt);
    Builder.CreateCall(StartFn);

    // insert instrument_end before returns
    for (BasicBlock &BB : F) {

      Instruction *Term = BB.getTerminator();

      if (isa<ReturnInst>(Term)) {
        IRBuilder<> EndBuilder(Term);
        EndBuilder.CreateCall(EndFn);
      }
    }

    Changed = true;
  }

  return Changed ? PreservedAnalyses::none()
                 : PreservedAnalyses::all();
}

} // namespace llvm::test2