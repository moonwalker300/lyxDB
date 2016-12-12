#include "RMManager.h"
#include <string>
#include "tool.h"
using namespace std;

int RMManager::FindFreePage() {
	int nowGAM = 1;
	int offset;
	bool ifFind = false;
	char *buffer = new char[PAGE_SIZE];
	while (true) {
		if (nowGAM >= nowDataBasePageNum) { //��չ�ļ�
			fileManager->alloc(nowDataBaseHandle, nowGAM - nowDataBasePageNum + 1); //���հ�
			char *zeroPAD = AllZeroFill(PAGE_SIZE);
			fileManager->write(nowDataBaseHandle, nowGAM, zeroPAD);
			delete[]zeroPAD;
		}
		fileManager->read(nowDataBaseHandle, nowGAM, buffer);
		for (int i = 1; i < PAGE_SIZE * BYTE_SIZE; i++) {
			char tmp = buffer[i / BYTE_SIZE];
			char mask = 1 << (BYTE_SIZE - 1 - i % BYTE_SIZE); //���ƻ������
			if (tmp & mask) {
				ifFind = true; //�ҵ���
				offset = i; //���GAMҳ��λ��
				break;
			}
		}
		if (ifFind)
			break;
		nowGAM += PAGE_SIZE;
	}

	if (nowGAM + offset >= nowDataBasePageNum) 
		fileManager->alloc(nowDataBaseHandle, nowGAM + offset - nowDataBasePageNum + 1); //��չ�ļ�

	//�����һҳ�ѱ�ʹ��
	char mask = (1 << (BYTE_SIZE - 1 - offset % BYTE_SIZE));
	buffer[offset / BYTE_SIZE] -= mask;
	fileManager->write(nowDataBaseHandle, nowGAM, buffer);

	return nowGAM + offset;
}

RMManager::RMManager(FileManager *fm) {
	this->fileManager = fm;
	this->nowDataBaseHandle = 0; //��ʼû��ʹ�����ݿ�
	this->nowDataBaseName = "";
}

int RMManager::CreateDataBase(char* dbName) {
	string dataBaseName(dbName);
	for (int i = 0; i < dbNames.size(); i++)
		if (dbNames[i] == dataBaseName)
			return -2;
	dataBaseName.append(DBSuffixName); //��Ӻ�׺��
	int64_t fileHandle = fileManager->open(dataBaseName);
	char* write = AllZeroFill(PAGE_SIZE); //ȫ0ҳ
	int64_t ret = fileManager->write(fileHandle, 0, write);
	delete[]write;
	fileManager->close(fileHandle);
	if (ret < 0)
		return -1;
	else {
		dbNames.push_back(string(dbName));
		return 0;
	}
}

int RMManager::ChangeDataBase(char* dbName) {
	string dataBaseName(dbName);
	int flag = 0;
	for (int i = 0; i < dbNames.size(); i++)
		if (dataBaseName == dbNames[i]) {
			flag = 1;
			break;
		}
	if (flag == 0) //�Ҳ������ݿ�
		return -1;
	dataBaseName.append(DBSuffixName);
	nowDataBaseHandle = fileManager->open(dataBaseName);
	nowDataBasePageNum = fileManager->size(nowDataBaseHandle);
	return nowDataBaseHandle;
}

int RMManager::CreateTable(char *tableNum, TableInfo tableInfo) {
	char *buffer = new char[PAGE_SIZE];
	fileManager->read(nowDataBaseHandle, 0, buffer);
	//Ѱ�ҿ���ҳ
	int TableHeadPageRank = FindFreePage();
	int FirstDataPageRank = 
}