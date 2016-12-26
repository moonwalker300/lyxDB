#pragma once
#include<string>
#include<map>
#include<tuple>
#include <vector>
#include "common.h"
class FileManager {
public:
	FileManager();
	~FileManager();
	void flush();
	//判断某个文件是否存在
	bool ifexist(std::string fileName);
	//打开一个文件，返回这个文件的描述符，无法打开时返回-1
	int64_t open(std::string fileName);
	//关闭文件描述符对应的文件
	int64_t close(int64_t fileDescriptor);
	//把文件描述符对应的文件中的某一页读到buffer中，如果文件不存在，返回-1，如果输入的页号在文件中没有对应，返回-2
	int64_t read(int64_t fileDescriptor, int64_t pageNumber, char* buffer);
	//把buffer中的数据写到文件描述符对应的文件中的某一页
	int64_t write(int64_t fileDescriptor, int64_t pageNumber, char* buffer);
	//获取文件描述符对应的文件的页数，如果文件描述符无效，返回-1
	int64_t size(int64_t fileDescriptor);
	//向文件描述符对应的文件后面添加若干个空白页，返回新的大小，如果文件描述符无效，返回-1
	int64_t alloc(int64_t fileDescriptor, int64_t pageNumber);
	std::vector<int64_t> fileMap1;
private:
	class BufClass;
	class File;
	std::vector<File*> fileMap2;
//	std::map<int64_t, File> fileMap;
};
extern FileManager fileManager;