#ifndef TOOL_H
#define TOOL_H
#include "common.h"
char* ZeroFill(char *src, int oriLen, int goalLen); 
//补零函数，若goalLen<oriLen则只复制前goalLen个字符
char* AllZeroFill(int goalLen);
//全零页
void ZeroClear(char* buffer);
int charToNum(char* numHead, int len);
#endif