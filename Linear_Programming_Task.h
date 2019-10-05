#pragma once
#include "Matrix_Sparse.h"
#include "Vector_Sparse.h"
#include <memory>

struct Linear_Programming_Task {
	std::unique_ptr<Matrix_Sparse> A;
	std::unique_ptr < Vector_Sparse> b;
	std::unique_ptr < Vector_Sparse> c;
	Linear_Programming_Task() {
		A = std::unique_ptr<Matrix_Sparse>(new Matrix_Sparse());
		b = std::unique_ptr<Vector_Sparse>(new Vector_Sparse());
		c = std::unique_ptr<Vector_Sparse>(new Vector_Sparse());
	}
};