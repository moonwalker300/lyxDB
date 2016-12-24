#ifndef IXMANAGER_H
#define IXMANAGER_H
#include "FileManager.h"
#include "RMManager.h"


class IXManager {
private:
	FileManager *fileManager;
	int64_t nowDataBaseHandle; //����ʹ�õ����ݿ��int handle, 0��ʾû��
	std::string nowDataBaseName;
	int FindFreePage();
	int InsertLeaf(int nowPage, char* index, int indexLen, int dataKind, int recordID, int pageRank, int slotRank);
	//����0��ʾ��û�б仯�����򷵻ر仯��ĸ�
	int InsertNoLeaf(int nowPage, char* index, int indexLen, int dataKind, int recordid, int newPage);
	//����0��ʾ��û�б仯�����򷵻ر仯��ĸ�
public:
	IXManager(FileManager *fm);
	int ChangeDataBase(char* dbName); //��ĳ���ݿ�,����-1��ʾû��������ݿ⣬���򷵻ؾ��
	int CreateIndex(char* tableName, char* columnName); //Ҫ��tableName����28,columnName����32
	int FindIndexRootPage(char* tableName, char* columnName);
	int SearchIndex(int rootPage, char* index, int indexLen, int dataKind, int& leafPage, int& indexRank, int findBigger); 
	//indexLen=-1����ʾNULL, indexLen����NULL��־,leafPage��indexRank����Ҷ�ڵ��Ҷ�ź����
	int InsertRecordAndIX(int rootPage, char* index, int indexLen, int dataKind, int pageRank, int slotRank, int recordID);
	//����0��ʾ��û�б仯�����򷵻ر仯��ĸ�,����-1��ʾ���ظ���
	int DeleteRecordAndIX(int rootPage, char* index, int indexLen, int dataKind, int recordID);
	//����0��ʾɾ���ɹ�������-1��ʾû������
	int UpDateRecordAndIX(int rootPage, char* index, int indexLen, int dataKind, int pageRank, int slotRank, int recordID);

	 
};

#endif