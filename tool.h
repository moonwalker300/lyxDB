#ifndef TOOL_H
#define TOOL_H
#include "common.h"
char* ZeroFill(char *src, int oriLen, int goalLen); 
//补零函数，若goalLen<oriLen则只复制前goalLen个字符
char* AllZeroFill(int goalLen);
//全零页
void ZeroClear(char* buffer);
int charToNum(char* numHead, int len);
void writeNum(char* numHead, int len, int num);
void writeStr(char* dest, int len, char* src);
void writeStr(char* dest, int len, char* src, int totLen); //后面totLen-len都补0
void moveStr(char* str, int from, int to, int len);
int compareStr(char* str1, char* str2, int len);
int compareIndex(char* index1, char* index2, int len);
int compareIndexAndID(char* index1, char* index2, int len, int id1, int id2);
#endif