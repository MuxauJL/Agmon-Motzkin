#include <iostream>
#include "Agmon_Motzkin.h"
//#include <iterator>
//#include "Blend_Parser.h"
//#include <memory>
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

	MPS_Parser parser;
	std::unique_ptr<Linear_Programming_Task> Abc =
		std::unique_ptr<Linear_Programming_Task>(parser.parse("netlib\\BLEND.SIF"));//AFIRO
	Agmon_Motzkin task(std::move(Abc));
	std::vector<double> x(task.get_width(), 1);
	task.set_x(x);
	for (size_t i = 0; i < 100000; ++i) {
		bool solved = true;
		x = task.next_x();
		/*for (size_t j = 1; j <= task.get_height(); ++j)
			if (task.check_limitation(j, task.get_x()) < -0.5)
				solved = false;
		std::cout << solved;*/
	}
	std::cout << "\n";
	double min = INT_MAX;
	for (size_t j = 1; j <= task.get_height(); ++j) {
		auto lim = task.check_limitation(j, task.get_x());
		if (min > lim)
			min = lim;
	}
	std::cout << min << " \n";
	/*for (auto x : task.get_x())
		std::cout << x << " ";*/
	system("pause");
	return 0;
}