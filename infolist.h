#ifndef TABLEINFO_H
#define TABLEINFO_H

const int TABLE_NAME_LEN = 28;
const int MAX_COLUMN_NUM = 64;

const int COLUMN_NAME_LEN = 32;
const int COLUMN_PROPERTY_LEN = 4;
const int COLUMN_KIND_LEN = 4;
const int COLUMN_LEN_LEN = 4;
const int INDEX_PLACE_LEN = 4;
//库页中
const int TABLE_NUM_LEN = 4;
const int TABLE_HEAD_PLACE_LEN = 4;
//表头页中
const int PAGE_RANK_LEN = 4;
//列信息
const int COLUMN_NUM_LEN = 4;
const int COLUMN_NAME_LEN = 32;
const int NOPAGE = 0;
//列数据类型
const int KIND_INT = 1;
const int KIND_CHAR = 2;
const int KIND_VARCHAR = 3;
//列属性
const int NONULLPLACE = 0;
const int INDEXPLACE = 1;
const int PRIMARYPLACE = 2;
//数据页
const int FIXEDCOLUMNNUM_LEN = 2;
const int FIXEDCOLUMNLEN_LEN = 2;
const int VARCOLUMNNUM_LEN = 2;
const int RECORDNUMLEN = 4;
const int RECORDPLACELEN = 2;
const int RECORDSIZELEN = 2;
const int DELETEDSIZE = 65535;
const int DELETEDPLACE = 65535;
const int FREEBOUNDLEN = 2;
struct ColumnInfo {
	char columnName[COLUMN_NAME_LEN];
	char columnProperty[COLUMN_PROPERTY_LEN];
	char columnKind[COLUMN_KIND_LEN];
	char columnLen[COLUMN_LEN_LEN];
	char indexPlace[INDEX_PLACE_LEN];
};

struct TableInfo {
	char tableName[TABLE_NAME_LEN];
	int columnNum;
	ColumnInfo columns[MAX_COLUMN_NUM];
};

#endif