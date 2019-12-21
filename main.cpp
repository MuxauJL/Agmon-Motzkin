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
	} while (count < N && d>E);
	return std::pair<size_t, double>(count, d);
}

int main() {
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
		rowLower.push_back(Abc->b->get(r));
		for (size_t c = 1; c <= width; ++c) {
			matrix.modifyCoefficient(r - 1, c - 1, A->get(r, c));
		}
	}
	std::vector<double> rowUpper(height, COIN_DBL_MAX);
	std::vector<double> colLower(width, 0.0);
	std::vector<double> colUpper(width, COIN_DBL_MAX);
	model.loadProblem(matrix, colLower.data(), colUpper.data(),
		objective.data(), rowLower.data(), rowUpper.data());
	model.setOptimizationDirection(1);

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
	constexpr double E = 0.00000001;
	constexpr size_t N = 1000000;
	auto [count, distance] = iterate(N, E, task);
	std::cout << "Count = " << count << "\n";
	std::cout << "Distance = " << distance << "\n";
	std::cout << "Biggest violation = " << task.get_biggest_violation() << " \n";
	double upper_bound = task.calculate_criterion();
	std::cout << "Upper bound = " << upper_bound << "\n\n";

	double k1 = 1000;
	double k2 = 400;

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
	} while (/*k1 - k2 > 1*/false);


	ClpSolve solvectl;
	solvectl.setSolveType(ClpSolve::usePrimal);
	solvectl.setPresolveType(ClpSolve::presolveOn);
	auto model_copy = model;
	auto clone = model;
	std::cout << "\nTOLERANCE:\n"
		<< "primal:" << clone.primalTolerance() << std::endl
		<< "dual:" << clone.dualTolerance() << std::endl
		<< "presolve:" << clone.presolveTolerance() << std::endl;
	std::cout << '\n' << "model:\n";
	model = clone;
	model.primal(1);
	std::cout << "Iterations count: " << model.getIterationCount() << '\n';
	auto solutionPrimal = model.primalColumnSolution();

	std::cout << '\n' << "model_copy:\n";
	model_copy = clone;
	model_copy.setColSolution(task.get_x().data());
	model_copy.checkSolution();
	model_copy.primal(1);

	std::cout << '\n' << "model_initial_solve:\n";
	model = clone;
	model.initialSolve(solvectl);
	solutionPrimal = model.primalColumnSolution();

	std::cout << '\n' << "model_copy_initial_solve:\n";
	model_copy = clone;
	model_copy.setColSolution(task.get_x().data());
	model_copy.checkSolution();
	model_copy.initialSolve(solvectl);

	std::cout << '\n' << "model_with_presolving:\n";
	model = clone;
	ClpPresolve presolveInfo;
	ClpSimplex* presolvedModel = presolveInfo.presolvedModel(model);
	if (presolvedModel)
		presolveInfo.postsolve(true);
	model.primal(1);
	std::cout << "Iterations count: " << model.getIterationCount() << '\n';
	auto solution = model.getColSolution();

	std::cout << '\n' << "model_copy_with_presolving:\n";
	model_copy = clone;
	model_copy.setColSolution(task.get_x().data());
	model_copy.checkSolution();
	presolveInfo;
	presolvedModel = presolveInfo.presolvedModel(model_copy);
	// at this point we have original model and a new model.  The  information
	// on the operations done is in presolveInfo 
	if (presolvedModel) {
		// was not found to be infeasible - so lets solve 
		// if presolvedModel was NULL then it was primal infeasible
		presolveInfo.postsolve(true);  // the true updates status arrays in original       
		/* If the presolved model was optimal then so should the
		   original be.
		   We can use checkSolution and test feasibility */
		model_copy.checkSolution();
		if (model_copy.numberDualInfeasibilities() ||
			model_copy.numberPrimalInfeasibilities())
			printf("%g dual %g(%d) Primal %g(%d)\n",
				model_copy.objectiveValue(),
				model_copy.sumDualInfeasibilities(),
				model_copy.numberDualInfeasibilities(),
				model_copy.sumPrimalInfeasibilities(),
				model.numberPrimalInfeasibilities());
		// Due to tolerances we can not guarantee that so you may wish to throw in
		model_copy.primal(1);
	}
	system("pause");
	return 0;
}