#pragma once
#include "PCH.h"

namespace X
{
	// A FileMemory object holds the entirety of a file in memory
	class CFileMemory
	{
		friend class CFileSystem;
	public:
		// Constructor
		// Sets data to 0
		CFileMemory();

		// Destructor.
		// Deletes data if there is any.
		~CFileMemory();

		// Frees all data and resets everything
		void free(void);

		// Loads data from a file stored on storage device into this object
		// If something went wrong, an exception occurs
		void setFromFile(const std::string& strFilename);

		// Sets data from given pointer and size
		// If something went wrong, an exception occurs
		void setFromMem(const char* pData, size_t stDataLength);

		// Attempts to save the contents of this object to a file on storage device
		// If something went wrong, an exception occurs.
		// The named file is overwritten.
		void saveToFile(const std::string& strFilename);

		// Attemps to append the contents of this object to an already opened file on storage device
		// If something went wrong, an exception occurs.
		void appendToFile(std::ofstream& file);

		// Gets a pointer to the beginning of the file data stored in memory
		// May be 0 if no data has been loaded via CFileSystem.
		char* getData(void);

		// Returns the size of the data stored in memory in bytes
		// May be 0 if no data has been loaded via CFileSystem.
		size_t getDataSize(void);

		// Set seek position to the start of the data stored in this object
		void seekBegin(void);

		// Sets seek position by the given offset from the start of this object's data.
		// stOffsetBytesFromStart The offset from the beginning of this object's data in bytes.
		// returns false if the given offset goes past the end, else true.
		bool seekToPos(size_t stOffsetBytesFromStart);

		// Offsets the current seek position from the current position by the specified amount in bytes.
		// stOffsetBytes The number of bytes to move the seek position in bytes.
		// returns false if the given offset would move the new position past the end of the data of the object.
		bool seekOffset(size_t stOffsetBytes);

		// Returns the current seek position in bytes.
		size_t seekGetPos(void);

		// Read a type from this object's data and move the seek position forward.
		// out This is a type that will hold the data being read from the file.
		// returns false if the type could not be read and _mbIsOK is set to false.
		template <class Type> bool read(Type& out);

		// Returns whether an error occurred either due to seeking to an invalid position or read() failing
		bool isOK(void);

		// Resets any errors so isOK() will return true again and resets seek position to beginning by calling seekBegin().
		void reset(void);
	private:

		char* _mpchDataStart;		// Bytes of data
		size_t _mstDataSizeBytes;	// Size of data in bytes

		char* _mpchSeekData;		// Pointer to data at current offset
		size_t _mstSeekPos;			// Current offset in bytes

		bool _mbIsOK;				// Has there been a failure? Either by seeking to incorrect position or read() failing
	};

	template<class Type> bool CFileMemory::read(Type& out)
	{
		size_t stTypeSize = sizeof(Type);

		// Past end of array?
		if (_mstSeekPos + stTypeSize > _mstDataSizeBytes)
		{
			_mbIsOK = false;
			return false;
		}

		// Store value
		out = *(Type*)_mpchSeekData;

		// Increment pointer and position
		_mpchSeekData += stTypeSize;
		_mstSeekPos += stTypeSize;
		return true;
	}
}