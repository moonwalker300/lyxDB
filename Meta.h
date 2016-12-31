#pragma once
#include<cstdint>
#include<string>
enum DataType {
	DTINT = 1, DTCHAR = 2, DTVARCHAR = 3, DTDATE = 4, DTDECIMAL = 5, DTPRIMARY
};
class ColumnMeta {
public:
	char name[32];
	int32_t attribute;
	int32_t dataType;
	int32_t length;
	int32_t indexLocation;
};
class TableMeta {
public:
	int32_t pageNumber;
	char name[28];
	int32_t firstDataPage;
	int32_t firstBlankPage;
	int32_t id;
	int32_t columnNumber;
	ColumnMeta columns[64];
	char checkLimit[100];
	int getNo(std::string& n){
		for (int i = 0; i < columnNumber; i++) {
			if (std::string(columns[i].name) == n) {
				return i;
			}
		}
		return -1;
	}
};
class TableInfo {
public:
	int32_t location;
	char name[28];
};
class DatabaseMeta {
public:
	int32_t tableNumber;
	TableInfo tables[64];
	int32_t valid;
	int getNo(std::string& n) {
		for (int i = 0; i < tableNumber; i++) {
			if (std::string(tables[i].name) == n) {
				return i;
			}
		}
		return -1;
	}
};
