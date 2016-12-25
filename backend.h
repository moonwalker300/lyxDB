#ifndef BACKEND_H
#define BACKEND_H
#include "IXManager.h"
#include "RMManager.h"

#include "Meta.h"
//֮�����ɾȥ�����Ķ���
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
class ContentEntry {
public:
	bool isString;
	bool isNull;
	int ival;
	std::string sval;
	ContentEntry() :isNull(true) {}
	ContentEntry(int i) :isString(false), isNull(true), ival(i) {}
	ContentEntry(std::string s) :isString(true), isNull(true), sval(s) {}
};

class IndexIterator {
public:
	bool flag;
	FileManager* fileManager;
	IXManager* im;
	RMManager* rm;
	int nowDataBaseHandle;
	int pageRank, slotRank;
	int recordPage, recordSlot;
	int limitPage, limitSlot;
	int indexLen, offset, indexNum;
	int columnRank;
	int columnNum;
	int recordLen;
	std::vector<ContentEntry> entries;
	std::vector<bool> change;
	std::vector<int> kinds;
	std::vector<int> sizes;
	std::vector<ContentEntry> lyxbuf;
	std::vector<int> columnbuf;
	std::vector<int> zhbuf;
	char buffer[PAGE_SIZE];
	char bufferTable[PAGE_SIZE]; //��ͷҳ����
	int tableHead; //��ͷҳ
//����
	void read();
	void operator++(int);
	std::vector<ContentEntry>& operator*();
	bool isFinished() { return flag; }
	void markChange(int i) { change[i] = true; }
	void Kill();
	void Update(int i, ContentEntry c);
};

class BackEnd {
	IXManager* im;
	RMManager* rm;
	FileManager* fm;
	int64_t nowDataBaseHandle;
	//std::vector<int> idLimit;
public:
	BackEnd() {}
	void createDataBase(std::string& name);
	void dropDataBase(std::string& name);
	std::vector<std::string> showDatabases();
	void useDataBase(std::string& name);
	DatabaseMeta getDataBaseMeta();
	void createTable(TableMeta& tableMeta);
	void dropTable(int no);
	TableMeta getTableMeta(int no);
	void insertRecord(int no, std::vector<ContentEntry>& line);
	IndexIterator begin(int no, Filter f);
};

#endif