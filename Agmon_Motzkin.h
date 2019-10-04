#pragma once
#include "Matrix_Sparse.h"
#include <vector>
class Agmon_Motzkin
{
private:
	Matrix_Sparse A;
	std::vector<double> b;
	std::vector<double> x;
public:
	Agmon_Motzkin(Matrix_Sparse matrix, std::vector<double> b) {
		A = matrix;
		this->b = b;
		x = std::vector<double>(matrix.get_width(), 0);
	}
	double check_limitation(size_t index, const std::vector<double>& x);
	std::vector<double> next_x();
	std::vector<double> get_x() { return x; }
	void set_x(std::vector<double> x) { this->x = x; }
};

