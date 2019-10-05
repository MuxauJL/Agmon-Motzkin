#pragma once
#include "Linear_Programming_Task.h"
#include <string>

class Blend_Parser
{
public:
	Linear_Programming_Task* parse(std::string filename);
};

