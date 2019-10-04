#pragma once
#include <map>

using position = std::pair<size_t, size_t>;

class Matrix_Sparse
{
private:
	std::map<position, double> matrix;
	size_t height;
	size_t width;
public:
	Matrix_Sparse() {
		height = 0;
		width = 0;
	};
	Matrix_Sparse(size_t m, size_t n) {
		height = m;
		width = n;
	}
	~Matrix_Sparse() = default;
	void set(size_t row, size_t col, double val);
	double get(size_t row, size_t col);
	size_t get_width() { return width; }
	size_t get_heigth() { return height; }
};

