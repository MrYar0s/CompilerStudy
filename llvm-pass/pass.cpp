#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"

using namespace llvm;

namespace {

struct LoggerPass : public FunctionPass {
    static char ID;
    LoggerPass() : FunctionPass(ID) {}

    bool isFuncLogger(StringRef name)
    {
        return name == "instrLog";
    }

    virtual bool runOnFunction(Function &F)
    {
        outs() << F.getName() << "\n";
        if (isFuncLogger(F.getName())) {
            return false;
        }

        LLVMContext &Ctx = F.getContext();
        IRBuilder<> builder(Ctx);
        Type *retType = Type::getVoidTy(Ctx);

        std::vector<Type *> instrLogParamTypes = {builder.getInt8Ty()->getPointerTo()};
        FunctionType *instrLogFuncType = FunctionType::get(retType, instrLogParamTypes, false);
        FunctionCallee instrLogFunc = F.getParent()->getOrInsertFunction("instrLog", instrLogFuncType);

        for (auto &B : F) {
            for (auto &I : B) {
                if (auto *phi = dyn_cast<PHINode>(&I)) {
                    continue;
                }
                if (auto *call = dyn_cast<CallInst>(&I)) {
                    Function *callee = call->getCalledFunction();
                    if (callee && isFuncLogger(callee->getName())) {
                        continue;
                    }
                }
                builder.SetInsertPoint(&I);
                Value *instrName = builder.CreateGlobalStringPtr(I.getOpcodeName());
                if (instrName) {
                    Value *args[] = {instrName};
                    builder.CreateCall(instrLogFunc, args);
                }
            }
        }

        return false;
    }
};

char LoggerPass::ID = 0;

static void registerLoggerPass(const PassManagerBuilder &, legacy::PassManagerBase &PM)
{
    PM.add(new LoggerPass());
}

static RegisterStandardPasses RegisterMyPass(PassManagerBuilder::EP_OptimizerLast, registerLoggerPass);

}  // namespace
