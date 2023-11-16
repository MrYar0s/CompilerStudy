; Function Attrs: noreturn nounwind uwtable
define dso_local i32 @main() {
  %1 = alloca [160000 x i32], align 16
  %2 = alloca [160000 x i32], align 16
  %3 = bitcast [160000 x i32]* %1 to i8*
  call void @llvm.lifetime.start.p0i8(i64 640000, i8* nonnull %3)
  %4 = bitcast [160000 x i32]* %2 to i8*
  call void @llvm.lifetime.start.p0i8(i64 640000, i8* nonnull %4)
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
  %32 = tail call i32 (...) @generate() #3
  %33 = srem i32 %32, 2
  %34 = add nuw nsw i64 %31, %8
  %35 = getelementptr inbounds [160000 x i32], [160000 x i32]* %1, i64 0, i64 %34
  store i32 %33, i32* %35, align 4
  %36 = add nuw nsw i64 %31, 1
  %37 = icmp eq i64 %36, 400
  br i1 %37, label %27, label %30

38:                                               ; preds = %128, %25
  %39 = phi i32* [ %26, %25 ], [ %40, %128 ]
  %40 = phi i32* [ %5, %25 ], [ %39, %128 ]
  tail call void (...) @display() #3
  br label %41

41:                                               ; preds = %54, %38
  %42 = phi i64 [ 0, %38 ], [ %55, %54 ]
  %43 = mul nuw nsw i64 %42, 400
  %44 = trunc i64 %42 to i32
  %45 = add i32 %44, 399
  %46 = urem i32 %45, 400
  %47 = mul nuw nsw i32 %46, 400
  %48 = add i32 %44, 400
  %49 = urem i32 %48, 400
  %50 = mul nuw nsw i32 %49, 400
  %51 = add i32 %44, 401
  %52 = urem i32 %51, 400
  %53 = mul nuw nsw i32 %52, 400
  br label %57

54:                                               ; preds = %57
  %55 = add nuw nsw i64 %42, 1
  %56 = icmp eq i64 %55, 400
  br i1 %56, label %124, label %41

57:                                               ; preds = %57, %41
  %58 = phi i64 [ 0, %41 ], [ %122, %57 ]
  %59 = add nuw nsw i64 %58, %43
  %60 = getelementptr inbounds i32, i32* %40, i64 %59
  %61 = load i32, i32* %60, align 4
  %62 = icmp eq i32 %61, 1
  %63 = sext i1 %62 to i32
  %64 = trunc i64 %58 to i32
  %65 = add i32 %64, 399
  %66 = urem i32 %65, 400
  %67 = add nuw nsw i32 %66, %47
  %68 = zext i32 %67 to i64
  %69 = getelementptr inbounds i32, i32* %40, i64 %68
  %70 = load i32, i32* %69, align 4
  %71 = add nsw i32 %70, %63
  %72 = add i32 %64, 400
  %73 = urem i32 %72, 400
  %74 = add nuw nsw i32 %73, %47
  %75 = zext i32 %74 to i64
  %76 = getelementptr inbounds i32, i32* %40, i64 %75
  %77 = load i32, i32* %76, align 4
  %78 = add nsw i32 %71, %77
  %79 = add i32 %64, 401
  %80 = urem i32 %79, 400
  %81 = add nuw nsw i32 %80, %47
  %82 = zext i32 %81 to i64
  %83 = getelementptr inbounds i32, i32* %40, i64 %82
  %84 = load i32, i32* %83, align 4
  %85 = add nsw i32 %78, %84
  %86 = add nuw nsw i32 %66, %50
  %87 = zext i32 %86 to i64
  %88 = getelementptr inbounds i32, i32* %40, i64 %87
  %89 = load i32, i32* %88, align 4
  %90 = add nsw i32 %85, %89
  %91 = add nuw nsw i32 %73, %50
  %92 = zext i32 %91 to i64
  %93 = getelementptr inbounds i32, i32* %40, i64 %92
  %94 = load i32, i32* %93, align 4
  %95 = add nsw i32 %90, %94
  %96 = add nuw nsw i32 %80, %50
  %97 = zext i32 %96 to i64
  %98 = getelementptr inbounds i32, i32* %40, i64 %97
  %99 = load i32, i32* %98, align 4
  %100 = add nsw i32 %95, %99
  %101 = add nuw nsw i32 %66, %53
  %102 = zext i32 %101 to i64
  %103 = getelementptr inbounds i32, i32* %40, i64 %102
  %104 = load i32, i32* %103, align 4
  %105 = add nsw i32 %100, %104
  %106 = add nuw nsw i32 %73, %53
  %107 = zext i32 %106 to i64
  %108 = getelementptr inbounds i32, i32* %40, i64 %107
  %109 = load i32, i32* %108, align 4
  %110 = add nsw i32 %105, %109
  %111 = add nuw nsw i32 %80, %53
  %112 = zext i32 %111 to i64
  %113 = getelementptr inbounds i32, i32* %40, i64 %112
  %114 = load i32, i32* %113, align 4
  %115 = add nsw i32 %110, %114
  %116 = icmp eq i32 %115, 3
  %117 = icmp eq i32 %115, 2
  %118 = select i1 %117, i1 %62, i1 false
  %119 = or i1 %116, %118
  %120 = zext i1 %119 to i32
  %121 = getelementptr inbounds i32, i32* %39, i64 %59
  store i32 %120, i32* %121, align 4
  %122 = add nuw nsw i64 %58, 1
  %123 = icmp eq i64 %122, 400
  br i1 %123, label %54, label %57

124:                                              ; preds = %54, %128
  %125 = phi i64 [ %129, %128 ], [ 0, %54 ]
  %126 = mul nuw nsw i64 %125, 400
  %127 = trunc i64 %125 to i32
  br label %131

128:                                              ; preds = %131
  %129 = add nuw nsw i64 %125, 1
  %130 = icmp eq i64 %129, 400
  br i1 %130, label %38, label %124

131:                                              ; preds = %131, %124
  %132 = phi i64 [ 0, %124 ], [ %137, %131 ]
  %133 = add nuw nsw i64 %132, %126
  %134 = getelementptr inbounds i32, i32* %39, i64 %133
  %135 = load i32, i32* %134, align 4
  %136 = trunc i64 %132 to i32
  tail call void @setPixel(i32 noundef %136, i32 noundef %127, i32 noundef %135)
  %137 = add nuw nsw i64 %132, 1
  %138 = icmp eq i64 %137, 400
  br i1 %138, label %128, label %131
}

declare void @llvm.lifetime.start.p0i8(i64 immarg, i8* nocapture)

declare i32 @generate(...)

declare void @init(i32 noundef, i32 noundef)

declare void @display(...)

declare void @setPixel(i32 noundef, i32 noundef, i32 noundef)