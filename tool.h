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
void writeStr(char* dest, int len, char* src);
void writeStr(char* dest, int len, char* src, int totLen); //����totLen-len����0
void moveStr(char* str, int from, int to, int len);
int compareStr(char* str1, char* str2, int len);
int compareIndex(char* index1, char* index2, int len);
int compareIndexAndID(char* index1, char* index2, int len, int id1, int id2);
#endif