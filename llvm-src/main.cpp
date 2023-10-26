#include <cstdlib>
#include <memory>

#include "llvm/IR/Value.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/GenericValue.h"

#include "api.h"

using namespace llvm;

int main(int argc, char *argv[]) {
	bool isNeedToRun = true;
	{
		// To disable interpreter pass 0 value as input parameter in program
		if (argc > 1) {
			isNeedToRun = static_cast<bool>(atoi(argv[1]));
		}
	}

	LLVMContext ctx;
	// ; ModuleID = 'main.c'
	// source_filename = "main.c"
	Module *module = new Module("main.c", ctx);
	IRBuilder<> builder(ctx);

	// definition of external functions

	// declare void @setPixel(i32, i32, i32)
	Type *voidType = Type::getVoidTy(ctx);
	std::vector<Type *> setPixelParamTypes = {Type::getInt32Ty(ctx), Type::getInt32Ty(ctx), Type::getInt32Ty(ctx)};
	FunctionType *setPixelType = FunctionType::get(voidType, setPixelParamTypes, false);
	FunctionCallee setPixelFunc = module->getOrInsertFunction("setPixel", setPixelType);

	// declare void @init(i32, i32)
	std::vector<Type *> initParamTypes = {Type::getInt32Ty(ctx), Type::getInt32Ty(ctx)};
	FunctionType *initType = FunctionType::get(voidType, initParamTypes, false);
	FunctionCallee initFunc = module->getOrInsertFunction("init", initType);

	// declare void @generate()
	FunctionType *generateType = FunctionType::get(Type::getInt32Ty(ctx), voidType, false);
	FunctionCallee generateFunc = module->getOrInsertFunction("generate", generateType);

	// declare void @display()
	FunctionType *displayType = FunctionType::get(voidType, voidType, false);
	FunctionCallee displayFunc = module->getOrInsertFunction("display", displayType);

	// define void @draw(i32* noundef %0)
	FunctionType *drawFuncType = FunctionType::get(builder.getVoidTy(), Type::getInt32PtrTy(ctx), false);
	Function *drawFunc = Function::Create(drawFuncType, Function::ExternalLinkage, "draw", module);

	{
		// Preset of function argument %0
		Value *val0 = drawFunc->getArg(0);

		// Basic block definitions
		BasicBlock *BB1 = BasicBlock::Create(ctx, "", drawFunc);
		BasicBlock *BB2 = BasicBlock::Create(ctx, "", drawFunc);
		BasicBlock *BB6 = BasicBlock::Create(ctx, "", drawFunc);
		BasicBlock *BB7 = BasicBlock::Create(ctx, "", drawFunc);
		BasicBlock *BB10 = BasicBlock::Create(ctx, "", drawFunc);

		// 1:
		builder.SetInsertPoint(BB1);

		// br label %2
		builder.CreateBr(BB2);

		// 2:
		builder.SetInsertPoint(BB2);

		// %3 = phi i64 [ 0, %1 ], [ %8, %7 ]
		PHINode *val3 = builder.CreatePHI(Type::getInt64Ty(ctx), 2);
		// %4 = mul nuw nsw i64 %3, 400
		Value *val4 = builder.CreateMul(val3, builder.getInt64(400), "", true, true);
		// %5 = trunc i64 %3 to i32
		Value *val5 = builder.CreateTrunc(val3, Type::getInt32Ty(ctx));
		// br label %10
		builder.CreateBr(BB10);

		// 6:
		builder.SetInsertPoint(BB6);

		// ret void
		builder.CreateRetVoid();

		// 7:
		builder.SetInsertPoint(BB7);

		// %8 = add nuw nsw i64 %3, 1
		Value *val8 = builder.CreateAdd(val3, builder.getInt32(1), "", true, true);
		// %9 = icmp eq i64 %8, 400
		Value *val9 = builder.CreateICmpEQ(val8, builder.getInt64(400));
		// br i1 %9, label %6, label %2
		builder.CreateCondBr(val9, BB6, BB2);

		// 10:
		builder.SetInsertPoint(BB10);

		// %11 = phi i64 [ 0, %2 ], [ %16, %10 ]
		PHINode *val11 = builder.CreatePHI(Type::getInt64Ty(ctx), 2);
		// %12 = add nuw nsw i64 %11, %4
		Value *val12 = builder.CreateAdd(val11, val4, "", true, true);
		// %13 = getelementptr inbounds i32, i32* %0, i64 %12
		Value *val13 = builder.CreateInBoundsGEP(Type::getInt32Ty(ctx), val0, val12);
		// %14 = load i32, i32* %13, align 4, !tbaa !7
		Value *val14 = builder.CreateLoad(Type::getInt32Ty(ctx), val13);
		// %15 = trunc i64 %11 to i32
		Value *val15 = builder.CreateTrunc(val11, Type::getInt32Ty(ctx));
		// tail call void @setPixel(i32 noundef %15, i32 noundef %5, i32 noundef %14)
		Value *Args0[] = {val15, val5, val14};
		builder.CreateCall(setPixelFunc, Args0);
		// %16 = add nuw nsw i64 %11, 1
		Value *val16 = builder.CreateAdd(val11, builder.getInt32(1), "", true, true);
		// %17 = icmp eq i64 %16, 400
		Value *val17 = builder.CreateICmpEQ(val16, builder.getInt64(400));
		// br i1 %17, label %7, label %10
		builder.CreateCondBr(val17, BB7, BB10);

		// Link PHI nodes
		val3->addIncoming(builder.getInt64(0), BB1);
		val3->addIncoming(val8, BB7);

		val11->addIncoming(builder.getInt64(0), BB2);
		val11->addIncoming(val16, BB10);

	}
	// declare void @calc(i32* %0, i32* %1)
	std::vector<Type *> calcParamTypes = {Type::getInt32PtrTy(ctx), Type::getInt32PtrTy(ctx)};
	FunctionType *calcFuncType = FunctionType::get(voidType, calcParamTypes, false);
	Function *calcFunc = Function::Create(calcFuncType, Function::ExternalLinkage, "calc", module);
	{
		// Preset of function arguments %0, %1
		Value *val0 = calcFunc->getArg(0);
		Value *val1 = calcFunc->getArg(1);

		// Basic block definitions
		BasicBlock *BB2 = BasicBlock::Create(ctx, "", calcFunc);
		BasicBlock *BB3 = BasicBlock::Create(ctx, "", calcFunc);
		BasicBlock *BB18 = BasicBlock::Create(ctx, "", calcFunc);
		BasicBlock *BB19 = BasicBlock::Create(ctx, "", calcFunc);
		BasicBlock *BB22 = BasicBlock::Create(ctx, "", calcFunc);

		// 2:
		builder.SetInsertPoint(BB2);
		
		// br label %3
		builder.CreateBr(BB3);

		// 3:
		builder.SetInsertPoint(BB3);

		// %4 = phi i64 [ 0, %2 ], [ %20, %19 ]
		PHINode* val4 = builder.CreatePHI(Type::getInt64Ty(ctx), 2);
		// %5 = mul nuw nsw i64 %4, 400
		Value *val5 = builder.CreateMul(val4, builder.getInt64(400), "", true, true);
		// %6 = trunc i64 %4 to i32
		Value *val6 = builder.CreateTrunc(val4, Type::getInt32Ty(ctx));
		// %7 = add i32 %6, 399
		Value *val7 = builder.CreateAdd(val6, builder.getInt32(399));
		// %8 = urem i32 %7, 400
		Value *val8 = builder.CreateURem(val7, builder.getInt32(400));
		// %9 = mul nuw nsw i32 %8, 400
		Value *val9 = builder.CreateMul(val8, builder.getInt32(400), "", true, true);
		// %10 = trunc i64 %4 to i32
		Value *val10 = builder.CreateTrunc(val4, Type::getInt32Ty(ctx));
		// %11 = add i32 %10, 400
		Value *val11 = builder.CreateAdd(val10, builder.getInt32(400));
		// %12 = urem i32 %11, 400
		Value *val12 = builder.CreateURem(val11, builder.getInt32(400));
		// %13 = mul nuw nsw i32 %12, 400
		Value *val13 = builder.CreateMul(val12, builder.getInt32(400), "", true, true);
		// %14 = trunc i64 %4 to i32
		Value *val14 = builder.CreateTrunc(val4, Type::getInt32Ty(ctx));
		// %15 = add i32 %14, 401
		Value *val15 = builder.CreateAdd(val14, builder.getInt32(401));
		// %16 = urem i32 %15, 400
		Value *val16 = builder.CreateURem(val15, builder.getInt32(400));
		// %17 = mul nuw nsw i32 %16, 400
		Value *val17 = builder.CreateMul(val16, builder.getInt32(400), "", true, true);
		// br label %22
		builder.CreateBr(BB22);

		// 12:
		builder.SetInsertPoint(BB18);
		
		// ret void
		builder.CreateRetVoid();

		// 19:
		builder.SetInsertPoint(BB19);

		// %20 = add nuw nsw i64 %4, 1
		Value *val20 = builder.CreateAdd(val4, builder.getInt32(1), "", true, true);
		// %21 = icmp eq i64 %20, 400
		Value *val21 = builder.CreateICmpEQ(val20, builder.getInt64(400));
		// br i1 %21, label %18, label %3
		builder.CreateCondBr(val21, BB18, BB3);

		// 22:
		builder.SetInsertPoint(BB22);

		// %23 = phi i64 [ 0, %3 ], [ %89, %22 ]
		PHINode* val23 = builder.CreatePHI(Type::getInt64Ty(ctx), 2);
		// %24 = add nuw nsw i64 %23, %5
		Value *val24 = builder.CreateAdd(val23, val5, "", true, true);
		// %25 = getelementptr inbounds i32, i32* %0, i64 %24
		Value *val25 = builder.CreateInBoundsGEP(Type::getInt32Ty(ctx), val0, val24);
		// %26 = load i32, i32* %25, align 4
		Value *val26 = builder.CreateLoad(Type::getInt32Ty(ctx), val25);
		// %27 = icmp eq i32 %26, 1
		Value *val27 = builder.CreateICmpEQ(val26, builder.getInt32(1));
		// %28 = sext i1 %27 to i32
		Value *val28 = builder.CreateSExt(val27, Type::getInt32Ty(ctx));
		// %29 = trunc i64 %23 to i32
		Value *val29 = builder.CreateTrunc(val23, Type::getInt32Ty(ctx));
		// %30 = add i32 %29, 399
		Value *val30 = builder.CreateAdd(val29, builder.getInt32(399));
		// %31 = urem i32 %30, 400
		Value *val31 = builder.CreateURem(val30, builder.getInt32(400));
		// %32 = add nuw nsw i32 %31, %9
		Value *val32 = builder.CreateAdd(val31, val9, "", true, true);
		// %33 = zext i32 %32 to i64
		Value *val33 = builder.CreateZExt(val32, Type::getInt64Ty(ctx));
		// %34 = getelementptr inbounds i32, i32* %0, i64 %33
		Value *val34 = builder.CreateInBoundsGEP(Type::getInt32Ty(ctx), val0, val33);
		// %35 = load i32, i32* %34, align 4
		Value *val35 = builder.CreateLoad(Type::getInt32Ty(ctx), val34);
		// %36 = add nsw i32 %35, %28
		Value *val36 = builder.CreateNSWAdd(val35, val28);
		// %37 = trunc i64 %23 to i32
		Value *val37 = builder.CreateTrunc(val23, Type::getInt32Ty(ctx));
		// %38 = add i32 %37, 400
		Value *val38 = builder.CreateAdd(val37, builder.getInt32(400));
		// %39 = urem i32 %38, 400
		Value *val39 = builder.CreateURem(val38, builder.getInt32(400));
		// %40 = add nuw nsw i32 %39, %9
		Value *val40 = builder.CreateAdd(val39, val9, "", true, true);
		// %41 = zext i32 %40 to i64
		Value *val41 = builder.CreateZExt(val40, Type::getInt64Ty(ctx));
		// %42 = getelementptr inbounds i32, i32* %0, i64 %41
		Value *val42 = builder.CreateInBoundsGEP(Type::getInt32Ty(ctx), val0, val41);
		// %43 = load i32, i32* %42, align 4
		Value *val43 = builder.CreateLoad(Type::getInt32Ty(ctx), val42);
		// %44 = add nsw i32 %43, %36
		Value *val44 = builder.CreateNSWAdd(val43, val36);
		// %45 = trunc i64 %23 to i32
		Value *val45 = builder.CreateTrunc(val23, Type::getInt32Ty(ctx));
		// %46 = add i32 %45, 401
		Value *val46 = builder.CreateAdd(val45, builder.getInt32(401));
		// %47 = urem i32 %46, 400
		Value *val47 = builder.CreateURem(val46, builder.getInt32(400));
		// %48 = add nuw nsw i32 %47, %9
		Value *val48 = builder.CreateAdd(val47, val9, "", true, true);
		// %49 = zext i32 %48 to i64
		Value *val49 = builder.CreateZExt(val48, Type::getInt64Ty(ctx));
		// %50 = getelementptr inbounds i32, i32* %0, i64 %49
		Value *val50 = builder.CreateInBoundsGEP(Type::getInt32Ty(ctx), val0, val49);
		// %51 = load i32, i32* %50, align 4
		Value *val51 = builder.CreateLoad(Type::getInt32Ty(ctx), val50);
		// %52 = add nsw i32 %51, %44
		Value *val52 = builder.CreateNSWAdd(val51, val44);
		// %53 = add nuw nsw i32 %31, %13
		Value *val53 = builder.CreateAdd(val31, val13, "", true, true);
		// %54 = zext i32 %53 to i64
		Value *val54 = builder.CreateZExt(val53, Type::getInt64Ty(ctx));
		// %55 = getelementptr inbounds i32, i32* %0, i64 %54
		Value *val55 = builder.CreateInBoundsGEP(Type::getInt32Ty(ctx), val0, val54);
		// %56 = load i32, i32* %55, align 4
		Value *val56 = builder.CreateLoad(Type::getInt32Ty(ctx), val55);
		// %57 = add nsw i32 %56, %52
		Value *val57 = builder.CreateNSWAdd(val56, val52);
		// %58 = add nuw nsw i32 %39, %13
		Value *val58 = builder.CreateAdd(val39, val13, "", true, true);
		// %59 = zext i32 %58 to i64
		Value *val59 = builder.CreateZExt(val58, Type::getInt64Ty(ctx));
		// %60 = getelementptr inbounds i32, i32* %0, i64 %59
		Value *val60 = builder.CreateInBoundsGEP(Type::getInt32Ty(ctx), val0, val59);
		// %61 = load i32, i32* %60, align 4
		Value *val61 = builder.CreateLoad(Type::getInt32Ty(ctx), val60);
		// %62 = add nsw i32 %61, %57
		Value *val62 = builder.CreateNSWAdd(val61, val57);
		// %63 = add nuw nsw i32 %47, %13
		Value *val63 = builder.CreateAdd(val47, val13, "", true, true);
		// %64 = zext i32 %63 to i64
		Value *val64 = builder.CreateZExt(val63, Type::getInt64Ty(ctx));
		// %65 = getelementptr inbounds i32, i32* %0, i64 %64
		Value *val65 = builder.CreateInBoundsGEP(Type::getInt32Ty(ctx), val0, val64);
		// %66 = load i32, i32* %65, align 4
		Value *val66 = builder.CreateLoad(Type::getInt32Ty(ctx), val65);
		// %67 = add nsw i32 %66, %62
		Value *val67 = builder.CreateNSWAdd(val66, val62);
		// %68 = add nuw nsw i32 %31, %17
		Value *val68 = builder.CreateAdd(val31, val17, "", true, true);
		// %69 = zext i32 %68 to i64
		Value *val69 = builder.CreateZExt(val68, Type::getInt64Ty(ctx));
		// %70 = getelementptr inbounds i32, i32* %0, i64 %69
		Value *val70 = builder.CreateInBoundsGEP(Type::getInt32Ty(ctx), val0, val69);
		// %71 = load i32, i32* %70, align 4
		Value *val71 = builder.CreateLoad(Type::getInt32Ty(ctx), val70);
		// %72 = add nsw i32 %71, %67
		Value *val72 = builder.CreateNSWAdd(val71, val67);
		// %73 = add nuw nsw i32 %39, %17
		Value *val73 = builder.CreateAdd(val39, val17, "", true, true);
		// %74 = zext i32 %73 to i64
		Value *val74 = builder.CreateZExt(val73, Type::getInt64Ty(ctx));
		// %75 = getelementptr inbounds i32, i32* %0, i64 %74
		Value *val75 = builder.CreateInBoundsGEP(Type::getInt32Ty(ctx), val0, val74);
		// %76 = load i32, i32* %75, align 4
		Value *val76 = builder.CreateLoad(Type::getInt32Ty(ctx), val75);
		// %77 = add nsw i32 %76, %72
		Value *val77 = builder.CreateNSWAdd(val76, val72);
		// %78 = add nuw nsw i32 %47, %17
		Value *val78 = builder.CreateAdd(val47, val17, "", true, true);
		// %79 = zext i32 %78 to i64
		Value *val79 = builder.CreateZExt(val78, Type::getInt64Ty(ctx));
		// %80 = getelementptr inbounds i32, i32* %0, i64 %79
		Value *val80 = builder.CreateInBoundsGEP(Type::getInt32Ty(ctx), val0, val79);
		// %81 = load i32, i32* %80, align 4
		Value *val81 = builder.CreateLoad(Type::getInt32Ty(ctx), val80);
		// %82 = add nsw i32 %81, %77
		Value *val82 = builder.CreateNSWAdd(val81, val77);
		// %83 = icmp eq i32 %82, 3
		Value *val83 = builder.CreateICmpEQ(val82, builder.getInt32(3));
		// %84 = icmp eq i32 %82, 2
		Value *val84 = builder.CreateICmpEQ(val82, builder.getInt32(2));
		// %85 = select i1 %84, i1 %27, i1 false
		Value *val85 = builder.CreateSelect(val84, val27, builder.getFalse());
		// %86 = or i1 %83, %85
		Value *val86 = builder.CreateOr(val83, val85);
		// %87 = zext i1 %86 to i32
		Value *val87 = builder.CreateZExt(val86, Type::getInt32Ty(ctx));
		// %88 = getelementptr inbounds i32, i32* %1, i64 %24
		Value *val88 = builder.CreateInBoundsGEP(Type::getInt32Ty(ctx), val1, val24);
		// store i32 %87, i32* %88, align 4
		builder.CreateStore(val87, val88);
		// %89 = add nuw nsw i64 %23, 1
		Value *val89 = builder.CreateAdd(val23, builder.getInt32(1), "", true, true);
		// %90 = icmp eq i64 %89, 400
		Value *val90 = builder.CreateICmpEQ(val89, builder.getInt64(400));
		// br i1 %90, label %19, label %22
		builder.CreateCondBr(val90, BB19, BB22);

		// Link PHI nodes
		val4->addIncoming(builder.getInt64(0), BB2);
		val4->addIncoming(val20, BB19);

		val23->addIncoming(builder.getInt64(0), BB3);
		val23->addIncoming(val89, BB22);

	}
	// define i32 @main()
	std::vector<Type *> mainParamTypes = {};
	FunctionType *mainFuncType = FunctionType::get(voidType, mainParamTypes, false);
	Function *mainFunc = Function::Create(mainFuncType, Function::ExternalLinkage, "main", module);

	{

		Type *ArrayTy = ArrayType::get(Type::getInt32Ty(ctx), 160000);

		// Basic block definitions
		BasicBlock *BB0 = BasicBlock::Create(ctx, "", mainFunc);
		BasicBlock *BB6 = BasicBlock::Create(ctx, "", mainFunc);
		BasicBlock *BB9 = BasicBlock::Create(ctx, "", mainFunc);
		BasicBlock *BB10 = BasicBlock::Create(ctx, "", mainFunc);
		BasicBlock *BB14 = BasicBlock::Create(ctx, "", mainFunc);
		BasicBlock *BB17 = BasicBlock::Create(ctx, "", mainFunc);
		BasicBlock *BB25 = BasicBlock::Create(ctx, "", mainFunc);
		BasicBlock *BB27 = BasicBlock::Create(ctx, "", mainFunc);
		BasicBlock *BB30 = BasicBlock::Create(ctx, "", mainFunc);
		BasicBlock *BB38 = BasicBlock::Create(ctx, "", mainFunc);
		BasicBlock *BB41 = BasicBlock::Create(ctx, "", mainFunc);
		BasicBlock *BB45 = BasicBlock::Create(ctx, "", mainFunc);
		BasicBlock *BB48 = BasicBlock::Create(ctx, "", mainFunc);

		// 0:
		builder.SetInsertPoint(BB0);

		// %1 = alloca [160000 x i32], align 16
		Value *val1 = builder.CreateAlloca(ArrayTy);
		// %2 = alloca [160000 x i32], align 16
		Value *val2 = builder.CreateAlloca(ArrayTy);
		// %3 = bitcast [160000 x i32]* %1 to i8*
		Value *val3 = builder.CreateBitCast(val1, Type::getInt8PtrTy(ctx));
		// %4 = bitcast [160000 x i32]* %2 to i8*
		Value *val4 = builder.CreateBitCast(val2, Type::getInt8PtrTy(ctx));
		// %5 = getelementptr inbounds [160000 x i32], [160000 x i32]* %1, i64 0, i64 0
		std::vector<Value *> Idxs0 = {builder.getInt64(0), builder.getInt64(0)};
		Value *val5 = builder.CreateInBoundsGEP(ArrayTy, val1, Idxs0);
		// br label %6
		builder.CreateBr(BB6);

		// 6:
		builder.SetInsertPoint(BB6);

		// %7 = phi i64 [ 0, %0 ], [ %28, %27 ]
		PHINode *val7 = builder.CreatePHI(Type::getInt64Ty(ctx), 2);
		// %8 = mul nuw nsw i64 %7, 400
		Value *val8 = builder.CreateMul(val7, builder.getInt64(400), "", true, true);
		// br label %30
		builder.CreateBr(BB30);

		// 9:
		builder.SetInsertPoint(BB9);

		// tail call void @init(i32 noundef 400, i32 noundef 400) #5
		Value *Args0[] = {builder.getInt32(400), builder.getInt32(400)};
		builder.CreateCall(initFunc, Args0);
		// br label %10
		builder.CreateBr(BB10);

		// 10:
		builder.SetInsertPoint(BB10);

		// %11 = phi i64 [ 0, %9 ], [ %15, %14 ]
		PHINode *val11 = builder.CreatePHI(Type::getInt64Ty(ctx), 2);
		// %12 = mul nuw nsw i64 %11, 400
		Value *val12 = builder.CreateMul(val11, builder.getInt64(400), "", true, true);
		// %13 = trunc i64 %11 to i32
		Value *val13 = builder.CreateTrunc(val11, Type::getInt32Ty(ctx));
		// br label %17
		builder.CreateBr(BB17);

		// 14:
		builder.SetInsertPoint(BB14);

		// %15 = add nuw nsw i64 %11, 1
		Value *val15 = builder.CreateAdd(val11, builder.getInt64(1), "", true, true);
		// %16 = icmp eq i64 %15, 400
		Value *val16 = builder.CreateICmpEQ(val15, builder.getInt64(400));
		// br i1 %16, label %25, label %10
		builder.CreateCondBr(val16, BB25, BB10);

		// 17:
		builder.SetInsertPoint(BB17);

		// %18 = phi i64 [ 0, %10 ], [ %23, %17 ]
		PHINode *val18 = builder.CreatePHI(Type::getInt64Ty(ctx), 2);
		// %19 = add nuw nsw i64 %18, %12
		Value *val19 = builder.CreateAdd(val18, val12, "", true, true);
		// %20 = getelementptr inbounds [160000 x i32], [160000 x i32]* %1, i64 0, i64 %19
		std::vector<Value *> Idxs1 = {builder.getInt64(0), val19};
		Value *val20 = builder.CreateInBoundsGEP(ArrayTy, val1, Idxs1);
		// %21 = load i32, i32* %20, align 4
		Value *val21 = builder.CreateLoad(Type::getInt32Ty(ctx), val20);
		// %22 = trunc i64 %18 to i32
		Value *val22 = builder.CreateTrunc(val18, Type::getInt32Ty(ctx));
		// tail call void @setPixel(i32 noundef %22, i32 noundef %13, i32 noundef %21)
		Value *Args1[] = {val22, val13, val21};
		builder.CreateCall(setPixelFunc, Args1);
		// %23 = add nuw nsw i64 %18, 1
		Value *val23 = builder.CreateAdd(val18, builder.getInt64(1), "", true, true);
		// %24 = icmp eq i64 %23, 400
		Value *val24 = builder.CreateICmpEQ(val23, builder.getInt64(400));
		// br i1 %24, label %14, label %17
		builder.CreateCondBr(val24, BB14, BB17);

		// 25:
		builder.SetInsertPoint(BB25);

		// %26 = getelementptr inbounds [160000 x i32], [160000 x i32]* %2, i64 0, i64 0
		std::vector<Value *> Idxs2 = {builder.getInt64(0), builder.getInt64(0)};
		Value *val26 = builder.CreateInBoundsGEP(ArrayTy, val2, Idxs2);
		// br label %38
		builder.CreateBr(BB38);

		// 27:
		builder.SetInsertPoint(BB27);

		// %28 = add nuw nsw i64 %7, 1
		Value *val28 = builder.CreateAdd(val7, builder.getInt64(1), "", true, true);
		// %29 = icmp eq i64 %28, 400
		Value *val29 = builder.CreateICmpEQ(val28, builder.getInt64(400));
		// br i1 %29, label %9, label %6, !llvm.loop !14
		builder.CreateCondBr(val29, BB9, BB6);

		// 30:
		builder.SetInsertPoint(BB30);

		// %31 = phi i64 [ 0, %6 ], [ %36, %30 ]
		PHINode *val31 = builder.CreatePHI(Type::getInt64Ty(ctx), 2);
		// %32 = tail call i32 (...) @generate()
		Value *val32 = builder.CreateCall(generateFunc);
		// %33 = srem i32 %32, 2
		Value *val33 = builder.CreateSRem(val32, builder.getInt32(2));
		// %34 = add nuw nsw i64 %31, %8
		Value *val34 = builder.CreateAdd(val31, val8, "", true, true);
		// %35 = getelementptr inbounds [160000 x i32], [160000 x i32]* %1, i64 0, i64 %34
		std::vector<Value *> Idxs3 = {builder.getInt64(0), val34};
		Value *val35 = builder.CreateInBoundsGEP(ArrayTy, val1, Idxs3);
		// store i32 %33, i32* %35, align 4
		builder.CreateStore(val33, val35);
		// %36 = add nuw nsw i64 %31, 1
		Value *val36 = builder.CreateAdd(val31, builder.getInt64(1), "", true, true);
		// %37 = icmp eq i64 %36, 400
		Value *val37 = builder.CreateICmpEQ(val36, builder.getInt64(400));
		// br i1 %37, label %27, label %30
		builder.CreateCondBr(val37, BB27, BB30);

		// 38:
		builder.SetInsertPoint(BB38);

		// %39 = phi i32* [ %26, %25 ], [ %40, %45 ]
		PHINode *val39 = builder.CreatePHI(Type::getInt32PtrTy(ctx), 2);
		// %40 = phi i32* [ %5, %25 ], [ %39, %45 ]
		PHINode *val40 = builder.CreatePHI(Type::getInt32PtrTy(ctx), 2);
		// tail call void (...) @display()
		builder.CreateCall(displayFunc);
		// call void @calc(i32* noundef nonnull %40, i32* noundef %39)
		Value *Args2[] = {val40, val39};
		builder.CreateCall(calcFunc, Args2);
		// br label %41
		builder.CreateBr(BB41);

		// 41:
		builder.SetInsertPoint(BB41);

		// %42 = phi i64 [ 0, %38 ], [ %46, %45 ]
		PHINode *val42 = builder.CreatePHI(Type::getInt64Ty(ctx), 2);
		// %43 = mul nuw nsw i64 %42, 400
		Value *val43 = builder.CreateMul(val42, builder.getInt64(400), "", true, true);
		// %44 = trunc i64 %42 to i32
		Value *val44 = builder.CreateTrunc(val42, Type::getInt32Ty(ctx));
		// br label %48
		builder.CreateBr(BB48);

		// 45:
		builder.SetInsertPoint(BB45);

		// %46 = add nuw nsw i64 %42, 1
		Value *val46 = builder.CreateAdd(val42, builder.getInt64(1), "", true, true);
		// %47 = icmp eq i64 %46, 400
		Value *val47 = builder.CreateICmpEQ(val46, builder.getInt64(400));
		// br i1 %47, label %38, label %41
		builder.CreateCondBr(val47, BB38, BB41);

		// 48:
		builder.SetInsertPoint(BB48);

		// %49 = phi i64 [ 0, %41 ], [ %54, %48 ]
		PHINode *val49 = builder.CreatePHI(Type::getInt64Ty(ctx), 2);
		// %50 = add nuw nsw i64 %49, %43
		Value *val50 = builder.CreateAdd(val49, val43, "", true, true);
		// %51 = getelementptr inbounds i32, i32* %39, i64 %50
		Value *val51 = builder.CreateInBoundsGEP(Type::getInt32Ty(ctx), val39, val50);
		// %52 = load i32, i32* %51, align 4
		Value *val52 = builder.CreateLoad(Type::getInt32Ty(ctx), val51);
		// %53 = trunc i64 %49 to i32
		Value *val53 = builder.CreateTrunc(val49, Type::getInt32Ty(ctx));
		// tail call void @setPixel(i32 noundef %53, i32 noundef %44, i32 noundef %52) #5
		Value *Args3[] = {val53, val44, val52};
		builder.CreateCall(setPixelFunc, Args3);
		// %54 = add nuw nsw i64 %49, 1
		Value *val54 = builder.CreateAdd(val49, builder.getInt64(1), "", true, true);
		// %55 = icmp eq i64 %54, 400
		Value *val55 = builder.CreateICmpEQ(val54, builder.getInt64(400));
		// br i1 %55, label %45, label %48, !llvm.loop !11
		builder.CreateCondBr(val55, BB45, BB48);

		// Link PHI nodes
		val7->addIncoming(builder.getInt64(0), BB0);
		val7->addIncoming(val28, BB27);

		val11->addIncoming(builder.getInt64(0), BB9);
		val11->addIncoming(val15, BB14);

		val18->addIncoming(builder.getInt64(0), BB10);
		val18->addIncoming(val23, BB17);

		val31->addIncoming(builder.getInt64(0), BB6);
		val31->addIncoming(val36, BB30);

		val39->addIncoming(val26, BB25);
		val39->addIncoming(val40, BB45);

		val40->addIncoming(val5, BB25);
		val40->addIncoming(val39, BB45);

		val42->addIncoming(builder.getInt64(0), BB38);
		val42->addIncoming(val46, BB45);

		val49->addIncoming(builder.getInt64(0), BB41);
		val49->addIncoming(val54, BB48);
	}

	// Dump LLVM IR
	module->print(outs(), nullptr);

	// Interpreter of LLVM IR
	if (isNeedToRun) {
		outs() << "Running code...\n";
		InitializeNativeTarget();
		InitializeNativeTargetAsmPrinter();

		ExecutionEngine *ee = EngineBuilder(std::unique_ptr<Module>(module)).create();
		ee->InstallLazyFunctionCreator([&](const std::string &fnName) -> void * {
			if (fnName == "setPixel") {
				return reinterpret_cast<void *>(setPixel);
			}
			if (fnName == "generate") {
				return reinterpret_cast<void *>(generate);
			}
			if (fnName == "init") {
				return reinterpret_cast<void *>(init);
			}
			if (fnName == "display") {
				return reinterpret_cast<void *>(display);
			}
			return nullptr;
		});
		ee->finalizeObject();

		ArrayRef<GenericValue> noargs;
		GenericValue v = ee->runFunction(mainFunc, noargs);

		outs() << "Code was run\n";
	}
	return 0;
}