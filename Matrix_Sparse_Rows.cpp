#include "Matrix_Sparse_Rows.h"

void Matrix_Sparse_Rows::set(size_t row, size_t col, double val)
{
	if (row > height) {
		while (matrix.size() != row)
			matrix.emplace_back(Vector_Sparse(width));
		height = row;
		matrix.back().set(col, val);
	}
	else
		matrix[row - 1].set(col, val);
	if (col > width)
		width = col;
}

double Matrix_Sparse_Rows::get(size_t row, size_t col)
{
	if (row > matrix.size())
		matrix.at(row - 1);
	matrix[row - 1].set_length(width);
	return matrix[row - 1].get(col);
}
