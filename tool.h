#ifndef TOOL_H
#define TOOL_H
#include "common.h"
char* ZeroFill(char *src, int oriLen, int goalLen); 
//���㺯������goalLen<oriLen��ֻ����ǰgoalLen���ַ�
char* AllZeroFill(int goalLen);
//ȫ��ҳ
void ZeroClear(char* buffer);
int charToNum(char* numHead, int len);
void writeNum(char* numHead, int len, int num);
void writeStr(char* dest, int len, char* src, int totLen); //����totLen-len����0
void moveStr(char* str, int from, int to, int len);
#endif