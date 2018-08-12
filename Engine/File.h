#pragma once
#include "Surface.h"
#include <string.h>

class File
{
public:
	File(std::string filename);
	Surface& GetFile();
private:
	Surface file;
};