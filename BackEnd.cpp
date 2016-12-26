#include "backend.h"
#include "tool.h"
#include "infolist.h"
using namespace std;
BackEnd::BackEnd() {
	fm = new FileManager();
	rm = new RMManager(fm);
	im = new IXManager(fm);
}

void BackEnd::createDataBase(std::string& name) {
	rm->CreateDataBase(name.c_str());
}

void BackEnd::dropDataBase(std::string& name) {

}

void BackEnd::useDataBase(std::string& name) {
	/*
	if (nowDataBaseHandle != 0) {
		//将id上限写回去
		char buffer[PAGE_SIZE];
		char buffer2[PAGE_SIZE];
		fm->read(nowDataBaseHandle, 0, buffer);
		int tableNum = charToNum(buffer, TABLE_NUM_LEN);
		for (int i = 0; i < tableNum; i++) {
			int tableHead = charToNum(buffer, TABLE_NUM_LEN + i * (TABLE_HEAD_PLACE_LEN + TABLE_NAME_LEN));
			fm->read(nowDataBaseHandle, tableHead, buffer2);
			writeNum(buffer2 + PAGE_RANK_LEN + TABLE_NAME_LEN + PAGE_RANK_LEN * 2, IDLEN, idLimit[i]);
			fm->write(nowDataBaseHandle, tableHead, buffer2);
		}
	}
	*/
	rm->ChangeDataBase(name.c_str());
	im->ChangeDataBase(name.c_str());
	nowDataBaseHandle = rm->getNowDataBaseHandle();
}

DatabaseMeta BackEnd::getDataBaseMeta() {
	char buffer[PAGE_SIZE];
	fm->read(nowDataBaseHandle, 0, buffer); //取出库页
	DatabaseMeta ret;
	ret.tableNumber = charToNum(buffer, TABLE_NUM_LEN);
	int offset = TABLE_NUM_LEN;
	for (int i = 0; i < ret.tableNumber; i++) {
		ret.tables[i].location = charToNum(buffer, TABLE_HEAD_PLACE_LEN);
		offset += TABLE_HEAD_PLACE_LEN;
		writeStr(ret.tables[i].name, TABLE_NAME_LEN, buffer + offset);
		offset += TABLE_NAME_LEN;
	}
	return ret;
}

void BackEnd::createTable(TableMeta& tableMeta) {
	TableInfomation tableInfo;
	//拷贝表信息
	writeStr(tableInfo.tableName, TABLE_NAME_LEN, tableMeta.name);
	tableInfo.columnNum = tableMeta.columnNumber;
	for (int i = 0; i < tableInfo.columnNum; i++) {	
		writeNum(tableInfo.columns[i].columnKind, COLUMN_KIND_LEN, tableMeta.columns[i].dataType);
		writeNum(tableInfo.columns[i].columnLen, COLUMN_LEN_LEN, tableMeta.columns[i].length);
		writeStr(tableInfo.columns[i].columnName, COLUMN_NAME_LEN, tableMeta.columns[i].name);
		writeNum(tableInfo.columns[i].columnProperty, COLUMN_PROPERTY_LEN, tableMeta.columns[i].attribute);
	}
	int tableRank = rm->CreateTable(tableInfo);
	//建立索引
	for (int i = 0; i < tableInfo.columnNum; i++)
		im->CreateIndex(tableRank, i);
}

void BackEnd::dropTable(int no) {
	char buffer[PAGE_SIZE];
	fm->read(nowDataBaseHandle, 0, buffer); //取出库页面
	int tableNum = charToNum(buffer, TABLE_NUM_LEN);
	int offset = TABLE_NUM_LEN + (TABLE_HEAD_PLACE_LEN + TABLE_NAME_LEN) * no;
	int headPlace = charToNum(buffer + offset, TABLE_HEAD_PLACE_LEN);
	//修改库页
	writeNum(buffer, 0, tableNum - 1);
	moveStr(buffer, offset + TABLE_HEAD_PLACE_LEN + TABLE_NAME_LEN, offset, (tableNum - no - 1) * (TABLE_HEAD_PLACE_LEN + TABLE_NAME_LEN));
	fm->write(nowDataBaseHandle, 0, buffer);

	//更改id上限
//	for (int i = no; i < tableNum - 1; i++)
//		idLimit[i] = idLimit[i + 1];

	fm->read(nowDataBaseHandle, headPlace, buffer);
	int columnNum = charToNum(buffer + PAGE_RANK_LEN + TABLE_NAME_LEN + PAGE_RANK_LEN * 2 + IDLEN, COLUMN_NUM_LEN);
	offset = PAGE_RANK_LEN + TABLE_NAME_LEN + PAGE_RANK_LEN * 2 + IDLEN + COLUMN_NUM_LEN;
	int totLen = COLUMN_NAME_LEN + COLUMN_PROPERTY_LEN + COLUMN_KIND_LEN + COLUMN_LEN_LEN + INDEXLENLEN;
	for (int i = 0; i < columnNum; i++) {
		int indexRoot = charToNum(buffer + offset + i * totLen + COLUMN_NAME_LEN + COLUMN_PROPERTY_LEN + COLUMN_KIND_LEN + COLUMN_LEN_LEN, INDEXLENLEN);
		im->destoryIndex(indexRoot);
	}

	rm->destroyTable(headPlace);
}

TableMeta BackEnd::getTableMeta(int no) {
	char buffer[PAGE_SIZE];
	fm->read(nowDataBaseHandle, 0, buffer); //取出库页面
	int tableNum = charToNum(buffer, TABLE_NUM_LEN);
	int offset = TABLE_NUM_LEN + (TABLE_HEAD_PLACE_LEN + TABLE_NAME_LEN) * no;
	int headPlace = charToNum(buffer + offset, TABLE_HEAD_PLACE_LEN);

	fm->read(nowDataBaseHandle, headPlace, buffer);
	//复制
	TableMeta ret;
	offset = 0;
	ret.pageNumber = charToNum(buffer + offset, PAGE_RANK_LEN);
	offset += PAGE_RANK_LEN;
	writeStr(ret.name, TABLE_NAME_LEN, buffer + offset);
	offset += TABLE_NAME_LEN;
	ret.firstDataPage = charToNum(buffer + offset, PAGE_RANK_LEN);
	offset += PAGE_RANK_LEN;
	ret.firstBlankPage = charToNum(buffer + offset, PAGE_RANK_LEN);
	offset += PAGE_RANK_LEN;
	ret.id = charToNum(buffer + offset, IDLEN);
	offset += IDLEN;

	ret.columnNumber = charToNum(buffer + offset, COLUMN_NUM_LEN);
	offset += COLUMN_NUM_LEN;
	for (int i = 0; i < ret.columnNumber; i++) {
		writeStr(ret.columns[i].name, COLUMN_NAME_LEN, buffer + offset);
		offset += COLUMN_NAME_LEN;
		ret.columns[i].attribute = charToNum(buffer + offset, COLUMN_PROPERTY_LEN);
		offset += COLUMN_PROPERTY_LEN;
		ret.columns[i].dataType = charToNum(buffer + offset, COLUMN_KIND_LEN);
		offset += COLUMN_KIND_LEN;
		ret.columns[i].length = charToNum(buffer + offset, COLUMN_LEN_LEN);
		offset += COLUMN_LEN_LEN;
		ret.columns[i].indexLocation = charToNum(buffer + offset, INDEXLENLEN);
		offset += INDEXLENLEN;
	}
	return ret;
}

void BackEnd::insertRecord(int no, std::vector<ContentEntry>& line) {
	//更新id上限
//	if (line[0].ival >= idLimit[no])
//		idLimit[no] = line[0].ival + 1;
	
	char buffer[PAGE_SIZE];
	fm->read(nowDataBaseHandle, 0, buffer); //取出库页面
	int tableNum = charToNum(buffer, TABLE_NUM_LEN);
	int offset = TABLE_NUM_LEN + (TABLE_HEAD_PLACE_LEN + TABLE_NAME_LEN) * no;
	int headPlace = charToNum(buffer + offset, TABLE_HEAD_PLACE_LEN);

	char tmpRecord[PAGE_SIZE];
	int recordLen = 0;
	fm->read(nowDataBaseHandle, headPlace, buffer);
	//形成定长数据
	offset = PAGE_RANK_LEN + TABLE_NAME_LEN + PAGE_RANK_LEN * 2 + IDLEN + COLUMN_NUM_LEN;
	int cn = line.size();
	for (int i = 0; i < cn; i++) {
		offset += COLUMN_NAME_LEN + COLUMN_PROPERTY_LEN + COLUMN_KIND_LEN;
		int len = charToNum(buffer + offset, COLUMN_LEN_LEN);
		if (!line[i].isNull) {
			if (line[i].isString)
				writeStr(tmpRecord + recordLen, line[i].sval.length(), line[i].sval.c_str(), len);
			else
				writeNum(tmpRecord + recordLen, len, line[i].ival);
		}
		recordLen += len;
		offset += COLUMN_LEN_LEN + INDEX_PLACE_LEN;
	}
	//放NULL位图
	int count = 0; 
	for (int i = 0; i < cn; i++) {
		if (count == 0) {
			count = 7;
			recordLen++;
			tmpRecord[recordLen - 1] = 0;
		}
		else
			count--;
		if (line[i].isNull)
			tmpRecord[recordLen - 1] |= (1 << count);
	}
	int pageRank, slotRank;
	//插入
	rm->InsertRecord(headPlace, pageRank, slotRank, tmpRecord, recordLen, buffer);

	//放入索引
	offset = PAGE_RANK_LEN + TABLE_NAME_LEN + PAGE_RANK_LEN * 2 + IDLEN + COLUMN_NUM_LEN;
	int id = line[0].ival;
	for (int i = 0; i < cn; i++) {
		if (line[i].isNull)
			writeNum(tmpRecord, 1, 1); //用1byte存NULL
		else
			writeNum(tmpRecord, 1, 0);

		offset += COLUMN_NAME_LEN + COLUMN_PROPERTY_LEN;
		int kind = charToNum(buffer + offset, COLUMN_KIND_LEN);
		offset += COLUMN_KIND_LEN;
		int len = charToNum(buffer + offset, COLUMN_LEN_LEN);
		offset += COLUMN_LEN_LEN;
		if (!line[i].isNull) {
			if (line[i].isString)
				writeStr(tmpRecord + 1, line[i].sval.length(), line[i].sval.c_str(), len);
			else
				writeNum(tmpRecord + 1, len, line[i].ival);
		}
		int indexRoot = charToNum(buffer + offset, INDEXLENLEN);
		int newRoot = im->InsertRecordAndIX(indexRoot, tmpRecord, len + 1, kind, pageRank, slotRank, id);
		if (newRoot != 0)  //根发生了变化
			writeNum(buffer + offset, INDEXLENLEN, newRoot);
		offset += INDEXLENLEN;
	}
	fm->write(nowDataBaseHandle, headPlace, buffer);
}

void IndexIterator::operator++(int) {
	if (flag)
		return;
	while (true) {
		if (slotRank == indexNum - 1) {
			offset += (LEAFPAGELEN + LEAFSLOTLEN + indexLen + IDLEN + LEAFNEXTSPACE);
			pageRank = charToNum(buffer + offset, LEAFNEXTLEN - LEAFNEXTSPACE);
			if (pageRank == NOPAGE) {
				flag = true;
				break;
			}
			fileManager->read(nowDataBaseHandle, pageRank, buffer);
			offset = PAGE_RANK_LEN * 2 + ISROOTLEN + ISLEAFLEN + NLEN;
			indexNum = charToNum(buffer + offset, INDEXNUMLEN);
			offset += (INDEXNUMLEN + INDEXKINDLEN + INDEXLENLEN);
			slotRank = 0;
		}
		else {
			offset += (LEAFPAGELEN + LEAFSLOTLEN + indexLen + IDLEN);
			slotRank++;
		}
		if (charToNum(buffer + offset, LEAFPAGELEN) != NORECORD)
			break;
	}

	if ((pageRank == limitPage) && (slotRank == limitSlot))
		flag = true;

	if (!flag)
		read();
}

void IndexIterator::read() {
	recordPage = charToNum(buffer + offset, LEAFPAGELEN);
	recordSlot = charToNum(buffer + offset + LEAFPAGELEN, LEAFSLOTLEN);
	
	for (int i = 0; i < columnNum; i++)
		change[i] = false;
	char tmp[PAGE_SIZE];
	fileManager->read(nowDataBaseHandle, recordPage, tmp);
	int tmpoffset = PAGE_RANK_LEN * 5 + FIXEDCOLUMNLEN_LEN + FIXEDCOLUMNNUM_LEN + RECORDLENLEN + FIRSTSLOTLEN;
	tmpoffset += recordLen * recordSlot;
	char str[300];
	for (int i = 0; i < columnNum; i++) {
		if (kinds[i] == KIND_INT) {
			entries[i].isString = false;
			entries[i].ival = charToNum(tmp + tmpoffset, sizes[i]);
			tmpoffset += sizes[i];
		}
		else {
			entries[i].isString = true;
			writeStr(str, sizes[i], tmp + tmpoffset, sizes[i] + 1);
			entries[i].sval = string(str);
			tmpoffset += sizes[i];
		}
	}
	int count = 7;
	for (int i = 0; i < columnNum; i++) {
		if (tmp[tmpoffset] & (1 << count) > 0)
			entries[i].isNull = true;
		else
			entries[i].isNull = false;
		if (count == 0) {
			tmpoffset++;
			count = 7;
		}
		else
			count--;
	}
}

vector<ContentEntry>& IndexIterator::operator*() {
	return entries;
}

IndexIterator BackEnd::begin(int no, Filter f) {
	char buffer[PAGE_SIZE];
	fm->read(nowDataBaseHandle, 0, buffer); //取出库页面
	int tableNum = charToNum(buffer, TABLE_NUM_LEN);
	int offset = TABLE_NUM_LEN + (TABLE_HEAD_PLACE_LEN + TABLE_NAME_LEN) * no;
	int headPlace = charToNum(buffer + offset, TABLE_HEAD_PLACE_LEN);

	char tmpRecord[PAGE_SIZE];
	int recordLen = 0;
	fm->read(nowDataBaseHandle, headPlace, buffer);
	
	IndexIterator ret;
	writeStr(ret.bufferTable, PAGE_SIZE, buffer);
	ret.flag = false;
	ret.fileManager = fm;
	ret.im = im;
	ret.rm = rm;
	ret.nowDataBaseHandle = nowDataBaseHandle;
	ret.columnRank = f.columnNumber;
	ret.tableHead = headPlace;
	offset = PAGE_RANK_LEN + TABLE_NAME_LEN + PAGE_RANK_LEN * 2 + IDLEN;

	ret.columnNum = charToNum(buffer + offset, COLUMN_NUM_LEN);
	ret.entries.resize(ret.columnNum);
	ret.sizes.resize(ret.columnNum);
	ret.change.resize(ret.columnNum, false);
	ret.kinds.resize(ret.columnNum);
	offset += COLUMN_NUM_LEN;
	ret.recordLen = 0;
	int totLen = COLUMN_NAME_LEN + COLUMN_KIND_LEN + COLUMN_PROPERTY_LEN + COLUMN_LEN_LEN;
	int indexRoot = 0;
	for (int i = 0; i < ret.columnNum; i++) {
		ret.sizes[i] = charToNum(buffer + offset + i * (totLen + INDEX_PLACE_LEN) + totLen - COLUMN_LEN_LEN, COLUMN_LEN_LEN);
		ret.recordLen += ret.sizes[i];
		ret.kinds[i] = charToNum(buffer + offset + i * (totLen + INDEX_PLACE_LEN) + totLen - COLUMN_LEN_LEN - COLUMN_KIND_LEN, COLUMN_KIND_LEN);
		if (i == ret.columnRank)
			indexRoot = charToNum(buffer + offset + i * (totLen + INDEX_PLACE_LEN) + totLen, INDEX_PLACE_LEN);
	}
	ret.recordLen += (ret.columnNum - 1) / BYTE_SIZE + 1;
	char lyxIndex[300];
	if ((f.op != FIsNull) && (f.op != FZERO)) {
		if (f.type == IntFilter) {
			lyxIndex[0] = 0;
			writeNum(lyxIndex + 1, 4, f.ival); //int长度为4
			ret.indexLen = 1 + 4;
		}
		else {
			lyxIndex[0] = 0;
			writeStr(lyxIndex + 1, f.sval.length(), f.sval.c_str(), ret.sizes[ret.columnRank]);
			ret.indexLen = ret.sizes[ret.columnRank] + 1;
		}
	}
	else {
		lyxIndex[0] = 1;
		if (f.type == IntFilter)
			ret.indexLen = 1 + 4;
		else 
			ret.indexLen = ret.sizes[ret.columnRank] + 1;
	}
	if (f.op == FEQ) {
		im->SearchIndex(indexRoot, lyxIndex, ret.indexLen, ret.kinds[ret.columnRank], ret.pageRank, ret.slotRank, 0);
		im->SearchIndex(indexRoot, lyxIndex, ret.indexLen, ret.kinds[ret.columnRank], ret.limitPage, ret.limitSlot, 1);
	}
	else if (f.op == FLE) {
		im->SearchIndex(indexRoot, lyxIndex, ret.indexLen, ret.kinds[ret.columnRank], ret.limitPage, ret.limitSlot, 1);
		lyxIndex[0] = 1;
		im->SearchIndex(indexRoot, lyxIndex, ret.indexLen, ret.kinds[ret.columnRank], ret.pageRank, ret.slotRank, 1);
	}
	else if (f.op == FLT) {
		im->SearchIndex(indexRoot, lyxIndex, ret.indexLen, ret.kinds[ret.columnRank], ret.limitPage, ret.limitSlot, 0);
		lyxIndex[0] = 1;
		im->SearchIndex(indexRoot, lyxIndex, ret.indexLen, ret.kinds[ret.columnRank], ret.pageRank, ret.slotRank, 1);
	}
	else if (f.op == FGE) {
		im->SearchIndex(indexRoot, lyxIndex, ret.indexLen, ret.kinds[ret.columnRank], ret.pageRank, ret.slotRank, 0);
		ret.limitPage = -1;
		ret.limitSlot = -1;
	}
	else if (f.op == FGT) {
		im->SearchIndex(indexRoot, lyxIndex, ret.indexLen, ret.kinds[ret.columnRank], ret.pageRank, ret.slotRank, 1);
		ret.limitPage = -1;
		ret.limitSlot = -1;
	}
	else if (f.op == FIsNull) {
		im->SearchIndex(indexRoot, lyxIndex, ret.indexLen, ret.kinds[ret.columnRank], ret.pageRank, ret.slotRank, 0);
		im->SearchIndex(indexRoot, lyxIndex, ret.indexLen, ret.kinds[ret.columnRank], ret.limitPage, ret.limitSlot, 1);
	}
	else if (f.op == FZERO) {
		im->SearchIndex(indexRoot, lyxIndex, ret.indexLen, ret.kinds[ret.columnRank], ret.pageRank, ret.slotRank, 0);
		ret.limitPage = -1;
		ret.limitSlot = -1;
	}
	if ((ret.pageRank == ret.limitPage) && (ret.slotRank == ret.limitSlot))
		ret.flag = true;
	else if (ret.pageRank > 0) {
		fm->read(nowDataBaseHandle, ret.pageRank, ret.buffer);
		ret.offset = PAGE_RANK_LEN * 2 + ISROOTLEN + ISLEAFLEN + NLEN + INDEXNUMLEN + INDEXKINDLEN + INDEXLENLEN + ret.slotRank * (LEAFPAGELEN + LEAFSLOTLEN + ret.indexLen + IDLEN);
		ret.indexNum = charToNum(ret.buffer + PAGE_RANK_LEN * 2 + ISROOTLEN + ISLEAFLEN + NLEN, INDEXNUMLEN);
		ret.read();
	} else
		ret.flag = true;

	return ret;
}

void IndexIterator::Kill() {
	if (flag)
		return;
	char tmpIndex[300];
	int tmpoffset = PAGE_RANK_LEN + TABLE_NAME_LEN + PAGE_RANK_LEN * 2 + IDLEN + COLUMN_NUM_LEN;
	int totLen = COLUMN_NAME_LEN + COLUMN_KIND_LEN + COLUMN_PROPERTY_LEN + COLUMN_LEN_LEN;
	for (int i = 0; i < columnNum; i++) {
		if (entries[i].isNull)
			tmpIndex[0] = 1;
		else
			tmpIndex[0] = 0;

		if (kinds[i] == KIND_INT)
			writeNum(tmpIndex + 1, sizes[i], entries[i].ival);
		else
			writeStr(tmpIndex + 1, entries[i].sval.length(), entries[i].sval.c_str(), sizes[i]);

		int indexRoot = charToNum(bufferTable + tmpoffset + i * (totLen + INDEX_PLACE_LEN) + totLen, INDEX_PLACE_LEN);
		im->DeleteRecordAndIX(indexRoot, tmpIndex, sizes[i] + 1, kinds[i], entries[0].ival);
	}
	fileManager->read(nowDataBaseHandle, pageRank, buffer);
	rm->DeleteRecord(tableHead, recordPage, recordSlot);
	this->operator++(0);
}

void IndexIterator::Update(int i, ContentEntry c) {
	if (flag)
		return;
	char buf[PAGE_SIZE];
	fileManager->read(nowDataBaseHandle, recordPage, buf);
	int totLen = charToNum(buf + PAGE_RANK_LEN * 5 + FIXEDCOLUMNLEN_LEN + FIXEDCOLUMNNUM_LEN, RECORDLENLEN);
	int tmpoffset = PAGE_RANK_LEN * 5 + FIXEDCOLUMNLEN_LEN + FIXEDCOLUMNNUM_LEN + RECORDLENLEN + FIRSTSLOTLEN;

	tmpoffset += totLen * recordSlot;

	int tmpoffset2 = tmpoffset + totLen - ((columnNum - 1) / BYTE_SIZE + 1) + (i - 1) / BYTE_SIZE + 1;
	char tmpIndex[300];
	//找出原来的值
	if (buf[tmpoffset2] & (1 << (BYTE_SIZE - 1 - i % BYTE_SIZE)) > 0)
		tmpIndex[0] = 1;
	else
		tmpIndex[0] = 0;
	
	for (int j = 0; j < i; j++)
		tmpoffset += sizes[j];
	writeStr(tmpIndex + 1, sizes[i], buf + tmpoffset);

	if (c.isNull) {
		buf[tmpoffset2] |= (1 << (BYTE_SIZE - 1 - i % BYTE_SIZE));
	}
	else {
		if (buf[tmpoffset2] & (1 << (BYTE_SIZE - 1 - i % BYTE_SIZE)) > 0)
			buf[tmpoffset2] -= (1 << (BYTE_SIZE - 1 - i % BYTE_SIZE));
		if (c.isString) {
			writeStr(buf + tmpoffset, c.sval.length(), c.sval.c_str(), sizes[i]);
		}
		else {
			writeNum(buf + tmpoffset, sizes[i], c.ival);
		}
	}

	fileManager->write(nowDataBaseHandle, recordPage, buf);
	
	lyxbuf.push_back(c);
	columnbuf.push_back(i);
	zhbuf.push_back(entries[0].ival);
	pageBuf.push_back(recordPage);
	slotBuf.push_back(recordSlot);
	int indexoffset = PAGE_RANK_LEN * 3 + IDLEN + TABLE_NAME_LEN + COLUMN_NUM_LEN;
	int tLen = COLUMN_NAME_LEN + COLUMN_KIND_LEN + COLUMN_PROPERTY_LEN + COLUMN_LEN_LEN;
	int indexRoot = charToNum(bufferTable + indexoffset + i * (tLen + INDEX_PLACE_LEN) + tLen, INDEX_PLACE_LEN);

	im->DeleteRecordAndIX(indexRoot, tmpIndex, sizes[i] + 1, kinds[i], entries[0].ival);
	entries[i] = c;
}

void IndexIterator::UpdateFlush() {
	int num = lyxbuf.size();
	char tmpIndex[300];
	int tLen = COLUMN_NAME_LEN + COLUMN_KIND_LEN + COLUMN_PROPERTY_LEN + COLUMN_LEN_LEN;
	int indexoffset = PAGE_RANK_LEN * 3 + IDLEN + TABLE_NAME_LEN + COLUMN_NUM_LEN;
	for (int i = 0; i < num; i++) {
		//准备好新索引
		if (lyxbuf[i].isNull)
			tmpIndex[0] = 1;
		else {
			tmpIndex[0] = 0;
			if (lyxbuf[i].isString) {
				writeStr(tmpIndex + 1, lyxbuf[i].sval.length(), lyxbuf[i].sval.c_str(), sizes[columnbuf[i]]);
			}
			else {
				writeNum(tmpIndex + 1, sizes[columnbuf[i]], lyxbuf[i].ival);
			}
		}
		int indexRoot = charToNum(bufferTable + indexoffset + columnbuf[i] * (tLen + INDEX_PLACE_LEN) + tLen, INDEX_PLACE_LEN);
		int nextRoot = im->InsertRecordAndIX(indexRoot, tmpIndex, sizes[columnbuf[i]] + 1, kinds[columnbuf[i]], pageBuf[i], slotBuf[i], zhbuf[i]);
		if (nextRoot > 0)
			writeNum(bufferTable + indexoffset + columnbuf[i] * (tLen + INDEX_PLACE_LEN) + tLen, INDEX_PLACE_LEN, nextRoot);
	}
	fileManager->write(nowDataBaseHandle, tableHead, bufferTable);
}