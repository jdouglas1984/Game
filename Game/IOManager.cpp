#include "IOManager.h"

#include <fstream>

bool IOManager::readFileToBuffer(std::string filePath, std::vector<unsigned char>& buffer)
{
	std::ifstream file(filePath, std::ios::binary);
	if (file.fail())
	{
		perror(filePath.c_str());
		return false;
	}

	//seek the end
	file.seekg(0, std::ios::end);

	//get file size
	int fileSize = file.tellg();

	//seek to beginning
	file.seekg(0, std::ios::beg);

	//removes header bytes that may be present
	fileSize -= file.tellg();

	buffer.resize(fileSize);

	file.read((char *)&(buffer[0]), fileSize);

	file.close();

	return true;
}