
#include <iostream>
#include "Agmon_Motzkin.h"
#include <fstream>
#include <iterator>
#include "Blend_Parser.h"
//#include <memory>

int main() {
	Blend_Parser parser;
	std::unique_ptr<Linear_Programming_Task> Abc =
		std::unique_ptr<Linear_Programming_Task>(parser.parse("BLEND.SIF"));
	Agmon_Motzkin task(std::move(Abc));
	std::vector<double> x(task.get_width(), 0);
	task.set_x(x);
	for (size_t i = 0; i < 100; ++i) {
		bool solved = true;
		x = task.next_x();
		for (size_t i = 0; i < task.get_height(); ++i)
			if (task.check_limitation(i, task.get_x()) < 0.001)
				solved = false;
		std::cout << solved;
	}
	system("pause");
	return 0;
}