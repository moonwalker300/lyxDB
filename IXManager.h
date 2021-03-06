#pragma once
#include "FileManager.h"
#include "RMManager.h"
#include <vector>

class IXManager {
private:
	FileManager *fileManager;
	int64_t nowDataBaseHandle; //现在使用的数据库的int handle, 0表示没有
	std::string nowDataBaseName;
	std::vector<int> tmpWorker; //临时工
	int FindFreePage();
	void freePage(std::vector<int>& pageRanks);
	int InsertLeaf(int nowPage, char* index, int indexLen, int dataKind, int recordID, int pageRank, int slotRank);
	//返回0表示根没有变化，否则返回变化后的根
	int InsertNoLeaf(int nowPage, char* index, int indexLen, int dataKind, int recordid, int newPage);
	//返回0表示根没有变化，否则返回变化后的根
public:
	IXManager(FileManager *fm);
	int ChangeDataBase(const char* dbName); //打开某数据库,返回-1表示没有这个数据库，否则返回句柄
	int CreateIndex(int tableRank, int columnRank/*const char* tableName, const char* columnName*/); //要求tableName长度28,columnName长度32
	int FindIndexRootPage(char* tableName, char* columnName);
	int SearchIndex(int rootPage, char* index, int indexLen, int dataKind, int& leafPage, int& indexRank, int findBigger); 
	//indexLen=-1，表示NULL, indexLen包含NULL标志,leafPage和indexRank返回叶节点的叶号和序号
	int InsertRecordAndIX(int rootPage, char* index, int indexLen, int dataKind, int pageRank, int slotRank, int recordID);
	//返回0表示根没有变化，否则返回变化后的根,返回-1表示有重复项
	int DeleteRecordAndIX(int rootPage, char* index, int indexLen, int dataKind, int recordID);
	//返回0表示删除成功，返回-1表示没有这项
	int UpDateRecordAndIX(int rootPage, char* index, int indexLen, int dataKind, int pageRank, int slotRank, int recordID);
	void countPage(int nowPage, std::vector<int>& pages);
	void destoryIndex(int indexRoot);
	void closeDataBase() { nowDataBaseHandle = 0; nowDataBaseName = ""; }
};
