#pragma once
#include "Linear_Programming_Task.h"
#include <vector>
class Agmon_Motzkin
{
private:
	std::unique_ptr<Linear_Programming_Task> Abc;
	IMatrix_Sparse* A;
	Vector_Sparse* b;
	Vector_Sparse* c;
	std::vector<double> x;
	double best_criterion;
	std::vector<double> record;
public:
	Agmon_Motzkin(std::unique_ptr<IMatrix_Sparse> matrix, std::unique_ptr < Vector_Sparse> b, std::unique_ptr < Vector_Sparse> c) {
		Abc = std::unique_ptr<Linear_Programming_Task>(new Linear_Programming_Task());
		Abc->A = std::move(matrix);
		Abc->b = std::move(b);
		Abc->c = std::move(c);
		A = Abc->A.get();
		this->b = Abc->b.get();
		this->c = Abc->c.get();
		x = std::vector<double>(matrix->get_width(), 0);
		best_criterion = 0;
	}
	Agmon_Motzkin(std::unique_ptr<Linear_Programming_Task> task) {
		Abc = std::move(task);
		task = nullptr;
		A = Abc->A.get();
		b = Abc->b.get();
		c = Abc->c.get();
		x = std::vector<double>(A->get_width(), 0);
	}
	double check_limitation(size_t index, const std::vector<double>* x = nullptr);
	double calculate_criterion(const std::vector<double>* x = nullptr);
	void next_x();
	std::vector<double> get_x() { return x; }
	void set_x(std::vector<double> x) { this->x = x; }
	void set_A(size_t row, size_t col, double val) { A->set(row, col, val); }
	void set_b(size_t row, double val) { b->set(row, val); }
	double get_A(size_t row, size_t col) { return A->get(row, col); }
	double get_b(size_t row) { return b->get(row); }
	size_t get_width() { return A->get_width(); }
	size_t get_height() { return A->get_height(); }
	Vector_Sparse get_criterion() { return *c; }
	void add_limitation(const Vector_Sparse& lim, const double b = 0.0);
	double get_biggest_violation();
	std::pair<double, std::vector<double>>get_best_results() { return std::pair<double, std::vector<double>>(best_criterion, record); }
};

