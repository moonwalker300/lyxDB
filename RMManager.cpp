#include "RMManager.h"
#include <string>
#include "tool.h"
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

int RMManager::CreateDataBase(char* dbName) {
	string dataBaseName(dbName);
	dataBaseName.append(DBSuffixName); //添加后缀名
	if (fileManager->ifexist(dataBaseName))
		return -2; //已经存在
	int64_t fileHandle = fileManager->open(dataBaseName);
	char* write = AllZeroFill(PAGE_SIZE); //全0页
	int64_t ret = fileManager->write(fileHandle, 0, write);
	delete[]write;
	fileManager->close(fileHandle);
	if (ret < 0)
		return -1;
	else 
		return 0;
}

int RMManager::ChangeDataBase(char* dbName) {
	string dataBaseName(dbName);
	dataBaseName.append(DBSuffixName);
	if (!fileManager->ifexist(dataBaseName))
		return -1;
	nowDataBaseHandle = fileManager->open(dataBaseName);
	nowDataBaseName = string(dbName);
//	nowDataBasePageNum = fileManager->size(nowDataBaseHandle);
	return nowDataBaseHandle;
}

int RMManager::CreateTable(/*char *tableNum,*/ TableInfo tableInfo) {
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
	int varColumnNum = 0;
	for (int i = 0; i < tableInfo.columnNum; i++) {
		int dataKind = charToNum(tableInfo.columns[i].columnProperty, COLUMN_PROPERTY_LEN);
		if (dataKind != KIND_VARCHAR) {
			fixedColumnNum++;
			int dataLen = charToNum(tableInfo.columns[i].columnLen, COLUMN_LEN_LEN);
			fixedColumnLen += dataLen;
		}
		else
			varColumnNum++;
	}
	writeNum(buffer + offset, FIXEDCOLUMNNUM_LEN, fixedColumnNum);
	offset += FIXEDCOLUMNNUM_LEN;
	writeNum(buffer + offset, FIXEDCOLUMNLEN_LEN, fixedColumnLen);
	offset += FIXEDCOLUMNLEN_LEN;
	writeNum(buffer + offset, VARCOLUMNNUM_LEN, varColumnNum);
	offset += VARCOLUMNNUM_LEN;
	writeNum(buffer + offset, RECORDNUMLEN, 0); //一开始没有记录
	offset += RECORDNUMLEN;
	writeNum(buffer + offset, FREEBOUNDLEN, PAGE_SIZE); //右侧边界
	offset += FREEBOUNDLEN;

	fileManager->write(nowDataBaseHandle, FirstDataPageRank, buffer);

//	delete[]buffer;
	
	return 0;
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

	int offset = PAGE_RANK_LEN * 5 + FIXEDCOLUMNNUM_LEN + FIXEDCOLUMNLEN_LEN + VARCOLUMNNUM_LEN + FREEBOUNDLEN;
	int recordNum = charToNum(buffer + offset, RECORDNUMLEN);
	offset += RECORDNUMLEN;
	if (recordNum <= slotRank) { //没有这条记录
//		delete[]buffer;
		return -3;
	}
	offset += (RECORDSIZELEN + RECORDPLACELEN) * slotRank;
	int recordPlace = charToNum(buffer + offset, RECORDPLACELEN);
	int recordSize = charToNum(buffer + offset + RECORDPLACELEN, RECORDSIZELEN);
	
	if ((recordPlace = DELETEDPLACE) || (recordPlace == DELETEDSIZE)) {
//		delete[]buffer;
		return -4; //该槽已被删除了
	}

	writeStr(buffer + recordPlace, recordSize, result, recordSize);
	recordLen = recordSize;

//	delete[]buffer;
	return 0;
}

int RMManager::InsertRecord(int pageRank, int& slotRank, char* record, int recordLen) {
	if ((nowDataBaseHandle == 0) || (nowDataBaseName == "")) //当前没有数据库
		return -1;

	char buffer[PAGE_SIZE];
	int ret = fileManager->read(nowDataBaseHandle, pageRank, buffer);
	if (ret < 0) { //没有这一页
//		delete[]buffer;
		return -2;
	}

	int offset = PAGE_RANK_LEN * 5 + FIXEDCOLUMNNUM_LEN + FIXEDCOLUMNLEN_LEN + VARCOLUMNNUM_LEN;
	int freeRightBound = charToNum(buffer + offset, FREEBOUNDLEN);
	offset += FREEBOUNDLEN;
	int recordNum = charToNum(buffer + offset, RECORDNUMLEN);
	offset += RECORDNUMLEN;

	slotRank = recordNum;
	for (int i = 0; i < recordNum; i++) {
		int recordPlace = charToNum(buffer + offset + i * (RECORDPLACELEN + RECORDSIZELEN), RECORDPLACELEN);
		if (recordPlace == DELETEDPLACE) {
			slotRank = i;
			break;
		}
	}
	int remainSpace;
	if (slotRank == recordNum) {
		int freeLeftBound = offset + (slotRank + 1) * (RECORDPLACELEN + RECORDSIZELEN);
		if (freeRightBound - freeLeftBound < recordLen) {
//			delete[]buffer;
			return -3; //没有空余空间了
		}
		offset += slotRank * (RECORDPLACELEN + RECORDSIZELEN);
		int recordStartPlace = freeLeftBound - recordLen;
		writeNum(buffer + offset, RECORDPLACELEN, recordStartPlace);
		offset += RECORDPLACELEN;
		writeNum(buffer + offset, RECORDSIZELEN, recordLen);
		offset += RECORDSIZELEN;
		writeStr(buffer + recordStartPlace, recordLen, record, recordLen);
		//返回去写记录数
		offset = PAGE_RANK_LEN * 5 + FIXEDCOLUMNNUM_LEN + FIXEDCOLUMNLEN_LEN + VARCOLUMNNUM_LEN + FREEBOUNDLEN;
		writeNum(buffer + offset, RECORDNUMLEN, recordNum + 1);
		//写空区域右边界
		offset -= FREEBOUNDLEN;
		writeNum(buffer + offset, FREEBOUNDLEN, recordStartPlace);

		fileManager->write(nowDataBaseHandle, pageRank, buffer);
		
		remainSpace = recordStartPlace - freeLeftBound;
	}
	else{
		int freeLeftBound = offset + recordNum * (RECORDPLACELEN + RECORDSIZELEN);
		if (freeRightBound - freeLeftBound < recordLen) {
//			delete[]buffer;
			return -3; //没有空余空间了
		}
		offset += slotRank * (RECORDPLACELEN + RECORDSIZELEN);
		int recordStartPlace = freeLeftBound - recordLen;
		writeNum(buffer + offset, RECORDPLACELEN, recordStartPlace);
		offset += RECORDPLACELEN;
		writeNum(buffer + offset, RECORDSIZELEN, recordLen);
		offset += RECORDSIZELEN;
		writeStr(buffer + recordStartPlace, recordLen, record, recordLen);
		//无需写记录数

		//写空区域右边界
		offset -= FREEBOUNDLEN;
		writeNum(buffer + offset, FREEBOUNDLEN, recordStartPlace);
		
		fileManager->write(nowDataBaseHandle, pageRank, buffer);
		
		remainSpace = recordStartPlace - freeLeftBound;
	}
//	delete[]buffer;
	return remainSpace; //返回剩余空间
}

int RMManager::DeleteRecord(int pageRank, int slotRank) {
	if ((nowDataBaseHandle == 0) || (nowDataBaseName == "")) //当前没有数据库
		return -1;
	
	char buffer[PAGE_SIZE];
	int ret = fileManager->read(nowDataBaseHandle, pageRank, buffer);
	if (ret < 0) { //没有这一页
//		delete[]buffer;
		return -2;
	}
	int offset = PAGE_RANK_LEN * 5 + FIXEDCOLUMNNUM_LEN + FIXEDCOLUMNLEN_LEN + VARCOLUMNNUM_LEN;
	int recordStartPlace = charToNum(buffer + offset, FREEBOUNDLEN);
	offset += FREEBOUNDLEN;
	int recordNum = charToNum(buffer + offset, RECORDNUMLEN);
	if (recordNum <= slotRank) {
//		delete[]buffer;
		return -3;
	}

	offset += RECORDNUMLEN;
	offset += slotRank * (RECORDPLACELEN + RECORDSIZELEN);
	int recordPlace = charToNum(buffer + offset, RECORDPLACELEN);
	offset += RECORDPLACELEN;
	int recordSize = charToNum(buffer + offset, RECORDSIZELEN);
	offset += RECORDSIZELEN;
	if ((recordPlace == DELETEDPLACE) || (recordSize == DELETEDSIZE)) {
		delete[]buffer;
		return -3;
	}

	offset -= (RECORDPLACELEN + RECORDSIZELEN);
	writeNum(buffer + offset, RECORDPLACELEN, DELETEDPLACE);
	offset += RECORDPLACELEN;
	writeNum(buffer + offset, RECORDSIZELEN, DELETEDSIZE);
	//移动数据
	moveStr(buffer, recordStartPlace, recordStartPlace + recordSize, recordPlace - recordStartPlace);
	//更新其他槽的起始位置
	offset = PAGE_RANK_LEN * 5 + FIXEDCOLUMNNUM_LEN + FIXEDCOLUMNLEN_LEN + VARCOLUMNNUM_LEN;
	offset += FREEBOUNDLEN;
	offset += RECORDNUMLEN;
	for (int i = 0; i < recordNum; i++) {
		//其他被移动槽的起始位置和大小
		int recordPlace2 = charToNum(buffer + offset + i * (RECORDPLACELEN + RECORDSIZELEN), RECORDPLACELEN);
		int recordSize2 = charToNum(buffer + offset + i * (RECORDPLACELEN + RECORDSIZELEN), RECORDPLACELEN);
		if ((recordPlace2 != DELETEDPLACE) && (recordSize2 != DELETEDSIZE))
			if (recordPlace2 < recordPlace + recordSize)
				writeNum(buffer + offset + i * (RECORDPLACELEN + RECORDSIZELEN), RECORDPLACELEN, recordPlace2 + recordSize);
	}

	//更新空闲右侧边界
	offset = PAGE_RANK_LEN * 5 + FIXEDCOLUMNNUM_LEN + FIXEDCOLUMNLEN_LEN + VARCOLUMNNUM_LEN;
	writeNum(buffer + offset, FREEBOUNDLEN, recordStartPlace + recordSize);

	int freeStart = (PAGE_RANK_LEN * 5 + FIXEDCOLUMNNUM_LEN + FIXEDCOLUMNLEN_LEN + VARCOLUMNNUM_LEN + FREEBOUNDLEN + RECORDNUMLEN + recordNum * (RECORDPLACELEN + RECORDSIZELEN));
	int remain = recordStartPlace + recordSize - freeStart;

	fileManager->write(nowDataBaseHandle, pageRank, buffer);

//	delete[]buffer;
	return remain;
}

int RMManager::GetFreeSpace(int pageRank) {
	if ((nowDataBaseHandle == 0) || (nowDataBaseName == "")) //当前没有数据库
		return -1;

	char buffer[PAGE_SIZE];
	int ret = fileManager->read(nowDataBaseHandle, pageRank, buffer);
	if (ret < 0) { //没有这一页
//		delete[]buffer;
		return -2;
	}

	int offset = PAGE_RANK_LEN * 5 + FIXEDCOLUMNNUM_LEN + FIXEDCOLUMNLEN_LEN + VARCOLUMNNUM_LEN;
	int freeRightBound = charToNum(buffer + offset, FREEBOUNDLEN);
	offset += FREEBOUNDLEN;
	int recordNum = charToNum(buffer + offset, RECORDNUMLEN);
	offset += RECORDNUMLEN;

	int freeLeftBound = offset + recordNum * (RECORDPLACELEN + RECORDSIZELEN);

//	delete[]buffer;
	return freeRightBound - freeLeftBound;
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

	int offset = PAGE_RANK_LEN * 5 + FIXEDCOLUMNLEN_LEN + FIXEDCOLUMNNUM_LEN + VARCOLUMNNUM_LEN;
	int recordStartPlace = charToNum(buffer + offset, FREEBOUNDLEN);
	offset += FREEBOUNDLEN;
	int recordNum = charToNum(buffer + offset, RECORDNUMLEN);
	offset += RECORDNUMLEN;
	int freeLeftBound = offset + recordNum * (RECORDPLACELEN + RECORDSIZELEN);

	if (recordNum <= slotRank) {
//		delete[]buffer;
		return -3;
	}

	offset += slotRank * (RECORDPLACELEN + RECORDSIZELEN);
	int recordPlace = charToNum(buffer + offset, RECORDPLACELEN);
	offset += RECORDPLACELEN;
	int recordSize = charToNum(buffer + offset, RECORDSIZELEN); //原长度
	offset += RECORDSIZELEN;

	if ((recordPlace == DELETEDPLACE) || (recordSize == DELETEDSIZE)) {
//		delete[]buffer;
		return -3;
	}

	if (recordStartPlace - freeLeftBound > recordLen - recordSize) {
		//移动其他数据
		moveStr(buffer, recordStartPlace, recordStartPlace - (recordLen - recordSize), recordPlace - recordStartPlace);
		//写入新数据
		writeStr(buffer + recordPlace + recordSize - recordLen, recordLen, record, recordLen);
		//更新位置
		offset = PAGE_RANK_LEN * 5 + FIXEDCOLUMNNUM_LEN + FIXEDCOLUMNLEN_LEN + VARCOLUMNNUM_LEN;
		offset += FREEBOUNDLEN;
		offset += RECORDNUMLEN;
		for (int i = 0; i < recordNum; i++) {
			//其他被移动槽的起始位置和大小
			int recordPlace2 = charToNum(buffer + offset + i * (RECORDPLACELEN + RECORDSIZELEN), RECORDPLACELEN);
			int recordSize2 = charToNum(buffer + offset + i * (RECORDPLACELEN + RECORDSIZELEN), RECORDPLACELEN);
			if ((recordPlace2 != DELETEDPLACE) && (recordSize2 != DELETEDSIZE))
				if (recordPlace2 < recordPlace + recordSize)
					writeNum(buffer + offset + i * (RECORDPLACELEN + RECORDSIZELEN), RECORDPLACELEN, recordPlace2 - (recordLen - recordSize));
		}
		writeNum(buffer + offset + slotRank * (RECORDPLACELEN + RECORDSIZELEN), RECORDPLACELEN, recordPlace - (recordLen - recordSize));
		writeNum(buffer + offset + slotRank * (RECORDPLACELEN + RECORDSIZELEN) + RECORDPLACELEN, RECORDNUMLEN, recordLen);
		fileManager->write(nowDataBaseHandle, pageRank, buffer);

		int remainSpace = recordStartPlace - freeLeftBound - (recordLen - recordSize);
//		delete[]buffer;
		return remainSpace; //返回剩余空间
	}
	else{
//		delete[]buffer;
		return -4; //页的空余长度不够了
	}
}




 