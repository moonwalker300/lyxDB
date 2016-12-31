#include<iostream>
#include<fstream>
#include<string.h>
#include<Windows.h>
#include<cstdlib>
#include "tool.h"
#include "FileManager.h"
using namespace std;
class FileManager::BufClass {
public:
	BufClass() :prev(this), next(this), pageNumber(-1), dirty(0) {
		buffer = new char[4096];
	}
	~BufClass() {
		delete[] buffer;
	}
	char* buffer;
	bool dirty;
	int64_t pageNumber;
	BufClass* prev;
	BufClass* next;
};
class FileManager::File {
public:
	File(std::string _fileName, int64_t _fileSize) :fileName(_fileName), fileSize(_fileSize) {
		for (int64_t i = 0; i < 100000; i++) {
			insertAsFirst(i);
		}
	}
	File() {}
	~File() {
		for (int64_t i = 0; i < 100000; i++) {
			if (bufs[i].dirty) {
				writeFile(bufs[i].pageNumber, bufs[i].buffer);
			}
		}
	}
	int64_t read(int64_t pageNumber, char* buffer) {
		if (pageNumber >= fileSize) {
			return -2;
		}
		int64_t location = getLocation(pageNumber);
		if (location >= 0) {
			writeStr(buffer, 4096, bufs[location].buffer);
			insertAsFirst(location);
		}
		else {
			int64_t last = getLast();
			if ((bufs[last].pageNumber >= 0) && bufs[last].dirty) {
				writeFile(bufs[last].pageNumber, bufs[last].buffer);
			}
			hashMap.erase(bufs[last].pageNumber);
			readFile(pageNumber, bufs[last].buffer);
			bufs[last].pageNumber = pageNumber;
			bufs[last].dirty = false;
			writeStr(buffer, 4096, bufs[last].buffer);
			insertAsFirst(last);
			hashMap.insert(pair<int64_t, int64_t>(pageNumber, last));
		}
		return 0;
	}
	int64_t write(int64_t pageNumber, char* buffer) {
		if (pageNumber >= fileSize) {
			return -2;
		}
		int64_t location = getLocation(pageNumber);
		if (location >= 0) {
			writeStr(bufs[location].buffer, 4096, buffer);
			insertAsFirst(location);
			bufs[location].dirty = true;
		}
		else {
			int64_t last = getLast();
			if ((bufs[last].pageNumber >= 0) && bufs[last].dirty) {
				writeFile(bufs[last].pageNumber, bufs[last].buffer);
			}
			hashMap.erase(bufs[last].pageNumber);
			writeStr(bufs[last].buffer, 4096, buffer);
			bufs[last].pageNumber = pageNumber;
			insertAsFirst(last);
			bufs[last].dirty = true;
			hashMap.insert(pair<int64_t, int64_t>(pageNumber, last));
		}
		return 0;
	}
	int64_t size() {
		return fileSize;
	}
	int64_t alloc(int64_t pageNumber) {
		fileSize += pageNumber;
		return fileSize;
	}
	std::string fileName;
private:
	int64_t fileSize;
	BufClass listHead;
	BufClass bufs[100000];
	map<int64_t, int64_t> hashMap;
	int64_t readFile(int64_t pageNumber, char* buffer) {
		ifstream in(fileName, ios::binary);
		if (!in) {
			return -1;
		}
		in.seekg(0, fstream::end);
		int64_t size = in.tellg() / 4096;
		if (pageNumber >= size) {
			ZeroClear(buffer);
			return 0;
		}
		in.seekg(pageNumber * 4096, fstream::beg);
		in.read(buffer, 4096);
		in.close();
		return 0;
	}
	int64_t writeFile(int64_t pageNumber, char* buffer) {
		ofstream out(fileName, ios::in | ios::out | ios::binary);
		if (!out) {
			return -1;
		}
		out.seekp(0, fstream::end);
		int64_t size = out.tellp() / 4096;
		char* zeroPad = AllZeroFill(PAGE_SIZE);
		while (size < pageNumber) {
			out.write(zeroPad, 4096);
			size++;
		}
		delete[]zeroPad;
		out.seekp(pageNumber * 4096, fstream::beg);
		out.write(buffer, 4096);
		out.close();
		return 0;
	}
	int64_t getLocation(int64_t index) {
		auto it = hashMap.find(index);
		if (it != hashMap.end()) {
			return it->second;
		}
		return -1;
	}
	void insertAsFirst(int64_t index) {
		bufs[index].prev->next = bufs[index].next;
		bufs[index].next->prev = bufs[index].prev;
		bufs[index].prev = &listHead;
		bufs[index].next = listHead.next;
		listHead.next->prev = &bufs[index];
		listHead.next = &bufs[index];
	}
	int64_t getLast() {
		return listHead.prev - (BufClass*)(bufs);
	}
};
FileManager::FileManager() {
	fileMap1.clear();
	fileMap2.clear();
}
FileManager::~FileManager() {
	//for (auto it = fileMap.begin(); it != fileMap.end(); ++it) {
	//	close(it->first);
	//}
	fileMap1.clear();
	for (int i = 0; i < fileMap2.size(); i++)
		delete fileMap2[i];

	fileMap2.clear();
}

void FileManager::flush() {
	fileMap1.clear();
	for (int i = 0; i < fileMap2.size(); i++)
		delete fileMap2[i];

	fileMap2.clear();
}

bool FileManager::ifexist(std::string fileName) {
	for (int i = 0; i < fileMap1.size(); i++)
		if (fileMap2[i]->fileName == fileName) {
			return true;
		}

	ifstream fin(fileName);
	if (!fin) {
		return false;
	}
	else {
		fin.close();
		return true;
	}
}

int64_t FileManager::open(std::string fileName) {
	for (int i = 0; i < fileMap1.size(); i++)
		if (fileMap2[i]->fileName == fileName) {
			return fileMap1[i];
		}
	ofstream out(fileName, ios::app | ios::binary);
	if (!out) {
		return -1;
	}
	out.seekp(0, ios::end);
	int64_t fileSize = out.tellp() / 4096;
	int64_t minUnusedDescriptor = 1;
	for (int i = 0; i < fileMap1.size(); i++)
		if (fileMap1[i] >= minUnusedDescriptor)
			minUnusedDescriptor = fileMap1[i] + 1;
	File *file = new File(fileName, fileSize);
	fileMap1.push_back(minUnusedDescriptor);
	fileMap2.push_back(file);
	//fileMap[minUnusedDescriptor] = file;
	//fileMap.insert(pair<int64_t, File>(minUnusedDescriptor, file));
	return minUnusedDescriptor;
}
int64_t FileManager::close(int64_t fileDescriptor) {
	//	for (auto it = fileMap.begin(); it != fileMap.end(); it++) {
	//		if (it->first == fileDescriptor) {
	//			fileMap.erase(it);
	//			return 0;
	//		}
	//	}
	cout << "JCZ" << endl;
	for (int i = 0; i < fileMap1.size(); i++)
		if (fileMap1[i] == fileDescriptor) {
			cout << i << endl;
			fileMap1.erase(fileMap1.begin() + i);
			delete fileMap2[i];
			fileMap2.erase(fileMap2.begin() + i);
			return 0;
		}
	return -1;
}
int64_t FileManager::read(int64_t fileDescriptor, int64_t pageNumber, char * buffer) {
	for (int i = 0; i < fileMap1.size(); i++)
		if (fileMap1[i] == fileDescriptor)
			return fileMap2[i]->read(pageNumber, buffer);
	return -1;
}
int64_t FileManager::write(int64_t fileDescriptor, int64_t pageNumber, char * buffer) {
	for (int i = 0; i < fileMap1.size(); i++)
		if (fileMap1[i] == fileDescriptor)
			return fileMap2[i]->write(pageNumber, buffer);
	return -1;
}
int64_t FileManager::size(int64_t fileDescriptor) {
	for (int i = 0; i < fileMap1.size(); i++)
		if (fileMap1[i] == fileDescriptor)
			return fileMap2[i]->size();
	return -1;
}
int64_t FileManager::alloc(int64_t fileDescriptor, int64_t pageNumber) {
	for (int i = 0; i < fileMap1.size(); i++)
		if (fileMap1[i] == fileDescriptor)
			return fileMap2[i]->alloc(pageNumber);
	return -1;
}
int64_t FileManager::del(std::string & fileName){
	system((std::string("del ") + fileName).c_str());
	return 0;
}
std::vector<std::string> FileManager::getFileList(){
	std::vector<std::string> ret;
	std::string path = "./*.lyx";
	WIN32_FIND_DATA fd;
	HANDLE hFind = FindFirstFile(path.c_str(), &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				ret.push_back(fd.cFileName);
			}
		} while (FindNextFile(hFind, &fd));
		FindClose(hFind);
	}
	return ret;
}
