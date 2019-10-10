#include "Agmon_Motzkin.h"

double Agmon_Motzkin::check_limitation(size_t index, const std::vector<double>* x)
{
	if (x == nullptr)
		x = &this->x;
	double result = 0;
	for (size_t i = 1; i <= A->get_width(); ++i)
		result += A->get(index, i) * (*x)[i - 1];
	return result - b->get(index);
}

double Agmon_Motzkin::get_criterion(const std::vector<double>* x)
{
	if (x == nullptr)
		x = &this->x;
	double result = 0;
	for (size_t i = 1; i <= A->get_width(); ++i)
		result += c->get(i) * (*x)[i - 1];
	return result;
}

std::vector<double> Agmon_Motzkin::next_x()
{
	bool is_solved = true;
	std::vector<size_t> indexes;
	for (size_t i = 1; i <= A->get_height(); ++i) {
		if (check_limitation(i) < 0) {
			is_solved = false;
			indexes.emplace_back(i);
		}
	}
	if (is_solved)
		return x;

	auto sum_of_squares = [this](size_t index) {
		double result = 0;
		for (size_t i = 1; i <= A->get_width(); ++i) {
			double a = A->get(index, i);
			result += a * a;
		}
		return result;
	};

	size_t index_of_max = indexes[0];
	double max = -check_limitation(index_of_max) / sum_of_squares(index_of_max);
	for (size_t i = 1; i < indexes.size(); ++i) {
		double result = -check_limitation(indexes[i]) / sum_of_squares(indexes[i]);
		if (max < result) {
			max = result;
			index_of_max = indexes[i];
		}
	}

	for (size_t i = 1; i <= A->get_width(); ++i)
		x[i - 1] += max * A->get(index_of_max, i);
	return x;
}

void Agmon_Motzkin::add_limitation(const Vector_Sparse& lim)
{
	size_t row = A->get_height() + 1;
	for (size_t i = 1; i <= A->get_width(); ++i) {
		A->set(row, i, lim.get(i));
	}
}
