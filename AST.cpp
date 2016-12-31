#include "AST.h"
#include <stdio.h>
#include <string.h>
using namespace std;
Expr* Expr::makeOpUnary(OperatorType op, Expr* expr) {
    Expr* e = new Expr(kExprOperator);
    e->op_type = op;
    e->expr = expr;
    e->expr2 = NULL;
    return e;
}
Expr* Expr::makeOpBinary(Expr* expr1, OperatorType op, Expr* expr2) {
    Expr* e = new Expr(kExprOperator);
    e->op_type = op;
    e->expr = expr1;
    e->expr2 = expr2;
    return e;
}
Expr* Expr::makeLiteral(int64_t val) {
    Expr* e = new Expr(kExprLiteralInt);
    e->ival = val;
    return e;
}
Expr* Expr::makeLiteral(char* string) {
    Expr* e = new Expr(kExprLiteralString);
    e->name = string;
    return e;
}
Expr* Expr::makeColumnRef(char* name) {
    Expr* e = new Expr(kExprColumnRef);
    e->name = name;
    return e;
}
Expr* Expr::makeColumnRef(char* table, char* name) {
    Expr* e = new Expr(kExprColumnRef);
    e->name = name;
    e->table = table;
    return e;
}
Expr* Expr::makeFunctionRef(char* func_name, Expr* expr, bool distinct) {
    Expr* e = new Expr(kExprFunctionRef);
    e->name = func_name;
    e->expr = expr;
    e->distinct = distinct;
    return e;
}
bool intComparator(std::vector<ResultEntry>& a, std::vector<ResultEntry>& b){
	return a[0].ival < b[0].ival;
}
bool stringComparator(std::vector<ResultEntry>& a, std::vector<ResultEntry>& b){
	return a[0].sval < b[0].sval;
}
static std::string ReplaceAll(std::string& str, const std::string& from, const std::string& to){
	size_t start_pos = 0;
	while((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); //Handles case where 'to' is a substring of 'from'
	}
	return str;
}
bool matchLike(std::string & p, std::string a){
	//replace other regex identifier
	vector<string> regexIds  = { "\\", "(", ")", "?", ":", "[", "]", "*", "+","^", "$", "|" };
	for (string regexId : regexIds)
	{
		ReplaceAll(p, regexId, "\\" + regexId);
	}
	// translate sql to regex
	ReplaceAll(p, "_", "(.)");
	ReplaceAll(p, "%", "(.)*");
	// find match
	return regex_match(a, regex(p));
}
bool validDate(string& a) {
	if (a.size() != 8) {
		return false;
	}
	int d[8];
	for (int i = 0; i < 8; i++) {
		if (a[i]<'0' || a[i]>'9') {
			return false;
		}
		d[i] = a[i] - '0';
	}
	int year = d[0] * 1000 + d[1] * 100 + d[2] * 10 + d[3];
	bool ss = false;
	if (year % 4 == 0) {
		if (year % 100 == 0) {
			if (year % 400 == 0) {
				ss = true;
			}
		}
		else {
			ss = true;
		}
	}
	int month = d[4] * 10 + d[5];
	int maxd[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
	if (month == 0 || month > 12) {
		return false;
	}
	int day = d[6] * 10 + d[7];
	if (day == 0) {
		return false;
	}
	if (month != 2) {
		if (day > maxd[month - 1]) {
			return false;
		}
	}
	if (month == 2) {
		if (ss) {
			if (day > 29) {
				return false;
			}
		}
		else {
			if (day > 28) {
				return false;
			}
		}
	}
	return true;
}
bool validDecimal(std::string& e) {
	for (int i = 0; i < e.size(); i++) {
		if (e[i]<'0' || e[i]>'9') {
			return false;
		}
	}
	return true;
}
checkParser cParse(std::string & e){
	return checkParser();
}
