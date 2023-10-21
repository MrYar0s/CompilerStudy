; ModuleID = 'main.c'
source_filename = "main.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @draw(i32* noundef %0) #0 {
  %2 = alloca i32*, align 8
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store i32* %0, i32** %2, align 8
  store i32 0, i32* %3, align 4
  br label %5

5:                                                ; preds = %27, %1
  %6 = load i32, i32* %3, align 4
  %7 = icmp ult i32 %6, 400
  br i1 %7, label %8, label %30

8:                                                ; preds = %5
  store i32 0, i32* %4, align 4
  br label %9

9:                                                ; preds = %23, %8
  %10 = load i32, i32* %4, align 4
  %11 = icmp ult i32 %10, 500
  br i1 %11, label %12, label %26

12:                                               ; preds = %9
  %13 = load i32, i32* %4, align 4
  %14 = load i32, i32* %3, align 4
  %15 = load i32*, i32** %2, align 8
  %16 = load i32, i32* %3, align 4
  %17 = mul i32 %16, 500
  %18 = load i32, i32* %4, align 4
  %19 = add i32 %17, %18
  %20 = zext i32 %19 to i64
  %21 = getelementptr inbounds i32, i32* %15, i64 %20
  %22 = load i32, i32* %21, align 4
  call void @setPixel(i32 noundef %13, i32 noundef %14, i32 noundef %22)
  br label %23

23:                                               ; preds = %12
  %24 = load i32, i32* %4, align 4
  %25 = add i32 %24, 1
  store i32 %25, i32* %4, align 4
  br label %9, !llvm.loop !6

26:                                               ; preds = %9
  br label %27

27:                                               ; preds = %26
  %28 = load i32, i32* %3, align 4
  %29 = add i32 %28, 1
  store i32 %29, i32* %3, align 4
  br label %5, !llvm.loop !8

30:                                               ; preds = %5
  ret void
}

declare void @setPixel(i32 noundef, i32 noundef, i32 noundef) #1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @calc(i32* noundef %0, i32* noundef %1) #0 {
  %3 = alloca i32*, align 8
  %4 = alloca i32*, align 8
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  %8 = alloca i32, align 4
  %9 = alloca i32, align 4
  %10 = alloca i32, align 4
  %11 = alloca i32, align 4
  store i32* %0, i32** %3, align 8
  store i32* %1, i32** %4, align 8
  store i32 0, i32* %5, align 4
  br label %12

12:                                               ; preds = %89, %2
  %13 = load i32, i32* %5, align 4
  %14 = icmp slt i32 %13, 400
  br i1 %14, label %15, label %92

15:                                               ; preds = %12
  store i32 0, i32* %6, align 4
  br label %16

16:                                               ; preds = %85, %15
  %17 = load i32, i32* %6, align 4
  %18 = icmp slt i32 %17, 500
  br i1 %18, label %19, label %88

19:                                               ; preds = %16
  %20 = load i32*, i32** %3, align 8
  %21 = load i32, i32* %5, align 4
  %22 = mul nsw i32 %21, 500
  %23 = load i32, i32* %6, align 4
  %24 = add nsw i32 %22, %23
  %25 = sext i32 %24 to i64
  %26 = getelementptr inbounds i32, i32* %20, i64 %25
  %27 = load i32, i32* %26, align 4
  store i32 %27, i32* %7, align 4
  %28 = load i32, i32* %7, align 4
  %29 = icmp eq i32 %28, 1
  %30 = zext i1 %29 to i64
  %31 = select i1 %29, i32 -1, i32 0
  store i32 %31, i32* %8, align 4
  store i32 -1, i32* %9, align 4
  br label %32

32:                                               ; preds = %62, %19
  %33 = load i32, i32* %9, align 4
  %34 = icmp sle i32 %33, 1
  br i1 %34, label %35, label %65

35:                                               ; preds = %32
  store i32 -1, i32* %10, align 4
  br label %36

36:                                               ; preds = %58, %35
  %37 = load i32, i32* %10, align 4
  %38 = icmp sle i32 %37, 1
  br i1 %38, label %39, label %61

39:                                               ; preds = %36
  %40 = load i32*, i32** %3, align 8
  %41 = load i32, i32* %5, align 4
  %42 = load i32, i32* %9, align 4
  %43 = add nsw i32 %41, %42
  %44 = add nsw i32 %43, 400
  %45 = srem i32 %44, 400
  %46 = mul nsw i32 %45, 500
  %47 = load i32, i32* %6, align 4
  %48 = load i32, i32* %10, align 4
  %49 = add nsw i32 %47, %48
  %50 = add nsw i32 %49, 500
  %51 = srem i32 %50, 500
  %52 = add nsw i32 %46, %51
  %53 = sext i32 %52 to i64
  %54 = getelementptr inbounds i32, i32* %40, i64 %53
  %55 = load i32, i32* %54, align 4
  %56 = load i32, i32* %8, align 4
  %57 = add nsw i32 %56, %55
  store i32 %57, i32* %8, align 4
  br label %58

58:                                               ; preds = %39
  %59 = load i32, i32* %10, align 4
  %60 = add nsw i32 %59, 1
  store i32 %60, i32* %10, align 4
  br label %36, !llvm.loop !9

61:                                               ; preds = %36
  br label %62

62:                                               ; preds = %61
  %63 = load i32, i32* %9, align 4
  %64 = add nsw i32 %63, 1
  store i32 %64, i32* %9, align 4
  br label %32, !llvm.loop !10

65:                                               ; preds = %32
  store i32 0, i32* %11, align 4
  %66 = load i32, i32* %8, align 4
  %67 = icmp eq i32 %66, 3
  br i1 %67, label %68, label %69

68:                                               ; preds = %65
  store i32 1, i32* %11, align 4
  br label %69

69:                                               ; preds = %68, %65
  %70 = load i32, i32* %8, align 4
  %71 = icmp eq i32 %70, 2
  br i1 %71, label %72, label %76

72:                                               ; preds = %69
  %73 = load i32, i32* %7, align 4
  %74 = icmp eq i32 %73, 1
  br i1 %74, label %75, label %76

75:                                               ; preds = %72
  store i32 1, i32* %11, align 4
  br label %76

76:                                               ; preds = %75, %72, %69
  %77 = load i32, i32* %11, align 4
  %78 = load i32*, i32** %4, align 8
  %79 = load i32, i32* %5, align 4
  %80 = mul nsw i32 %79, 500
  %81 = load i32, i32* %6, align 4
  %82 = add nsw i32 %80, %81
  %83 = sext i32 %82 to i64
  %84 = getelementptr inbounds i32, i32* %78, i64 %83
  store i32 %77, i32* %84, align 4
  br label %85

85:                                               ; preds = %76
  %86 = load i32, i32* %6, align 4
  %87 = add nsw i32 %86, 1
  store i32 %87, i32* %6, align 4
  br label %16, !llvm.loop !11

88:                                               ; preds = %16
  br label %89

89:                                               ; preds = %88
  %90 = load i32, i32* %5, align 4
  %91 = add nsw i32 %90, 1
  store i32 %91, i32* %5, align 4
  br label %12, !llvm.loop !12

92:                                               ; preds = %12
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main(i32 noundef %0, i8** noundef %1) #0 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i8**, align 8
  %6 = alloca [200000 x i32], align 16
  %7 = alloca [200000 x i32], align 16
  %8 = alloca i32*, align 8
  %9 = alloca i32*, align 8
  %10 = alloca i32, align 4
  %11 = alloca i32, align 4
  %12 = alloca i32*, align 8
  store i32 0, i32* %3, align 4
  store i32 %0, i32* %4, align 4
  store i8** %1, i8*** %5, align 8
  %13 = getelementptr inbounds [200000 x i32], [200000 x i32]* %6, i64 0, i64 0
  store i32* %13, i32** %8, align 8
  %14 = getelementptr inbounds [200000 x i32], [200000 x i32]* %7, i64 0, i64 0
  store i32* %14, i32** %9, align 8
  store i32 0, i32* %10, align 4
  br label %15

15:                                               ; preds = %36, %2
  %16 = load i32, i32* %10, align 4
  %17 = icmp ult i32 %16, 400
  br i1 %17, label %18, label %39

18:                                               ; preds = %15
  store i32 0, i32* %11, align 4
  br label %19

19:                                               ; preds = %32, %18
  %20 = load i32, i32* %11, align 4
  %21 = icmp ult i32 %20, 500
  br i1 %21, label %22, label %35

22:                                               ; preds = %19
  %23 = call i32 (...) @generate()
  %24 = srem i32 %23, 2
  %25 = load i32*, i32** %8, align 8
  %26 = load i32, i32* %10, align 4
  %27 = mul i32 %26, 500
  %28 = load i32, i32* %11, align 4
  %29 = add i32 %27, %28
  %30 = zext i32 %29 to i64
  %31 = getelementptr inbounds i32, i32* %25, i64 %30
  store i32 %24, i32* %31, align 4
  br label %32

32:                                               ; preds = %22
  %33 = load i32, i32* %11, align 4
  %34 = add i32 %33, 1
  store i32 %34, i32* %11, align 4
  br label %19, !llvm.loop !13

35:                                               ; preds = %19
  br label %36

36:                                               ; preds = %35
  %37 = load i32, i32* %10, align 4
  %38 = add i32 %37, 1
  store i32 %38, i32* %10, align 4
  br label %15, !llvm.loop !14

39:                                               ; preds = %15
  call void @init(i32 noundef 500, i32 noundef 400)
  %40 = load i32*, i32** %8, align 8
  call void @draw(i32* noundef %40)
  call void (...) @display()
  br label %41

41:                                               ; preds = %39, %41
  call void (...) @flush()
  %42 = load i32*, i32** %8, align 8
  %43 = load i32*, i32** %9, align 8
  call void @calc(i32* noundef %42, i32* noundef %43)
  %44 = load i32*, i32** %9, align 8
  call void @draw(i32* noundef %44)
  call void (...) @display()
  %45 = load i32*, i32** %8, align 8
  store i32* %45, i32** %12, align 8
  %46 = load i32*, i32** %9, align 8
  store i32* %46, i32** %8, align 8
  %47 = load i32*, i32** %12, align 8
  store i32* %47, i32** %9, align 8
  br label %41
}

declare i32 @generate(...) #1

declare void @init(i32 noundef, i32 noundef) #1

declare void @display(...) #1

declare void @flush(...) #1

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 1}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"Ubuntu clang version 14.0.0-1ubuntu1.1"}
!6 = distinct !{!6, !7}
!7 = !{!"llvm.loop.mustprogress"}
!8 = distinct !{!8, !7}
!9 = distinct !{!9, !7}
!10 = distinct !{!10, !7}
!11 = distinct !{!11, !7}
!12 = distinct !{!12, !7}
!13 = distinct !{!13, !7}
!14 = distinct !{!14, !7}
