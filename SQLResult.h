#pragma once
#include<string>
#include<vector>
#include<iostream>
class ResultEntry {
public:
	int64_t ival;
	std::string sval;
	bool nval;
	bool isInt;
	ResultEntry() :nval(true) {}
	ResultEntry(std::string s) :sval(s), nval(false),isInt(false) {}
	ResultEntry(int64_t i) :ival(i), nval(false),isInt(true) {}
	friend std::ostream& operator<<(std::ostream& os, ResultEntry& e) {
		if (e.nval) {
			os << "null\t";
		}
		else if (e.isInt) {
			os << e.ival<<"\t";
		}
		else {
			os << e.sval<<"\t";
		}
		return os;
	}
};
class TableHeader {
public:
	std::string name;
	bool isInt;
	TableHeader(std::string n, bool i) :name(n), isInt(i) {}
};
class SQLResult {
public:
	bool valid;
	std::string info;
	std::vector<TableHeader> tableHeader;
	std::vector<std::vector<ResultEntry>> table;
	void print() {
		if (valid) {
			std::cout << "ok" << std::endl;
		}
		else {
			std::cout << "error" << std::endl;
			std::cout << info << std::endl;
		}
		for (auto item : tableHeader) {
			std::cout << item.name << "(";
			if (item.isInt) {
				std::cout << "int";
			}
			else {
				std::cout << "string";
			}
			std::cout << ") ";
		}
		std::cout << std::endl;
		for (auto item : table) {
			for (auto j : item) {
				std::cout << j << " ";
			}
			std::cout << std::endl;
		}
	}
};