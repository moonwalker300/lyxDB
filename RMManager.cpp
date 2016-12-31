#include "RMManager.h"
#include <string>
#include "tool.h"
#include <iostream>
using namespace std;

int RMManager::FindFreePage() {
	int nowGAM = 1;
	int offset;
	bool ifFind = false;
	int totPageNum;
	char buffer[PAGE_SIZE];
	while (true) {
		totPageNum = fileManager->size(nowDataBaseHandle);
		if (nowGAM >= totPageNum) { //扩展文件
			fileManager->alloc(nowDataBaseHandle, nowGAM - totPageNum + 1); //填充空白
			char zeroPAD[PAGE_SIZE];
			for (int i = 0; i < PAGE_SIZE; i++)
				zeroPAD[i] = 255;
			//char *zeroPAD = AllOneFill(PAGE_SIZE);
			fileManager->write(nowDataBaseHandle, nowGAM, zeroPAD);
			//delete[]zeroPAD;
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
		nowGAM += (PAGE_SIZE * BYTE_SIZE);
	}
	totPageNum = fileManager->size(nowDataBaseHandle);
	if (nowGAM + offset >= totPageNum) 
		fileManager->alloc(nowDataBaseHandle, nowGAM + offset - totPageNum + 1); //扩展文件

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

int RMManager::CreateDataBase(const char* dbName) {
	string dataBaseName(dbName);
	dataBaseName.append(DBSuffixName); //添加后缀名
	if (fileManager->ifexist(dataBaseName))
		return -2; //已经存在
	int64_t fileHandle = fileManager->open(dataBaseName);
	fileManager->alloc(fileHandle, 1);
	char* write = AllZeroFill(PAGE_SIZE); //全0页
	int64_t ret = fileManager->write(fileHandle, 0, write);
	delete[]write;
	fileManager->close(fileHandle);
	if (ret < 0)
		return -1;
	else 
		return 0;
}

int RMManager::ChangeDataBase(const char* dbName) {
	string dataBaseName(dbName);
	dataBaseName.append(DBSuffixName);
	if (!fileManager->ifexist(dataBaseName))
		return -1;
	nowDataBaseHandle = fileManager->open(dataBaseName);
	nowDataBaseName = string(dbName);
//	nowDataBasePageNum = fileManager->size(nowDataBaseHandle);
	return nowDataBaseHandle;
}

int RMManager::CreateTable(/*char *tableNum,*/ TableInfomation tableInfo) {
	if ((nowDataBaseHandle == 0) || (nowDataBaseName == "")) //当前没有数据库
		return -1;

	char buffer[PAGE_SIZE];
	fileManager->read(nowDataBaseHandle, 0, buffer); //0表示库页

	//To do 没有加重复表的判断

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
	writeStr(tableInfoPlace + TABLE_HEAD_PLACE_LEN, strlen(tableInfo.tableName), tableInfo.tableName, TABLE_NAME_LEN);
	//写表头页
	fileManager->write(nowDataBaseHandle, 0, buffer); //0表示库页

	int offset = 0;
	//写页号
	writeNum(buffer + offset, PAGE_RANK_LEN, TableHeadPageRank);
	offset += PAGE_RANK_LEN;
	//写表名
	writeStr(buffer + offset, TABLE_NAME_LEN, tableInfo.tableName, TABLE_NAME_LEN);
	offset += TABLE_NAME_LEN;
	//写数据页首页
	writeNum(buffer + offset, PAGE_RANK_LEN, FirstDataPageRank);
	offset += PAGE_RANK_LEN;
	//写第一有空页
	writeNum(buffer + offset, PAGE_RANK_LEN, FirstDataPageRank);
	offset += PAGE_RANK_LEN;
	//写id上限
	writeNum(buffer + offset, IDLEN, 0);
	offset += IDLEN;
	//写列数
	writeNum(buffer + offset, COLUMN_NUM_LEN, tableInfo.columnNum);
	offset += COLUMN_NUM_LEN;
	//写列信息
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
	//写CheckLimit
	writeStr(buffer + CHECK_LIMIT_PLACE, CHECK_LIMIT_LEN, tableInfo.checkLimit);
	fileManager->write(nowDataBaseHandle, TableHeadPageRank, buffer);

	//写数据首页
	offset = 0;
	writeNum(buffer + offset, PAGE_RANK_LEN, TableHeadPageRank);
	offset += PAGE_RANK_LEN;
	writeNum(buffer + offset, PAGE_RANK_LEN, FirstDataPageRank);
	offset += PAGE_RANK_LEN;
	writeNum(buffer + offset, PAGE_RANK_LEN, NOPAGE);
	offset += PAGE_RANK_LEN;
	writeNum(buffer + offset, PAGE_RANK_LEN, NOPAGE);
	offset += PAGE_RANK_LEN;
	writeNum(buffer + offset, PAGE_RANK_LEN, NOPAGE); //下一有空页
	offset += PAGE_RANK_LEN;

	int fixedColumnNum = 0;
	int fixedColumnLen = 0;
	int totLen = 0;
	fixedColumnNum = tableInfo.columnNum;
	for (int i = 0; i < tableInfo.columnNum; i++) {
		int dataLen = charToNum(tableInfo.columns[i].columnLen, COLUMN_LEN_LEN);
		fixedColumnLen += dataLen;
	}
	totLen = (fixedColumnNum - 1) / BYTE_SIZE + 1 + fixedColumnLen;
	writeNum(buffer + offset, FIXEDCOLUMNNUM_LEN, fixedColumnNum);
	offset += FIXEDCOLUMNNUM_LEN;
	writeNum(buffer + offset, FIXEDCOLUMNLEN_LEN, fixedColumnLen);
	offset += FIXEDCOLUMNLEN_LEN;
	writeNum(buffer + offset, RECORDLENLEN, totLen);
	offset += RECORDLENLEN;
	writeNum(buffer + offset, FIRSTSLOTLEN, 0); //一开始没有记录
	offset += FIRSTSLOTLEN;
	//在该页上形成空槽链
	int nowSlot = 0;
	while (offset + totLen <= PAGE_SIZE) {
		if (offset + totLen + totLen <= PAGE_SIZE)
			writeNum(buffer + offset, FIRSTSLOTLEN, changeToFreeSlot(nowSlot + 1));
		else
			writeNum(buffer + offset, FIRSTSLOTLEN, changeToFreeSlot(NOSLOT));
		offset += totLen;
		nowSlot++;
	}

	fileManager->write(nowDataBaseHandle, FirstDataPageRank, buffer);

//	delete[]buffer;
	
	return tableNum - 1;
}

int RMManager::GetRecord(int pageRank, int slotRank, char* result, int& recordLen) {
	if ((nowDataBaseHandle == 0) || (nowDataBaseName == "")) //当前没有数据库
		return -1;

	char buffer[PAGE_SIZE];
	int ret = fileManager->read(nowDataBaseHandle, pageRank, buffer);
	if (ret < 0) { //没有这一页
//		delete[]buffer;
		return -2;
	}

	int offset = PAGE_RANK_LEN * 5 + FIXEDCOLUMNNUM_LEN + FIXEDCOLUMNLEN_LEN;
	recordLen = charToNum(buffer + offset, RECORDLENLEN);
	offset += (RECORDLENLEN + FIRSTSLOTLEN);
	if (offset + recordLen * (slotRank + 1) > PAGE_SIZE) { //没有这条记录
//		delete[]buffer;
		return -3;
	}
	offset += recordLen * slotRank;
	int tmp = charToNum(buffer + offset, IDLEN); //检查是否是空槽
	
	if (isFreeSlot(tmp)) {
//		delete[]buffer;
		return -4; //该槽已被删除了
	}

	writeStr(buffer + offset, recordLen, result);
//	delete[]buffer;
	return 0;
}

int RMManager::InsertRecord(int headPlace, int& pageRank, int& slotRank, char* record, int recordLen, char* bufferTable) {
	if ((nowDataBaseHandle == 0) || (nowDataBaseName == "")) //当前没有数据库
		return -1;

	char buffer[PAGE_SIZE];
//	char bufferTable[PAGE_SIZE];
	int offset;
	/*
	//先找到表头页
	fileManager->read(nowDataBaseHandle, 0, buffer); //0表示库页

	int tableNum = charToNum(buffer, TABLE_NUM_LEN);
	int offset = TABLE_NUM_LEN;
	if (tableRank >= tableNum)
		return -2; //没有这一表
	int headPlace = -1; //寻找的表的表头页

	offset += tableRank * (TABLE_NAME_LEN + TABLE_HEAD_PLACE_LEN);
	headPlace = charToNum(buffer + offset, TABLE_HEAD_PLACE_LEN);
	*/
	//读表头页
	//fileManager->read(nowDataBaseHandle, headPlace, bufferTable);

	offset = PAGE_RANK_LEN + TABLE_NAME_LEN + PAGE_RANK_LEN;
	pageRank = charToNum(bufferTable + offset, PAGE_RANK_LEN);
	//新建立一页
	if (pageRank == NOPAGE) {
		int tmp = charToNum(bufferTable + PAGE_RANK_LEN + TABLE_NAME_LEN, PAGE_RANK_LEN);
		fileManager->read(nowDataBaseHandle, tmp, buffer);
		pageRank = FindFreePage();
		offset = PAGE_RANK_LEN;
		writeNum(buffer + offset, PAGE_RANK_LEN, pageRank);
		offset += 2 * PAGE_RANK_LEN;
		writeNum(buffer + offset, PAGE_RANK_LEN, tmp); //链入表中
		writeNum(bufferTable + PAGE_RANK_LEN + TABLE_NAME_LEN, PAGE_RANK_LEN, pageRank);
		writeNum(bufferTable + PAGE_RANK_LEN + TABLE_NAME_LEN + PAGE_RANK_LEN, PAGE_RANK_LEN, pageRank); //更新下一有空页
		offset += PAGE_RANK_LEN;
		writeNum(buffer + offset, PAGE_RANK_LEN, NOPAGE);
		offset += PAGE_RANK_LEN;
		offset += FIXEDCOLUMNNUM_LEN + FIXEDCOLUMNLEN_LEN + RECORDLENLEN;
		int totLen = charToNum(buffer + offset - RECORDLENLEN, RECORDLENLEN);
		writeNum(buffer + offset, FIRSTSLOTLEN, 0); //一开始没有记录
		offset += FIRSTSLOTLEN;
		int nowSlot = 0;
		while (offset + totLen <= PAGE_SIZE) {
			if (offset + totLen + totLen <= PAGE_SIZE)
				writeNum(buffer + offset, FIRSTSLOTLEN, changeToFreeSlot(nowSlot + 1));
			else
				writeNum(buffer + offset, FIRSTSLOTLEN, changeToFreeSlot(NOSLOT));
			offset += totLen;
			nowSlot++;
		}
	} else
		fileManager->read(nowDataBaseHandle, pageRank, buffer); //读数据页
	offset = PAGE_RANK_LEN * 5 + FIXEDCOLUMNLEN_LEN + FIXEDCOLUMNNUM_LEN + RECORDLENLEN;
	slotRank = charToNum(buffer + offset, FIRSTSLOTLEN);
	offset += FIRSTSLOTLEN;
	offset += slotRank * recordLen;
	//下一有空槽
	int nextSlot = getSlot(charToNum(buffer + offset, FIRSTSLOTLEN));
	writeStr(buffer + offset, recordLen, record);
	offset = PAGE_RANK_LEN * 5 + FIXEDCOLUMNLEN_LEN + FIXEDCOLUMNNUM_LEN + RECORDLENLEN;
	writeNum(buffer + offset, FIRSTSLOTLEN, nextSlot);
	//该页的空槽已被用完
	if (nextSlot == NOSLOT) {
		int tmp = charToNum(buffer + PAGE_RANK_LEN * 4, PAGE_RANK_LEN);
		writeNum(bufferTable + PAGE_RANK_LEN + TABLE_NAME_LEN + PAGE_RANK_LEN, PAGE_RANK_LEN, tmp);
	}
	//fileManager->write(nowDataBaseHandle, headPlace, bufferTable);
	fileManager->write(nowDataBaseHandle, pageRank, buffer); 
	return 0; 
}

int RMManager::DeleteRecord(int headPlace, int pageRank, int slotRank) {
	if ((nowDataBaseHandle == 0) || (nowDataBaseName == "")) //当前没有数据库
		return -1;

	char buffer[PAGE_SIZE];
	int ret = fileManager->read(nowDataBaseHandle, pageRank, buffer);
	if (ret < 0) { //没有这一页
		//		delete[]buffer;
		return -2;
	}

	int offset = PAGE_RANK_LEN * 5 + FIXEDCOLUMNNUM_LEN + FIXEDCOLUMNLEN_LEN;
	int recordLen = charToNum(buffer + offset, RECORDLENLEN);
	offset += (RECORDLENLEN + FIRSTSLOTLEN);
	if (offset + recordLen * (slotRank + 1) > PAGE_SIZE) { //没有这条记录
		//		delete[]buffer;
		return -3;
	}

	offset += slotRank * recordLen;
	int tmp = charToNum(buffer + offset, IDLEN); //检查是否是空槽

	if (isFreeSlot(tmp)) {
		return -4; //该槽已被删除了
	}
	//更新空槽位
	int firstFreeSlot = charToNum(buffer + PAGE_RANK_LEN * 5 + FIXEDCOLUMNLEN_LEN + FIXEDCOLUMNNUM_LEN, FIRSTSLOTLEN);
	writeNum(buffer + offset, FIRSTSLOTLEN, changeToFreeSlot(firstFreeSlot));
	writeNum(buffer + PAGE_RANK_LEN * 5 + FIXEDCOLUMNLEN_LEN + FIXEDCOLUMNNUM_LEN, FIRSTSLOTLEN, slotRank);

	if (firstFreeSlot == NOSLOT) {
		char bufferTable[PAGE_SIZE];
		fileManager->read(nowDataBaseHandle, headPlace, bufferTable);
		int firstFreePage = charToNum(bufferTable + PAGE_RANK_LEN + TABLE_NAME_LEN + PAGE_RANK_LEN, PAGE_RANK_LEN);
		writeNum(bufferTable + PAGE_RANK_LEN + TABLE_NAME_LEN + PAGE_RANK_LEN, PAGE_RANK_LEN, pageRank);
		writeNum(buffer + PAGE_RANK_LEN * 4, PAGE_RANK_LEN, firstFreePage);
		fileManager->write(nowDataBaseHandle, headPlace, bufferTable);
	}

	return 0;
}


int RMManager::UpdateRecord(int pageRank, int slotRank, char* record, int recordLen) {
	if ((nowDataBaseHandle == 0) || (nowDataBaseName == "")) //当前没有数据库
		return -1;

	char buffer[PAGE_SIZE];
	int ret = fileManager->read(nowDataBaseHandle, pageRank, buffer);
	if (ret < 0) { //没有这一页
//		delete[]buffer;
		return -2;
	}

	int offset = PAGE_RANK_LEN * 5 + FIXEDCOLUMNNUM_LEN + FIXEDCOLUMNLEN_LEN;
	recordLen = charToNum(buffer + offset, RECORDLENLEN);
	offset += (RECORDLENLEN + FIRSTSLOTLEN);
	if (offset + recordLen * (slotRank + 1) > PAGE_SIZE) { //没有这条记录
		//		delete[]buffer;
		return -3;
	}
	offset += recordLen * slotRank;
	writeStr(buffer + offset, recordLen, record);
	fileManager->write(nowDataBaseHandle, pageRank, buffer);
	return 0;
}


void RMManager::freePage(vector<int>& pageRanks) {
	int nowGAM = -1;
	char buffer[PAGE_SIZE];
	//可以排序或许可以更快
	for (int i = 0; i < pageRanks.size(); i++) {
		int GAMPage = ((pageRanks[i] - 1) / (PAGE_SIZE * BYTE_SIZE)) * (PAGE_SIZE * BYTE_SIZE) + 1;
		int offset = 0;
		if (GAMPage != nowGAM) {
			if (nowGAM > 0)
				fileManager->write(nowDataBaseHandle, nowGAM, buffer);
			nowGAM = GAMPage;
			fileManager->read(nowDataBaseHandle, nowGAM, buffer);
		}
		offset = pageRanks[i] - nowGAM;
		buffer[offset / BYTE_SIZE] |= 1 << (BYTE_SIZE - 1 - offset % BYTE_SIZE);
	}
	if (nowGAM > 0)
		fileManager->write(nowDataBaseHandle, nowGAM, buffer);
}

void RMManager::destroyTable(int tableHead) {
	char buffer[PAGE_SIZE];
	fileManager->read(nowDataBaseHandle, tableHead, buffer);
	tmpWorker.clear();
	tmpWorker.push_back(tableHead);
	int nowPage = charToNum(buffer + PAGE_RANK_LEN + TABLE_NAME_LEN, PAGE_RANK_LEN);
	while (true) {
		if (nowPage == NOPAGE)
			break;
		tmpWorker.push_back(nowPage);
		fileManager->read(nowDataBaseHandle, nowPage, buffer);
		nowPage = charToNum(buffer + PAGE_RANK_LEN * 3, PAGE_RANK_LEN);
	}
	freePage(tmpWorker);
}