#ifndef RMMANAGER_H
#define RMMANAGER_H

#include "FileManager.h"
#include "infolist.h"
#include <vector>
const char DBSuffixName[] = ".lyx";
//��ʾһ����¼��ҳ�ţ��ۺ�
class RMManager {
private:
	FileManager *fileManager;
	int64_t nowDataBaseHandle; //����ʹ�õ����ݿ��int handle, 0��ʾû��
	std::string nowDataBaseName;
//	int nowDataBasePageNum;
	int FindFreePage();
public:
	RMManager(FileManager *fm);
	~RMManager();
	int CreateDataBase(char* dbName);//�������ݿ⣬-2��ʾ�������ݿ⣬-1��ʾдʧ�ܣ�0��ʾ�ɹ�
	int ChangeDataBase(char* dbName); //��ĳ���ݿ�,����-1��ʾû��������ݿ⣬���򷵻ؾ��
	int CreateTable(/*char *tableNum,*/ TableInfo tableInfo);  //����-1��ʾ��ǰû�����ݿ�
	int GetRecord(int pageRank, int slotRank, char* result, int& recordLen); //����-1��ʾû�����ݿ⣬-2��ʾû����һҳ��-3��ʾû����һ��¼, -4��ʾ����۱�ɾ����
	int InsertRecord(int pageRank, int& slotRank, char* record, int recordLen); 
	//�����¼��slotRank���زۺţ���������ʣ��ռ䣬����-1��ʾû�����ݿ⣬-2��ʾû����һҳ��-3��ʾ�ռ䲻��
	int DeleteRecord(int pageRank, int slotRank);
	//ɾ����¼,��������ʣ��ռ䣬����-1��ʾû�����ݿ⣬-2��ʾû����һҳ��-3��ʾû����һ��¼
	int GetFreeSpace(int pageRank); //����-1��ʾû�����ݿ⣬-2��ʾû����һҳ
	int UpdateRecord(int pageRank, int slotRank, char* record, int recordLen);
};

#endif