#include "Agmon_Motzkin.h"
#include <omp.h>

double Agmon_Motzkin::check_limitation(size_t index, const std::vector<double>* x)
{
	if (x == nullptr)
		x = &this->x;
	double result = 0;
	for (size_t i = 1; i <= A->get_width(); ++i)
		result += A->get(index, i) * (*x)[i - 1];
	return result - b->get(index);
}

double Agmon_Motzkin::calculate_criterion(const std::vector<double>* x)
{
	if (x == nullptr)
		x = &this->x;
	double result = 0;
	for (size_t i = 1; i <= A->get_width(); ++i)
		result += c->get(i) * (*x)[i - 1];
	if (best_criterion > result) {
		best_criterion = result;
		record = *x;
	}
	return result;
}

void Agmon_Motzkin::next_x()
{
	bool is_solved = true;
	std::vector<size_t> indexes;
	std::vector<double> limitations(A->get_height());
#pragma omp parallel for
	for (int i = 1; i <= A->get_height(); ++i) 
		limitations[i - 1] = check_limitation(i);
	
	for (size_t i = 0; i < limitations.size();++i) {
		if (limitations[i] < 0) {
			is_solved = false;
			indexes.push_back(i + 1);
		}
	}
	if (is_solved)
		return;

	auto sum_of_squares = [this](size_t index) {
		double result = 0;
		for (size_t i = 1; i <= A->get_width(); ++i) {
			double a = A->get(index, i);
			result += a * a;
		}
		return result;
	};

	size_t index_of_max = indexes[0];
	double max = -limitations[index_of_max - 1] / sum_of_squares(index_of_max);
	for (size_t i = 1; i < indexes.size(); ++i) {
		double result = -limitations[indexes[i] - 1] / sum_of_squares(indexes[i]);
		if (max < result) {
			max = result;
			index_of_max = indexes[i];
		}
	}

	for (size_t i = 1; i <= A->get_width(); ++i)
		x[i - 1] += max * A->get(index_of_max, i);
	return;
}

void Agmon_Motzkin::add_limitation(const Vector_Sparse& lim, const double b)
{
	size_t row = A->get_height() + 1;
	for (size_t i = 1; i <= A->get_width(); ++i) {
		A->set(row, i, lim.get(i));
	}
	this->b->set(row, b);
}

double Agmon_Motzkin::get_biggest_violation() {
	double min = INT_MAX;
	for (size_t j = 1; j <= A->get_height(); ++j) {
		auto lim = check_limitation(j);
		if (min > lim)
			min = lim;
	}
	return min;
}