
#include <iostream>
#include "Agmon_Motzkin.h"
#include <fstream>
#include <iterator>

int main() {
	/*Matrix_Sparse A(5, 2);
	A.set(0, 0, -1);
	A.set(0, 1, -2);
	A.set(1, 0, -2);
	A.set(1, 1, -1);
	A.set(2, 0, 0);
	A.set(2, 1, -1);
	A.set(3, 0, 1);
	A.set(3, 1, 0);
	A.set(4, 0, 0);
	A.set(4, 1, 1);
	std::vector<double> b = { -6, -8, -2, 0, 0 };
	Agmon_Motzkin task(A, b);
	task.set_x({ 10,-10 });*/
	size_t m, n;
	Matrix_Sparse A;
	std::vector<double> b;
	std::fstream fs("data.txt");
	if (fs.is_open()) {	
		fs >> m;
		fs >> n;
		A = Matrix_Sparse(m, n);
		b = std::vector<double>(m, 0);
		while (!fs.eof()) {
			size_t r, c;
			double val;
			fs >> r >> c >> val;
			if (c == n)
				b[r] = val;
			else
				A.set(r, c, val);
		}
	}
	else
		return -1;
	Agmon_Motzkin task(A, b);
	task.set_x({ 5,-10 });
	for (auto e : task.get_x())
		std::cout << e << " ";
	std::cout << "\n";

	/*for(size_t i=0;i<b.size();++i)
		std::cout << task.check_limitation(i,task.get_x()) << " ";
	std::cout << "\n";*/

	for (size_t i = 0; i < 100; ++i) {
		/*for (size_t i = 0; i < b.size(); ++i)
			std::cout << task.check_limitation(i, task.get_x()) << " ";
		std::cout << "\n";*/
		for (auto e : task.next_x())
			std::cout << e << " ";
		std::cout << "\n";
	}
	system("pause");
	return 0;
}