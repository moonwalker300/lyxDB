#ifndef RMMANAGER_H
#define RMMANAGER_H

#include "FileManager.h"
#include "infolist.h"
#include <vector>
const char DBSuffixName[] = ".lyx";
//��ʾһ����¼��ҳ�ţ��ۺ�
struct RID {
	int pageRank, slotRank;
};

class RMManager {
private:
	std::vector<std::string> dbNames;
	FileManager *fileManager;
	int64_t nowDataBaseHandle; //����ʹ�õ����ݿ��int handle, 0��ʾû��
	string nowDataBaseName;
	int nowDataBasePageNum;
	int FindFreePage();
public:
	RMManager(FileManager *fm);
	~RMManager();
	int CreateDataBase(char* dbName);//�������ݿ⣬-2��ʾ�������ݿ⣬-1��ʾдʧ�ܣ�0��ʾ�ɹ�
	int ChangeDataBase(char* dbName); //��ĳ���ݿ�,����-1��ʾû��������ݿ⣬���򷵻ؾ��
	int CreateTable(/*char *tableNum,*/ TableInfo tableInfo);  //����-1��ʾ��ǰû�����ݿ�
	int GetRecord(int pageRank, int slotRank, char* result, int& recordLen);
};

#endif