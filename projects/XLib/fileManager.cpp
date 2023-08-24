#include "PCH.h"
#include "fileManager.h"


namespace X
{
	SCFileManager::SCFileManager()
	{
		SCLog::getPointer()->add("SCFileManager::SCFileManager() called.");
	}

	void SCFileManager::write(std::ofstream& file, std::string& strString) const
	{
		ThrowIfFalse(file.is_open(), "SCFileManager::write() failed. The given ofstream is not open.");
		ThrowIfTrue(file.bad(), "SCFileManager::write() failed. The given ofstream is bad.");

		// Write out length of string
		size_t stringLength = strString.size();
		file.write((char*)&stringLength, sizeof(size_t));

		// Write out characters
		file.write(strString.c_str(), stringLength);

		ThrowIfTrue(file.bad(), "SCFileManager::write() failed. The given ofstream is bad.");
	}

	void SCFileManager::read(std::ifstream& file, std::string& strString) const
	{
		ThrowIfFalse(file.is_open(), "SCFileManager::read() failed. The given ifstream is not open.");
		ThrowIfTrue(file.bad(), "SCFileManager::read() failed. The given ifstream is bad.");

		// Read in length of string
		size_t stringLength;
		file.read((char*)&stringLength, sizeof(size_t));

		// Read in characters
		char* charTemp = new char[stringLength];
		ThrowIfFalse(charTemp, "SCFileManager::read() failed. Temporary memory allocation failed.");
		file.read(charTemp, stringLength);
		ThrowIfTrue(file.bad(), "SCFileManager::read() failed. The given ifstream is bad.");

		strString.clear();
		strString.append(charTemp);
		delete[] charTemp;
	}

	bool SCFileManager::getFileExists(const std::string& strFilename) const
	{
		/*
		FILE* f;
		if (fopen_s(&f, strFilename.c_str(), "rb"))
			return false;
		fclose(f);
		return true;
		*/
		bool bExists = false;
		std::ifstream file(strFilename);
		if (file.is_open())
		{
			bExists = true;
			file.close();
		}
		return bExists;
	}

	bool SCFileManager::deleteFile(const std::string& strFilenameToDelete) const
	{
		std::remove(strFilenameToDelete.c_str());
		return !getFileExists(strFilenameToDelete);
	}

	bool SCFileManager::renameFile(const std::string& strOldFilename, const std::string& strNewFilename) const
	{
		if (0 == std::rename(strOldFilename.c_str(), strNewFilename.c_str()))
			return true;
		return false;
	}
}