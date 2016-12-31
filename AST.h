#pragma once
#include <stdlib.h>
#include <memory>
#include <vector>
#include <stdio.h>
#include <string>
#include <algorithm>
#include <set>
#include <map>
#include <regex>
#include "Meta.h"
#include "SQLResult.h"
#include "backend.h"
#include "filter.h"
extern bool intComparator(std::vector<ResultEntry>& a, std::vector<ResultEntry>& b);
extern bool stringComparator(std::vector<ResultEntry>& a, std::vector<ResultEntry>& b);
extern bool matchLike(std::string& p, std::string a);
extern bool validDate(std::string& a);
extern bool validDecimal(std::string& a);
class checkParser {
public:
	bool valid;
	std::string colName;
	std::vector<std::string> names;
};
extern checkParser cParse(std::string& e);
typedef enum {
	kExprLiteralString, kExprLiteralInt, kExprStar, kExprColumnRef, kExprFunctionRef, kExprOperator
} ExprType;
typedef enum {
	RTString, RTInt, RTBool, RTError, RTUnknown
} RunTimeType;
struct Expr {
	typedef enum {
		EQ, NE, LE, GE, LT, GT, LIKE, AND, ISNULL, ISNOTNULL
	} OperatorType;
	Expr(ExprType type) :type(type), expr(NULL), expr2(NULL), name(NULL), table(NULL), RTType(RTUnknown) {};
	~Expr() {
		delete expr;
		delete expr2;
		delete name;
		delete table;
	}
	ExprType type;
	Expr* expr;
	Expr* expr2;
	char* name;
	char* table;
	int64_t ival;
	OperatorType op_type;
	bool distinct;
	RunTimeType RTType;
	int tableNumber;
	int columnNumber;
	inline bool isType(ExprType e_type) {
		return e_type == type;
	}
	inline bool isLiteral() {
		return isType(kExprLiteralInt) || isType(kExprLiteralString);
	}
	inline bool hasTable() {
		return table != NULL;
	}
	inline char* getName() {
		return name;
	}
	static Expr* makeOpUnary(OperatorType op, Expr* expr);
	static Expr* makeOpBinary(Expr* expr1, OperatorType op, Expr* expr2);
	static Expr* makeLiteral(int64_t val);
	static Expr* makeLiteral(char* val);
	static Expr* makeColumnRef(char* name);
	static Expr* makeColumnRef(char* table, char* name);
	static Expr* makeFunctionRef(char* func_name, Expr* expr, bool distinct);
	void whereCheck(TableMeta& meta) {
		switch (type) {
		case kExprLiteralString:
			RTType = RTString;
			break;
		case kExprLiteralInt:
			RTType = RTInt;
			break;
		case kExprStar:
			RTType = RTError;
			break;
		case kExprColumnRef:
			if (table != NULL) {
				if (std::string(table) != std::string(meta.name)) {
					RTType = RTError;
				}
			}
			if (RTType != RTError) {
				int no = meta.getNo(std::string(name));
				if (no < 0) {
					RTType = RTError;
				}
				else {
					switch (meta.columns[no].dataType) {
					case DTINT:
						RTType = RTInt;
						break;
					case DTCHAR:
						RTType = RTString;
						break;
					case DTVARCHAR:
						RTType = RTString;
						break;
					case DTDATE:
						RTType = RTString;
						break;
					case DTDECIMAL:
						RTType = RTString;
						break;
					default:
						RTType = RTError;
						break;
					}
				}
			}
			break;
		case kExprFunctionRef:
			RTType = RTError;
			break;
		case kExprOperator:
			if (op_type == ISNOTNULL || op_type == ISNULL) {
				expr->whereCheck(meta);
				if ((expr->RTType == RTInt || expr->RTType == RTString) && (expr->type == kExprColumnRef)) {
					RTType = RTBool;
				}
				else {
					RTType = RTError;
				}
			}
			else if (op_type == LIKE) {
				expr->whereCheck(meta);
				expr2->whereCheck(meta);
				if (expr->type == kExprColumnRef&&expr->RTType == RTString&&expr2->RTType == RTString) {
					RTType = RTBool;
				}
				else {
					RTType = RTError;
				}
			}
			else if (op_type == AND) {
				expr->whereCheck(meta);
				expr2->whereCheck(meta);
				if (expr->RTType == RTBool&&expr2->RTType == RTBool) {
					RTType = RTBool;
				}
				else {
					RTType = RTError;
				}
			}
			else if (op_type == EQ || op_type == NE || op_type == GT || op_type == GE || op_type == LT || op_type == GT) {
				expr->whereCheck(meta);
				expr2->whereCheck(meta);
				if (expr->type == kExprColumnRef && ((expr->RTType == RTString&&expr2->RTType == RTString) || (expr->RTType == RTInt&&expr2->RTType == RTInt))) {
					RTType = RTBool;
				}
				else {
					RTType = RTError;
				}
			}
			else {
				RTType = RTError;
			}
			break;
		default:
			RTType = RTError;
			break;
		}
	}
	void whereCheck(TableMeta& meta1, TableMeta& meta2) {
		switch (type) {
		case kExprLiteralString:
			RTType = RTString;
			break;
		case kExprLiteralInt:
			RTType = RTInt;
			break;
		case kExprStar:
			RTType = RTError;
			break;
		case kExprColumnRef:
			if (table == NULL) {
				RTType = RTError;
			}
			if (RTType != RTError) {
				if (std::string(table) != std::string(meta1.name) && std::string(table) != std::string(meta2.name)) {
					RTType = RTError;
				}
				if (std::string(table) == std::string(meta1.name)) {
					tableNumber = 0;
				}
				else {
					tableNumber = 1;
				}
			}
			if (RTType != RTError) {
				columnNumber = -1;
				if (tableNumber == 0) {
					columnNumber = meta1.getNo(std::string(name));
				}
				else {
					columnNumber = meta2.getNo(std::string(name));
				}
				if (columnNumber < 0) {
					RTType = RTError;
				}
				else {
					if (tableNumber == 0) {
						switch (meta1.columns[columnNumber].dataType) {
						case DTINT:
							RTType = RTInt;
							break;
						case DTCHAR:
							RTType = RTString;
							break;
						case DTVARCHAR:
							RTType = RTString;
							break;
						case DTDATE:
							RTType = RTString;
							break;
						case DTDECIMAL:
							RTType = RTString;
							break;
						default:
							RTType = RTError;
							break;
						}
					}
					else {
						switch (meta2.columns[columnNumber].dataType) {
						case DTINT:
							RTType = RTInt;
							break;
						case DTCHAR:
							RTType = RTString;
							break;
						case DTVARCHAR:
							RTType = RTString;
							break;
						case DTDATE:
							RTType = RTString;
							break;
						case DTDECIMAL:
							RTType = RTString;
							break;
						default:
							RTType = RTError;
							break;
						}
					}
				}
			}
			break;
		case kExprFunctionRef:
			RTType = RTError;
			break;
		case kExprOperator:
			if (op_type == ISNOTNULL || op_type == ISNULL) {
				expr->whereCheck(meta1, meta2);
				if ((expr->RTType == RTInt || expr->RTType == RTString) && (expr->type == kExprColumnRef)) {
					RTType = RTBool;
				}
				else {
					RTType = RTError;
				}
			}
			else if (op_type == LIKE) {
				expr->whereCheck(meta1, meta2);
				expr2->whereCheck(meta1, meta2);
				if (expr->type == kExprColumnRef&&expr->RTType == RTString&&expr2->RTType == RTString) {
					RTType = RTBool;
				}
				else {
					RTType = RTError;
				}
			}
			else if (op_type == AND) {
				expr->whereCheck(meta1, meta2);
				expr2->whereCheck(meta1, meta2);
				if (expr->RTType == RTBool&&expr2->RTType == RTBool) {
					RTType = RTBool;
				}
				else {
					RTType = RTError;
				}
			}
			else if (op_type == EQ || op_type == NE || op_type == GT || op_type == GE || op_type == LT || op_type == GT) {
				expr->whereCheck(meta1, meta2);
				expr2->whereCheck(meta1, meta2);
				if (expr->type == kExprColumnRef && ((expr->RTType == RTString&&expr2->RTType == RTString) || (expr->RTType == RTInt&&expr2->RTType == RTInt))) {
					RTType = RTBool;
				}
				else {
					RTType = RTError;
				}
			}
			else {
				RTType = RTError;
			}
			break;
		default:
			RTType = RTError;
			break;
		}
	}
	void whereCheck(TableMeta& meta0, TableMeta& meta1, TableMeta& meta2) {
		switch (type) {
		case kExprLiteralString:
			RTType = RTString;
			break;
		case kExprLiteralInt:
			RTType = RTInt;
			break;
		case kExprStar:
			RTType = RTError;
			break;
		case kExprColumnRef:
			if (table == NULL) {
				RTType = RTError;
			}
			if (RTType != RTError) {
				if (std::string(table) != std::string(meta0.name) && std::string(table) != std::string(meta1.name) && std::string(table) != std::string(meta2.name)) {
					RTType = RTError;
				}
				if (std::string(table) == std::string(meta0.name)) {
					tableNumber = 0;
				}
				else if (std::string(table) == std::string(meta1.name)) {
					tableNumber = 1;
				}
				else {
					tableNumber = 2;
				}
			}
			if (RTType != RTError) {
				columnNumber = -1;
				if (tableNumber == 0) {
					columnNumber = meta0.getNo(std::string(name));
				}
				else if (tableNumber == 1) {
					columnNumber = meta1.getNo(std::string(name));
				}
				else {
					columnNumber = meta2.getNo(std::string(name));
				}
				if (columnNumber < 0) {
					RTType = RTError;
				}
				else {
					if (tableNumber == 0) {
						switch (meta0.columns[columnNumber].dataType) {
						case DTINT:
							RTType = RTInt;
							break;
						case DTCHAR:
							RTType = RTString;
							break;
						case DTVARCHAR:
							RTType = RTString;
							break;
						case DTDATE:
							RTType = RTString;
							break;
						case DTDECIMAL:
							RTType = RTString;
							break;
						default:
							RTType = RTError;
							break;
						}
					}
					else if (tableNumber == 1) {
						switch (meta1.columns[columnNumber].dataType) {
						case DTINT:
							RTType = RTInt;
							break;
						case DTCHAR:
							RTType = RTString;
							break;
						case DTVARCHAR:
							RTType = RTString;
							break;
						case DTDATE:
							RTType = RTString;
							break;
						case DTDECIMAL:
							RTType = RTString;
							break;
						default:
							RTType = RTError;
							break;
						}
					}
					else {
						switch (meta2.columns[columnNumber].dataType) {
						case DTINT:
							RTType = RTInt;
							break;
						case DTCHAR:
							RTType = RTString;
							break;
						case DTVARCHAR:
							RTType = RTString;
							break;
						case DTDATE:
							RTType = RTString;
							break;
						case DTDECIMAL:
							RTType = RTString;
							break;
						default:
							RTType = RTError;
							break;
						}
					}
				}
			}
			break;
		case kExprFunctionRef:
			RTType = RTError;
			break;
		case kExprOperator:
			if (op_type == ISNOTNULL || op_type == ISNULL) {
				expr->whereCheck(meta0, meta1, meta2);
				if ((expr->RTType == RTInt || expr->RTType == RTString) && (expr->type == kExprColumnRef)) {
					RTType = RTBool;
				}
				else {
					RTType = RTError;
				}
			}
			else if (op_type == LIKE) {
				expr->whereCheck(meta0, meta1, meta2);
				expr2->whereCheck(meta0, meta1, meta2);
				if (expr->type == kExprColumnRef&&expr->RTType == RTString&&expr2->RTType == RTString) {
					RTType = RTBool;
				}
				else {
					RTType = RTError;
				}
			}
			else if (op_type == AND) {
				expr->whereCheck(meta0, meta1, meta2);
				expr2->whereCheck(meta0, meta1, meta2);
				if (expr->RTType == RTBool&&expr2->RTType == RTBool) {
					RTType = RTBool;
				}
				else {
					RTType = RTError;
				}
			}
			else if (op_type == EQ || op_type == NE || op_type == GT || op_type == GE || op_type == LT || op_type == GT) {
				expr->whereCheck(meta0, meta1, meta2);
				expr2->whereCheck(meta0, meta1, meta2);
				if (expr->type == kExprColumnRef && ((expr->RTType == RTString&&expr2->RTType == RTString) || (expr->RTType == RTInt&&expr2->RTType == RTInt))) {
					RTType = RTBool;
				}
				else {
					RTType = RTError;
				}
			}
			else {
				RTType = RTError;
			}
			break;
		default:
			RTType = RTError;
			break;
		}
	}
	void whereSplit(std::vector<Expr*>& t) {
		if (type == kExprOperator&&op_type == AND) {
			expr->whereSplit(t);
			expr2->whereSplit(t);
		}
		else {
			t.push_back(this);
		}
	}
};
typedef enum {
	kStmtError, kStateCreateDatabase, kStateDropDatabase, kStateShowDatabases, kStateUse, kStateShowTables, kStateDesc,
	kStateCreateIndex, kStateDropIndex, kStmtSelect, kStmtInsert, kStmtUpdate, kStmtDelete, kStmtCreate, kStmtDrop
} StatementType;
struct SQLStatement {
	SQLStatement(StatementType type) : _type(type) {};
	virtual ~SQLStatement() {}
	virtual StatementType type() {
		return _type;
	}
	virtual SQLResult execute() = 0;
private:
	StatementType _type;
};
struct CreateDatabaseStatement : SQLStatement {
	CreateDatabaseStatement() :SQLStatement(kStateCreateDatabase), databaseName(NULL) {}
	~CreateDatabaseStatement() {
		delete databaseName;
	}
	virtual SQLResult execute() {
		std::string name(databaseName);
		SQLResult result;
		auto databaseList = be.showDatabases();
		for (auto item : databaseList) {
			if (item == name + ".lyx") {
				result.valid = false;
				result.info = "the database has existed.";
				return result;
			}
		}
		be.createDataBase(name);
		result.valid = true;
		return result;
	}
	char* databaseName;
};
struct DropDatabaseStatement : SQLStatement {
	DropDatabaseStatement() :SQLStatement(kStateDropDatabase), databaseName(NULL) {}
	~DropDatabaseStatement() {
		delete databaseName;
	}
	virtual SQLResult execute() {
		std::string name(databaseName);
		SQLResult result;
		auto databaseList = be.showDatabases();
		for (auto item : databaseList) {
			if (item == name + ".lyx") {
				be.dropDataBase(name);
				result.valid = true;
				return result;
			}
		}
		result.valid = false;
		result.info = "the database does not exist.";
		return result;
	}
	char* databaseName;
};
struct ShowDatabasesStatement : SQLStatement {
	ShowDatabasesStatement() : SQLStatement(kStateShowDatabases) {}
	~ShowDatabasesStatement() {}
	virtual SQLResult execute() {
		SQLResult result;
		result.valid = true;
		result.tableHeader.push_back(TableHeader(std::string("database"), false));
		auto databaseList = be.showDatabases();
		for (auto item : databaseList) {
			std::vector<ResultEntry> line;
			line.push_back(ResultEntry(item.substr(0, item.length() - 4)));
			result.table.push_back(line);
		}
		return result;
	}
};
struct UseStatement : SQLStatement {
	UseStatement() :SQLStatement(kStateUse), databaseName(NULL) {}
	~UseStatement() {
		delete databaseName;
	}
	virtual SQLResult execute() {
		std::string name(databaseName);
		SQLResult result;
		auto databaseList = be.showDatabases();
		for (auto item : databaseList) {
			if (item == name + ".lyx") {
				be.useDataBase(name);
				result.valid = true;
				return result;
			}
		}
		result.valid = false;
		result.info = "the database does not exist.";
		return result;
	}
	char* databaseName;
};
struct ShowTablesStatement : SQLStatement {
	ShowTablesStatement() : SQLStatement(kStateShowTables) {}
	~ShowTablesStatement() {}
	virtual SQLResult execute() {
		DatabaseMeta meta = be.getDataBaseMeta();
		if (!meta.valid) {
			SQLResult unvalid;
			unvalid.valid = false;
			unvalid.info = "please use a database";
			return unvalid;
		}
		SQLResult result;
		result.valid = true;
		result.tableHeader.push_back(TableHeader(std::string("tables"), false));
		for (int i = 0; i < meta.tableNumber; i++) {
			std::vector<ResultEntry> line;
			line.push_back(ResultEntry(meta.tables[i].name));
			result.table.push_back(line);
		}
		return result;
	}
};
struct DescStatement : SQLStatement {
	DescStatement() :SQLStatement(kStateDesc) {}
	~DescStatement() {
		delete tableName;
	}
	char* tableName;
	virtual SQLResult execute() {
		SQLResult result;
		DatabaseMeta meta = be.getDataBaseMeta();
		int no = meta.getNo(std::string(tableName));
		if (no < 0) {
			result.valid = false;
			result.info = "the table does not exist.";
			return result;
		}
		else {
			TableMeta tableMeta = be.getTableMeta(no);
			result.valid = true;
			result.tableHeader.push_back(TableHeader("name", false));
			result.tableHeader.push_back(TableHeader("notnull", false));
			result.tableHeader.push_back(TableHeader("primary", false));
			result.tableHeader.push_back(TableHeader("datatype", false));
			result.tableHeader.push_back(TableHeader("length", true));
			for (int i = 0; i < tableMeta.columnNumber; i++) {
				std::vector<ResultEntry> line;
				line.push_back(ResultEntry(std::string(tableMeta.columns[i].name)));
				if (tableMeta.columns[i].attribute & (1 << 24)) {
					line.push_back(ResultEntry(std::string("yes")));
				}
				else {
					line.push_back(ResultEntry(std::string("no")));
				}
				if (tableMeta.columns[i].attribute & (1 << 8)) {
					line.push_back(ResultEntry(std::string("yes")));
				}
				else {
					line.push_back(ResultEntry(std::string("no")));
				}
				switch (tableMeta.columns[i].dataType) {
				case 1:
					line.push_back(ResultEntry(std::string("int")));
					break;
				case 2:
					line.push_back(ResultEntry(std::string("char")));
					break;
				case 3:
					line.push_back(ResultEntry(std::string("varchar")));
					break;
				case 4:
					line.push_back(ResultEntry(std::string("date")));
					break;
				case 5:
					line.push_back(ResultEntry(std::string("decimal")));
					break;
				default:
					line.push_back(ResultEntry(std::string("?")));
					break;
				}
				line.push_back(ResultEntry(tableMeta.columns[i].length));
				result.table.push_back(line);
			}
		}
		return result;
	}
};
struct CreateIndexStatement : SQLStatement {
	CreateIndexStatement() :SQLStatement(kStateCreateIndex) {}
	~CreateIndexStatement() {
		delete tableName;
		delete columnName;
	}
	virtual SQLResult execute() {
		SQLResult result;
		DatabaseMeta meta = be.getDataBaseMeta();
		if (!meta.valid) {
			SQLResult unvalid;
			unvalid.valid = false;
			unvalid.info = "please use a database"; 
			return unvalid;
		}
		int no = meta.getNo(std::string(tableName));
		if (no < 0) {
			result.valid = false;
			result.info = "the table does not exist.";
			return result;
		}
		else {
			TableMeta tableMeta = be.getTableMeta(no);
			int tableNo = tableMeta.getNo(std::string(columnName));
			if (tableNo >= 0) {
				result.valid = true;
			}
			else {
				result.valid = false;
				result.info = "the column does not exist.";
			}
			return result;
		}
	}
	char* tableName;
	char* columnName;
};
struct DropIndexStatement : SQLStatement {
	DropIndexStatement() :SQLStatement(kStateDropIndex) {}
	~DropIndexStatement() {
		delete tableName;
		delete columnName;
	}
	virtual SQLResult execute() {
		SQLResult result;
		DatabaseMeta meta = be.getDataBaseMeta();
		if (!meta.valid) {
			SQLResult unvalid;
			unvalid.valid = false;
			unvalid.info = "please use a database";
			return unvalid;
		}
		int no = meta.getNo(std::string(tableName));
		if (no < 0) {
			result.valid = false;
			result.info = "the table does not exist.";
			return result;
		}
		else {
			TableMeta tableMeta = be.getTableMeta(no);
			int tableNo = tableMeta.getNo(std::string(columnName));
			if (tableNo >= 0) {
				result.valid = true;
			}
			else {
				result.valid = false;
				result.info = "the column does not exist.";
			}
			return result;
		}
	}
	char* tableName;
	char* columnName;
};
struct ColumnDefinition {
	ColumnDefinition(char* name, DataType type) : name(name), type(type) {}
	virtual ~ColumnDefinition() {
		delete name;
	}
	char* name;
	int64_t length;
	bool notnull;
	DataType type;
};
struct CreateStatement : SQLStatement {
	CreateStatement() : SQLStatement(kStmtCreate), tableName(NULL), columns(NULL) {};
	virtual ~CreateStatement() {
		delete columns;
		delete tableName;
	}
	const char* tableName;
	std::vector<ColumnDefinition*>* columns;
	virtual SQLResult execute() {
		SQLResult result;
		DatabaseMeta meta = be.getDataBaseMeta();
		if (!meta.valid) {
			SQLResult unvalid;
			unvalid.valid = false;
			unvalid.info = "please use a database";
			return unvalid;
		}
		int no = meta.getNo(std::string(tableName));
		if (no >= 0) {
			result.valid = false;
			result.info = "the table has existed.";
			return result;
		}
		TableMeta tableMeta;
		strcpy(tableMeta.name, tableName);
		std::string primaryName;
		if ((columns->operator[](columns->size() - 1))->type != DTPRIMARY) {
			primaryName = "__no__";
		}
		else {
			primaryName = std::string(columns->operator[](columns->size() - 1)->name);
			columns->pop_back();
		}
		int size = columns->size() + 1;
		strcpy(tableMeta.columns[0].name, "__no__");
		tableMeta.columns[0].dataType = (int32_t)DTINT;
		tableMeta.columns[0].length = 4;
		tableMeta.columns[0].attribute = 0;
		tableMeta.columnNumber = size;
		for (int i = 0; i < size - 1; i++) {
			strcpy(tableMeta.columns[i + 1].name, (columns->operator[](i))->name);
			DataType type = (columns->operator[](i))->type;
			if (type != DTINT && type != DTCHAR && type != DTVARCHAR && type != DTDATE && type != DTDECIMAL) {
				result.valid = false;
				result.info = "the type must be int, char or varchar.";
				return result;
			}
			tableMeta.columns[i + 1].dataType = (int32_t)type;
			if (type == DTINT) {
				tableMeta.columns[i + 1].length = 4;
			}
			else if (type == DTDATE) {
				tableMeta.columns[i + 1].length = 8;
			}
			else {
				tableMeta.columns[i + 1].length = (columns->operator[](i))->length;
			}
			tableMeta.columns[i + 1].attribute = ((columns->operator[](i))->notnull) ? (1 << 24) : 0;
		}
		for (int i = 0; i < size; i++) {
			if (std::string(tableMeta.columns[i].name) == primaryName) {
				tableMeta.columns[i].attribute |= (1 << 8);
			}
		}
		for (int i = 0; i < tableMeta.columnNumber; i++) {
			for (int j = 0; j < i; j++) {
				if (std::string(tableMeta.columns[i].name) == std::string(tableMeta.columns[j].name)) {
					result.valid = false;
					result.info = "the names of columns cannot equal";
					return result;
				}
			}
		}
		be.createTable(tableMeta);
		result.valid = true;
		return result;
	}
};
struct DropStatement : SQLStatement {
	DropStatement() : SQLStatement(kStmtDrop), name(NULL) {}
	virtual ~DropStatement() {
		delete name;
	}
	const char* name;
	virtual SQLResult execute() {
		SQLResult result;
		DatabaseMeta meta = be.getDataBaseMeta();
		if (!meta.valid) {
			SQLResult unvalid;
			unvalid.valid = false;
			unvalid.info = "please use a database";
			return unvalid;
		}
		int no = meta.getNo(std::string(name));
		if (no >= 0) {
			be.dropTable(no);
			result.valid = true;
			return result;
		}
		else {
			result.valid = false;
			result.info = "the table does not exist.";
			return result;
		}
	}
};
typedef enum {
	kTableName, kTableCrossProduct
} TableRefType;
struct TableRef {
	TableRef(TableRefType type) : type(type), name(NULL), list(NULL) {}
	virtual ~TableRef() {
		delete name;
		delete list;
	}
	TableRefType type;
	char* name;
	std::vector<TableRef*>* list;
	inline char* getName() {
		return name;
	}
};
struct InsertStatement : SQLStatement {
	InsertStatement() : SQLStatement(kStmtInsert), tableName(NULL), valuelists(NULL) {}
	virtual ~InsertStatement() {
		delete tableName;
		delete valuelists;
	}
	const char* tableName;
	std::vector<std::vector<Expr*>*>* valuelists;
	virtual SQLResult execute() {
		SQLResult result;
		DatabaseMeta meta = be.getDataBaseMeta();
		if (!meta.valid) {
			SQLResult unvalid;
			unvalid.valid = false;
			unvalid.info = "please use a database";
			return unvalid;
		}
		int no = meta.getNo(std::string(tableName));
		if (no < 0) {
			result.valid = false;
			result.info = "the table does not exist.";
			return result;
		}
		TableMeta tableMeta = be.getTableMeta(no);
		for (int k = 0; k < valuelists->size(); k++) {
			if (valuelists->operator[](k)->size() != tableMeta.columnNumber - 1) {
				result.valid = false;
				result.info = "the num of values does not match";
				return result;
			}
			for (int i = 0; i < tableMeta.columnNumber - 1; i++) {
				Expr* expr = valuelists->operator[](k)->operator[](i);
				if (expr == NULL && (tableMeta.columns[i + 1].attribute & (1 << 24))) {
					result.valid = false;
					result.info = "can not insert NULL to a not NULL column";
					return result;
				}
				if (expr != NULL) {
					ExprType exprType = expr->type;
					if (exprType != kExprLiteralString&&exprType != kExprLiteralInt) {
						result.valid = false;
						result.info = "can not insert an expr whose type is not string or int";
						return result;
					}
					if (exprType == kExprLiteralInt) {
						if (tableMeta.columns[i + 1].dataType != DTINT) {
							result.valid = false;
							result.info = "can not insert an int to a column whose type is not int";
							return result;
						}
					}
					if (exprType == kExprLiteralString) {
						if (tableMeta.columns[i + 1].dataType != DTCHAR && tableMeta.columns[i + 1].dataType != DTVARCHAR && tableMeta.columns[i + 1].dataType != DTDATE && tableMeta.columns[i + 1].dataType != DTDECIMAL) {
							result.valid = false;
							result.info = "can not isnert a string to a column whose type is not char or varchar";
							return result;
						}
						if (tableMeta.columns[i + 1].dataType == DTCHAR || tableMeta.columns[i + 1].dataType == DTVARCHAR) {
							int length = strlen(expr->name);
							if (length > tableMeta.columns[i + 1].length) {
								result.valid = false;
								result.info = "the string is too long";
								return result;
							}
						}
						if (tableMeta.columns[i + 1].dataType == DTDATE) {
							if (!validDate(std::string(expr->name))) {
								result.valid = false;
								result.info = "invalid date";
								return result;
							}
						}
						if (tableMeta.columns[i + 1].dataType == DTDECIMAL) {
							int length = strlen(expr->name);
							if (length > tableMeta.columns[i + 1].length) {
								result.valid = false;
								result.info = "the decimal number is too long";
								return result;
							}
							if (!validDecimal(std::string(expr->name))) {
								result.valid = false;
								result.info = "invalid decimal";
								return result;
							}
						}
					}
				}
			}
		}
		int mainLocation = -1;
		for (int i = 0; i < tableMeta.columnNumber; i++) {
			if (tableMeta.columns[i].attribute&(1 << 8)) {
				mainLocation = i;
				break;
			}
		}
		if (mainLocation > 0) {
			for (int k = 0; k < valuelists->size(); k++) {
				Filter f;
				f.columnNumber = mainLocation;
				if (tableMeta.columns[mainLocation].dataType == DTINT) {
					f.type = IntFilter;
					f.ival = valuelists->operator[](k)->operator[](mainLocation - 1)->ival;
				}
				else {
					f.type = StringFilter;
					f.sval = std::string(valuelists->operator[](k)->operator[](mainLocation - 1)->name);
				}
				f.op = FEQ;
				if (!(be.begin(no, f)).isFinished()) {
					result.valid = false;
					result.info = "the primary key has existed";
					return result;
				}
			}
			DataType primaryType = (DataType)tableMeta.columns[mainLocation].dataType;
			if (primaryType == DTINT) {
				std::vector<int> vs;
				for (int i = 0; i < valuelists->size(); i++) {
					vs.push_back(valuelists->operator[](i)->operator[](mainLocation - 1)->ival);
				}
				sort(vs.begin(), vs.end());
				auto it = unique(vs.begin(), vs.end());
				if (it != vs.end()) {
					result.valid = false;
					result.info = "the primary can not equal";
					return result;
				}
			}
			else {
				std::vector<std::string> vs;
				for (int i = 0; i < valuelists->size(); i++) {
					vs.push_back(std::string(valuelists->operator[](i)->operator[](mainLocation - 1)->name));
				}
				sort(vs.begin(), vs.end());
				auto it = unique(vs.begin(), vs.end());
				if (it != vs.end()) {
					result.valid = false;
					result.info = "the primary can not equal";
					return result;
				}
			}
		}
		std::vector<ContentEntry> line;
		for (int k = 0; k < valuelists->size(); k++) {
			line.push_back(ContentEntry(tableMeta.id));
			tableMeta.id++;
			for (int i = 0; i < tableMeta.columnNumber - 1; i++) {
				Expr* expr = valuelists->operator[](k)->operator[](i);
				if (tableMeta.columns[i + 1].dataType == (int32_t)DTINT) {
					if (expr != NULL) {
						line.push_back(ContentEntry(expr->ival));
					}
					else {
						line.push_back(ContentEntry());
					}
				}
				else if (tableMeta.columns[i + 1].dataType == (int32_t)DTCHAR || tableMeta.columns[i + 1].dataType == (int32_t)DTVARCHAR || tableMeta.columns[i+1].dataType == (int32_t)DTDATE || tableMeta.columns[i + 1].dataType == (int32_t)DTDECIMAL) {
					if (expr != NULL) {
						line.push_back(ContentEntry(std::string(expr->name)));
					}
					else {
						line.push_back(ContentEntry());
						std::cout << "null" << std::endl;
					}
				}
			}
			if (k == valuelists->size() - 1) {
				int a = 0;
			}
			be.insertRecord(no, line);
			line.clear();
		}
		result.valid = true;
		return result;
	}
};
struct DeleteStatement : SQLStatement {
	DeleteStatement() : SQLStatement(kStmtDelete), tableName(NULL), expr(NULL) {};
	virtual ~DeleteStatement() {
		delete tableName;
		delete expr;
	}
	char* tableName;
	Expr* expr;
	virtual SQLResult execute() {
		SQLResult result;
		DatabaseMeta meta = be.getDataBaseMeta();
		if (!meta.valid) {
			SQLResult unvalid;
			unvalid.valid = false;
			unvalid.info = "please use a database";
			return unvalid;
		}
		int no = meta.getNo(std::string(tableName));
		if (no < 0) {
			result.valid = false;
			result.info = "the table does not exist.";
			return result;
		}
		TableMeta tableMeta = be.getTableMeta(no);
		expr->whereCheck(tableMeta);
		if (expr->RTType != RTBool) {
			result.valid = false;
			result.info = "the where expr must be bool type";
		}
		std::vector<Expr*> conditions;
		expr->whereSplit(conditions);
		int iterno = -1;
		for (int i = 0; i < conditions.size(); i++) {
			if (conditions[i]->op_type == Expr::EQ) {
				iterno = i;
			}
		}
		if (iterno < 0) {
			for (int i = 0; i < conditions.size(); i++) {
				if (conditions[i]->op_type == Expr::LE || conditions[i]->op_type == Expr::LT || conditions[i]->op_type == Expr::GT || conditions[i]->op_type == Expr::GE || conditions[i]->op_type == Expr::ISNULL) {
					iterno = i;
				}
			}
		}
		Filter f;
		switch (conditions[iterno]->op_type) {
		case Expr::EQ:
			f.op = FEQ;
			break;
		case Expr::LE:
			f.op = FLE;
			break;
		case Expr::LT:
			f.op = FLT;
			break;
		case Expr::GT:
			f.op = FGT;
			break;
		case Expr::GE:
			f.op = FGE;
			break;
		case Expr::ISNULL:
			f.op = FIsNull;
			break;
		default:
			f.op = FZERO;
			break;
		}
		if (f.op != FIsNull) {
			if (conditions[iterno]->expr->RTType == RTInt) {
				f.type = IntFilter;
				f.ival = conditions[iterno]->expr2->ival;
			}
			else {
				f.type = StringFilter;
				f.sval = std::string(conditions[iterno]->expr2->name);
			}
		}
		else {
			if (conditions[iterno]->expr->RTType == RTInt) {
				f.type = IntFilter;
			}
			else {
				f.type = StringFilter;
			}
		}
		f.columnNumber = tableMeta.getNo(std::string(conditions[iterno]->expr->name));
		for (auto iter = be.begin(no, f); !iter.isFinished();) {
			bool match = true;
			for (auto item : conditions) {
				int columnNo = tableMeta.getNo(std::string(item->expr->name));
				if (item->expr->RTType != RTInt) {
					switch (item->op_type) {
					case Expr::EQ:
						if ((*iter)[columnNo].sval != std::string(item->expr2->name)) {
							match = false;
						}
						break;
					case Expr::NE:
						if ((*iter)[columnNo].sval == std::string(item->expr2->name)) {
							match = false;
						}
						break;
					case Expr::LE:
						if ((*iter)[columnNo].sval > std::string(item->expr2->name)) {
							match = false;
						}
						break;
					case Expr::GE:
						if ((*iter)[columnNo].sval < std::string(item->expr2->name)) {
							match = false;
						}
						break;
					case Expr::LT:
						if ((*iter)[columnNo].sval >= std::string(item->expr2->name)) {
							match = false;
						}
						break;
					case Expr::GT:
						if ((*iter)[columnNo].sval <= std::string(item->expr2->name)) {
							match = false;
						}
						break;
					case Expr::LIKE:
						if (!matchLike(std::string(item->expr2->name), (*iter)[columnNo].sval)) {
							match = false;
						}
						break;
					case Expr::ISNULL:
						if ((*iter)[columnNo].isNull == false) {
							match = false;
						}
						break;
					case Expr::ISNOTNULL:
						if ((*iter)[columnNo].isNull != false) {
							match = false;
						}
						break;
					}
				}
				else {
					switch (item->op_type) {
					case Expr::EQ:
						if ((*iter)[columnNo].ival != item->expr2->ival) {
							match = false;
						}
						break;
					case Expr::NE:
						if ((*iter)[columnNo].ival == item->expr2->ival) {
							match = false;
						}
						break;
					case Expr::LE:
						if ((*iter)[columnNo].ival > item->expr2->ival) {
							match = false;
						}
						break;
					case Expr::GE:
						if ((*iter)[columnNo].ival < item->expr2->ival) {
							match = false;
						}
						break;
					case Expr::LT:
						if ((*iter)[columnNo].ival >= item->expr2->ival) {
							match = false;
						}
						break;
					case Expr::GT:
						if ((*iter)[columnNo].ival <= item->expr2->ival) {
							match = false;
						}
						break;
					case Expr::LIKE:
						continue;
						break;
					case Expr::ISNULL:
						if ((*iter)[columnNo].isNull == false) {
							match = false;
						}
						break;
					case Expr::ISNOTNULL:
						if ((*iter)[columnNo].isNull != false) {
							match = false;
						}
						break;
					}
				}

			}
			if (match) {
				iter.Kill();
			}
			else {
				iter++;
			}
		}
		result.valid = true;
		return result;
	}
};
struct UpdateClause {
	char* column;
	Expr* value;
};
struct UpdateStatement : SQLStatement {
	UpdateStatement() : SQLStatement(kStmtUpdate), table(NULL), updates(NULL), where(NULL) {}
	virtual ~UpdateStatement() {
		delete table;
		delete updates;
		delete where;
	}
	TableRef* table;
	std::vector<UpdateClause*>* updates;
	Expr* where;
	virtual SQLResult execute() {
		SQLResult result;
		DatabaseMeta meta = be.getDataBaseMeta();
		if (!meta.valid) {
			SQLResult unvalid;
			unvalid.valid = false;
			unvalid.info = "please use a database";
			return unvalid;
		}
		int no = meta.getNo(std::string(table->name));
		if (no < 0) {
			result.valid = false;
			result.info = "the table does not exist.";
			return result;
		}
		TableMeta tableMeta = be.getTableMeta(no);
		where->whereCheck(tableMeta);
		if (where->RTType != RTBool) {
			result.valid = false;
			result.info = "the where expr must be bool type";
			return result;
		}
		std::vector<Expr*> conditions;
		where->whereSplit(conditions);
		for (int i = 0; i < updates->size(); i++) {
			std::string updateName = std::string(updates->operator[](i)->column);
			int upno = tableMeta.getNo(updateName);
			if (upno < 0) {
				result.valid = false;
				result.info = "the update column does not exist";
				return result;
			}
		}
		int iterno = -1;
		for (int i = 0; i < conditions.size(); i++) {
			if (conditions[i]->op_type == Expr::EQ) {
				iterno = i;
			}
		}
		if (iterno < 0) {
			for (int i = 0; i < conditions.size(); i++) {
				if (conditions[i]->op_type == Expr::LE || conditions[i]->op_type == Expr::LT || conditions[i]->op_type == Expr::GT || conditions[i]->op_type == Expr::GE || conditions[i]->op_type == Expr::ISNULL) {
					iterno = i;
				}
			}
		}
		for (int i = 0; i < updates->size(); i++) {
			UpdateClause* up = updates->operator[](i);
			int colno = tableMeta.getNo(std::string(up->column));
			if (tableMeta.columns[colno].dataType == (int32_t)DTDATE) {
				if (!validDate(std::string(up->value->name))) {
					result.valid = false;
					result.info = "the date is not valid";
					return result;
				}
			}
			if (tableMeta.columns[colno].dataType == (int32_t)DTDECIMAL) {
				if (!validDecimal(std::string(up->value->name))) {
					result.valid = false;
					result.info = "the decimal is not valid";
					return result;
				}
			}
		}
		Filter f;
		switch (conditions[iterno]->op_type) {
		case Expr::EQ:
			f.op = FEQ;
			break;
		case Expr::LE:
			f.op = FLE;
			break;
		case Expr::LT:
			f.op = FLT;
			break;
		case Expr::GT:
			f.op = FGT;
			break;
		case Expr::GE:
			f.op = FGE;
			break;
		case Expr::ISNULL:
			f.op = FIsNull;
			break;
		default:
			f.op = FZERO;
			break;
		}
		if (conditions[iterno]->expr->RTType == RTInt) {
			f.type = IntFilter;
			f.ival = conditions[iterno]->expr2->ival;
		}
		else {
			f.type = StringFilter;
			f.sval = std::string(conditions[iterno]->expr2->name);
		}
		f.columnNumber = tableMeta.getNo(std::string(conditions[iterno]->expr->name));
		auto iter = be.begin(no, f);
		for (; !iter.isFinished(); iter++) {
			bool match = true;
			for (auto item : conditions) {
				int columnNo = tableMeta.getNo(std::string(item->expr->name));
				if (item->expr->RTType != RTInt) {
					switch (item->op_type) {
					case Expr::EQ:
						if ((*iter)[columnNo].sval != std::string(item->expr2->name)) {
							match = false;
						}
						break;
					case Expr::NE:
						if ((*iter)[columnNo].sval == std::string(item->expr2->name)) {
							match = false;
						}
						break;
					case Expr::LE:
						if ((*iter)[columnNo].sval > std::string(item->expr2->name)) {
							match = false;
						}
						break;
					case Expr::GE:
						if ((*iter)[columnNo].sval < std::string(item->expr2->name)) {
							match = false;
						}
						break;
					case Expr::LT:
						if ((*iter)[columnNo].sval >= std::string(item->expr2->name)) {
							match = false;
						}
						break;
					case Expr::GT:
						if ((*iter)[columnNo].sval <= std::string(item->expr2->name)) {
							match = false;
						}
						break;
					case Expr::LIKE:
						if (!matchLike(std::string(item->expr2->name), (*iter)[columnNo].sval)) {
							match = false;
						}
						break;
					case Expr::ISNULL:
						if ((*iter)[columnNo].isNull == false) {
							match = false;
						}
						break;
					case Expr::ISNOTNULL:
						if ((*iter)[columnNo].isNull != false) {
							match = false;
						}
						break;
					}
				}
				else {
					switch (item->op_type) {
					case Expr::EQ:
						if ((*iter)[columnNo].ival != item->expr2->ival) {
							match = false;
						}
						break;
					case Expr::NE:
						if ((*iter)[columnNo].ival == item->expr2->ival) {
							match = false;
						}
						break;
					case Expr::LE:
						if ((*iter)[columnNo].ival > item->expr2->ival) {
							match = false;
						}
						break;
					case Expr::GE:
						if ((*iter)[columnNo].ival < item->expr2->ival) {
							match = false;
						}
						break;
					case Expr::LT:
						if ((*iter)[columnNo].ival >= item->expr2->ival) {
							match = false;
						}
						break;
					case Expr::GT:
						if ((*iter)[columnNo].ival <= item->expr2->ival) {
							match = false;
						}
						break;
					case Expr::LIKE:
						continue;
						break;
					case Expr::ISNULL:
						if ((*iter)[columnNo].isNull == false) {
							match = false;
						}
						break;
					case Expr::ISNOTNULL:
						if ((*iter)[columnNo].isNull != false) {
							match = false;
						}
						break;
					}
				}
			}
			if (match) {
				for (int i = 0; i < updates->size(); i++) {
					UpdateClause* up = updates->operator[](i);
					int colno = tableMeta.getNo(std::string(up->column));
					ContentEntry tmp = (*iter)[colno];
					if (up->value->type != kExprLiteralInt) {
						tmp.sval = std::string(up->value->name);
					}
					else {
						tmp.ival = up->value->ival;
					}
					iter.Update(colno, tmp);
				}
			}
		}
		iter.UpdateFlush();
		result.valid = true;
		return result;
	}
};
struct GroupByDescription {
	GroupByDescription() : columns(NULL), having(NULL) {}
	~GroupByDescription() {
		delete columns;
		delete having;
	}
	std::vector<Expr*>* columns;
	Expr* having;
};
enum SelectFunc {
	FSUM, FAVG, FMAX, FMIN
};
struct SelectStatement : SQLStatement {
	SelectStatement() : SQLStatement(kStmtSelect), fromTable(NULL), selectDistinct(false), selectList(NULL), whereClause(NULL), groupBy(NULL) {}
	virtual ~SelectStatement() {
		delete fromTable;
		delete selectList;
		delete whereClause;
		delete groupBy;
	}
	TableRef* fromTable;
	bool selectDistinct;
	std::vector<Expr*>* selectList;
	Expr* whereClause;
	GroupByDescription* groupBy;
	bool allShow;
	std::vector<std::string> headers;
	int accColumn;
	int groupColumn;
	std::string accName;
	std::string groupName;
	virtual SQLResult execute() {
		if (fromTable->type == kTableName) {
			return execute1();
		}
		if (fromTable->list->size() == 2) {
			return execute2();
		}
		if (fromTable->list->size() == 3) {
			return execute3();
		}
		SQLResult result;
		result.valid = false;
		result.info = "the table number should be 1, 2 or 3";
		return result;
	}
	SQLResult execute1() {
		SQLResult result;
		DatabaseMeta meta = be.getDataBaseMeta();
		if (!meta.valid) {
			SQLResult unvalid;
			unvalid.valid = false;
			unvalid.info = "please use a database";
			return unvalid;
		}
		int no = meta.getNo(std::string(fromTable->name));
		if (no < 0) {
			result.valid = false;
			result.info = "the table does not exist.";
			return result;
		}
		TableMeta tableMeta;
		tableMeta = be.getTableMeta(no);
		if (whereClause != NULL) {
			whereClause->whereCheck(tableMeta);
			if (whereClause->RTType != RTBool) {
				result.valid = false;
				result.info = "the where expr must be bool type";
				return result;
			}
		}
		accColumn = -1;
		SelectFunc func;
		if (selectList->size() == 1 && selectList->operator[](0)->type == kExprStar) {
			allShow = true;
		}
		else {
			allShow = false;
			for (int i = 0; i < selectList->size(); i++) {
				if (selectList->operator[](i)->type == kExprFunctionRef) {
					Expr* tmp = selectList->operator[](i);
					if (accColumn >= 0) {
						result.valid = false;
						result.info = "cannot use two function";
						return result;
					}
					accColumn = i;
					accName = std::string(tmp->expr->name);
					if (std::string(tmp->name) == std::string("SUM") || std::string(tmp->name) == std::string("sum")) {
						func = FSUM;
					}
					else if (std::string(tmp->name) == std::string("AVG") || std::string(tmp->name) == std::string("avg")) {
						func = FAVG;
					}
					else if (std::string(tmp->name) == std::string("MAX") || std::string(tmp->name) == std::string("max")) {
						func = FMAX;
					}
					else if (std::string(tmp->name) == std::string("MIN") || std::string(tmp->name) == std::string("min")) {
						func = FMIN;
					}
					else {
						result.valid = false;
						result.info = "unknown function type";
					}
				}
			}
			if (accColumn >= 0) {
				if (groupBy != NULL) {
					if (selectList->size() != 2) {
						result.valid = false;
						result.info = "when using function with group by statement, the size of the select list must be 2";
						return result;
					}
					std::vector<Expr*>* groupList = groupBy->columns;
					if (groupList->size() != 1) {
						result.valid = false;
						result.info = "we only support 1 group column.";
						return result;
					}
					groupName = std::string(groupList->operator[](0)->name);
					if (groupName == accName) {
						result.valid = false;
						result.info = "the groupName and the accName cannot equal.";
						return result;
					}
				}
				if (groupBy == NULL) {
					if (selectList->size() != 1) {
						result.valid = false;
						result.info = "when using function without group by statement, the size of the select list must be 1";
						return result;
					}
				}
			}
			for (int i = 0; i < selectList->size(); i++) {
				Expr* exp = selectList->operator[](i);
				if (exp->type != kExprColumnRef && exp->type != kExprFunctionRef) {
					result.valid = false;
					result.info = "the select list must be column ref or function";
					return result;
				}
				int tno = -1;
				if (exp->type == kExprColumnRef) {
					tno = tableMeta.getNo(std::string(exp->name));
				}
				else {
					tno = tableMeta.getNo(std::string(exp->expr->name));
				}
				if (tno < 0) {
					result.valid = false;
					result.info = "the select column ref does not exist";
					return result;
				}
				if (exp->type == kExprColumnRef) {
					headers.push_back(std::string(exp->name));
				}
				else {
					headers.push_back(std::string(exp->expr->name));
				}

			}
		}
		result.valid = true;
		for (int i = 0; i < tableMeta.columnNumber; i++) {
			result.tableHeader.push_back(TableHeader(std::string(tableMeta.columns[i].name), tableMeta.columns[i].dataType == (int32_t)DTINT));
		}
		std::vector<Expr*> conditions;
		int iterno = -1;
		if (whereClause != NULL) {
			whereClause->whereSplit(conditions);
			for (int i = 0; i < conditions.size(); i++) {
				if (conditions[i]->op_type == Expr::EQ) {
					iterno = i;
				}
			}
			if (iterno < 0) {
				for (int i = 0; i < conditions.size(); i++) {
					if (conditions[i]->op_type == Expr::LE || conditions[i]->op_type == Expr::LT || conditions[i]->op_type == Expr::GT || conditions[i]->op_type == Expr::GE || conditions[i]->op_type == Expr::ISNULL) {
						iterno = i;
					}
				}
			}
		}
		Filter f;
		if (iterno < 0) {
			f.op = FZERO;
			f.type = IntFilter;
			f.ival = 0;
			f.columnNumber = 0;
		}
		else {
			switch (conditions[iterno]->op_type) {
			case Expr::EQ:
				f.op = FEQ;
				break;
			case Expr::LE:
				f.op = FLE;
				break;
			case Expr::LT:
				f.op = FLT;
				break;
			case Expr::GT:
				f.op = FGT;
				break;
			case Expr::GE:
				f.op = FGE;
				break;
			case Expr::ISNULL:
				f.op = FIsNull;
				break;
			default:
				f.op = FZERO;
				break;
			}
			if (f.op == FEQ || f.op == FLE || f.op == FLT || f.op == FGT || f.op == FGE) {
				if (conditions[iterno]->expr->RTType == RTInt) {
					f.type = IntFilter;
					f.ival = conditions[iterno]->expr2->ival;
				}
				else {
					f.type = StringFilter;
					f.sval = std::string(conditions[iterno]->expr2->name);
				}
			}
			if (f.op == FIsNull) {
				if (conditions[iterno]->expr->RTType == RTInt) {
					f.type = IntFilter;
				}
				else {
					f.type = StringFilter;
				}
			}
			f.columnNumber = tableMeta.getNo(std::string(conditions[iterno]->expr->name));
		}
		for (auto iter = be.begin(no, f); !iter.isFinished(); iter++) {
			bool match = true;
			for (auto item : conditions) {
				int columnNo = tableMeta.getNo(std::string(item->expr->name));
				if (item->expr->RTType == RTString) {
					switch (item->op_type) {
					case Expr::EQ:
						if ((*iter)[columnNo].sval != std::string(item->expr2->name)) {
							match = false;
						}
						break;
					case Expr::NE:
						if ((*iter)[columnNo].sval == std::string(item->expr2->name)) {
							match = false;
						}
						break;
					case Expr::LE:
						if ((*iter)[columnNo].sval > std::string(item->expr2->name)) {
							match = false;
						}
						break;
					case Expr::GE:
						if ((*iter)[columnNo].sval < std::string(item->expr2->name)) {
							match = false;
						}
						break;
					case Expr::LT:
						if ((*iter)[columnNo].sval >= std::string(item->expr2->name)) {
							match = false;
						}
						break;
					case Expr::GT:
						if ((*iter)[columnNo].sval <= std::string(item->expr2->name)) {
							match = false;
						}
						break;
					case Expr::LIKE:
						if (!matchLike(std::string(item->expr2->name), (*iter)[columnNo].sval)) {
							match = false;
						}
						break;
					case Expr::ISNULL:
						if ((*iter)[columnNo].isNull == false) {
							match = false;
						}
						break;
					case Expr::ISNOTNULL:
						if ((*iter)[columnNo].isNull != false) {
							match = false;
						}
						break;
					}
				}
				else {
					switch (item->op_type) {
					case Expr::EQ:
						if ((*iter)[columnNo].ival != item->expr2->ival) {
							match = false;
						}
						break;
					case Expr::NE:
						if ((*iter)[columnNo].ival == item->expr2->ival) {
							match = false;
						}
						break;
					case Expr::LE:
						if ((*iter)[columnNo].ival > item->expr2->ival) {
							match = false;
						}
						break;
					case Expr::GE:
						if ((*iter)[columnNo].ival < item->expr2->ival) {
							match = false;
						}
						break;
					case Expr::LT:
						if ((*iter)[columnNo].ival >= item->expr2->ival) {
							match = false;
						}
						break;
					case Expr::GT:
						if ((*iter)[columnNo].ival <= item->expr2->ival) {
							match = false;
						}
						break;
					case Expr::LIKE:
						continue;
						break;
					case Expr::ISNULL:
						if ((*iter)[columnNo].isNull == false) {
							match = false;
						}
						break;
					case Expr::ISNOTNULL:
						if ((*iter)[columnNo].isNull != false) {
							match = false;
						}
						break;
					}
				}
			}
			if (match) {
				std::vector<ContentEntry> line = *iter;
				std::vector<ResultEntry> t;
				for (auto item : line) {
					if (item.isNull) {
						t.push_back(ResultEntry());
					}
					else {
						if (item.isString) {
							t.push_back(ResultEntry(item.sval));
						}
						else {
							t.push_back(ResultEntry(item.ival));
						}
					}
				}
				result.table.push_back(t);
			}
		}
		SQLResult result0;
		result0.valid = result.valid;
		result0.info = result.info;
		std::vector<int> retIndex;
		if (allShow) {
			for (int i = 0; i < result.tableHeader.size(); i++) {
				if (result.tableHeader[i].name != std::string("__no__")) {
					retIndex.push_back(i);
				}
			}
		}
		else {
			for (auto& item : headers) {
				for (int i = 0; i < result.tableHeader.size(); i++) {
					if (result.tableHeader[i].name == item) {
						retIndex.push_back(i);
					}
				}
			}
		}
		for (auto i : retIndex) {
			result0.tableHeader.push_back(result.tableHeader[i]);
		}
		for (auto& e : result.table) {
			std::vector<ResultEntry> ee;
			for (auto i : retIndex) {
				ee.push_back(e[i]);
			}
			result0.table.push_back(ee);
		}
		if (accColumn >= 0) {
			SQLResult tmp = result0;
			result0 = SQLResult();
			result0.valid = true;
			result0.tableHeader = tmp.tableHeader;
			if (groupBy != NULL) {
				if (accName != tmp.tableHeader[0].name && accName != tmp.tableHeader[1].name) {
					SQLResult re;
					re.valid = false;
					re.info = "the column ref of the function does not exist.";
					return re;
				}
				if (groupName != tmp.tableHeader[0].name && groupName != tmp.tableHeader[1].name) {
					SQLResult re;
					re.valid = false;
					re.info = "the column ref of the group by statement does not exist.";
					return re;
				}
				int nacc = -1;
				if (accName == tmp.tableHeader[0].name) {
					std::swap(tmp.tableHeader[0], tmp.tableHeader[1]);
					std::swap(result0.tableHeader[0], result0.tableHeader[1]);
					for (auto& item : tmp.table) {
						std::swap(item[0], item[1]);
					}
				}
				if (result0.tableHeader[0].isInt) {
					sort(tmp.table.begin(), tmp.table.end(), intComparator);
					bool isFirst = true;
					int count = 0;
					for (int i = 0; i < tmp.table.size(); i++) {
						if (isFirst) {
							result0.table.push_back(tmp.table[i]);
							isFirst = false;
							count++;
						}
						else {
							if (tmp.table[i][0].ival == result0.table[result0.table.size() - 1][0].ival) {
								switch (func) {
								case FSUM:
									result0.table[result0.table.size() - 1][1].ival += tmp.table[i][1].ival;
									break;
								case FAVG:
									result0.table[result0.table.size() - 1][1].ival += tmp.table[i][1].ival;
									break;
								case FMAX:
									if (result0.table[result0.table.size() - 1][i].ival < tmp.table[i][1].ival) {
										result0.table[result0.table.size() - 1][i].ival = tmp.table[i][1].ival;
									}
									break;
								case FMIN:
									if (result0.table[result0.table.size() - 1][i].ival > tmp.table[i][1].ival) {
										result0.table[result0.table.size() - 1][i].ival = tmp.table[i][1].ival;
									}
									break;
								}
								count++;
							}
							else {
								if (func == FAVG) {
									result0.table[result0.table.size() - 1][1].ival /= count;
								}
								result0.table.push_back(tmp.table[i]);
								count = 1;
							}
						}
					}
					if (func == FAVG) {
						result0.table[result0.table.size() - 1][1].ival /= count;
					}
				}
				else {
					sort(result0.table.begin(), result0.table.end(), stringComparator);
					bool isFirst = true;
					int count = 0;
					for (int i = 0; i < tmp.table.size(); i++) {
						if (isFirst) {
							result0.table.push_back(tmp.table[i]);
							isFirst = false;
							count++;
						}
						else {
							if (tmp.table[i][0].sval == result0.table[result0.table.size() - 1][0].sval) {
								switch (func) {
								case FSUM:
									result0.table[result0.table.size() - 1][1].ival += tmp.table[i][1].ival;
									break;
								case FAVG:
									result0.table[result0.table.size() - 1][1].ival += tmp.table[i][1].ival;
									break;
								case FMAX:
									if (result0.table[result0.table.size() - 1][i].ival < tmp.table[i][1].ival) {
										result0.table[result0.table.size() - 1][i].ival = tmp.table[i][1].ival;
									}
									break;
								case FMIN:
									if (result0.table[result0.table.size() - 1][i].ival > tmp.table[i][1].ival) {
										result0.table[result0.table.size() - 1][i].ival = tmp.table[i][1].ival;
									}
									break;
								}
								count++;
							}
							else {
								if (func == FAVG) {
									result0.table[result0.table.size() - 1][1].ival /= count;
								}
								result0.table.push_back(tmp.table[i]);
								count = 1;
							}
						}
					}
					if (func == FAVG) {
						result0.table[result0.table.size() - 1][1].ival /= count;
					}
				}
			}
			else {
				if (accName != tmp.tableHeader[0].name) {
					SQLResult re;
					re.valid = false;
					re.info = "the column ref of the function does not exist.";
					return re;
				}
				int kr = 0;
				if (tmp.tableHeader[0].isInt) {
					if (func == FMIN || func == FMAX) {
						kr = tmp.table[0][0].ival;
					}
					switch (func) {
					case FSUM:
						for (int i = 0; i < tmp.table.size(); i++) {
							if (tmp.table[i][0].nval == false) {
								kr += tmp.table[i][0].ival;
							}
						}
						break;
					case FAVG:
						for (int i = 0; i < tmp.table.size(); i++) {
							if (tmp.table[i][0].nval == false) {
								kr += tmp.table[i][0].ival;
							}
						}
						kr /= tmp.table.size();
						break;
					case FMAX:
						for (int i = 0; i < tmp.table.size(); i++) {
							if (tmp.table[i][0].nval == false) {
								if (kr < tmp.table[i][0].ival) {
									kr = tmp.table[i][0].ival;
								}

							}
						}
						break;
					case FMIN:
						for (int i = 0; i < tmp.table.size(); i++) {
							if (tmp.table[i][0].nval == false) {
								if (kr > tmp.table[i][0].ival) {
									kr = tmp.table[i][0].ival;
								}
							}
						}
						break;
					}
					std::vector<ResultEntry> line;
					line.push_back(ResultEntry(kr));
					result0.table.push_back(line);
				}
				else {
					SQLResult re;
					re.valid = false;
					re.info = "the function cannot be used for string.";
					return re;
				}
			}
		}
		return result0;
	}
	SQLResult execute2() {
		SQLResult result;
		DatabaseMeta meta = be.getDataBaseMeta();
		if (!meta.valid) {
			SQLResult unvalid;
			unvalid.valid = false;
			unvalid.info = "please use a database";
			return unvalid;
		}
		std::vector<int> nos;
		std::vector<TableRef*>* refs = fromTable->list;
		for (int i = 0; i < refs->size(); i++) {
			TableRef* tablePtr = refs->operator[](i);
			int no = meta.getNo(std::string(tablePtr->name));
			if (no < 0) {
				result.valid = false;
				result.info = "the table does not exist.";
				return result;
			}
			nos.push_back(no);
		}
		std::set<int> set1(nos.begin(), nos.end());
		if (set1.size() < nos.size()) {
			result.valid = false;
			result.info = "the table name cannot equal.";
			return result;
		}
		if (whereClause == NULL) {
			result.valid = false;
			result.info = "we does not support select on multiple tables without where clause";
			return result;
		}
		TableMeta tableMeta[2];
		tableMeta[0] = be.getTableMeta(nos[0]);
		tableMeta[1] = be.getTableMeta(nos[1]);
		whereClause->whereCheck(tableMeta[0], tableMeta[1]);
		if (whereClause->RTType != RTBool) {
			result.valid = false;
			result.info = "the where expr must be bool type";
			return result;
		}
		result.valid = true;
		for (int i = 0; i < tableMeta[0].columnNumber; i++) {
			result.tableHeader.push_back(TableHeader(std::string(tableMeta[0].name) + "." + std::string(tableMeta[0].columns[i].name), tableMeta[0].columns[i].dataType == (int32_t)DTINT));
		}
		for (int i = 0; i < tableMeta[1].columnNumber; i++) {
			result.tableHeader.push_back(TableHeader(std::string(tableMeta[1].name) + "." + std::string(tableMeta[1].columns[i].name), tableMeta[1].columns[i].dataType == (int32_t)DTINT));
		}
		std::vector<Expr*> conditions;
		whereClause->whereSplit(conditions);
		if (selectList->size() == 1 && selectList->operator[](0)->type == kExprStar) {
			allShow = true;
		}
		else {
			allShow = false;
			for (int i = 0; i < selectList->size(); i++) {
				Expr* exp = selectList->operator[](i);
				if (exp->type != kExprColumnRef) {
					result.valid = false;
					result.info = "the select list must be column ref";
					return result;
				}
				int cno = -1;
				if (std::string(tableMeta[0].name) == std::string(exp->table)) {
					cno = 0;
				}
				else if (std::string(tableMeta[1].name) == std::string(exp->table)) {
					cno = 1;
				}
				else {
					result.valid = false;
					result.info = "the select list's column ref does not exist.";
					return result;
				}
				int tno = tableMeta[cno].getNo(std::string(exp->name));
				if (tno < 0) {
					result.valid = false;
					result.info = "the select column ref does not exist";
					return result;
				}
				headers.push_back(std::string(exp->table) + "." + std::string(exp->name));
				std::cout << "push" << std::endl;
			}
		}
		int iterno = -1;
		int level = 0;
		for (int i = 0; i < conditions.size(); i++) {
			if (conditions[i]->op_type == Expr::EQ && conditions[i]->expr2->type != kExprColumnRef) {
				iterno = i;
				level = 3;
			}
		}
		if (iterno < 0) {
			for (int i = 0; i < conditions.size(); i++) {
				if (conditions[i]->op_type == Expr::EQ && conditions[i]->expr->tableNumber != conditions[i]->expr2->tableNumber) {
					iterno = i;
					level = 2;
				}
			}
		}
		if (iterno < 0) {
			for (int i = 0; i < conditions.size(); i++) {
				if (conditions[i]->op_type == Expr::LE || conditions[i]->op_type == Expr::LT || conditions[i]->op_type == Expr::GT || conditions[i]->op_type == Expr::GE || conditions[i]->op_type == Expr::ISNULL) {
					if (conditions[i]->expr2->type != kExprColumnRef) {
						iterno = i;
						level = 1;
					}
				}
			}
		}
		Filter f[2];
		if (level != 2) {
			int activeTable = -1;
			if (level == 0) {
				f[0].op = FZERO;
				f[0].columnNumber = 0;
				f[0].ival = 0;
				f[0].type = IntFilter;
				activeTable = 0;
			}
			if (level == 1 || level == 3) {
				switch (conditions[iterno]->op_type) {
				case Expr::EQ:
					f[0].op = FEQ;
					break;
				case Expr::LE:
					f[0].op = FLE;
					break;
				case Expr::LT:
					f[0].op = FLT;
					break;
				case Expr::GT:
					f[0].op = FGT;
					break;
				case Expr::GE:
					f[0].op = FGE;
					break;
				case Expr::ISNULL:
					f[0].op = FIsNull;
					break;
				default:
					f[0].op = FZERO;
					break;
				}
				if (f[0].op == FEQ || f[0].op == FLE || f[0].op == FLT || f[0].op == FGT || f[0].op == FGE) {
					if (conditions[iterno]->expr->RTType == RTInt) {
						f[0].type = IntFilter;
						f[0].ival = conditions[iterno]->expr2->ival;
					}
					else {
						f[0].type = StringFilter;
						f[0].sval = std::string(conditions[iterno]->expr2->name);
					}
				}
				if (f[0].op == FIsNull) {
					if (conditions[iterno]->expr->RTType == RTInt) {
						f[0].type = IntFilter;
					}
					else {
						f[0].type = StringFilter;
					}
				}
				activeTable = conditions[iterno]->expr->tableNumber;
				f[0].columnNumber = conditions[iterno]->expr->columnNumber;
			}
			f[1].op = FZERO;
			f[1].columnNumber = 0;
			f[1].ival = 0;
			f[1].type = IntFilter;
			for (auto iter = be.begin(nos[activeTable], f[0]); !iter.isFinished(); iter++) {
				for (auto iter1 = be.begin(nos[1 - activeTable], f[1]); !iter1.isFinished(); iter1++) {
					bool match = true;
					for (auto item : conditions) {
						if (item->expr->RTType == RTString) {
							ContentEntry leftValue;
							ContentEntry rightValue;
							if (activeTable == item->expr->tableNumber) {
								leftValue = (*iter)[item->expr->columnNumber];
							}
							else {
								leftValue = (*iter1)[item->expr->columnNumber];
							}
							if (item->expr2->type == kExprColumnRef) {
								if (activeTable == item->expr2->tableNumber) {
									rightValue = (*iter)[item->expr2->columnNumber];
								}
								else {
									rightValue = (*iter1)[item->expr2->columnNumber];
								}
							}
							else {
								rightValue = ContentEntry(item->expr2->name);
							}
							switch (item->op_type) {
							case Expr::EQ:
								if (leftValue.sval != rightValue.sval) {
									match = false;
								}
								break;
							case Expr::NE:
								if (leftValue.sval == rightValue.sval) {
									match = false;
								}
								break;
							case Expr::LE:
								if (leftValue.sval > rightValue.sval) {
									match = false;
								}
								break;
							case Expr::GE:
								if (leftValue.sval < rightValue.sval) {
									match = false;
								}
								break;
							case Expr::LT:
								if (leftValue.sval >= rightValue.sval) {
									match = false;
								}
								break;
							case Expr::GT:
								if (leftValue.sval <= rightValue.sval) {
									match = false;
								}
								break;
							case Expr::LIKE:
								if (!matchLike(rightValue.sval, leftValue.sval)) {
									match = false;
								}
								break;
							case Expr::ISNULL:
								if (leftValue.isNull == false) {
									match = false;
								}
								break;
							case Expr::ISNOTNULL:
								if (leftValue.isNull != false) {
									match = false;
								}
								break;
							}
						}
						else {
							ContentEntry leftValue;
							ContentEntry rightValue;
							if (activeTable == item->expr->tableNumber) {
								leftValue = (*iter)[item->expr->columnNumber];
							}
							else {
								leftValue = (*iter1)[item->expr->columnNumber];
							}
							if (item->expr2->type == kExprColumnRef) {
								if (activeTable == item->expr2->tableNumber) {
									rightValue = (*iter)[item->expr2->columnNumber];
								}
								else {
									rightValue = (*iter1)[item->expr2->columnNumber];
								}
							}
							else {
								rightValue = ContentEntry(item->expr2->ival);
							}
							switch (item->op_type) {
							case Expr::EQ:
								if (leftValue.ival != rightValue.ival) {
									match = false;
								}
								break;
							case Expr::NE:
								if (leftValue.ival == rightValue.ival) {
									match = false;
								}
								break;
							case Expr::LE:
								if (leftValue.ival > rightValue.ival) {
									match = false;
								}
								break;
							case Expr::GE:
								if (leftValue.ival < rightValue.ival) {
									match = false;
								}
								break;
							case Expr::LT:
								if (leftValue.ival >= rightValue.ival) {
									match = false;
								}
								break;
							case Expr::GT:
								if (leftValue.ival <= rightValue.ival) {
									match = false;
								}
								break;
							case Expr::LIKE:
								if (!matchLike(rightValue.sval, leftValue.sval)) {
									match = false;
								}
								break;
							case Expr::ISNULL:
								if (leftValue.isNull == false) {
									match = false;
								}
								break;
							case Expr::ISNOTNULL:
								if (leftValue.isNull != false) {
									match = false;
								}
								break;
							}
						}
					}
					if (match) {
						std::vector<ResultEntry> t;
						if (activeTable == 0) {
							for (auto item : *iter) {
								if (item.isNull) {
									t.push_back(ResultEntry());
								}
								else {
									if (item.isString) {
										t.push_back(ResultEntry(item.sval));
									}
									else {
										t.push_back(ResultEntry(item.ival));
									}
								}
							}
							for (auto item : *iter1) {
								if (item.isNull) {
									t.push_back(ResultEntry());
								}
								else {
									if (item.isString) {
										t.push_back(ResultEntry(item.sval));
									}
									else {
										t.push_back(ResultEntry(item.ival));
									}
								}
							}
						}
						else {
							for (auto item : *iter1) {
								if (item.isNull) {
									t.push_back(ResultEntry());
								}
								else {
									if (item.isString) {
										t.push_back(ResultEntry(item.sval));
									}
									else {
										t.push_back(ResultEntry(item.ival));
									}
								}
							}
							for (auto item : *iter) {
								if (item.isNull) {
									t.push_back(ResultEntry());
								}
								else {
									if (item.isString) {
										t.push_back(ResultEntry(item.sval));
									}
									else {
										t.push_back(ResultEntry(item.ival));
									}
								}
							}
						}
						result.table.push_back(t);
					}
				}
			}
		}
		if (level == 2) {
			Expr* expr = conditions[iterno];
			f[0].op = FZERO;
			f[1].op = FZERO;
			if (expr->expr->tableNumber == 0) {
				f[0].columnNumber = expr->expr->columnNumber;
				f[1].columnNumber = expr->expr2->columnNumber;
			}
			else {
				f[0].columnNumber = expr->expr2->columnNumber;
				f[1].columnNumber = expr->expr->columnNumber;
			}
			if (expr->expr->RTType == RTInt) {
				f[0].type = IntFilter;
				f[0].ival = 0;
				f[1].type = IntFilter;
				f[1].ival = 0;
			}
			else {
				f[0].type = StringFilter;
				f[0].sval = "";
				f[1].type = StringFilter;
				f[1].sval = "";
			}
			auto iter0 = be.begin(nos[0], f[0]);
			auto iter1 = be.begin(nos[1], f[1]);
			while (!iter0.isFinished() && !iter1.isFinished()) {
				std::vector<std::vector<ContentEntry>> i0;
				std::vector<std::vector<ContentEntry>> i1;
				if (expr->expr->RTType == RTInt) {
					if ((*iter0)[f[0].columnNumber].isNull) {
						iter0++;
						continue;
					}
					if ((*iter1)[f[1].columnNumber].isNull) {
						iter1++;
						continue;
					}
					int a = (*iter0)[f[0].columnNumber].ival;
					int b = (*iter1)[f[1].columnNumber].ival;
					if (a < b) {
						iter0++;
						continue;
					}
					else if (a > b) {
						iter1++;
						continue;
					}
					else {
						while (!iter0.isFinished() && a == (*iter0)[f[0].columnNumber].ival) {
							i0.push_back(*iter0);
							iter0++;
						}
						while (!iter1.isFinished() && b == (*iter1)[f[1].columnNumber].ival) {
							i1.push_back(*iter1);
							iter1++;
						}
					}
				}
				else {
					if ((*iter0)[f[0].columnNumber].isNull) {
						iter0++;
						continue;
					}
					if ((*iter1)[f[1].columnNumber].isNull) {
						iter1++;
						continue;
					}
					std::string a = (*iter0)[f[0].columnNumber].sval;
					std::string b = (*iter1)[f[1].columnNumber].sval;
					if (a < b) {
						iter0++;
						continue;
					}
					else if (a > b) {
						iter1++;
						continue;
					}
					else {
						while (!iter0.isFinished() && a == (*iter0)[f[0].columnNumber].sval) {
							i0.push_back(*iter0);
							iter0++;
						}
						while (!iter1.isFinished() && b == (*iter1)[f[1].columnNumber].sval) {
							i1.push_back(*iter1);
							iter1++;
						}
					}
				}
				for (auto& j0 : i0) {
					for (auto& j1 : i1) {
						bool match = true;
						for (auto& item : conditions) {
							if (item->expr->RTType == RTString) {
								ContentEntry leftValue;
								ContentEntry rightValue;
								if (0 == item->expr->tableNumber) {
									leftValue = j0[item->expr->columnNumber];
								}
								else {
									leftValue = j1[item->expr->columnNumber];
								}
								if (item->expr2->type == kExprColumnRef) {
									if (0 == item->expr2->tableNumber) {
										rightValue = j0[item->expr2->columnNumber];
									}
									else {
										rightValue = j1[item->expr2->columnNumber];
									}
								}
								else {
									rightValue = ContentEntry(item->expr2->name);
								}
								switch (item->op_type) {
								case Expr::EQ:
									if (leftValue.sval != rightValue.sval) {
										match = false;
									}
									break;
								case Expr::NE:
									if (leftValue.sval == rightValue.sval) {
										match = false;
									}
									break;
								case Expr::LE:
									if (leftValue.sval > rightValue.sval) {
										match = false;
									}
									break;
								case Expr::GE:
									if (leftValue.sval < rightValue.sval) {
										match = false;
									}
									break;
								case Expr::LT:
									if (leftValue.sval >= rightValue.sval) {
										match = false;
									}
									break;
								case Expr::GT:
									if (leftValue.sval <= rightValue.sval) {
										match = false;
									}
									break;
								case Expr::LIKE:
									if (!matchLike(rightValue.sval, leftValue.sval)) {
										match = false;
									}
								case Expr::ISNULL:
									if (leftValue.isNull == false) {
										match = false;
									}
									break;
								case Expr::ISNOTNULL:
									if (leftValue.isNull != false) {
										match = false;
									}
									break;
								}
							}
							else {
								ContentEntry leftValue;
								ContentEntry rightValue;
								if (0 == item->expr->tableNumber) {
									leftValue = j0[item->expr->columnNumber];
								}
								else {
									leftValue = j1[item->expr->columnNumber];
								}
								if (item->expr2->type == kExprColumnRef) {
									if (0 == item->expr2->tableNumber) {
										rightValue = j0[item->expr2->columnNumber];
									}
									else {
										rightValue = j1[item->expr2->columnNumber];
									}
								}
								else {
									rightValue = ContentEntry(item->expr2->ival);
								}
								switch (item->op_type) {
								case Expr::EQ:
									if (leftValue.ival != rightValue.ival) {
										match = false;
									}
									break;
								case Expr::NE:
									if (leftValue.ival == rightValue.ival) {
										match = false;
									}
									break;
								case Expr::LE:
									if (leftValue.ival > rightValue.ival) {
										match = false;
									}
									break;
								case Expr::GE:
									if (leftValue.ival < rightValue.ival) {
										match = false;
									}
									break;
								case Expr::LT:
									if (leftValue.ival >= rightValue.ival) {
										match = false;
									}
									break;
								case Expr::GT:
									if (leftValue.ival <= rightValue.ival) {
										match = false;
									}
									break;
								case Expr::LIKE:
									if (!matchLike(rightValue.sval, leftValue.sval)) {
										match = false;
									}
									break;
								case Expr::ISNULL:
									if (leftValue.isNull == false) {
										match = false;
									}
									break;
								case Expr::ISNOTNULL:
									if (leftValue.isNull != false) {
										match = false;
									}
									break;
								}
							}
						}
						if (match) {
							std::vector<ResultEntry> t;
							for (auto item : j0) {
								if (item.isNull) {
									t.push_back(ResultEntry());
								}
								else {
									if (item.isString) {
										t.push_back(ResultEntry(item.sval));
									}
									else {
										t.push_back(ResultEntry(item.ival));
									}
								}
							}
							for (auto item : j1) {
								if (item.isNull) {
									t.push_back(ResultEntry());
								}
								else {
									if (item.isString) {
										t.push_back(ResultEntry(item.sval));
									}
									else {
										t.push_back(ResultEntry(item.ival));
									}
								}
							}
							result.table.push_back(t);
						}
					}
				}
			}
		}
		SQLResult result0;
		result0.valid = result.valid;
		result0.info = result.info;
		std::vector<int> retIndex;
		if (allShow) {
			for (int i = 0; i < result.tableHeader.size(); i++) {
				if (result.tableHeader[i].name[result.tableHeader[i].name.size() - 1] != '_') {
					retIndex.push_back(i);
				}
			}
		}
		else {
			for (auto& item : headers) {
				std::cout << item << std::endl;
				for (int i = 0; i < result.tableHeader.size(); i++) {
					if (result.tableHeader[i].name == item) {
						retIndex.push_back(i);
					}
				}
			}
		}
		for (auto i : retIndex) {
			result0.tableHeader.push_back(result.tableHeader[i]);
		}
		for (auto& e : result.table) {
			std::vector<ResultEntry> ee;
			for (auto i : retIndex) {
				ee.push_back(e[i]);
			}
			result0.table.push_back(ee);
		}
		return result0;
	}
	SQLResult execute3() {
		SQLResult result;
		DatabaseMeta meta = be.getDataBaseMeta();
		if (!meta.valid) {
			SQLResult unvalid;
			unvalid.valid = false;
			unvalid.info = "please use a database";
			return unvalid;
		}
		std::vector<int> nos;
		std::vector<TableRef*>* refs = fromTable->list;
		for (int i = 0; i < refs->size(); i++) {
			TableRef* tablePtr = refs->operator[](i);
			int no = meta.getNo(std::string(tablePtr->name));
			if (no < 0) {
				result.valid = false;
				result.info = "the table does not exist.";
				return result;
			}
			nos.push_back(no);
		}
		std::set<int> set1(nos.begin(), nos.end());
		if (set1.size() < nos.size()) {
			result.valid = false;
			result.info = "the table name cannot equal.";
			return result;
		}
		if (whereClause == NULL) {
			result.valid = false;
			result.info = "we does not support select on multiple tables without where clause";
			return result;
		}
		TableMeta tableMeta[3];
		tableMeta[0] = be.getTableMeta(nos[0]);
		tableMeta[1] = be.getTableMeta(nos[1]);
		tableMeta[2] = be.getTableMeta(nos[2]);
		whereClause->whereCheck(tableMeta[0], tableMeta[1], tableMeta[2]);
		if (whereClause->RTType != RTBool) {
			result.valid = false;
			result.info = "the where expr must be bool type";
			return result;
		}
		result.valid = true;
		for (int i = 0; i < tableMeta[0].columnNumber; i++) {
			result.tableHeader.push_back(TableHeader(std::string(tableMeta[0].name) + "." + std::string(tableMeta[0].columns[i].name), tableMeta[0].columns[i].dataType == (int32_t)DTINT));
		}
		for (int i = 0; i < tableMeta[1].columnNumber; i++) {
			result.tableHeader.push_back(TableHeader(std::string(tableMeta[1].name) + "." + std::string(tableMeta[1].columns[i].name), tableMeta[1].columns[i].dataType == (int32_t)DTINT));
		}
		for (int i = 0; i < tableMeta[2].columnNumber; i++) {
			result.tableHeader.push_back(TableHeader(std::string(tableMeta[2].name) + "." + std::string(tableMeta[2].columns[i].name), tableMeta[2].columns[i].dataType == (int32_t)DTINT));
		}
		std::vector<Expr*> conditions;
		whereClause->whereSplit(conditions);
		if (selectList->size() == 1 && selectList->operator[](0)->type == kExprStar) {
			allShow = true;
		}
		else {
			allShow = false;
			for (int i = 0; i < selectList->size(); i++) {
				Expr* exp = selectList->operator[](i);
				if (exp->type != kExprColumnRef) {
					result.valid = false;
					result.info = "the select list must be column ref";
					return result;
				}
				int cno = -1;
				if (std::string(tableMeta[0].name) == std::string(exp->table)) {
					cno = 0;
				}
				else if (std::string(tableMeta[1].name) == std::string(exp->table)) {
					cno = 1;
				}
				else if (std::string(tableMeta[2].name) == std::string(exp->table)) {
					cno = 2;
				}
				else {
					result.valid = false;
					result.info = "the select list's column ref does not exist.";
					return result;
				}
				int tno = tableMeta[cno].getNo(std::string(exp->name));
				if (tno < 0) {
					result.valid = false;
					result.info = "the select column ref does not exist";
					return result;
				}
				headers.push_back(std::string(exp->table) + "." + std::string(exp->name));
				std::cout << "push" << std::endl;
			}
		}
		int level = 0;
		int focus = -1;
		int hel[2];
		int colN[2][2];
		RunTimeType rttype[2];
		if (conditions.size() == 3) {
			int goodConditionNum = 0;
			std::vector<int> goodConditions;
			int ttr[3] = { 0,0,0 };
			for (int i = 0; i < conditions.size(); i++) {
				Expr* cc = conditions[i];
				if (cc->expr->type == kExprColumnRef && cc->expr2->type == kExprColumnRef && cc->expr->tableNumber != cc->expr2->tableNumber) {
					goodConditionNum++;
					goodConditions.push_back(i);
					ttr[cc->expr->tableNumber]++;
					ttr[cc->expr2->tableNumber]++;
				}
			}
			if (goodConditions.size() == 2 && ttr[0] * ttr[1] * ttr[2] == 2) {
				level = 1;
				if (ttr[0] == 2) {
					focus = 0;
					for (int i = 0; i < goodConditions.size(); i++) {
						Expr* cc = conditions[goodConditions[i]];
						if (cc->expr->tableNumber == 0) {
							colN[i][0] = cc->expr->columnNumber;
							colN[i][1] = cc->expr2->columnNumber;
							hel[i] = cc->expr2->tableNumber;
							rttype[i] = cc->expr->RTType;
						}
						else {
							colN[i][1] = cc->expr->columnNumber;
							colN[i][0] = cc->expr2->columnNumber;
							hel[i] = cc->expr->tableNumber;
							rttype[i] = cc->expr->RTType;
						}
					}
				}
				if (ttr[1] == 2) {
					focus = 1;
					for (int i = 0; i < goodConditions.size(); i++) {
						Expr* cc = conditions[goodConditions[i]];
						if (cc->expr->tableNumber == 1) {
							colN[i][0] = cc->expr->columnNumber;
							colN[i][1] = cc->expr2->columnNumber;
							hel[i] = cc->expr2->tableNumber;
							rttype[i] = cc->expr->RTType;
						}
						else {
							colN[i][1] = cc->expr->columnNumber;
							colN[i][0] = cc->expr2->columnNumber;
							hel[i] = cc->expr->tableNumber;
							rttype[i] = cc->expr->RTType;
						}
					}
				}
				if (ttr[2] == 2) {
					focus = 2;
					for (int i = 0; i < goodConditions.size(); i++) {
						Expr* cc = conditions[goodConditions[i]];
						if (cc->expr->tableNumber == 2) {
							colN[i][0] = cc->expr->columnNumber;
							colN[i][1] = cc->expr2->columnNumber;
							hel[i] = cc->expr2->tableNumber;
							rttype[i] = cc->expr->RTType;
						}
						else {
							colN[i][1] = cc->expr->columnNumber;
							colN[i][0] = cc->expr2->columnNumber;
							hel[i] = cc->expr->tableNumber;
							rttype[i] = cc->expr->RTType;
						}
					}
				}
			}
		}
		Filter f[3];
		if (level == 1) {
			f[0].op = FZERO;
			f[1].op = FZERO;
			f[0].columnNumber = colN[0][0];
			f[1].columnNumber = colN[0][1];
			if (rttype[0] == RTInt) {
				f[0].type = IntFilter;
				f[1].type = IntFilter;
			}
			else {
				f[0].type = StringFilter;
				f[1].type = StringFilter;
			}
			auto iter0 = be.begin(nos[focus], f[0]);
			auto iter1 = be.begin(nos[hel[0]], f[1]);
			while (!iter0.isFinished() && !iter1.isFinished()) {
				std::vector<std::vector<ContentEntry>> i0;
				std::vector<std::vector<ContentEntry>> i1;
				if (rttype[0] == RTInt) {
					if ((*iter0)[f[0].columnNumber].isNull) {
						iter0++;
						continue;
					}
					if ((*iter1)[f[1].columnNumber].isNull) {
						iter1++;
						continue;
					}
					int a = (*iter0)[f[0].columnNumber].ival;
					int b = (*iter1)[f[1].columnNumber].ival;
					if (a < b) {
						iter0++;
						continue;
					}
					else if (a > b) {
						iter1++;
						continue;
					}
					else {
						while (!iter0.isFinished() && a == (*iter0)[f[0].columnNumber].ival) {
							i0.push_back(*iter0);
							iter0++;
						}
						while (!iter1.isFinished() && b == (*iter1)[f[1].columnNumber].ival) {
							i1.push_back(*iter1);
							iter1++;
						}
					}
				}
				else {
					if ((*iter0)[f[0].columnNumber].isNull) {
						iter0++;
						continue;
					}
					if ((*iter1)[f[1].columnNumber].isNull) {
						iter1++;
						continue;
					}
					std::string a = (*iter0)[f[0].columnNumber].sval;
					std::string b = (*iter1)[f[1].columnNumber].sval;
					if (a < b) {
						iter0++;
						continue;
					}
					else if (a > b) {
						iter1++;
						continue;
					}
					else {
						while (!iter0.isFinished() && a == (*iter0)[f[0].columnNumber].sval) {
							i0.push_back(*iter0);
							iter0++;
						}
						while (!iter1.isFinished() && b == (*iter1)[f[1].columnNumber].sval) {
							i1.push_back(*iter1);
							iter1++;
						}
					}
				}
				for (auto& j0 : i0) {
					for (auto& j1 : i1) {
						f[2].columnNumber = colN[1][1];
						if (j0[colN[1][0]].isNull) {
							f[2].op = FIsNull;
						}
						else {
							f[2].op = FEQ;
						}
						if (rttype[1] == RTInt) {
							f[2].type = IntFilter;
							f[2].ival = j0[colN[1][0]].ival;
						}
						else {
							f[2].type = StringFilter;
							f[2].sval = j0[colN[1][0]].sval;
						}
						for (auto iter2 = be.begin(nos[hel[1]], f[2]); !iter2.isFinished(); iter2++) {
							bool match = true;
							for (auto& item : conditions) {
								if (item->expr->RTType == RTInt) {
									ContentEntry leftValue;
									ContentEntry rightValue;
									if (item->expr->tableNumber == focus) {
										leftValue = j0[item->expr->columnNumber];
									}
									if (item->expr->tableNumber == hel[0]) {
										leftValue = j1[item->expr->columnNumber];
									}
									if (item->expr->tableNumber == hel[1]) {
										leftValue = (*iter2)[item->expr->columnNumber];
									}
									if (item->expr2->type == kExprColumnRef) {
										if (item->expr2->tableNumber == focus) {
											rightValue = j0[item->expr2->columnNumber];
										}
										if (item->expr2->tableNumber == hel[0]) {
											rightValue = j1[item->expr2->columnNumber];
										}
										if (item->expr2->tableNumber == hel[1]) {
											rightValue = (*iter2)[item->expr2->columnNumber];
										}
									}
									else {
										rightValue = ContentEntry(item->expr2->ival);
									}
									switch (item->op_type) {
									case Expr::EQ:
										if (leftValue.ival != rightValue.ival) {
											match = false;
										}
										break;
									case Expr::NE:
										if (leftValue.ival == rightValue.ival) {
											match = false;
										}
										break;
									case Expr::LE:
										if (leftValue.ival > rightValue.ival) {
											match = false;
										}
										break;
									case Expr::GE:
										if (leftValue.ival < rightValue.ival) {
											match = false;
										}
										break;
									case Expr::LT:
										if (leftValue.ival >= rightValue.ival) {
											match = false;
										}
										break;
									case Expr::GT:
										if (leftValue.ival <= rightValue.ival) {
											match = false;
										}
										break;
									case Expr::LIKE:
										continue;
										break;
									case Expr::ISNULL:
										if (leftValue.isNull == false) {
											match = false;
										}
										break;
									case Expr::ISNOTNULL:
										if (leftValue.isNull != false) {
											match = false;
										}
										break;
									}
								}
								else {
									ContentEntry leftValue;
									ContentEntry rightValue;
									if (item->expr->tableNumber == focus) {
										leftValue = j0[item->expr->columnNumber];
									}
									if (item->expr->tableNumber == hel[0]) {
										leftValue = j1[item->expr->columnNumber];
									}
									if (item->expr->tableNumber == hel[1]) {
										leftValue = (*iter2)[item->expr->columnNumber];
									}
									if (item->expr2->type == kExprColumnRef) {
										if (item->expr2->tableNumber == focus) {
											rightValue = j0[item->expr2->columnNumber];
										}
										if (item->expr2->tableNumber == hel[0]) {
											rightValue = j1[item->expr2->columnNumber];
										}
										if (item->expr2->tableNumber == hel[1]) {
											rightValue = (*iter2)[item->expr2->columnNumber];
										}
									}
									else {
										rightValue = ContentEntry(item->expr2->name);
									}
									switch (item->op_type) {
									case Expr::EQ:
										if (leftValue.sval != rightValue.sval) {
											match = false;
										}
										break;
									case Expr::NE:
										if (leftValue.sval == rightValue.sval) {
											match = false;
										}
										break;
									case Expr::LE:
										if (leftValue.sval > rightValue.sval) {
											match = false;
										}
										break;
									case Expr::GE:
										if (leftValue.sval < rightValue.sval) {
											match = false;
										}
										break;
									case Expr::LT:
										if (leftValue.sval >= rightValue.sval) {
											match = false;
										}
										break;
									case Expr::GT:
										if (leftValue.sval <= rightValue.sval) {
											match = false;
										}
										break;
									case Expr::LIKE:
										continue;
										break;
									case Expr::ISNULL:
										if (leftValue.isNull == false) {
											match = false;
										}
										break;
									case Expr::ISNOTNULL:
										if (leftValue.isNull != false) {
											match = false;
										}
										break;
									}
								}
							}
							if (match) {
								std::vector<ResultEntry> t;
								int oorder[3];
								oorder[focus] = 0;
								oorder[hel[0]] = 1;
								oorder[hel[1]] = 2;
								for (int i = 0; i < 3; i++) {
									if (oorder[i] == 0) {
										for (auto item : j0) {
											if (item.isNull) {
												t.push_back(ResultEntry());
											}
											else {
												if (item.isString) {
													t.push_back(ResultEntry(item.sval));
												}
												else {
													t.push_back(ResultEntry(item.ival));
												}
											}
										}
									}
									if (oorder[i] == 1) {
										for (auto item : j1) {
											if (item.isNull) {
												t.push_back(ResultEntry());
											}
											else {
												if (item.isString) {
													t.push_back(ResultEntry(item.sval));
												}
												else {
													t.push_back(ResultEntry(item.ival));
												}
											}
										}
									}
									if (oorder[i] == 2) {
										for (auto item : *iter2) {
											if (item.isNull) {
												t.push_back(ResultEntry());
											}
											else {
												if (item.isString) {
													t.push_back(ResultEntry(item.sval));
												}
												else {
													t.push_back(ResultEntry(item.ival));
												}
											}
										}
									}
								}
								result.table.push_back(t);
							}
						}
					}
				}
			}
		}
		else {
			f[0].op = FZERO;
			f[1].op = FZERO;
			f[2].op = FZERO;
			f[0].columnNumber = 0;
			f[1].columnNumber = 0;
			f[2].columnNumber = 0;
			f[0].ival = 0;
			f[1].ival = 0;
			f[2].ival = 0;
			f[0].type = IntFilter;
			f[1].type = IntFilter;
			f[2].type = IntFilter;
			for (auto iter0 = be.begin(nos[0], f[0]); !iter0.isFinished(); iter0++) {
				for (auto iter1 = be.begin(nos[1], f[1]); !iter1.isFinished(); iter1++) {
					for (auto iter2 = be.begin(nos[2], f[2]); !iter2.isFinished(); iter2++) {
						bool match = true;
						for (auto& item : conditions) {
							if (item->expr->RTType == RTInt) {
								ContentEntry leftValue;
								ContentEntry rightValue;
								if (item->expr->tableNumber == 0) {
									leftValue = (*iter0)[item->expr->columnNumber];
								}
								if (item->expr->tableNumber == 1) {
									leftValue = (*iter1)[item->expr->columnNumber];
								}
								if (item->expr->tableNumber == 2) {
									leftValue = (*iter2)[item->expr->columnNumber];
								}
								if (item->expr2->type == kExprColumnRef) {
									if (item->expr2->tableNumber == 0) {
										rightValue = (*iter0)[item->expr2->columnNumber];
									}
									if (item->expr2->tableNumber == 1) {
										rightValue = (*iter1)[item->expr2->columnNumber];
									}
									if (item->expr2->tableNumber == 2) {
										rightValue = (*iter2)[item->expr2->columnNumber];
									}
								}
								else {
									rightValue = ContentEntry(item->expr2->ival);
								}
								switch (item->op_type) {
								case Expr::EQ:
									if (leftValue.ival != rightValue.ival) {
										match = false;
									}
									break;
								case Expr::NE:
									if (leftValue.ival == rightValue.ival) {
										match = false;
									}
									break;
								case Expr::LE:
									if (leftValue.ival > rightValue.ival) {
										match = false;
									}
									break;
								case Expr::GE:
									if (leftValue.ival < rightValue.ival) {
										match = false;
									}
									break;
								case Expr::LT:
									if (leftValue.ival >= rightValue.ival) {
										match = false;
									}
									break;
								case Expr::GT:
									if (leftValue.ival <= rightValue.ival) {
										match = false;
									}
									break;
								case Expr::LIKE:
									continue;
									break;
								case Expr::ISNULL:
									if (leftValue.isNull == false) {
										match = false;
									}
									break;
								case Expr::ISNOTNULL:
									if (leftValue.isNull != false) {
										match = false;
									}
									break;
								}
							}
							else {
								ContentEntry leftValue;
								ContentEntry rightValue;
								if (item->expr->tableNumber == 0) {
									leftValue = (*iter0)[item->expr->columnNumber];
								}
								if (item->expr->tableNumber == 1) {
									leftValue = (*iter1)[item->expr->columnNumber];
								}
								if (item->expr->tableNumber == 2) {
									leftValue = (*iter2)[item->expr->columnNumber];
								}
								if (item->expr2->type == kExprColumnRef) {
									if (item->expr2->tableNumber == 0) {
										rightValue = (*iter0)[item->expr2->columnNumber];
									}
									if (item->expr2->tableNumber == 1) {
										rightValue = (*iter1)[item->expr2->columnNumber];
									}
									if (item->expr2->tableNumber == 2) {
										rightValue = (*iter2)[item->expr2->columnNumber];
									}
								}
								else {
									rightValue = ContentEntry(item->expr2->name);
								}
								switch (item->op_type) {
								case Expr::EQ:
									if (leftValue.sval != rightValue.sval) {
										match = false;
									}
									break;
								case Expr::NE:
									if (leftValue.sval == rightValue.sval) {
										match = false;
									}
									break;
								case Expr::LE:
									if (leftValue.sval > rightValue.sval) {
										match = false;
									}
									break;
								case Expr::GE:
									if (leftValue.sval < rightValue.sval) {
										match = false;
									}
									break;
								case Expr::LT:
									if (leftValue.sval >= rightValue.sval) {
										match = false;
									}
									break;
								case Expr::GT:
									if (leftValue.sval <= rightValue.sval) {
										match = false;
									}
									break;
								case Expr::LIKE:
									continue;
									break;
								case Expr::ISNULL:
									if (leftValue.isNull == false) {
										match = false;
									}
									break;
								case Expr::ISNOTNULL:
									if (leftValue.isNull != false) {
										match = false;
									}
									break;
								}
							}
						}
						if (match) {
							std::vector<ResultEntry> t;
							for (auto item : *iter0) {
								if (item.isNull) {
									t.push_back(ResultEntry());
								}
								else {
									if (item.isString) {
										t.push_back(ResultEntry(item.sval));
									}
									else {
										t.push_back(ResultEntry(item.ival));
									}
								}
							}
							for (auto item : *iter1) {
								if (item.isNull) {
									t.push_back(ResultEntry());
								}
								else {
									if (item.isString) {
										t.push_back(ResultEntry(item.sval));
									}
									else {
										t.push_back(ResultEntry(item.ival));
									}
								}
							}
							for (auto item : *iter2) {
								if (item.isNull) {
									t.push_back(ResultEntry());
								}
								else {
									if (item.isString) {
										t.push_back(ResultEntry(item.sval));
									}
									else {
										t.push_back(ResultEntry(item.ival));
									}
								}
							}
							result.table.push_back(t);
						}
					}
				}
			}
		}
		SQLResult result0;
		result0.valid = result.valid;
		result0.info = result.info;
		std::vector<int> retIndex;
		if (allShow) {
			for (int i = 0; i < result.tableHeader.size(); i++) {
				if (result.tableHeader[i].name[result.tableHeader[i].name.size() - 1] != '_') {
					retIndex.push_back(i);
				}
			}
		}
		else {
			for (auto& item : headers) {
				std::cout << item << std::endl;
				for (int i = 0; i < result.tableHeader.size(); i++) {
					if (result.tableHeader[i].name == item) {
						retIndex.push_back(i);
					}
				}
			}
		}
		for (auto i : retIndex) {
			result0.tableHeader.push_back(result.tableHeader[i]);
		}
		for (auto& e : result.table) {
			std::vector<ResultEntry> ee;
			for (auto i : retIndex) {
				ee.push_back(e[i]);
			}
			result0.table.push_back(ee);
		}
		return result0;
	}
};
class AST {
public:
	AST() : isValid(true), errorMsg(NULL) {};
	AST(SQLStatement* stmt) : isValid(true), errorMsg(NULL) {
		addStatement(stmt);
	};
	~AST() {
		for (std::vector<SQLStatement*>::iterator it = statements.begin(); it != statements.end(); ++it) {
			delete *it;
		}
		delete errorMsg;
	}
	void addStatement(SQLStatement* stmt) {
		statements.push_back(stmt);
	}
	SQLStatement* getStatement(int id) {
		return statements[id];
	}
	size_t size() {
		return statements.size();
	}
	std::vector<SQLResult> execute() {
		std::vector<SQLResult> result;
		for (auto ptr : statements) {
			SQLResult res = ptr->execute();
			res.print();
			result.push_back(res);
			if (res.valid == false) {
				break;
			}
		}
		return result;
	}
	std::vector<SQLStatement*> statements;
	bool isValid;
	const char* errorMsg;
	int errorLine;
	int errorColumn;
};
