#include "Agmon_Motzkin.h"

double Agmon_Motzkin::check_limitation(size_t index, const std::vector<double>& x)
{
	double result = 0;
	for (size_t c = 0; c < A->get_width(); ++c)
		result += A->get(index, c) * x[c];
	return result - b->get(index);
}

std::vector<double> Agmon_Motzkin::next_x()
{
	bool is_solved = true;
	std::vector<size_t> indexes;
	for (size_t i = 0; i < A->get_height(); ++i) {
		if (check_limitation(i, x) < 0) {
			is_solved = false;
			indexes.emplace_back(i);
		}
	}
	if (is_solved)
		return x;

	auto sum_of_squares = [this](size_t index) {
		double result = 0;
		for (size_t c = 0; c < A->get_width(); ++c) {
			double a = A->get(index, c);
			result += a * a;
		}
		return result;
	};

	size_t index_of_max = indexes[0];
	double max = -check_limitation(index_of_max, x) / sum_of_squares(index_of_max);
	for (size_t i = 1; i < indexes.size(); ++i) {
		double result = -check_limitation(indexes[i], x) / sum_of_squares(indexes[i]);
		if (max < result) {
			max = result;
			index_of_max = indexes[i];
		}
	}

	for (size_t c = 0; c < x.size(); ++c)
		x[c] += max * A->get(index_of_max, c);
	return x;
}
