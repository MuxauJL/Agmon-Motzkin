#include <iostream>
#include "Agmon_Motzkin.h"
#include "MPS_Parser.h"

double get_distance(const std::vector<double>& v1, const std::vector<double>& v2) {
	if (v1.size() != v2.size())
		throw std::invalid_argument("Different sizes of vectors");
	double result = 0;
	for (size_t i = 0; i < v1.size(); ++i) {
		double tmp = v1[i] - v2[i];
		result += tmp * tmp;
	}
	return sqrt(result);
}

std::pair<size_t,double> iterate(size_t N, double E, Agmon_Motzkin& task) {
	size_t count = 0;
	double d;
	auto current_x = task.get_x();
	do {
		task.next_x();
		auto next_x = task.get_x();
		d = get_distance(current_x, next_x);
		++count;
		current_x = next_x;
		//std::cout << count << ") " << d << '\n' /*<< task.calculate_criterion() << '\n'*/;
		/*for (auto e :current_x)
					std::cout << e << " ";
		std::cout << '\n';*/
	} while (count < N && d>E);
	return std::pair<size_t, double>(count, d);
}

int main() {
	//std::unique_ptr<Linear_Programming_Task> Abc =
	//	std::unique_ptr<Linear_Programming_Task>(new Linear_Programming_Task());
	//Abc->A->set(1, 1, -4);
	//Abc->A->set(1, 2, -7);
	//Abc->A->set(2, 1, -8);
	//Abc->A->set(2, 2, -3);
	//Abc->A->set(3, 1, -9);
	//Abc->A->set(3, 2, -5);
	//Abc->A->set(4, 1, 1);
	//Abc->A->set(4, 2, 0);
	//Abc->A->set(5, 1, 0);
	//Abc->A->set(5, 2, 1);
	//Abc->b->set(1, -49);
	//Abc->b->set(2, -51);
	//Abc->b->set(3, -45);
	//Abc->b->set_length(Abc->A->get_height());
	//Abc->c->set(1, -6);
	//Abc->c->set(2, -5);
	//Abc->c->set_length(Abc->A->get_height());
	//Agmon_Motzkin test_task(std::move(Abc));
	//test_task.set_x({ 0,0 }); // opt_x = {70./43, 261./43} opt_f = 1725./43
	//for (auto e : test_task.get_x())
	//	std::cout << e << " ";
	//std::cout << "\n";
	//constexpr size_t N = 10000;
	//iterate(N, 0, test_task);
	//for (auto e : test_task.get_x())
	//	std::cout << e << " ";
	//std::cout << "\n" << test_task.get_biggest_violation() << '\n';

	//double k1 = 1000;
	//double k2 = k1 / 2;
	///*k1 = 42;
	//k2 = 38;*/
	//double upper_bound = test_task.calculate_criterion();
	//std::cout << "Upper bound = " << upper_bound << "\n\n";
	//upper_bound = test_task.calculate_criterion();
	//auto criterion = test_task.get_criterion();
	//test_task.add_limitation(criterion, upper_bound - k1);

	//for (size_t i = 1; i <= criterion.get_length(); ++i) {
	//	auto current = criterion.get(i);
	//	if (current != 0.0)
	//		criterion.set(i, -current);
	//}
	//test_task.add_limitation(criterion, -upper_bound + k2);

	//do {
	//	auto [count, distance] = iterate(N, 0, test_task);
	//	std::cout << "Count = " << count << "\n";
	//	std::cout << "Distance = " << distance << "\n";
	//	std::cout << "k1 = " << k1 << ", k2 = " << k2 << std::endl;
	//	std::cout << "Criterion = " << test_task.calculate_criterion() << '\n';
	//	auto biggest_violation = test_task.get_biggest_violation();
	//	std::cout << "Biggest violation = " << biggest_violation << " \n";
	//	for (auto e : test_task.get_x())
	//		std::cout << e << " ";
	//	std::cout << '\n';
	//	if (distance <= 0) {
	//		k2 += (k1 - k2) / 2;
	//	}
	//	else {
	//		k1 -= (k1 - k2) / 2;
	//		k2 = k1 / 2;
	//	}
	//	size_t row = test_task.get_height();
	//	test_task.set_b(row - 1, upper_bound - k1);
	//	test_task.set_b(row, -upper_bound + k2);
	//} while (k1 - k2 > 1);

	//for (size_t i = 0; i < 100; ++i) {
	//	/*for (size_t i = 1; i <= task.get_height(); ++i)
	//		std::cout << task.check_limitation(i, task.get_x()) << " ";
	//	std::cout << "\n";*/
	//	for (auto e : test_task.next_x())
	//		std::cout << e << " ";
	//	std::cout << "\n";
	//}

	MPS_Parser parser;
	std::unique_ptr<Linear_Programming_Task> Abc =
		std::unique_ptr<Linear_Programming_Task>(parser.parse("netlib\\BLEND.SIF"));//BLEND,OSA-60,CRE-B,AFIRO
	Agmon_Motzkin task(std::move(Abc));
	for (size_t i = 1; i <= task.get_width(); ++i) {
		Vector_Sparse lim;
		lim.set(i, 1);
		lim.set_length(task.get_width());
		task.add_limitation(lim);
	}
	std::vector<double> x(task.get_width(), 10);
	task.set_x(x);
	constexpr double E =  0.00000001;
	constexpr size_t N = 100000;
	auto [count, distance] = iterate(N, E, task);
	std::cout << "Count = " << count << "\n";
	std::cout << "Distance = " << distance << "\n";

	/*auto get_biggest_violation = [&task]() {
		double min = INT_MAX;
		for (size_t j = 1; j <= task.get_height(); ++j) {
			auto lim = task.check_limitation(j);
			if (min > lim)
				min = lim;
		}
		return min;
	};*/
	/*auto get_current_distance = [&task]() {
		auto current_x = task.get_x();
		task.next_x();
		auto next_x = task.get_x();
		return distance(current_x, next_x);
	};*/

	std::cout << "Biggest violation = " << task.get_biggest_violation() << " \n";

	/*for (auto x : task.get_x())
		std::cout << x << " ";*/

	//	// f(x) -> min
	double k1 = 1000;
	double k2 = k1 / 2;
	/*k1 = 580;
	k2 = 570;*/
	/*k1 = 1500;
	k2 = 1000;*/
	double upper_bound = task.calculate_criterion();
	std::cout << "Upper bound = " << upper_bound << "\n\n";
	upper_bound = task.calculate_criterion();
	auto criterion = task.get_criterion();
	task.add_limitation(criterion, upper_bound - k1);

	for (size_t i = 1; i <= criterion.get_length(); ++i) {
		auto current = criterion.get(i);
		if (current != 0.0)
			criterion.set(i, -current);
	}
	task.add_limitation(criterion, -upper_bound + k2);

	do {
		auto [count, distance] = iterate(N, E, task);
		std::cout << "Count = " << count << "\n";
		std::cout << "Distance = " << distance << "\n";
		std::cout << "k1 = " << k1 << ", k2 = " << k2 << std::endl;
		std::cout << "Criterion = " << task.calculate_criterion() << '\n';
		auto biggest_violation = task.get_biggest_violation();
		std::cout << "Biggest violation = " << biggest_violation << " \n";
		if (distance <= E) {
			k2 += (k1 - k2) / 2;
		}
		else {
			k1 = k2;
			k2 = k1 / 2;
		}
		size_t row = task.get_height();
		task.set_b(row - 1, upper_bound - k1);
		task.set_b(row, -upper_bound + k2);
	} while (k1 - k2 > 1);
	std::ofstream os("best_results.txt");
	if (os.is_open()) {
		auto [c, x] = task.get_best_results();
		os << c << '\n';
		for (auto& e : x)
			os << e << ' ';
	}
	os.close();
	system("pause");
	return 0;
}