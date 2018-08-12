#include "File.h"

File::File(std::string filename)
	:
	file(Surface(filename))
{
}

Surface& File::GetFile()
{
	return file;
}
