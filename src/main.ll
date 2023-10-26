; ModuleID = 'main.c'
source_filename = "main.c"

define dso_local void @draw(i32* nocapture noundef readonly %0) {
  br label %2

2:                                                ; preds = %1, %7
  %3 = phi i64 [ 0, %1 ], [ %8, %7 ]
  %4 = mul nuw nsw i64 %3, 400
  %5 = trunc i64 %3 to i32
  br label %10

6:                                                ; preds = %7
  ret void

7:                                                ; preds = %10
  %8 = add nuw nsw i64 %3, 1
  %9 = icmp eq i64 %8, 400
  br i1 %9, label %6, label %2

10:                                               ; preds = %2, %10
  %11 = phi i64 [ 0, %2 ], [ %16, %10 ]
  %12 = add nuw nsw i64 %11, %4
  %13 = getelementptr inbounds i32, i32* %0, i64 %12
  %14 = load i32, i32* %13, align 4
  %15 = trunc i64 %11 to i32
  tail call void @setPixel(i32 noundef %15, i32 noundef %5, i32 noundef %14)
  %16 = add nuw nsw i64 %11, 1
  %17 = icmp eq i64 %16, 400
  br i1 %17, label %7, label %10
}

declare void @llvm.lifetime.start.p0i8(i64 immarg, i8* nocapture)

declare void @setPixel(i32 noundef, i32 noundef, i32 noundef) local_unnamed_addr

define dso_local void @calc(i32* nocapture noundef readonly %0, i32* nocapture noundef writeonly %1) {
  br label %3

3:                                                ; preds = %2, %19
  %4 = phi i64 [ 0, %2 ], [ %20, %19 ]
  %5 = mul nuw nsw i64 %4, 400
  %6 = trunc i64 %4 to i32
  %7 = add i32 %6, 399
  %8 = urem i32 %7, 400
  %9 = mul nuw nsw i32 %8, 400
  %10 = trunc i64 %4 to i32
  %11 = add i32 %10, 400
  %12 = urem i32 %11, 400
  %13 = mul nuw nsw i32 %12, 400
  %14 = trunc i64 %4 to i32
  %15 = add i32 %14, 401
  %16 = urem i32 %15, 400
  %17 = mul nuw nsw i32 %16, 400
  br label %22

18:                                               ; preds = %19
  ret void

19:                                               ; preds = %22
  %20 = add nuw nsw i64 %4, 1
  %21 = icmp eq i64 %20, 400
  br i1 %21, label %18, label %3

22:                                               ; preds = %3, %22
  %23 = phi i64 [ 0, %3 ], [ %89, %22 ]
  %24 = add nuw nsw i64 %23, %5
  %25 = getelementptr inbounds i32, i32* %0, i64 %24
  %26 = load i32, i32* %25, align 4
  %27 = icmp eq i32 %26, 1
  %28 = sext i1 %27 to i32
  %29 = trunc i64 %23 to i32
  %30 = add i32 %29, 399
  %31 = urem i32 %30, 400
  %32 = add nuw nsw i32 %31, %9
  %33 = zext i32 %32 to i64
  %34 = getelementptr inbounds i32, i32* %0, i64 %33
  %35 = load i32, i32* %34, align 4
  %36 = add nsw i32 %35, %28
  %37 = trunc i64 %23 to i32
  %38 = add i32 %37, 400
  %39 = urem i32 %38, 400
  %40 = add nuw nsw i32 %39, %9
  %41 = zext i32 %40 to i64
  %42 = getelementptr inbounds i32, i32* %0, i64 %41
  %43 = load i32, i32* %42, align 4
  %44 = add nsw i32 %43, %36
  %45 = trunc i64 %23 to i32
  %46 = add i32 %45, 401
  %47 = urem i32 %46, 400
  %48 = add nuw nsw i32 %47, %9
  %49 = zext i32 %48 to i64
  %50 = getelementptr inbounds i32, i32* %0, i64 %49
  %51 = load i32, i32* %50, align 4
  %52 = add nsw i32 %51, %44
  %53 = add nuw nsw i32 %31, %13
  %54 = zext i32 %53 to i64
  %55 = getelementptr inbounds i32, i32* %0, i64 %54
  %56 = load i32, i32* %55, align 4
  %57 = add nsw i32 %56, %52
  %58 = add nuw nsw i32 %39, %13
  %59 = zext i32 %58 to i64
  %60 = getelementptr inbounds i32, i32* %0, i64 %59
  %61 = load i32, i32* %60, align 4
  %62 = add nsw i32 %61, %57
  %63 = add nuw nsw i32 %47, %13
  %64 = zext i32 %63 to i64
  %65 = getelementptr inbounds i32, i32* %0, i64 %64
  %66 = load i32, i32* %65, align 4
  %67 = add nsw i32 %66, %62
  %68 = add nuw nsw i32 %31, %17
  %69 = zext i32 %68 to i64
  %70 = getelementptr inbounds i32, i32* %0, i64 %69
  %71 = load i32, i32* %70, align 4
  %72 = add nsw i32 %71, %67
  %73 = add nuw nsw i32 %39, %17
  %74 = zext i32 %73 to i64
  %75 = getelementptr inbounds i32, i32* %0, i64 %74
  %76 = load i32, i32* %75, align 4
  %77 = add nsw i32 %76, %72
  %78 = add nuw nsw i32 %47, %17
  %79 = zext i32 %78 to i64
  %80 = getelementptr inbounds i32, i32* %0, i64 %79
  %81 = load i32, i32* %80, align 4
  %82 = add nsw i32 %81, %77
  %83 = icmp eq i32 %82, 3
  %84 = icmp eq i32 %82, 2
  %85 = select i1 %84, i1 %27, i1 false
  %86 = or i1 %83, %85
  %87 = zext i1 %86 to i32
  %88 = getelementptr inbounds i32, i32* %1, i64 %24
  store i32 %87, i32* %88, align 4
  %89 = add nuw nsw i64 %23, 1
  %90 = icmp eq i64 %89, 400
  br i1 %90, label %19, label %22
}

define dso_local i32 @main() local_unnamed_addr {
  %1 = alloca [160000 x i32], align 16
  %2 = alloca [160000 x i32], align 16
  %3 = bitcast [160000 x i32]* %1 to i8*
  %4 = bitcast [160000 x i32]* %2 to i8*
  %5 = getelementptr inbounds [160000 x i32], [160000 x i32]* %1, i64 0, i64 0
  br label %6

6:                                                ; preds = %0, %27
  %7 = phi i64 [ 0, %0 ], [ %28, %27 ]
  %8 = mul nuw nsw i64 %7, 400
  br label %30

9:                                                ; preds = %27
  tail call void @init(i32 noundef 400, i32 noundef 400)
  br label %10

10:                                               ; preds = %14, %9
  %11 = phi i64 [ 0, %9 ], [ %15, %14 ]
  %12 = mul nuw nsw i64 %11, 400
  %13 = trunc i64 %11 to i32
  br label %17

14:                                               ; preds = %17
  %15 = add nuw nsw i64 %11, 1
  %16 = icmp eq i64 %15, 400
  br i1 %16, label %25, label %10

17:                                               ; preds = %17, %10
  %18 = phi i64 [ 0, %10 ], [ %23, %17 ]
  %19 = add nuw nsw i64 %18, %12
  %20 = getelementptr inbounds [160000 x i32], [160000 x i32]* %1, i64 0, i64 %19
  %21 = load i32, i32* %20, align 4
  %22 = trunc i64 %18 to i32
  tail call void @setPixel(i32 noundef %22, i32 noundef %13, i32 noundef %21)
  %23 = add nuw nsw i64 %18, 1
  %24 = icmp eq i64 %23, 400
  br i1 %24, label %14, label %17

25:                                               ; preds = %14
  %26 = getelementptr inbounds [160000 x i32], [160000 x i32]* %2, i64 0, i64 0
  br label %38

27:                                               ; preds = %30
  %28 = add nuw nsw i64 %7, 1
  %29 = icmp eq i64 %28, 400
  br i1 %29, label %9, label %6

30:                                               ; preds = %6, %30
  %31 = phi i64 [ 0, %6 ], [ %36, %30 ]
  %32 = tail call i32 (...) @generate()
  %33 = srem i32 %32, 2
  %34 = add nuw nsw i64 %31, %8
  %35 = getelementptr inbounds [160000 x i32], [160000 x i32]* %1, i64 0, i64 %34
  store i32 %33, i32* %35, align 4
  %36 = add nuw nsw i64 %31, 1
  %37 = icmp eq i64 %36, 400
  br i1 %37, label %27, label %30

38:                                               ; preds = %45, %25
  %39 = phi i32* [ %26, %25 ], [ %40, %45 ]
  %40 = phi i32* [ %5, %25 ], [ %39, %45 ]
  tail call void (...) @display()
  call void @calc(i32* noundef nonnull %40, i32* noundef %39)
  br label %41

41:                                               ; preds = %45, %38
  %42 = phi i64 [ 0, %38 ], [ %46, %45 ]
  %43 = mul nuw nsw i64 %42, 400
  %44 = trunc i64 %42 to i32
  br label %48

45:                                               ; preds = %48
  %46 = add nuw nsw i64 %42, 1
  %47 = icmp eq i64 %46, 400
  br i1 %47, label %38, label %41

48:                                               ; preds = %48, %41
  %49 = phi i64 [ 0, %41 ], [ %54, %48 ]
  %50 = add nuw nsw i64 %49, %43
  %51 = getelementptr inbounds i32, i32* %39, i64 %50
  %52 = load i32, i32* %51, align 4
  %53 = trunc i64 %49 to i32
  tail call void @setPixel(i32 noundef %53, i32 noundef %44, i32 noundef %52)
  %54 = add nuw nsw i64 %49, 1
  %55 = icmp eq i64 %54, 400
  br i1 %55, label %45, label %48
}

declare i32 @generate(...)

declare void @init(i32 noundef, i32 noundef)

declare void @display(...)
