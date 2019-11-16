#include <iostream>
#include "Agmon_Motzkin.h"
#include "MPS_Parser.h"
#include <ClpSimplex.hpp>
#include <ClpPresolve.hpp>

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
		std::unique_ptr<Linear_Programming_Task>(parser.parse("netlib\\AFIRO.SIF"));//BLEND,OSA-60,CRE-B,AFIRO
	ClpSimplex model;
	auto& A = Abc->A;
	auto& c = Abc->c;
	size_t height = A->get_height();
	size_t width = A->get_width();
	std::vector<double> rowLower;
	std::vector<double> objective;
	for (size_t i = 1; i <= width; ++i) {
		objective.push_back(c->get(i));
	}
	CoinPackedMatrix  matrix;
	matrix.setDimensions(height, width);
	for (size_t r = 1; r <= height; ++r) {
		//std::cout << '\n';
		rowLower.push_back(Abc->b->get(r));
		for (size_t c = 1; c <= width; ++c) {
			matrix.modifyCoefficient(r - 1, c - 1, A->get(r, c));
			//std::cout << A->get(r, c) << ":" << matrix.getCoefficient(r - 1, c - 1) << " ";
		}
	}
	std::vector<double> rowUpper(rowLower.size(), COIN_DBL_MAX);
	std::vector<double> colLower(objective.size(), 0.0);
	std::vector<double> colUpper(objective.size(), COIN_DBL_MAX);
	model.loadProblem(matrix, colLower.data(), colUpper.data(),
		objective.data(), rowLower.data(), rowUpper.data());
	model.setOptimizationDirection(1);

	/*auto mat = model.matrix();
	for (size_t r = 1; r <= height; ++r) {
		for (size_t c = 1; c <= width; ++c) {
			std::cout << mat->getCoefficient(r - 1, c - 1) << " ";
		}
		std::cout <<'|'<<model.getRowLower()[r-1]<< '\n';
	}*/
	Agmon_Motzkin task(std::move(Abc));
	// x[i]>=0, i=1,...,n
	for (size_t i = 1; i <= width; ++i) {
		Vector_Sparse lim;
		lim.set(i, 1);
		lim.set_length(width);
		task.add_limitation(lim);
	}
	std::vector<double> x(width, 10);
	task.set_x(x);
	constexpr double E = 0.000000001;
	constexpr size_t N = 1000000;
	auto [count, distance] = iterate(N, E, task);
	std::cout << "Count = " << count << "\n";
	std::cout << "Distance = " << distance << "\n";
	std::cout << "Biggest violation = " << task.get_biggest_violation() << " \n";
	double upper_bound = task.calculate_criterion();
	std::cout << "Upper bound = " << upper_bound << "\n\n";

	double k1 = 1000;
	double k2 = 400;
	/*k1 = 618;
	k2 = 617;*/

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
			k2 = 0;
		}
		size_t row = task.get_height();
		task.set_b(row - 1, upper_bound - k1);
		task.set_b(row, -upper_bound + k2);
	} while (/*k1 - k2 > 0.1*/false);


	ClpSolve solvectl;
	solvectl.setSolveType(ClpSolve::usePrimal);
	solvectl.setPresolveType(ClpSolve::presolveOn);
	auto model_copy = model;
	auto clone = model;
	//clone.setPrimalTolerance(0.001);
	//clone.setDualTolerance(0.001);
	/*clone.setCurrentPrimalTolerance(0.1);
	clone.setCurrentDualTolerance(0.1);*/
	std::cout << "\nTOLERANCE:\n"
		<< "primal:" << clone.primalTolerance() << std::endl
		<< "dual:" << clone.dualTolerance() << std::endl
		<< "presolve:" << clone.presolveTolerance() << std::endl;
		/*<< "currentPrimal:" << clone.currentPrimalTolerance() << std::endl
		<< "currentDual:" << clone.currentDualTolerance() << std::endl;*/
	std::cout << '\n' << "model:\n";
	//model.setPrimalTolerance(0.1);
	model = clone;
	model.primal(1);
	auto solutionPrimal = model.primalColumnSolution();

	std::cout << '\n' << "model_copy:\n";
	model_copy = clone;
	model_copy.setColSolution(/*solutionPrimal*/task.get_x().data());
	model_copy.checkSolution();
	//model_copy.setPrimalTolerance(0.1);
	model_copy.primal(1);

	std::cout << '\n' << "model_initial_solve:\n";
	model = clone;
	//model.setPrimalTolerance(0.1);
	model.initialSolve(solvectl);
	solutionPrimal = model.primalColumnSolution();

	std::cout << '\n' << "model_copy_initial_solve:\n";
	model_copy = clone;
	model_copy.setColSolution(/*solutionPrimal*/task.get_x().data());
	model_copy.checkSolution();
	//model_copy.setPrimalTolerance(0.1);
	model_copy.initialSolve(solvectl);

	std::cout << '\n' << "model_with_presolving:\n";
	model = clone;
	ClpPresolve presolveInfo;
	ClpSimplex* presolvedModel = presolveInfo.presolvedModel(model);
	if (presolvedModel)
		presolveInfo.postsolve(true);
	model.primal(1);
	//std::cout << "\n " << model.getIterationCount() << '\n';
	std::cout << '\n';
	auto solution = model.getColSolution();

	std::cout << '\n' << "model_copy_with_presolving:\n";
	model_copy = clone;
	model_copy.setColSolution(/*solution*/task.get_x().data());
	model_copy.checkSolution();
	presolveInfo;
	presolvedModel = presolveInfo.presolvedModel(model_copy);
	// at this point we have original model and a new model.  The  information
	// on the operations done is in presolveInfo 
	if (presolvedModel) {
		// was not found to be infeasible - so lets solve 
		// if presolvedModel was NULL then it was primal infeasible and ... 
		//presolvedModel->primal(); // or whatever else we wish to do 
		//std::cout << '\n' << presolvedModel->getIterationCount() << '\n';
		presolveInfo.postsolve(true);  // the true updates status arrays in original       
		/* If the presolved model was optimal then so should the
		   original be.
		   We can use checkSolution and test feasibility */
		//std::cout << "\n " << model_copy.getIterationCount() << '\n';
		model_copy.checkSolution();
		if (model_copy.numberDualInfeasibilities() ||
			model_copy.numberPrimalInfeasibilities())
			printf("!!!%g dual %g(%d) Primal %g(%d)\n",
				model_copy.objectiveValue(),
				model_copy.sumDualInfeasibilities(),
				model_copy.numberDualInfeasibilities(),
				model_copy.sumPrimalInfeasibilities(),
				model.numberPrimalInfeasibilities());
		// Due to tolerances we can not guarantee that so you may wish to throw in
		//model_copy.initialSolve(solvectl);
		//model_copy.setPrimalTolerance(0.1);
		model_copy.primal(1);
		//std::cout << "\n " << model_copy.getIterationCount() << '\n';
	}


	//MPS_Parser parser;
	//std::unique_ptr<Linear_Programming_Task> Abc =
	//	std::unique_ptr<Linear_Programming_Task>(parser.parse("netlib\\AFIRO.SIF"));//BLEND,OSA-60,CRE-B,AFIRO
	//Agmon_Motzkin task(std::move(Abc));
	//// x[i]>=0, i=1,...,n
	//for (size_t i = 1; i <= task.get_width(); ++i) {
	//	Vector_Sparse lim;
	//	lim.set(i, 1);
	//	lim.set_length(task.get_width());
	//	task.add_limitation(lim);
	//}
	//std::vector<double> x(task.get_width(), 10);
	//task.set_x(x);
	//constexpr double E =  0/*.00001*/;
	//constexpr size_t N = 10000000;
	//auto [count, distance] = iterate(N, E, task);
	//std::cout << "Count = " << count << "\n";
	//std::cout << "Distance = " << distance << "\n";
	//std::cout << "Biggest violation = " << task.get_biggest_violation() << " \n";

	/*for (auto x : task.get_x())
		std::cout << x << " ";*/

		//	// f(x) -> min
		//double k1 = 1000;
		//double k2 = 0;
		///*k1 = 580;
		//k2 = 570;*/
		///*k1 = 1500;
		//k2 = 1000;*/
		//double upper_bound = task.calculate_criterion();
		//std::cout << "Upper bound = " << upper_bound << "\n\n";
		//upper_bound = task.calculate_criterion();
		//auto criterion = task.get_criterion();
		//task.add_limitation(criterion, upper_bound - k1);

		//for (size_t i = 1; i <= criterion.get_length(); ++i) {
		//	auto current = criterion.get(i);
		//	if (current != 0.0)
		//		criterion.set(i, -current);
		//}
		//task.add_limitation(criterion, -upper_bound + k2);

		//do {
		//	auto [count, distance] = iterate(N, E, task);
		//	std::cout << "Count = " << count << "\n";
		//	std::cout << "Distance = " << distance << "\n";
		//	std::cout << "k1 = " << k1 << ", k2 = " << k2 << std::endl;
		//	std::cout << "Criterion = " << task.calculate_criterion() << '\n';
		//	auto biggest_violation = task.get_biggest_violation();
		//	std::cout << "Biggest violation = " << biggest_violation << " \n";
		//	if (distance <= E) {
		//		k2 += (k1 - k2) / 2;
		//	}
		//	else {
		//		k1 = k2;
		//		k2 = 0;
		//	}
		//	size_t row = task.get_height();
		//	task.set_b(row - 1, upper_bound - k1);
		//	task.set_b(row, -upper_bound + k2);
		//} while (k1 - k2 > 1);
		//std::ofstream os("best_results.txt");
		//if (os.is_open()) {
		//	auto [c, x] = task.get_best_results();
		//	os << c << '\n';
		//	for (auto& e : x)
		//		os << e << ' ';
		//}
		//os.close();
	system("pause");
	return 0;
}