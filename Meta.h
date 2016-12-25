#pragma once
#include<cstdint>
#include<string>
enum DataType {
	INT = 1, CHAR = 2, VARCHAR = 3, PRIMARY
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
	int32_t columnNumber;
	int32_t id;
	ColumnMeta columns[64];
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
	int getNo(std::string& n) {
		for (int i = 0; i < tableNumber; i++) {
			if (std::string(tables[i].name) == n) {
				return i;
			}
		}
		return -1;
	}
};
