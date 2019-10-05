#pragma once
#include "Linear_Programming_Task.h"
#include <vector>
class Agmon_Motzkin
{
private:
	std::unique_ptr<Linear_Programming_Task> Abc;
	Matrix_Sparse* A;
	Vector_Sparse* b;
	Vector_Sparse* c;
	std::vector<double> x;
public:
	Agmon_Motzkin(std::unique_ptr<Matrix_Sparse> matrix, std::unique_ptr < Vector_Sparse> b, std::unique_ptr < Vector_Sparse> c) {
		Abc = std::unique_ptr<Linear_Programming_Task>(new Linear_Programming_Task());
		Abc->A = std::move(matrix);
		Abc->b = std::move(b);
		Abc->c = std::move(c);
		A = Abc->A.get();
		this->b = Abc->b.get();
		this->c = Abc->c.get();
		x = std::vector<double>(matrix->get_width(), 0);
	}
	Agmon_Motzkin(std::unique_ptr<Linear_Programming_Task> task) {
		Abc = std::move(task);
		task = nullptr;
		A = Abc->A.get();
		b = Abc->b.get();
		c = Abc->c.get();
		x = std::vector<double>(A->get_width(), 0);
	}
	double check_limitation(size_t index, const std::vector<double>& x);
	std::vector<double> next_x();
	std::vector<double> get_x() { return x; }
	void set_x(std::vector<double> x) { this->x = x; }
	size_t get_width() { return A->get_width(); }
	size_t get_height() { return A->get_height(); }
};

