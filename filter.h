#pragma once
#include<string>
enum FilterType {
	IntFilter, StringFilter
};
enum FilterOperator {
	FEQ, FLE, FLT, FGE, FGT, FIsNull, FZERO
};
class Filter {
public:
	int columnNumber;
	FilterType type;
	FilterOperator op;
	int ival;
	std::string sval;
};