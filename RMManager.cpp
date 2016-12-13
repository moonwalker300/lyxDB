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
		if (nowGAM >= nowDataBasePageNum) { //扩展文件
			fileManager->alloc(nowDataBaseHandle, nowGAM - nowDataBasePageNum + 1); //填充空白
			char *zeroPAD = AllZeroFill(PAGE_SIZE);
			fileManager->write(nowDataBaseHandle, nowGAM, zeroPAD);
			delete[]zeroPAD;
		}
		fileManager->read(nowDataBaseHandle, nowGAM, buffer);
		for (int i = 1; i < PAGE_SIZE * BYTE_SIZE; i++) {
			char tmp = buffer[i / BYTE_SIZE];
			char mask = 1 << (BYTE_SIZE - 1 - i % BYTE_SIZE); //左移获得掩码
			if (tmp & mask) {
				ifFind = true; //找到了
				offset = i; //相对GAM页的位移
				break;
			}
		}
		if (ifFind)
			break;
		nowGAM += PAGE_SIZE;
	}

	if (nowGAM + offset >= nowDataBasePageNum) 
		fileManager->alloc(nowDataBaseHandle, nowGAM + offset - nowDataBasePageNum + 1); //扩展文件

	//标记这一页已被使用
	char mask = (1 << (BYTE_SIZE - 1 - offset % BYTE_SIZE));
	buffer[offset / BYTE_SIZE] -= mask;
	fileManager->write(nowDataBaseHandle, nowGAM, buffer);

	return nowGAM + offset;
}

RMManager::RMManager(FileManager *fm) {
	this->fileManager = fm;
	this->nowDataBaseHandle = 0; //初始没有使用数据库
	this->nowDataBaseName = "";
}

int RMManager::CreateDataBase(char* dbName) {
	string dataBaseName(dbName);
	for (int i = 0; i < dbNames.size(); i++)
		if (dbNames[i] == dataBaseName)
			return -2;
	dataBaseName.append(DBSuffixName); //添加后缀名
	int64_t fileHandle = fileManager->open(dataBaseName);
	char* write = AllZeroFill(PAGE_SIZE); //全0页
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
	if (flag == 0) //找不到数据库
		return -1;
	dataBaseName.append(DBSuffixName);
	nowDataBaseHandle = fileManager->open(dataBaseName);
	nowDataBasePageNum = fileManager->size(nowDataBaseHandle);
	return nowDataBaseHandle;
}

int RMManager::CreateTable(/*char *tableNum,*/ TableInfo tableInfo) {
	if ((nowDataBaseHandle == 0) || (nowDataBaseName == "")) //当前没有数据库
		return -1;

	char *buffer = new char[PAGE_SIZE];
	fileManager->read(nowDataBaseHandle, 0, buffer); //0表示库页
	//寻找空闲页
	int TableHeadPageRank = FindFreePage();
	int FirstDataPageRank = FindFreePage();

	int tableNum = charToNum(buffer, TABLE_NUM_LEN);
	tableNum++;
	writeNum(buffer, TABLE_NUM_LEN, tableNum);
	//找到写table信息位置
	char *tableInfoPlace = buffer + TABLE_NUM_LEN + (TABLE_HEAD_PLACE_LEN + TABLE_NAME_LEN) * (tableNum - 1);
	//写库页
	writeNum(tableInfoPlace, TABLE_HEAD_PLACE_LEN, TableHeadPageRank);
	writeStr(tableInfoPlace + TABLE_HEAD_PLACE_LEN, TABLE_NAME_LEN, tableInfo.tableName, TABLE_NAME_LEN);
	//写表头页
	fileManager->write(nowDataBaseHandle, 0, buffer); //0表示库页

	//写页号
	writeNum(buffer, PAGE_RANK_LEN, TableHeadPageRank);
	//写表名
	writeStr(buffer + PAGE_RANK_LEN, TABLE_NAME_LEN, tableInfo.tableName, TABLE_NAME_LEN);
	//写数据页首页
	writeNum(buffer + PAGE_RANK_LEN + TABLE_NUM_LEN, PAGE_RANK_LEN, FirstDataPageRank);
	//写列数
	writeNum(buffer + PAGE_RANK_LEN + TABLE_NUM_LEN + PAGE_RANK_LEN, COLUMN_NUM_LEN, tableInfo.columnNum);
	//写列信息
	int offset = PAGE_RANK_LEN + TABLE_NUM_LEN + PAGE_RANK_LEN + COLUMN_NUM_LEN;
	for (int i = 0; i < tableInfo.columnNum; i++) {
		writeStr(buffer + offset, COLUMN_NAME_LEN, tableInfo.columns[i].columnName, COLUMN_NAME_LEN);
		offset += COLUMN_NAME_LEN;
		writeStr(buffer + offset, COLUMN_PROPERTY_LEN, tableInfo.columns[i].columnProperty, COLUMN_PROPERTY_LEN);
		offset += COLUMN_PROPERTY_LEN;
		writeStr(buffer + offset, COLUMN_KIND_LEN, tableInfo.columns[i].columnKind, COLUMN_KIND_LEN);
		offset += COLUMN_KIND_LEN;
		writeStr(buffer + offset, COLUMN_LEN_LEN, tableInfo.columns[i].columnLen, COLUMN_LEN_LEN);
		offset += COLUMN_LEN_LEN;
		writeStr(buffer + offset, INDEX_PLACE_LEN, tableInfo.columns[i].indexPlace, INDEX_PLACE_LEN);
		offset += INDEX_PLACE_LEN;
	}
	fileManager->write(nowDataBaseHandle, TableHeadPageRank, buffer);

	//写数据首页
	offset = 0;
	writeNum(buffer + offset, PAGE_RANK_LEN, FirstDataPageRank);
	offset += PAGE_RANK_LEN;
	writeNum(buffer + offset, PAGE_RANK_LEN, NOPAGE);
	offset += PAGE_RANK_LEN;
	writeNum(buffer + offset, PAGE_RANK_LEN, NOPAGE);
	offset += PAGE_RANK_LEN;

	int fixedColumnNum = 0;
	int fixedColumnLen = 0;
	for (int i = 0; i < tableInfo.columnNum; i++) {
		int dataKind = charToNum(tableInfo.columns[i].columnProperty, COLUMN_PROPERTY_LEN);
		if (dataKind != KIND_VARCHAR) {
			fixedColumnNum++;
			int dataLen = charToNum(tableInfo.columns[i].columnLen, COLUMN_LEN_LEN);
			fixedColumnLen += dataLen;
		} 
	}
	writeNum(buffer + offset, FIXEDCOLUMNNUM_LEN, fixedColumnNum);
	offset += FIXEDCOLUMNNUM_LEN;
	writeNum(buffer + offset, FIXEDCOLUMNLEN_LEN, fixedColumnLen);
	offset += FIXEDCOLUMNLEN_LEN;
	writeNum(buffer + offset, RECORDNUMLEN, 0); //一开始没有记录
	offset += RECORDNUMLEN;

	fileManager->write(nowDataBaseHandle, FirstDataPageRank, buffer);

	delete[]buffer;
	
	return 0;
}

int RMManager::GetRecord(int pageRank, int slotRank, char* result, int& recordLen) {
	if ((nowDataBaseHandle == 0) || (nowDataBaseName == "")) //当前没有数据库
		return -1;

	char* buffer = new char[PAGE_SIZE];
	int ret = fileManager->read(nowDataBaseHandle, pageRank, buffer);
	if (ret < 0) //没有这一页
		return -2;

	int offset = PAGE_RANK_LEN * 3 + FIXEDCOLUMNNUM_LEN + FIXEDCOLUMNLEN_LEN;
	int recordNum = charToNum(buffer + offset, RECORDNUMLEN);
	if (recordNum <= slotRank) //没有这条记录
		return -3;


	delete[]buffer;
}