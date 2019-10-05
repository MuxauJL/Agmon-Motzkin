#include "Matrix_Sparse.h"

void Matrix_Sparse::set(size_t row, size_t col, double val)
{
	if (row > height)
		height = row;
	if (col > width)
		width = col;
	matrix[position(row, col)] = val;
}

double Matrix_Sparse::get(size_t row, size_t col)
{
	if (row > height || col > width)
		matrix.at(position(row, col));
	else {
		auto it = matrix.find(position(row, col));
		if ( it!= matrix.end())
			return it->second;
		else
			return 0.0;
	}
}
