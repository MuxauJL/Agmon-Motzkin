#pragma once
#include "Linear_Programming_Task.h"
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>

class MPS_Parser
{
public:
	Linear_Programming_Task* parse(std::string filename) {
		Linear_Programming_Task* Abc = nullptr;
		std::fstream fs(filename);
		if (fs.is_open()) {
			Abc = new Linear_Programming_Task();
			std::map<std::string, size_t> rows;
			std::map<std::string, size_t> cols;
			size_t rows_count = 0;
			size_t cols_count = 0;
			std::string line;
			while (getline(fs, line) && line != "ROWS");
			std::vector<size_t> E; // have to create 2 equivalent rows of limitations
			std::vector<size_t> L; // have to *(-1) all
			std::vector<size_t> G;
			std::string C;
			while (fs >> line && line != "COLUMNS") {
				std::string row;
				fs >> row;
				if (line == "N") {
					C = row;
					continue;
				}
				auto it = rows.find(row);
				if (it == rows.end())
					rows[row] = ++rows_count;
				else
					throw std::exception("there are rows with the same name");
				if (line == "E") {
					E.emplace_back(rows_count);
					++rows_count;
				}
				if (line == "L") {
					L.emplace_back(rows_count);
				}
				if (line == "G")
					G.emplace_back(rows_count);
			}
			auto is_contains = [](const std::vector<size_t>& v, size_t val) {
				for (auto e : v)
					if (e == val)
						return true;
				return false;
			};
			size_t row, col;
			double value;
			while (getline(fs, line) && line != "RHS") {
				if (line == "")
					continue;
				std::stringstream sstr(line);
				sstr >> line;
				auto it = cols.find(line);
				if (it == cols.end())
					cols[line] = ++cols_count;
				col = cols[line];
				while (sstr >> line) {
					if (line == C) {
						sstr >> value;
						Abc->c->set(col, value);
					}
					else {
						auto it = rows.find(line);
						if (it != rows.end())
							row = it->second;
						else
							throw std::exception("there is not such row");
						sstr >> value;
						if (is_contains(E, row)) {
							Abc->A->set(row, col, value);
							Abc->A->set(row + 1, col, -1 * value);
						}
						if (is_contains(L, row))
							Abc->A->set(row, col, -1 * value);
						if (is_contains(G, row))
							Abc->A->set(row, col, value);
					}
				}
			}
			while (getline(fs, line) && line != "ENDATA") {
				if (line == "")
					continue;
				std::stringstream sstr(line);
				//sstr >> line; //name
				while (sstr >> line) {
					auto it = rows.find(line);
					if (it != rows.end())
						row = it->second;
					else
						throw std::exception("there is not such row");
					sstr >> value;
					if (is_contains(E, row)) {
						Abc->b->set(row, value);
						Abc->b->set(row + 1, -1 * value);
					}
					if (is_contains(L, row))
						Abc->b->set(row, -1 * value);
					if (is_contains(G, row))
						Abc->b->set(row, value);
				}
			}
			Abc->b->set_length(Abc->A->get_height());
			Abc->c->set_length(Abc->A->get_width());
		}
		fs.close();
		return Abc;
	}
};