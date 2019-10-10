#include <iostream>
#include "Agmon_Motzkin.h"
#include "MPS_Parser.h"

int main() {
	//std::unique_ptr<Linear_Programming_Task> Abc =
	//	std::unique_ptr<Linear_Programming_Task>(new Linear_Programming_Task());
	//Abc->A->set(1, 1, -1);
	//Abc->A->set(1, 2, -2);
	//Abc->A->set(2, 1, -2);
	//Abc->A->set(2, 2, -1);
	//Abc->A->set(3, 1, 0);
	//Abc->A->set(3, 2, -1);
	//Abc->A->set(4, 1, 1);
	//Abc->A->set(4, 2, 0);
	//Abc->A->set(5, 1, 0);
	//Abc->A->set(5, 2, 1);
	//Abc->b->set(1, -6);
	//Abc->b->set(2, -8);
	//Abc->b->set(3, -2);
	//Abc->b->set_length(Abc->A->get_height());
	//Agmon_Motzkin task(std::move(Abc));
	//task.set_x({ 10,10 });
	//for (auto e : task.get_x())
	//	std::cout << e << " ";
	//std::cout << "\n";

	//for (size_t i = 0; i < 100; ++i) {
	//	/*for (size_t i = 1; i <= task.get_height(); ++i)
	//		std::cout << task.check_limitation(i, task.get_x()) << " ";
	//	std::cout << "\n";*/
	//	for (auto e : task.next_x())
	//		std::cout << e << " ";
	//	std::cout << "\n";
	//}

	auto distance = [](const std::vector<double>& v1, const std::vector<double>& v2) {
		if (v1.size() != v2.size())
			throw std::invalid_argument("Different sizes of vectors");
		double result = 0;
		for (size_t i = 0; i < v1.size(); ++i) {
			double tmp = v1[i] - v2[i];
			result += tmp * tmp;
		}
		return sqrt(result);
	};
	MPS_Parser parser;
	std::unique_ptr<Linear_Programming_Task> Abc =
		std::unique_ptr<Linear_Programming_Task>(parser.parse("netlib\\BLEND.SIF"));//AFIRO
	Agmon_Motzkin task(std::move(Abc));
	std::vector<double> x(task.get_width(), 10);
	task.set_x(x);
	constexpr double E = 0.0001;
	constexpr size_t N = 10000;
	//bool solved = false;
	size_t count = 0;
	double d;
	do{
		auto current_x = task.get_x();
		d = distance(current_x, task.next_x());
		++count;
		//solved = true;
		x = task.next_x();
		/*for (size_t j = 1; j <= task.get_height(); ++j)
			if (task.check_limitation(j, task.get_x()) < 0-E)
				solved = false;*/
	} while (count < N&& d>E);
	std::cout << count<<"\n";
	double min = INT_MAX;
	for (size_t j = 1; j <= task.get_height(); ++j) {
		auto lim = task.check_limitation(j);
		if (min > lim)
			min = lim;
	}
	std::cout << min << " \n";
	
	for (auto x : task.get_x())
		std::cout << x << " ";

	std::cout << "\n";
	double k1 = 100;
	double k2 = k1 / 2;
	double current_criterion = task.get_criterion();
	std::cout << current_criterion;
	system("pause");
	return 0;
}