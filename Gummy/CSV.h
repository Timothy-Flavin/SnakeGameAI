#pragma once
#include<string>
struct csv {
public:
	char* name = NULL;
	std::string* data = NULL;
	int numLines = 0;
	int numVals = 0;
	double**numData = NULL;
};