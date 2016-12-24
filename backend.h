#ifndef BACKEND_H
#define BACKEND_H
#include "IXManager.h"
#include "RMManager.h"
class BackEnd {
	IXManager* im;
	RMManager* rm;
	FileManager* fm;
public:
	BackEnd() {}
	void createDataBase(std::string& name);
	void InsertRecord()
};

#endif