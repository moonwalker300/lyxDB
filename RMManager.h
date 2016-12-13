#ifndef RMMANAGER_H
#define RMMANAGER_H

#include "FileManager.h"
#include "infolist.h"
#include <vector>
const char DBSuffixName[] = ".lyx";
//表示一条记录，页号，槽号
struct RID {
	int pageRank, slotRank;
};

class RMManager {
private:
	std::vector<std::string> dbNames;
	FileManager *fileManager;
	int64_t nowDataBaseHandle; //现在使用的数据库的int handle, 0表示没有
	string nowDataBaseName;
	int nowDataBasePageNum;
	int FindFreePage();
public:
	RMManager(FileManager *fm);
	~RMManager();
	int CreateDataBase(char* dbName);//创建数据库，-2表示已有数据库，-1表示写失败，0表示成功
	int ChangeDataBase(char* dbName); //打开某数据库,返回-1表示没有这个数据库，否则返回句柄
	int CreateTable(/*char *tableNum,*/ TableInfo tableInfo);  //返回-1表示当前没有数据库
	int GetRecord(int pageRank, int slotRank, char* result, int& recordLen);
};

#endif