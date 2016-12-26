#include "backend.h"
#include "tool.h"
#include <iostream>
using namespace std;
int main() {
	BackEnd lyx;
	string zh("zh");
	lyx.createDataBase(zh);
	lyx.useDataBase(zh);
	TableMeta param;
	writeStr(param.name, 2, "ha", 28);
	param.columnNumber = 1;
	writeStr(param.columns[0].name, 2, "id", 32);
	param.columns[0].length = 4;
	param.columns[0].dataType = 1;
	param.columns[0].attribute = 0;
	lyx.createTable(param);
	
	ContentEntry c;
	c.isNull = false;
	c.isString = false;
	c.ival = 0;
	vector<ContentEntry> entries;
	entries.push_back(c);
	for (int i = 1000; i > 0; i--) {
		entries[0].ival = i;
		lyx.insertRecord(0, entries);
	}
	
	Filter f;
	f.columnNumber = 0;
	f.type = IntFilter;
	f.op = FGE;
	f.ival = 500;
	IndexIterator i = lyx.begin(0, f);
	for (; !i.isFinished(); i++ ) {
		ContentEntry tmp;
		tmp.isNull = false;
		tmp.isString = false;
		tmp.ival = (*i)[0].ival + 100;
		i.Update(0, tmp);
	}
	i.UpdateFlush();
	cout << endl;
	f.op = FZERO;
	cout << "-----------------------------------" << endl;
	for (IndexIterator i = lyx.begin(0, f); !i.isFinished(); i++) {
		cout << (*i)[0].ival << "DW";
	}
	
	lyx.fm->flush();
	return 0;
}