#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

using namespace llvm;

namespace {

struct LoggerPass : public FunctionPass {
	static char ID;
	LoggerPass() : FunctionPass(ID) {}

	virtual bool runOnFunction(Function &F) {
		outs() << "I saw a function called" << F.getName() << "!\n";
		return false;
	}
};

char LoggerPass::ID = 0;

static void registerLoggerPass(const PassManagerBuilder&, legacy::PassManagerBase& PM) {
	PM.add(new LoggerPass());
}

static RegisterStandardPasses RegisterMyPass(PassManagerBuilder::EP_OptimizerLast, registerLoggerPass);

}
