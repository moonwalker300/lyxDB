#include "IXManager.h"
#include "tool.h"

using namespace std;
int IXManager::FindFreePage() {
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
			fileManager->write(nowDataBaseHandle, nowGAM, zeroPAD);
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

int IXManager::CreateIndex(int tableRank, int columnRank) {
	if ((nowDataBaseHandle == 0) || (nowDataBaseName == "")) //当前没有数据库
		return -1;
	
	char buffer[PAGE_SIZE];
	fileManager->read(nowDataBaseHandle, 0, buffer); //0表示库页
	int offset = TABLE_NUM_LEN;
	int tableHeadPlace = charToNum(buffer + offset + (TABLE_HEAD_PLACE_LEN + TABLE_NAME_LEN) * tableRank, TABLE_HEAD_PLACE_LEN); //查找的表位置

	fileManager->read(nowDataBaseHandle, tableHeadPlace, buffer);
	offset = PAGE_RANK_LEN + TABLE_NAME_LEN + PAGE_RANK_LEN * 2 + IDLEN;
	int columnNum = charToNum(buffer + offset, COLUMN_NUM_LEN);
	offset += COLUMN_NUM_LEN;
	int totColumnInfoLen = COLUMN_NAME_LEN + COLUMN_PROPERTY_LEN + COLUMN_KIND_LEN + COLUMN_LEN_LEN + INDEX_PLACE_LEN;
	offset += totColumnInfoLen * columnRank;
	int indexKind = 0;
	int indexLen = 0;
	
	indexKind = charToNum(buffer + offset + COLUMN_NAME_LEN + COLUMN_PROPERTY_LEN, COLUMN_KIND_LEN);
	indexLen = charToNum(buffer + offset + COLUMN_NAME_LEN + COLUMN_PROPERTY_LEN + COLUMN_KIND_LEN, COLUMN_LEN_LEN);	

	if (*(buffer + offset + COLUMN_NAME_LEN + INDEXPLACE) == 1)
		return -4; //原来就有索引
	//写上有索引
	*(buffer + offset + COLUMN_NAME_LEN + INDEXPLACE) = ISINDEX;
	//加上索引根的位置
	int indexRoot = FindFreePage();
	writeNum(buffer + offset + COLUMN_NAME_LEN + COLUMN_PROPERTY_LEN + COLUMN_KIND_LEN + COLUMN_LEN_LEN, INDEX_PLACE_LEN, indexRoot);

	fileManager->write(nowDataBaseHandle, tableHeadPlace, buffer);
	writeNum(buffer, PAGE_RANK_LEN, 0);
	offset = PAGE_RANK_LEN;
	writeNum(buffer + offset, PAGE_RANK_LEN, indexRoot);
	offset += PAGE_RANK_LEN;
	//这是根
	writeNum(buffer + offset, ISROOTLEN, ISROOT);
	offset += ISROOTLEN;
	writeNum(buffer + offset, ISLEAFLEN, ISLEAF);
	offset += ISLEAFLEN;
	int n = (PAGE_SIZE - 32) / (1 + indexLen + IDLEN + NOTLEAFPAGELEN) - 1; //加上NULL标志的长度
	writeNum(buffer + offset, NLEN, n);
	offset += NLEN;
	writeNum(buffer + offset, INDEXNUMLEN, 0);
	offset += INDEXNUMLEN;
	writeNum(buffer + offset, INDEXKINDLEN, indexKind);
	offset += INDEXKINDLEN;
	writeNum(buffer + offset, INDEXLENLEN, (indexLen + 1)); //加上NULL标志长度
	offset += INDEXLENLEN;
	writeNum(buffer + offset, LEAFNEXTLEN, NOPAGE);
	fileManager->write(nowDataBaseHandle, indexRoot, buffer);
	//将所有记录全部插入索引中 To Do
	return 0;
}

IXManager::IXManager(FileManager *fm) {
	this->fileManager = fm;
	this->nowDataBaseHandle = 0; //初始没有使用数据库
	this->nowDataBaseName = "";
}

int IXManager::ChangeDataBase(const char* dbName) {
	string dataBaseName(dbName);
	dataBaseName.append(DBSuffixName);
	if (!fileManager->ifexist(dataBaseName))
		return -1;
	nowDataBaseHandle = fileManager->open(dataBaseName);
	nowDataBaseName = string(dbName);
//	nowDataBasePageNum = fileManager->size(nowDataBaseHandle);
	return nowDataBaseHandle;
}

int IXManager::FindIndexRootPage(char* tableName, char* columnName) {
	if ((nowDataBaseHandle == 0) || (nowDataBaseName == "")) //当前没有数据库
		return -1;

	char buffer[PAGE_SIZE];
	fileManager->read(nowDataBaseHandle, 0, buffer); //0表示库页
	
	int tableNum = charToNum(buffer, TABLE_NAME_LEN);
	int offset = TABLE_NAME_LEN;
	char tmpName[TABLE_NAME_LEN];
	int headPlace = -1; //寻找的表的表头页

	for (int i = 0; i < tableNum; i++) {
		int tableHeadPlace = charToNum(buffer + offset, TABLE_HEAD_PLACE_LEN);
		offset += TABLE_HEAD_PLACE_LEN;
		writeStr(tmpName, TABLE_NAME_LEN, buffer + offset, TABLE_NAME_LEN);
		if (compareStr(tmpName, tableName, TABLE_NAME_LEN) == 0) {
			headPlace = tableHeadPlace;
			break;
		}
	}

	if (headPlace == -1) {
//		delete[]buffer;
		return -1; //没有这个表
	}

	fileManager->read(nowDataBaseHandle, headPlace, buffer); //取出表头页
	offset = PAGE_RANK_LEN * 3 + TABLE_NAME_LEN + IDLEN;
	int columnNum = charToNum(buffer + offset, COLUMN_NUM_LEN);
	offset += COLUMN_NUM_LEN;
	
	int indexHeadPlace = -2;
	char tmpColumnName[32];
	char tmpColumnProperty[4];
	for (int i = 0; i < columnNum; i++) {
		writeStr(tmpColumnName, COLUMN_NAME_LEN, buffer + offset + i *  COLUMN_INFO_TOTLEN, COLUMN_NAME_LEN);
		if (compareStr(columnName, tmpColumnName, COLUMN_NAME_LEN) == 0) { //找到同名列
			writeStr(tmpColumnProperty, COLUMN_PROPERTY_LEN, buffer + offset + i * COLUMN_INFO_TOTLEN + COLUMN_NAME_LEN, COLUMN_PROPERTY_LEN);
			if (tmpColumnProperty[INDEXPLACE] == ISINDEX)  //是索引
				indexHeadPlace = charToNum(buffer + offset + i * COLUMN_INFO_TOTLEN + COLUMN_INFO_TOTLEN - INDEX_PLACE_LEN, INDEX_PLACE_LEN);
			else
				indexHeadPlace = -3;
			break;
		}
	}

//	delete[]buffer;
	return indexHeadPlace;
}

int IXManager::SearchIndex(int rootPage, char* index, int indexLen, int dataKind, int& leafPage, int& indexRank, int findBigger) {
	if ((nowDataBaseHandle == 0) || (nowDataBaseName == "")) //当前没有数据库
		return -1;
	
	char buffer[PAGE_SIZE];
	char tmpIndex[256];
	int nowPage = rootPage;
	int offset;
	int indexNum, indexKind;// , indexLen;
	fileManager->read(nowDataBaseHandle, rootPage, buffer);
	offset = PAGE_RANK_LEN + PAGE_RANK_LEN + ISROOTLEN + ISLEAFLEN + NLEN + INDEXNUMLEN;
	indexKind = charToNum(buffer + offset, INDEXKINDLEN);
	offset += INDEXKINDLEN;
	//indexLen = charToNum(buffer + offset, INDEXLENLEN);

	while (true) {
		fileManager->read(nowDataBaseHandle, nowPage, buffer);
		offset = PAGE_RANK_LEN + PAGE_RANK_LEN + ISROOTLEN;
		int isLeaf = charToNum(buffer + offset, ISLEAFLEN);
		if (isLeaf == ISLEAF)
			break;
		offset += (ISLEAFLEN + NLEN);
		indexNum = charToNum(buffer + offset, INDEXNUMLEN);
		offset += INDEXNUMLEN;
		offset += INDEXKINDLEN;
		offset += INDEXLENLEN;
		//遍历该结点上的索引
		
		int findNode = indexNum; //去向第几个儿子指针
		for (int i = 0; i < indexNum; i++) {
			writeStr(tmpIndex, indexLen, buffer + offset + i * (NOTLEAFPAGELEN + indexLen + IDLEN) + NOTLEAFPAGELEN, indexLen);
			if (compareIndex(tmpIndex, index, indexLen) >= 0) {
				findNode = i;
				break;
			}
		}
		nowPage = charToNum(buffer + offset + findNode * (NOTLEAFPAGELEN + indexLen + IDLEN) + NOTLEAFPAGESPACE, PAGE_RANK_LEN);
	}

	int retLeafPage = -1, retLeafRank = -1;
	int Flag = 0, Flag2 = 0;
	while (true) {
		if ((Flag == 1) || (nowPage == NOPAGE)) 
			break;
		
		fileManager->read(nowDataBaseHandle, nowPage, buffer);
		offset = PAGE_RANK_LEN + PAGE_RANK_LEN + ISROOTLEN + ISLEAFLEN + NLEN;
		indexNum = charToNum(buffer + offset, INDEXNUMLEN);
		offset += INDEXNUMLEN;
		offset += INDEXKINDLEN;
		offset += INDEXLENLEN;
		//遍历该结点上的索引

		int findNode = indexNum; //去向第几个儿子指针
		for (int i = 0; i < indexNum; i++) {
			if (Flag2 == 0) {
				writeStr(tmpIndex, indexLen, buffer + offset + i * (LEAFPAGELEN + LEAFSLOTLEN + indexLen + IDLEN) + LEAFPAGELEN + LEAFSLOTLEN, indexLen);
				int compareResult = compareIndex(tmpIndex, index, indexLen);
				if (compareResult > 0) {
					Flag2 = 1;
				}
				else if (compareResult == 0){
					if (findBigger == 0) {
						Flag2 = 1;
					}
				}
			}
			if (Flag2 == 1) {
				if (charToNum(buffer + offset + i * (LEAFPAGELEN + LEAFSLOTLEN + indexLen + IDLEN), LEAFPAGELEN) != NORECORD) {
					Flag = 1;
					retLeafPage = nowPage;
					retLeafRank = i;
					break;
				}
			}
		}
		if (Flag == 0) //切换成下一页
			nowPage = charToNum(buffer + offset + indexNum * (LEAFPAGELEN + LEAFSLOTLEN + indexLen + IDLEN) + LEAFPAGELEN, LEAFPAGELEN);
	}
	
	if ((nowPage == 0) || (retLeafPage == -1)) {
		leafPage = -1;
		indexRank = -1;
		return -2;
	}
	else {
		leafPage = retLeafPage;
		indexRank = retLeafRank;
		return 0;
	}
}

int IXManager::InsertRecordAndIX(int rootPage, char* index, int indexLen, int dataKind, int pageRank, int slotRank, int recordID) {
	char buffer[PAGE_SIZE];
	char tmpIndex[256];
	int nowPage = rootPage;
	int offset;
	int indexNum, indexKind;// , indexLen;
	fileManager->read(nowDataBaseHandle, rootPage, buffer);
	offset = PAGE_RANK_LEN + PAGE_RANK_LEN + ISROOTLEN + ISLEAFLEN + NLEN + INDEXNUMLEN;
	indexKind = charToNum(buffer + offset, INDEXKINDLEN);
	offset += INDEXKINDLEN;
	//indexLen = charToNum(buffer + offset, INDEXLENLEN);
	int yourFather = 0;

	while (true) {
		fileManager->read(nowDataBaseHandle, nowPage, buffer);
		writeNum(buffer, PAGE_RANK_LEN, yourFather);
		fileManager->write(nowDataBaseHandle, nowPage, buffer);
		offset = PAGE_RANK_LEN + PAGE_RANK_LEN + ISROOTLEN;
		int isLeaf = charToNum(buffer + offset, ISLEAFLEN);
		if (isLeaf == ISLEAF)
			break;
		offset += (ISLEAFLEN + NLEN);
		indexNum = charToNum(buffer + offset, INDEXNUMLEN);
		offset += INDEXNUMLEN;
		offset += INDEXKINDLEN;
		offset += INDEXLENLEN;
		//遍历该结点上的索引
		yourFather = nowPage;
		int findNode = indexNum; //去向第几个儿子指针
		for (int i = 0; i < indexNum; i++) {
			writeStr(tmpIndex, indexLen, buffer + offset + i * (NOTLEAFPAGELEN + indexLen + IDLEN) + NOTLEAFPAGELEN, indexLen);
			int tmpID = charToNum(buffer + offset + i * (NOTLEAFPAGELEN + indexLen + IDLEN) + NOTLEAFPAGELEN + indexLen, IDLEN);
			if (compareIndexAndID(tmpIndex, index, indexLen, tmpID, recordID) > 0) {
				findNode = i;
				break;
			}
		}
		nowPage = charToNum(buffer + offset + findNode * (NOTLEAFPAGELEN + indexLen + IDLEN) + NOTLEAFPAGESPACE, PAGE_RANK_LEN);
	}

	return InsertLeaf(nowPage, index, indexLen, dataKind, recordID, pageRank, slotRank);
}

int IXManager::InsertLeaf(int nowPage, char* index, int indexLen, int dataKind, int recordID, int pageRank, int slotRank) {
	char buffer[PAGE_SIZE];
	char tmpIndex[256];
	fileManager->read(nowDataBaseHandle, nowPage, buffer);

	int offset = PAGE_RANK_LEN * 2 + ISROOTLEN + ISLEAFLEN;
	int limit = charToNum(buffer + offset, NLEN);
	offset += NLEN;
	int indexNum = charToNum(buffer + offset, INDEXNUMLEN);
	offset += (INDEXNUMLEN + INDEXKINDLEN + INDEXLENLEN);

	//判断是否可以复活记录
	bool ok = false;
	bool fail = false;
	for (int i = 0; i < indexNum; i++) {
		writeStr(tmpIndex, indexLen, buffer + offset + i * (LEAFPAGELEN + LEAFSLOTLEN + indexLen + IDLEN) + LEAFPAGELEN + LEAFSLOTLEN, indexLen);
		int tmpID = charToNum(buffer + offset + i * (LEAFPAGELEN + LEAFSLOTLEN + indexLen + IDLEN) + LEAFPAGELEN + LEAFSLOTLEN + indexLen, IDLEN);
		if (compareIndexAndID(tmpIndex, index, indexLen, tmpID, recordID) == 0) {
			ok = true;
			int oriPage = charToNum(buffer + offset + i * (LEAFPAGELEN + LEAFSLOTLEN + indexLen + IDLEN), LEAFPAGELEN);
			int oriSlot = charToNum(buffer + offset + i * (LEAFPAGELEN + LEAFSLOTLEN + indexLen + IDLEN) + LEAFPAGELEN, LEAFSLOTLEN);
			if ((oriPage != NORECORD) || (oriSlot != NORECORD)) {
				fail = true;
			}
			else {
				writeNum(buffer + offset + i * (LEAFPAGELEN + LEAFSLOTLEN + indexLen + IDLEN), LEAFPAGELEN, pageRank);
				writeNum(buffer + offset + i * (LEAFPAGELEN + LEAFSLOTLEN + indexLen + IDLEN) + LEAFPAGELEN, LEAFSLOTLEN, slotRank);
				fail = false;
			}
			break;
		}
	}
	if (ok) {
		if (!fail) {
			fileManager->write(nowDataBaseHandle, nowPage, buffer);
			return 0;
		}
		else
			return -1;
	}

	if (indexNum < limit) {
		writeNum(buffer + offset - (INDEXNUMLEN + INDEXKINDLEN + INDEXLENLEN), INDEXNUMLEN, indexNum + 1);

		int findPlace = indexNum;
		//找到合适的插入位置
		for (int i = 0; i < indexNum; i++) {
			writeStr(tmpIndex, indexLen, buffer + offset + i * (LEAFPAGELEN + LEAFSLOTLEN + indexLen + IDLEN) + LEAFPAGELEN + LEAFSLOTLEN, indexLen);
			int tmpID = charToNum(buffer + offset + i * (LEAFPAGELEN + LEAFSLOTLEN + indexLen + IDLEN) + LEAFPAGELEN + LEAFSLOTLEN + indexLen, IDLEN);
			if (compareIndexAndID(tmpIndex, index, indexLen, tmpID, recordID) > 0) {
				findPlace = i;
				break;
			}
		}
		//右边部分右移
		int indexItemLen = LEAFPAGELEN + LEAFSLOTLEN + indexLen + IDLEN;
		moveStr(buffer,	offset + findPlace * indexItemLen, offset + (findPlace + 1) * indexItemLen, (indexNum - findPlace) * indexItemLen + 2 * PAGE_RANK_LEN);
		//写入新的项
		offset = offset + findPlace * indexItemLen;
		writeNum(buffer + offset, LEAFPAGELEN, pageRank);
		offset += LEAFPAGELEN;
		writeNum(buffer + offset, LEAFSLOTLEN, slotRank);
		offset += LEAFSLOTLEN;
		writeStr(buffer + offset, indexLen, index, indexLen);
		offset += indexLen;
		writeNum(buffer + offset, IDLEN, recordID);
		//写回
		fileManager->write(nowDataBaseHandle, nowPage, buffer);

		return 0;
	}
	else {
		if (nowPage == 21) {
			int lyx = 22;
		}
		int freePage = FindFreePage();
		char buffer2[PAGE_SIZE];
		
		int parent = charToNum(buffer, PAGE_RANK_LEN);
//		writeNum(buffer2, PAGE_RANK_LEN, parent);
		offset = PAGE_RANK_LEN;
		writeNum(buffer2 + offset, PAGE_RANK_LEN, freePage);
		offset += PAGE_RANK_LEN;
		writeNum(buffer2 + offset, ISROOTLEN, NOTROOT); //不是根
		offset += ISROOTLEN;
		writeNum(buffer2 + offset, ISLEAFLEN, ISLEAF);
		offset += ISLEAFLEN;
		int n = charToNum(buffer + offset, NLEN);
		writeNum(buffer2 + offset, NLEN, n);
		offset += NLEN;
		
		writeNum(buffer2 + offset + INDEXNUMLEN, INDEXKINDLEN, dataKind);
		writeNum(buffer2 + offset + INDEXNUMLEN + INDEXKINDLEN, INDEXLENLEN, indexLen);

		int part = indexNum / 2;
		int moveoffset = offset + INDEXNUMLEN + INDEXKINDLEN + INDEXLENLEN;

		//找到新索引的位置
		int putPlace = indexNum;
		for (int i = 0; i < indexNum; i++) {
			writeStr(tmpIndex, indexLen, buffer + moveoffset + (LEAFPAGELEN + LEAFSLOTLEN + indexLen + IDLEN) * i + LEAFPAGELEN + LEAFSLOTLEN, indexLen);
			int tmpID = charToNum(buffer + moveoffset + (LEAFPAGELEN + LEAFSLOTLEN + indexLen + IDLEN) * i + LEAFPAGELEN + LEAFSLOTLEN + indexLen, IDLEN);
			if (compareIndexAndID(tmpIndex, index, indexLen, tmpID, recordID) > 0) {
				putPlace = i;
				break;
			}
		}

		//分裂
		if (putPlace < part) {
			int oneRemain = part - putPlace;
			int totLen = (LEAFPAGELEN + LEAFSLOTLEN + indexLen + IDLEN);
			int moveLen = (indexNum + 1 - part) * totLen + LEAFNEXTLEN;
			int moveStart = (part - 1) * totLen;
			
			writeStr(buffer2 + moveoffset, moveLen, buffer + moveoffset + moveStart, moveLen);

			writeNum(buffer2 + offset, INDEXNUMLEN, indexNum + 1 - part);

			moveStr(buffer, moveoffset + putPlace * totLen, moveoffset + putPlace * totLen + totLen, (part - putPlace - 1) * totLen);
			//写新索引项
			writeNum(buffer + moveoffset + putPlace * totLen, LEAFPAGELEN, pageRank);
			writeNum(buffer + moveoffset + putPlace * totLen + LEAFPAGELEN, LEAFSLOTLEN, slotRank);
			int tmpoffset = moveoffset + putPlace * totLen + LEAFPAGELEN;
			tmpoffset += LEAFSLOTLEN;
			writeStr(buffer + tmpoffset, indexLen, index, indexLen);
			tmpoffset += indexLen;
			writeNum(buffer + tmpoffset, IDLEN, recordID);

			writeNum(buffer + offset, INDEXNUMLEN, part);
			writeNum(buffer + moveoffset + part * totLen + LEAFNEXTSPACE, LEAFNEXTLEN - LEAFNEXTSPACE, freePage);

//			fileManager->write(nowDataBaseHandle, nowPage, buffer);
//			fileManager->write(nowDataBaseHandle, freePage, buffer2);

		}
		else {
			int totLen = (LEAFPAGELEN + LEAFSLOTLEN + indexLen + IDLEN);
			int moveLen = (indexNum - part) * totLen + LEAFNEXTLEN;
			int moveStart = part * totLen;

			writeStr(buffer2 + moveoffset, moveLen, buffer + moveoffset + moveStart, moveLen);
			
			writeNum(buffer2 + offset, INDEXNUMLEN, indexNum + 1 - part);
			
			moveStr(buffer2, moveoffset + (putPlace - part) * totLen, moveoffset + (putPlace - part) * totLen + totLen, (indexNum - putPlace) * totLen + LEAFNEXTLEN);
			
			//写新索引项
			writeNum(buffer2 + moveoffset + (putPlace - part) * totLen, LEAFPAGELEN, pageRank);
			writeNum(buffer2 + moveoffset + (putPlace - part) * totLen + LEAFPAGELEN, LEAFSLOTLEN, slotRank);
			int tmpoffset = moveoffset + (putPlace - part) * totLen + LEAFPAGELEN;
			tmpoffset += LEAFSLOTLEN;
			writeStr(buffer2 + tmpoffset, indexLen, index, indexLen);
			tmpoffset += indexLen;
			writeNum(buffer2 + tmpoffset, IDLEN, recordID);
			
			writeNum(buffer + offset, INDEXNUMLEN, part);
			writeNum(buffer + moveoffset + part * totLen + LEAFNEXTSPACE, LEAFNEXTLEN - LEAFNEXTSPACE, freePage);

//			fileManager->write(nowDataBaseHandle, nowPage, buffer);
//			fileManager->write(nowDataBaseHandle, freePage, buffer2);

		}

		writeStr(tmpIndex, indexLen, buffer2 + moveoffset + LEAFPAGELEN + LEAFSLOTLEN, indexLen);
		int id = charToNum(buffer2 + moveoffset + LEAFPAGELEN + LEAFSLOTLEN + indexLen, IDLEN);
		
		if (parent != NOPARENT) {
			writeNum(buffer2, PAGE_RANK_LEN, parent);
			fileManager->write(nowDataBaseHandle, nowPage, buffer);
			fileManager->write(nowDataBaseHandle, freePage, buffer2);
			return InsertNoLeaf(parent, tmpIndex, indexLen, dataKind, id, freePage);
		}
		else {
			int newRoot = FindFreePage();
			char buffer3[PAGE_SIZE];
			//写新的root
			offset = 0;
			writeNum(buffer3, PAGE_RANK_LEN, NOPARENT);
			offset += PAGE_RANK_LEN;
			writeNum(buffer3 + offset, PAGE_RANK_LEN, newRoot);
			offset += PAGE_RANK_LEN;
			writeNum(buffer3 + offset, ISROOTLEN, ISROOT);
			offset += ISROOTLEN;
			writeNum(buffer3 + offset, ISLEAFLEN, NOTLEAF);
			offset += ISLEAFLEN;
			writeNum(buffer3 + offset, NLEN, limit);
			offset += NLEN;

			writeNum(buffer3 + offset, INDEXNUMLEN, 1);
			offset += INDEXNUMLEN;

			writeNum(buffer3 + offset, INDEXKINDLEN, dataKind);
			offset += INDEXKINDLEN;

			writeNum(buffer3 + offset, INDEXLENLEN, indexLen);
			offset += INDEXLENLEN;

			writeNum(buffer3 + offset, NOTLEAFPAGELEN, nowPage);
			offset += NOTLEAFPAGELEN;

			writeStr(buffer3 + offset, indexLen, tmpIndex);
			offset += indexLen;
			writeNum(buffer3 + offset, IDLEN, id);
			offset += IDLEN;
			writeNum(buffer3 + offset, NOTLEAFPAGELEN, freePage);

			writeNum(buffer, PAGE_RANK_LEN, newRoot);
			writeNum(buffer + PAGE_RANK_LEN * 2, ISROOTLEN, NOTROOT);
			writeNum(buffer2, PAGE_RANK_LEN, newRoot);

			fileManager->write(nowDataBaseHandle, nowPage, buffer);
			fileManager->write(nowDataBaseHandle, freePage, buffer2);
			fileManager->write(nowDataBaseHandle, newRoot, buffer3);

			return newRoot;
		}
	}
}

int IXManager::InsertNoLeaf(int nowPage, char* index, int indexLen, int dataKind, int recordid, int newPage) {
	char buffer[PAGE_SIZE];
	char tmpIndex[256];
	fileManager->read(nowDataBaseHandle, nowPage, buffer);

	int offset = PAGE_RANK_LEN * 2 + ISROOTLEN + ISLEAFLEN;
	int limit = charToNum(buffer + offset, NLEN);
	offset += NLEN;
	int indexNum = charToNum(buffer + offset, INDEXNUMLEN);
	offset += (INDEXNUMLEN + INDEXKINDLEN + INDEXLENLEN);

	if (indexNum < limit) {
		writeNum(buffer + offset - (INDEXNUMLEN + INDEXKINDLEN + INDEXLENLEN), INDEXNUMLEN, indexNum + 1);

		int findNode = indexNum;
		for (int i = 0; i < indexNum; i++) {
			writeStr(tmpIndex, indexLen, buffer + offset + (NOTLEAFPAGELEN + indexLen + IDLEN) * i + NOTLEAFPAGELEN, indexLen);
			int id = charToNum(buffer + offset + (NOTLEAFPAGELEN + indexLen + IDLEN) * i + NOTLEAFPAGELEN + indexLen, IDLEN);
			if (compareIndexAndID(tmpIndex, index, indexLen, id, recordid) > 0) {
				findNode = i;
				break;
			}
		}
		//右移
		moveStr(buffer, offset + findNode * (NOTLEAFPAGELEN + indexLen + IDLEN) + NOTLEAFPAGELEN, offset + (findNode + 1) * (NOTLEAFPAGELEN + indexLen + IDLEN) + NOTLEAFPAGELEN, (indexNum - findNode) * (NOTLEAFPAGELEN + indexLen + IDLEN));
		//写新项
		int itemLen = NOTLEAFPAGELEN + indexLen + IDLEN;
		offset = offset + findNode * itemLen + NOTLEAFPAGELEN;
		writeStr(buffer + offset, indexLen, index, indexLen);
		offset += indexLen;
		writeNum(buffer + offset, IDLEN, recordid);
		offset += IDLEN;
		writeNum(buffer + offset, NOTLEAFPAGELEN, newPage);

		//写回
		fileManager->write(nowDataBaseHandle, nowPage, buffer);

		return 0;
	}
	else {
		int parent = charToNum(buffer, PAGE_RANK_LEN);
		char buffer2[PAGE_SIZE];

		int freePage = FindFreePage();
		offset = PAGE_RANK_LEN;
		writeNum(buffer2 + offset, PAGE_RANK_LEN, freePage);
		offset += PAGE_RANK_LEN;
		writeNum(buffer2 + offset, ISROOTLEN, NOTROOT);
		offset += ISROOTLEN;
		writeNum(buffer2 + offset, ISLEAFLEN, NOTLEAF);
		offset += ISLEAFLEN;
		writeNum(buffer2 + offset, NLEN, limit);
		offset += NLEN;
		offset += INDEXNUMLEN;
		writeNum(buffer2 + offset, INDEXKINDLEN, dataKind);
		offset += INDEXKINDLEN;
		writeNum(buffer2 + offset, INDEXLENLEN, indexLen);
		offset += INDEXLENLEN;

		int part = indexNum / 2;
		
		int putPlace = indexNum;
		for (int i = 0; i < indexNum; i++) {
			writeStr(tmpIndex, indexLen, buffer + offset + (NOTLEAFPAGELEN + indexLen + IDLEN) * i + NOTLEAFPAGELEN, indexLen);
			int id = charToNum(buffer + offset + (NOTLEAFPAGELEN + indexLen + IDLEN) * i + NOTLEAFPAGELEN + indexLen, IDLEN);
			if (compareIndexAndID(tmpIndex, index, indexLen, id, recordid) > 0) {
				putPlace = i;
				break;
			}
		}
		//分裂
		int itemLen = NOTLEAFPAGELEN + indexLen + IDLEN;
		char upIndex[256];
		int upPage = freePage;
		int upRecordID;
		if (putPlace == part) {
			writeStr(buffer2 + offset + NOTLEAFPAGELEN, (indexNum - part) * itemLen, buffer + offset + part * itemLen + NOTLEAFPAGELEN, (indexNum - part) * itemLen);
			writeNum(buffer2 + offset, NOTLEAFPAGELEN, newPage);

			offset -= (INDEXLENLEN + INDEXKINDLEN + INDEXNUMLEN);
			writeNum(buffer + offset, INDEXNUMLEN, part);
			writeNum(buffer2 + offset, INDEXNUMLEN, indexNum - part);

			writeStr(upIndex, indexLen, index, indexLen);
			upRecordID = recordid;
		}
		else if (putPlace < part) {
			writeStr(upIndex, indexLen, buffer + offset + (part - 1) * itemLen + NOTLEAFPAGELEN, indexLen);
			upRecordID = charToNum(buffer + offset + (part - 1) * itemLen + NOTLEAFPAGELEN + indexLen, IDLEN);
			writeStr(buffer2 + offset, (indexNum - part) * itemLen + NOTLEAFPAGELEN, buffer + offset + part * itemLen, (indexNum - part) * itemLen + NOTLEAFPAGELEN);

			moveStr(buffer, offset + putPlace * itemLen + NOTLEAFPAGELEN, offset + (putPlace + 1) * itemLen + NOTLEAFPAGELEN, (part - putPlace - 1) * itemLen);
			
			writeStr(buffer + offset + putPlace * itemLen + NOTLEAFPAGELEN, indexLen, index, indexLen);
			writeNum(buffer + offset + putPlace * itemLen + NOTLEAFPAGELEN + indexLen, IDLEN, recordid);
			writeNum(buffer + offset + putPlace * itemLen + NOTLEAFPAGELEN + indexLen + IDLEN, NOTLEAFPAGELEN, newPage);

			offset -= (INDEXLENLEN + INDEXKINDLEN + INDEXNUMLEN);
			writeNum(buffer + offset, INDEXNUMLEN, part);
			writeNum(buffer2 + offset, INDEXNUMLEN, indexNum - part);
		}
		else {
			writeStr(upIndex, indexLen, buffer + offset + part * itemLen + NOTLEAFPAGELEN, indexLen);
			upRecordID = charToNum(buffer + offset + part * itemLen + NOTLEAFPAGELEN + indexLen, IDLEN);
			writeStr(buffer2 + offset, (indexNum - part - 1) * itemLen + NOTLEAFPAGELEN, buffer + offset + (part + 1) * itemLen, (indexNum - part - 1) * itemLen + NOTLEAFPAGELEN);
			  
			moveStr(buffer2, offset + (putPlace - part - 1) * itemLen + NOTLEAFPAGELEN, offset + (putPlace - part) * itemLen + NOTLEAFPAGELEN, (indexNum - putPlace) * itemLen);

			writeStr(buffer2 + offset + (putPlace - part - 1) * itemLen + NOTLEAFPAGELEN, indexLen, index, indexLen);
			writeNum(buffer2 + offset + (putPlace - part - 1) * itemLen + NOTLEAFPAGELEN + indexLen, IDLEN, recordid);
			writeNum(buffer2 + offset + (putPlace - part - 1) * itemLen + NOTLEAFPAGELEN + indexLen + IDLEN, NOTLEAFPAGELEN, newPage);
			offset -= (INDEXLENLEN + INDEXKINDLEN + INDEXNUMLEN);

			writeNum(buffer + offset, INDEXNUMLEN, part);
			writeNum(buffer2 + offset, INDEXNUMLEN, indexNum - part);
		}
		if (parent != NOPARENT) {
			writeNum(buffer2, PAGE_RANK_LEN, parent);

			fileManager->write(nowDataBaseHandle, nowPage, buffer);
			fileManager->write(nowDataBaseHandle, freePage, buffer2);

			return InsertNoLeaf(parent, upIndex, indexLen, dataKind, upRecordID, freePage);
		}
		else {
			writeNum(buffer + PAGE_RANK_LEN * 2, ISROOTLEN, NOTROOT);
			int newRoot = FindFreePage();
			char buffer3[PAGE_SIZE];
			//写新的root
			offset = 0;
			writeNum(buffer3, PAGE_RANK_LEN, NOPARENT);
			offset += PAGE_RANK_LEN;
			writeNum(buffer3 + offset, PAGE_RANK_LEN, newRoot);
			offset += PAGE_RANK_LEN;
			writeNum(buffer3 + offset, ISROOTLEN, ISROOT);
			offset += ISROOTLEN;
			writeNum(buffer3 + offset, ISLEAFLEN, NOTLEAF);
			offset += ISLEAFLEN;
			writeNum(buffer3 + offset, NLEN, limit);
			offset += NLEN;

			writeNum(buffer3 + offset, INDEXNUMLEN, 1);
			offset += INDEXNUMLEN;

			writeNum(buffer3 + offset, INDEXKINDLEN, dataKind);
			offset += INDEXKINDLEN;

			writeNum(buffer3 + offset, INDEXLENLEN, indexLen);
			offset += INDEXLENLEN;

			writeNum(buffer3 + offset, NOTLEAFPAGELEN, nowPage);
			offset += NOTLEAFPAGELEN;

			writeStr(buffer3 + offset, indexLen, upIndex, indexLen);
			offset += indexLen;
			writeNum(buffer3 + offset, IDLEN, upRecordID);
			offset += IDLEN;
			writeNum(buffer3 + offset, NOTLEAFPAGELEN, freePage);

			writeNum(buffer, PAGE_RANK_LEN, newRoot);
			writeNum(buffer + PAGE_RANK_LEN * 2, ISROOTLEN, NOTROOT);
			writeNum(buffer2, PAGE_RANK_LEN, newRoot);

			fileManager->write(nowDataBaseHandle, nowPage, buffer);
			fileManager->write(nowDataBaseHandle, freePage, buffer2);
			fileManager->write(nowDataBaseHandle, newRoot, buffer3);

			return newRoot;
		}
	}
}

int IXManager::DeleteRecordAndIX(int rootPage, char* index, int indexLen, int dataKind, int recordID) {
	char buffer[PAGE_SIZE];
	char tmpIndex[256];
	int nowPage = rootPage;
	int offset;
	int indexNum, indexKind;// , indexLen;
	fileManager->read(nowDataBaseHandle, rootPage, buffer);
	offset = PAGE_RANK_LEN + PAGE_RANK_LEN + ISROOTLEN + ISLEAFLEN + NLEN + INDEXNUMLEN;
	indexKind = charToNum(buffer + offset, INDEXKINDLEN);
	offset += INDEXKINDLEN;
	//indexLen = charToNum(buffer + offset, INDEXLENLEN);

	while (true) {
		fileManager->read(nowDataBaseHandle, nowPage, buffer);
		offset = PAGE_RANK_LEN + PAGE_RANK_LEN + ISROOTLEN;
		int isLeaf = charToNum(buffer + offset, ISLEAFLEN);
		if (isLeaf == ISLEAF)
			break;
		offset += (ISLEAFLEN + NLEN);
		indexNum = charToNum(buffer + offset, INDEXNUMLEN);
		offset += INDEXNUMLEN;
		offset += INDEXKINDLEN;
		offset += INDEXLENLEN;
		//遍历该结点上的索引

		int findNode = indexNum; //去向第几个儿子指针
		for (int i = 0; i < indexNum; i++) {
			writeStr(tmpIndex, indexLen, buffer + offset + i * (NOTLEAFPAGELEN + indexLen + IDLEN) + NOTLEAFPAGELEN, indexLen);
			int tmpID = charToNum(buffer + offset + i * (NOTLEAFPAGELEN + indexLen + IDLEN) + NOTLEAFPAGELEN + indexLen, IDLEN);
			if (compareIndexAndID(tmpIndex, index, indexLen, tmpID, recordID) > 0) {
				findNode = i;
				break;
			}
		}
		nowPage = charToNum(buffer + offset + findNode * (NOTLEAFPAGELEN + indexLen + IDLEN) + NOTLEAFPAGESPACE, PAGE_RANK_LEN);
	}

	offset = PAGE_RANK_LEN * 2 + ISROOTLEN + ISLEAFLEN + NLEN;

	indexNum = charToNum(buffer + offset, INDEXNUMLEN);

	offset += (INDEXNUMLEN + INDEXKINDLEN + INDEXLENLEN);
	//遍历叶结点寻找
	int findNode = -1;
	for (int i = 0; i < indexNum; i++) {
		writeStr(tmpIndex, indexLen, buffer + offset + i * (LEAFPAGELEN + LEAFSLOTLEN + indexLen + IDLEN) + LEAFPAGELEN + LEAFSLOTLEN, indexLen);
		int tmpID = charToNum(buffer + offset + i * (LEAFPAGELEN + LEAFSLOTLEN + indexLen + IDLEN) + LEAFPAGELEN + LEAFSLOTLEN + indexLen, IDLEN);
		int compareResult = compareIndexAndID(tmpIndex, index, indexLen, tmpID, recordID);
		if (compareResult == 0) {
			findNode = i;
			break;
		}
		else if (compareResult > 0)
			break;
	}

	if (findNode == -1)
		return -1;
	else {
		//标记为该记录不存在
		writeNum(buffer + offset + findNode * (LEAFPAGELEN + LEAFSLOTLEN + indexLen + IDLEN), LEAFPAGELEN, NORECORD);
		writeNum(buffer + offset + findNode * (LEAFPAGELEN + LEAFSLOTLEN + indexLen + IDLEN) + LEAFPAGELEN, LEAFSLOTLEN, NORECORD);
		fileManager->write(nowDataBaseHandle, nowPage, buffer);
		return 0;
	}
}

int IXManager::UpDateRecordAndIX(int rootPage, char* index, int indexLen, int dataKind, int pageRank, int slotRank, int recordID) {
	char buffer[PAGE_SIZE];
	char tmpIndex[256];
	int nowPage = rootPage;
	int offset;
	int indexNum, indexKind;// , indexLen;
	fileManager->read(nowDataBaseHandle, rootPage, buffer);
	offset = PAGE_RANK_LEN + PAGE_RANK_LEN + ISROOTLEN + ISLEAFLEN + NLEN + INDEXNUMLEN;
	indexKind = charToNum(buffer + offset, INDEXKINDLEN);
	offset += INDEXKINDLEN;
	//indexLen = charToNum(buffer + offset, INDEXLENLEN);

	while (true) {
		fileManager->read(nowDataBaseHandle, nowPage, buffer);
		offset = PAGE_RANK_LEN + PAGE_RANK_LEN + ISROOTLEN;
		int isLeaf = charToNum(buffer + offset, ISLEAFLEN);
		if (isLeaf == ISLEAF)
			break;
		offset += (ISLEAFLEN + NLEN);
		indexNum = charToNum(buffer + offset, INDEXNUMLEN);
		offset += INDEXNUMLEN;
		offset += INDEXKINDLEN;
		offset += INDEXLENLEN;
		//遍历该结点上的索引

		int findNode = indexNum; //去向第几个儿子指针
		for (int i = 0; i < indexNum; i++) {
			writeStr(tmpIndex, indexLen, buffer + offset + i * (NOTLEAFPAGELEN + indexLen + IDLEN) + NOTLEAFPAGELEN, indexLen);
			int tmpID = charToNum(buffer + offset + i * (NOTLEAFPAGELEN + indexLen + IDLEN) + NOTLEAFPAGELEN + indexLen, IDLEN);
			if (compareIndexAndID(tmpIndex, index, indexLen, tmpID, recordID) > 0) {
				findNode = i;
				break;
			}
		}
		nowPage = charToNum(buffer + offset + findNode * (NOTLEAFPAGELEN + indexLen + IDLEN) + NOTLEAFPAGESPACE, PAGE_RANK_LEN);
	}

	offset = PAGE_RANK_LEN * 2 + ISROOTLEN + ISLEAFLEN + NLEN;

	indexNum = charToNum(buffer + offset, INDEXNUMLEN);

	offset += (INDEXNUMLEN + INDEXKINDLEN + INDEXLENLEN);
	//遍历叶结点寻找
	int findNode = -1;
	for (int i = 0; i < indexNum; i++) {
		writeStr(tmpIndex, indexLen, buffer + offset + i * (LEAFPAGELEN + LEAFSLOTLEN + indexLen + IDLEN) + LEAFPAGELEN + LEAFSLOTLEN, indexLen);
		int tmpID = charToNum(buffer + offset + i * (LEAFPAGELEN + LEAFSLOTLEN + indexLen + IDLEN) + LEAFPAGELEN + LEAFSLOTLEN + indexLen, IDLEN);
		int compareResult = compareIndexAndID(tmpIndex, index, indexLen, tmpID, recordID);
		if (compareResult == 0) {
			findNode = i;
			break;
		}
		else if (compareResult > 0)
			break;
	}

	if (findNode == -1)
		return -1;
	else {
		int oriPage = charToNum(buffer + offset + findNode * (LEAFPAGELEN + LEAFSLOTLEN + indexLen + IDLEN), LEAFPAGELEN);
		int oriSlot = charToNum(buffer + offset + findNode * (LEAFPAGELEN + LEAFSLOTLEN + indexLen + IDLEN) + LEAFSLOTLEN, LEAFSLOTLEN);
		if ((oriPage == NORECORD) || (oriSlot == NORECORD))
			return -1;
		//更新该记录
		writeNum(buffer + offset + findNode * (LEAFPAGELEN + LEAFSLOTLEN + indexLen + IDLEN), LEAFPAGELEN, pageRank);
		writeNum(buffer + offset + findNode * (LEAFPAGELEN + LEAFSLOTLEN + indexLen + IDLEN) + LEAFPAGELEN, LEAFSLOTLEN, slotRank);
		fileManager->write(nowDataBaseHandle, nowPage, buffer);
		return 0;
	}
}

void IXManager::freePage(vector<int>& pageRanks) {
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

void IXManager::countPage(int nowPage, vector<int>& pages) {
	pages.push_back(nowPage);
	char buffer[PAGE_SIZE];
	int isLeaf = 0;
	int indexLen = 0;
	int indexNum = 0;
	fileManager->read(nowDataBaseHandle, nowPage, buffer);
	isLeaf = charToNum(buffer + PAGE_RANK_LEN * 2 + ISROOTLEN, ISLEAFLEN);
	if (!isLeaf) {
		int offset = PAGE_RANK_LEN * 2 + ISROOTLEN + ISLEAFLEN + NLEN;
		indexNum = charToNum(buffer + offset, INDEXNUMLEN);
		offset += INDEXNUMLEN + INDEXKINDLEN;
		indexLen = charToNum(buffer + offset, INDEXLENLEN);
		offset += INDEXLENLEN;
		for (int i = 0; i < indexNum + 1; i++) {
			int nextPage = charToNum(buffer + offset + NOTLEAFPAGESPACE, NOTLEAFPAGELEN - NOTLEAFPAGESPACE);
			countPage(nextPage, pages);
			offset += (NOTLEAFPAGELEN + indexLen + IDLEN);
		}
	}
}

void IXManager::destoryIndex(int indexRoot) {
	tmpWorker.clear();
	
	countPage(indexRoot, tmpWorker);

	freePage(tmpWorker);
}