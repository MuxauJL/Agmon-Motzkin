#pragma once
#include <map>

class Vector_Sparse
{
private:
	// position from 1
	std::map<size_t, double> vector;
	size_t length;
public:
	Vector_Sparse() {
		length = 0;
	};
	Vector_Sparse(size_t n) {
		length = n;
	}
	~Vector_Sparse() = default;
	void set(size_t pos, double val) {
		if (pos > length)
			length = pos;
		vector[pos] = val;
	}
	double get(size_t pos) const {
		if (pos > length)
			vector.at(pos);
		else {
			auto it = vector.find(pos);
			if (it != vector.end())
				return it->second;
			else
				return 0.0;
		}
	}
	size_t get_length(){ return length; }
	void set_length(size_t l) { length = l; }
};

