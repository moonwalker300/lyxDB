#include "tool.h"
#include "infolist.h"
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

void writeNum(char* numHead, int len, int num) {
	int radix = 1 << BYTE_SIZE;
	for (int i = len - 1; i >= 0; i--) {
		numHead[i] = num % radix;
		num /= radix;
	}
}

void writeStr(char* dest, int len, char* src, int totLen) {
	for (int i = 0; i < totLen; i++)
		if (i < len)
			dest[i] = src[i];
		else 
			dest[i] = 0;
}

void moveStr(char* str, int from, int to, int len) {
	if (from == to)
		return;
	if (from < to) {
		for (int i = len - 1; i >= 0; i--)
			str[to + i] = str[from + i];
	}
	else {
		for (int i = 0; i < len; i++)
			str[to + i] = str[from + i];
	}
}

int compareStr(char* str1, char* str2, int len) {
	for (int i = 0; i < len; i++)
		if (str1[i] > str2[i])
			return 1;
		else if (str1[i] < str2[i])
			return -1;
		else if (str1[i] == 0)
			return 0;
	return 0;
}

int compareIndex(char* index1, char* index2, int len) {
	if ((index1[0] == 1) && (index2[0] == 1))
		return 0;
	else if ((index1[0] == 1) && (index2[0] == 0))
		return -1;
	else if ((index1[0] == 0) && (index2[0] == 1))
		return 1;
	else
		return compareStr(index1, index2, len);
}

int compareIndexAndID(char* index1, char* index2, int len, int id1, int id2) {
	int comp1 = compareIndex(index1, index2, len - IDLEN);
	if (comp1 != 0)
		return comp1;
	else if (id1 > id2)
		return 1;
	else if (id1 < id2)
		return -1;
	else
		return 0;
}