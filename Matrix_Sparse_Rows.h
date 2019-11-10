#pragma once
#include "IMatrix_Sparse.h"
#include "Vector_Sparse.h"
#include <vector>
class Matrix_Sparse_Rows :
	public IMatrix_Sparse
{
private:
	// position from (1, 1) to (height, width)
	std::vector<Vector_Sparse> matrix;
	size_t height;
	size_t width;
public:
	Matrix_Sparse_Rows() {
		height = 0;
		width = 0;
	};
	Matrix_Sparse_Rows(size_t m, size_t n) {
		matrix = std::vector<Vector_Sparse>(m, Vector_Sparse(n));
		height = m;
		width = n;
	}
	~Matrix_Sparse_Rows() = default;
	void set(size_t row, size_t col, double val);
	double get(size_t row, size_t col);
	size_t get_width() { return width; }
	size_t get_height() { return height; }
	virtual std::map<size_t, double>::iterator begin(size_t row) { return matrix[row - 1].begin(); }
	virtual std::map<size_t, double>::iterator end(size_t row) { return matrix[row - 1].end(); }
};

