#include "backend.h"
#include "tool.h"
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
	lyx.fm->flush();
	return 0;
}