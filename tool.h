#ifndef TOOL_H
#define TOOL_H
#include "common.h"
char* ZeroFill(char *src, int oriLen, int goalLen); 
//���㺯������goalLen<oriLen��ֻ����ǰgoalLen���ַ�
char* AllZeroFill(int goalLen);
//ȫ��ҳ
void ZeroClear(char* buffer);
int charToNum(char* numHead, int len);
#endif