#pragma once
const int TABLE_NAME_LEN = 28;
const int MAX_COLUMN_NUM = 64;

const int COLUMN_NAME_LEN = 32;
const int COLUMN_PROPERTY_LEN = 4;
const int COLUMN_KIND_LEN = 4;
const int COLUMN_LEN_LEN = 4;
const int COLUMN_INFO_TOTLEN = COLUMN_NAME_LEN + COLUMN_PROPERTY_LEN + COLUMN_KIND_LEN + COLUMN_LEN_LEN;
const int INDEX_PLACE_LEN = 4;
//库页中
const int TABLE_NUM_LEN = 4;
const int TABLE_HEAD_PLACE_LEN = 4;
//表头页中
const int PAGE_RANK_LEN = 4;
const int CHECK_LIMIT_PLACE = 3880;
const int CHECK_LIMIT_LEN = 100;
//列信息
const int COLUMN_NUM_LEN = 4;
//const int COLUMN_NAME_LEN = 32;
const int NOPAGE = 0;
//列数据类型
const int KIND_INT = 1;
const int KIND_CHAR = 2;
const int KIND_VARCHAR = 3;
const int KIND_DATE = 4;
const int KIND_DECIMAL = 5;
//列属性
const int NONULLPLACE = 0;
const int INDEXPLACE = 1;
const int PRIMARYPLACE = 2;
const int NOTNULL = 1;
const int ISINDEX = 1;
const int ISPRIMARY = 1;
const int CANNULL = 1 - NOTNULL;
const int NOTINDEX = 1 - ISINDEX;
const int NOTPRIMARY = 1 - ISPRIMARY;
//数据页
const int FIXEDCOLUMNNUM_LEN = 2;
const int FIXEDCOLUMNLEN_LEN = 2;
const int RECORDLENLEN = 4;
const int FIRSTSLOTLEN = 4; //(第一)有空槽号长度
const int RECORDPLACELEN = 2;
const int RECORDSIZELEN = 2;
const int DELETEDSIZE = 65535;
const int DELETEDPLACE = 65535;
const int NOSLOT = 0;
//const int FREEBOUNDLEN = 2;
//索引页
const int ISROOTLEN = 4;
const int ISROOT = 1;
const int NOTROOT = 0;
const int ISLEAFLEN = 4;
const int ISLEAF = 1;
const int NOTLEAF = 0;
const int NLEN = 4;
const int INDEXNUMLEN = 4;
const int INDEXKINDLEN = 4;
const int INDEXLENLEN = 4;
const int NOTLEAFPAGELEN = 8;
const int NOTLEAFPAGESPACE = NOTLEAFPAGELEN - PAGE_RANK_LEN;
const int LEAFPAGELEN = 4;
const int LEAFSLOTLEN = 4;
const int LEAFNEXTLEN = 8;
const int LEAFNEXTSPACE = 4;
const int IDLEN = 4;
const int NOPARENT = 0;
const int NORECORD = 0;
struct ColumnInfo {
	char columnName[COLUMN_NAME_LEN];
	char columnProperty[COLUMN_PROPERTY_LEN];
	char columnKind[COLUMN_KIND_LEN];
	char columnLen[COLUMN_LEN_LEN];
	char indexPlace[INDEX_PLACE_LEN];
};
struct TableInfomation {
	char tableName[TABLE_NAME_LEN];
	int columnNum;
	ColumnInfo columns[MAX_COLUMN_NUM];
	char checkLimit[100];
};
