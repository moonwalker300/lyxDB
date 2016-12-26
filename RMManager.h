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
	std::vector<int> tmpWorker; //��ʱ��
	int FindFreePage();
	void freePage(std::vector<int>& pageRanks);
public:
	int64_t getNowDataBaseHandle() { return nowDataBaseHandle; }
	RMManager(FileManager *fm);
	~RMManager();
	int CreateDataBase(const char* dbName);//�������ݿ⣬-2��ʾ�������ݿ⣬-1��ʾдʧ�ܣ�0��ʾ�ɹ�
	int ChangeDataBase(const char* dbName); //��ĳ���ݿ�,����-1��ʾû��������ݿ⣬���򷵻ؾ��
	int CreateTable(/*char *tableNum,*/ TableInfomation tableInfo);  //����-1��ʾ��ǰû�����ݿ�
	int GetRecord(int pageRank, int slotRank, char* result, int& recordLen); //����-1��ʾû�����ݿ⣬-2��ʾû����һҳ��-3��ʾû����һ��¼, -4��ʾ����۱�ɾ����
	int InsertRecord(int headPlace, int& pageRank, int& slotRank, char* record, int recordLen, char* bufferTable); 
	//�����¼��pageRank����ҳ�ţ�slotRank���زۺţ���������ʣ��ռ䣬����-1��ʾû�����ݿ⣬-2��ʾû����һҳ��-3��ʾ�ռ䲻��
	int DeleteRecord(int headPlace, int pageRank, int slotRank);
	//ɾ����¼,��������ʣ��ռ䣬����-1��ʾû�����ݿ⣬-2��ʾû����һҳ��-3��ʾû����һ��¼
	int UpdateRecord(int pageRank, int slotRank, char* record, int recordLen);
	
	void destroyTable(int tableHead);
};

#endif