#include "Blend_Parser.h"
#include <fstream>
#include <sstream>
#include <vector>

Linear_Programming_Task* Blend_Parser::parse(std::string filename)
{
	Linear_Programming_Task* Abc = new Linear_Programming_Task();
	std::fstream fs(filename);
	if (fs.is_open()) {
		std::string line;
		while (getline(fs, line)&&line!="ROWS");
		std::vector<size_t> E;
		std::vector<size_t> L;
		//std::vector<size_t> G;
		std::string C;
		while (fs>>line && line != "COLUMNS") {
			size_t row;
			if (line == "E") {
				fs >> row;
				E.emplace_back(row);
			}
			if (line == "L") {
				fs >> row;
				L.emplace_back(row);
			}
			if (line == "N") {
				fs >> C;
			}
		}
		auto is_contains = [](const std::vector<size_t>& v, size_t val) {
			for (auto e : v)
				if (e == val)
					return true;
			return false;
		};
		size_t col, row;
		double value;
		while (getline(fs,line) && line != "RHS") {
			if (line == "")
				continue;
			std::stringstream sstr(line);
			sstr >> line;
			col = std::stoi(line);
			while (sstr >> line) {
				if (line == "C") {
					sstr >> value;
					Abc->c->set(col, value);
				}
				else {
					row = std::stoi(line);
					sstr >> value;
					if (is_contains(E, row)) {
						Abc->A->set(row, col, value);
						/*Abc->A->set(row, col, -1 * value);*/
					}
					if (is_contains(L, row))
						Abc->A->set(row, col, -1 * value);
				}
			}
		}
		while (fs >> line && line != "ENDATA") {
			fs >> value;
			Abc->b->set(std::stoi(line), value);
		}
	}
	fs.close();
	Abc->b->set_length(Abc->A->get_height());
	Abc->c->set_length(Abc->A->get_width());
	return Abc;
}
