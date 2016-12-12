#ifndef TABLEINFO_H
#define TABLEINFO_H

const int TABLE_NAME_LEN = 32;
const int MAX_COLUMN_NUM = 64;

const int COLUMN_NAME_LEN = 32;
const int COLUMN_PROPERTY_LEN = 4;
const int COLUMN_KIND_LEN = 4;
const int COLUMN_LEN_LEN = 4;
const int INDEX_PLACE_LEN = 4;


struct ColumnInfo {
	char columnName[COLUMN_NAME_LEN];
	char columnProperty[COLUMN_PROPERTY_LEN];
	char columnKind[COLUMN_KIND_LEN];
	char columnLen[COLUMN_LEN_LEN];
	char indexPlace[INDEX_PLACE_LEN];
};

struct TableInfo {
	char tableName[TABLE_NAME_LEN];
	ColumnInfo columns[MAX_COLUMN_NUM];
};

#endif