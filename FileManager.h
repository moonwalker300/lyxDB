#pragma once
#include<string>
#include<map>
#include<tuple>
#include "common.h"
class FileManager {
public:
	FileManager();
	~FileManager();
	//��һ���ļ�����������ļ������������޷���ʱ����-1
	int64_t open(std::string fileName);
	//�ر��ļ���������Ӧ���ļ�
	int64_t close(int64_t fileDescriptor);
	//���ļ���������Ӧ���ļ��е�ĳһҳ����buffer�У�����ļ������ڣ�����-1����������ҳ�����ļ���û�ж�Ӧ������-2
	int64_t read(int64_t fileDescriptor, int64_t pageNumber, char* buffer);
	//��buffer�е�����д���ļ���������Ӧ���ļ��е�ĳһҳ
	int64_t write(int64_t fileDescriptor, int64_t pageNumber, char* buffer);
	//��ȡ�ļ���������Ӧ���ļ���ҳ��������ļ���������Ч������-1
	int64_t size(int64_t fileDescriptor);
	//���ļ���������Ӧ���ļ������������ɸ��հ�ҳ�������µĴ�С������ļ���������Ч������-1
	int64_t alloc(int64_t fileDescriptor, int64_t pageNumber);
private:
	class BufClass;
	class File;
	std::map<int64_t, File> fileMap;
};
extern FileManager fileManager;