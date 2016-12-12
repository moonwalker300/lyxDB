#include "tool.h"

char* ZeroFill(char *src, int oriLen, int goalLen) {
	char* ret = new char[goalLen];
	for (int i = 0; i < goalLen; i++)
		if (i < oriLen)
			ret[i] = src[i];
		else
			ret[i] = 0;
	return ret;
}

char* AllZeroFill(int goalLen) {
	char* ret = new char[goalLen];
	for (int i = 0; i < goalLen; i++)
		ret[i] = 0;
	return ret;
}

void ZeroClear(char* buffer) {
	for (int i = 0; i < PAGE_SIZE; i++)
		buffer[i] = 0;
}

int charToNum(char* numHead, int len) {
	int ret = 0;
	int radix = 1 << BYTE_SIZE;
	for (int i = 0; i < len; i++)
		ret = ret * radix + numHead[i];
	return ret;
}