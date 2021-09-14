#include <vector>

#include <fstream>
#include <cstdint>
#include <iostream>

#define DATA_FILE_IO_GOOD 1
#define DATA_FILE_IO_BAD 0

namespace DataFileIO 
{
	/*
	 Loads the bytes that were stored using DataFileIO::Save and returns it in
	 Returns a pointer that you MUST call delete to
	 Takes a reference to an int that stores the size of the buffer
	*/
	template<typename T>
	T* Load(const char* filepath, unsigned int* size = nullptr);

	/*
	 Stores contiguous blocks of data onto the disk
	 Size parameter takes the number of bytes to be saved, NOT the number of elements
	*/
	bool Save(const char* filepath, const void* data, size_t size, bool append = false);
}

#ifdef _DEBUG
#define DATA_FILE_IO_DEBUG_PRINT(x) std::cout<<x<<std::endl
#else
#define DEBUG_PRINT(x)
#endif

#define DATA_FILE_IO_PREFIX "DATA_FILE_IO ERROR: "

template<typename T>
T* DataFileIO::Load(const char* filepath, unsigned int* size/*= nullptr*/)
{
	std::ifstream infile(filepath, std::ios_base::binary);

	if (infile.bad() || infile.fail())
	{
		DATA_FILE_IO_DEBUG_PRINT(DATA_FILE_IO_PREFIX << "Error opening file");
		return nullptr;
	}


	infile.seekg(0, std::ios_base::end);
	size_t fileSize = infile.tellg();		/*Gets the total size of the file in bytes*/
	infile.seekg(0, std::ios_base::beg);

	if (fileSize < 1) // If file is empty
	{
		DATA_FILE_IO_DEBUG_PRINT(DATA_FILE_IO_PREFIX << "File is empty");
		infile.close();
		return nullptr;
	}

	unsigned char* buffer = new unsigned char[fileSize];
	infile.read((char*)buffer, fileSize);

	if (size != nullptr)
		*size = fileSize / sizeof(T);

	infile.close();
	return reinterpret_cast<T*>(buffer);
}

bool DataFileIO::Save(const char* filepath, const void* data, size_t size, bool append/*= false*/)
{
	int writeMode = append ? std::ios_base::app : std::ios_base::trunc; // Determines if the user wants to either append, or truncate the file
	
	std::ofstream outfile(filepath, std::ios_base::binary | writeMode);

	if (outfile.bad())
	{
		DATA_FILE_IO_DEBUG_PRINT(DATA_FILE_IO_PREFIX << "Error saving to file");
		return DATA_FILE_IO_BAD;
	}

	outfile.write((const char*)data, size); // Writes the individual bytes into the file

	outfile.close();
	return DATA_FILE_IO_GOOD;
}
