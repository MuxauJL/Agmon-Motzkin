#pragma once
#include <map>
class IMatrix_Sparse {
public:
	// position from (1, 1) to (height, width)
	virtual void set(size_t row, size_t col, double val) = 0;
	virtual double get(size_t row, size_t col) = 0;
	virtual size_t get_width() = 0;
	virtual size_t get_height() = 0;
	virtual std::map<size_t, double>::iterator begin(size_t row) = 0;
	virtual std::map<size_t, double>::iterator end(size_t row) = 0;
};