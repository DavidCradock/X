#include "PCH.h"
#include "fileMemory.h"
#include "logging.h"
#include "utilities.h"


namespace X
{
	CFileMemory::CFileMemory()
	{
		_mpchDataStart = 0;
		_mstDataSizeBytes = 0;
		_mpchSeekData = 0;
		_mstSeekPos = 0;
		_mbIsOK = true;
	}

	CFileMemory::~CFileMemory()
	{
		free();
	}

	void CFileMemory::free(void)
	{
		if (_mpchDataStart)
		{
			delete _mpchDataStart;
			_mpchDataStart = 0;
			_mstDataSizeBytes = 0;
			_mpchSeekData = 0;
			_mstSeekPos = 0;
		}
		_mbIsOK = true;
	}

	void CFileMemory::setFromFile(const std::string& strFilename)
	{
		ThrowIfFalse(getFileExists(strFilename), "CFileMemory::setFromFile(\"" + strFilename + "\") failed. Given filename could not be found.");

		// Free CFileMemory object incase it's been used before
		free();

		// Get file size
		std::ifstream ifstream(strFilename, std::ios::binary | std::ios::ate);
		ThrowIfFalse(ifstream.is_open(), "CFileMemory::setFromFile(\"" + strFilename + "\") failed. Given filename could not be opened.");
		_mstDataSizeBytes = ifstream.tellg();
		ifstream.seekg(std::ios::beg);

		// Set members of CFileMemory object and attempt to read in the data from file into it
		_mpchDataStart = new char[_mstDataSizeBytes];
		_mpchSeekData = _mpchDataStart;
		ThrowIfFalse(_mpchDataStart, "CFileMemory::setFromFile(\"" + strFilename + "\") failed. Unable to allocate memory for data from file.");
		ifstream.read(_mpchDataStart, _mstDataSizeBytes);
		ThrowIfFalse(ifstream.good(), "CFileMemory::setFromFile(\"" + strFilename + "\") failed. Error whilst reading in data from file.");
		ifstream.close();
	}

	void CFileMemory::setFromMem(const char* pData, size_t stDataLength)
	{
		// Make sure given values are valid
		ThrowIfFalse(stDataLength, "CFileMemory::setFromMem() failed. Data length given is zero.");
		ThrowIfFalse(pData, "CFileMemory::setFromMem() failed. Given memory address is zero.");

		// Free CFileMemory object incase it's been used before
		free();

		_mstDataSizeBytes = stDataLength;
		_mpchDataStart = new char[stDataLength];
		ThrowIfFalse(_mpchDataStart, "CFileMemory::setFromMem() failed. Unable to allocate memory for storing data.");
		_mpchSeekData = _mpchDataStart;
		memcpy(_mpchDataStart, pData, stDataLength);
	}

	void CFileMemory::saveToFile(const std::string& strFilename)
	{
		// Make sure we've got data to save
		ThrowIfFalse(_mpchDataStart, "CFileMemory::saveToFile(\"" + strFilename + "\") failed. There is no data to save to file.");

		// Attempt to open for writing
		std::ofstream file(strFilename, std::ios::binary | std::ios::trunc);
		ThrowIfFalse(file.is_open(), "CFileMemory::saveToFile(\"" + strFilename + "\") failed. Unable to open file for writing.");

		// Attempt to write data to file
		file.write(_mpchDataStart, _mstDataSizeBytes);
		ThrowIfFalse(file.good(), "CFileMemory::saveToFile(\"" + strFilename + "\") failed. Error occurred whilst writing data to file.");
		file.close();
	}

	void CFileMemory::appendToFile(std::ofstream& file)
	{
		// Make sure we've got data to save
		ThrowIfFalse(_mpchDataStart, "CFileMemory::appendToFile() failed. There is no data to append to file.");

		// Make sure given file is both good and open
		ThrowIfFalse(file.is_open(), "CFileMemory::appendToFile() failed. Given file is not open for writing.");
		ThrowIfFalse(file.good(), "CFileMemory::appendToFile() failed. Given file is not good.");

		// Attempt to append data to file
		file.write(_mpchDataStart, _mstDataSizeBytes);
		ThrowIfFalse(file.good(), "CFileMemory::appendToFile() failed. Error occurred whilst appending data to given file.");
	}

	char* CFileMemory::getData(void)
	{
		return _mpchDataStart;
	}

	size_t CFileMemory::getDataSize(void)
	{
		return _mstDataSizeBytes;
	}

	void CFileMemory::seekBegin(void)
	{
		_mpchSeekData = _mpchDataStart;
		_mstSeekPos = 0;
	}

	bool CFileMemory::seekToPos(size_t stOffsetBytesFromStart)
	{
		if (stOffsetBytesFromStart >= _mstDataSizeBytes)
		{
			_mbIsOK = false;
			return false;
		}

		_mpchSeekData = _mpchDataStart;
		_mstSeekPos = 0;

		_mpchSeekData += stOffsetBytesFromStart;
		_mstSeekPos += stOffsetBytesFromStart;
		return true;
	}

	bool CFileMemory::seekOffset(size_t stOffsetBytes)
	{
		if (_mstSeekPos + stOffsetBytes >= _mstDataSizeBytes)
		{
			_mbIsOK = false;
			return false;
		}

		_mpchSeekData += stOffsetBytes;
		_mstSeekPos += stOffsetBytes;
		return true;
	}

	size_t CFileMemory::seekGetPos(void)
	{
		return _mstSeekPos;
	}

	bool CFileMemory::isOK(void)
	{
		return _mbIsOK;
	}

	void CFileMemory::reset(void)
	{
		_mbIsOK = true;
		seekBegin();
	}
}