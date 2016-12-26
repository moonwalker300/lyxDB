#ifndef RMMANAGER_H
#define RMMANAGER_H

#include "FileManager.h"
#include "infolist.h"
#include <vector>
const char DBSuffixName[] = ".lyx";
//表示一条记录，页号，槽号
class RMManager {
private:
	FileManager *fileManager;
	int64_t nowDataBaseHandle; //现在使用的数据库的int handle, 0表示没有
	std::string nowDataBaseName;
//	int nowDataBasePageNum;
	std::vector<int> tmpWorker; //临时工
	int FindFreePage();
	void freePage(std::vector<int>& pageRanks);
public:
	int64_t getNowDataBaseHandle() { return nowDataBaseHandle; }
	RMManager(FileManager *fm);
	~RMManager();
	int CreateDataBase(const char* dbName);//创建数据库，-2表示已有数据库，-1表示写失败，0表示成功
	int ChangeDataBase(const char* dbName); //打开某数据库,返回-1表示没有这个数据库，否则返回句柄
	int CreateTable(/*char *tableNum,*/ TableInfomation tableInfo);  //返回-1表示当前没有数据库
	int GetRecord(int pageRank, int slotRank, char* result, int& recordLen); //返回-1表示没有数据库，-2表示没有这一页，-3表示没有这一记录, -4表示这个槽被删除了
	int InsertRecord(int headPlace, int& pageRank, int& slotRank, char* record, int recordLen, char* bufferTable); 
	//插入记录，pageRank返回页号，slotRank返回槽号，函数返回剩余空间，返回-1表示没有数据库，-2表示没有这一页，-3表示空间不足
	int DeleteRecord(int headPlace, int pageRank, int slotRank);
	//删除记录,函数返回剩余空间，返回-1表示没有数据库，-2表示没有这一页，-3表示没有这一记录
	int UpdateRecord(int pageRank, int slotRank, char* record, int recordLen);
	
	void destroyTable(int tableHead);
};

#endif