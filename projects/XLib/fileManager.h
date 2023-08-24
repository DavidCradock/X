#pragma once
#include "PCH.h"
#include "singleton.h"
#include "logging.h"

namespace X
{
	// File manager responsible for dealing with files.
	// Lots of file helper methods in here as well as managing compressed archive files.
	class SCFileManager : public CSingleton<SCFileManager>
	{
	public:
		SCFileManager();

		// Helper method to write the given string to an already opened ofstream in binary mode.
		// If the given ofstream is not opened or an error occurred, an exception occurs
		void write(std::ofstream& file, std::string& strString) const;

		// Helper method to read a string from an already opened ifstream in binary mode.
		// If the given ifstream is not opened or an error occurred, an exception occurs
		// The given string is cleared before being set.
		void read(std::ifstream& file, std::string& strString) const;

		// Helper method. Returns true if the given filename exists on storage device
		bool getFileExists(const std::string& strFilename) const;

		// Read a type from an already opened ifstream in binary mode.
		// data This will hold the data being read from the stream.
		// If the given ifstream is not opened or an error occurred, an exception occurs
		// returns false if an error occurred.
		template <class Type> void read(std::ifstream& file, Type& data);

		// Write a type to an already opened ofstream in binary mode.
		// data This is what will be written to the ofstream.
		// If the given ofstream is not opened or an error occurred, an exception occurs
		// returns false if an error occurred.
		template <class Type> void write(std::ofstream& file, Type& data);

		// Attempts to delete the given file name.
		// If the file still exists after attempting to delete it, this method returns false, else true
		bool deleteFile(const std::string& strFilenameToDelete) const ;

		// Attempts to rename the given file name to the new filename.
		// Returns whether the operation was a success or not
		bool renameFile(const std::string& strOldFilename, const std::string& strNewFilename) const;
	};

	template<class Type> void SCFileManager::read(std::ifstream& file, Type& data)
	{
		ThrowIfFalse(file.is_open(), "SCFileManager::read() failed. The given ifstream is not open.");
		ThrowIfTrue(file.bad(), "SCFileManager::read() failed. The given ifstream is bad.");

		size_t stTypeSize = sizeof(Type);
		file.read((char*)&data, stTypeSize);
		ThrowIfTrue(file.bad(), "SCFileManager::read() failed. The given ifstream is bad.");
	}

	template<class Type> void SCFileManager::write(std::ofstream& file, Type& data)
	{
		ThrowIfFalse(file.is_open(), "SCFileManager::write() failed. The given ofstream is not open.");
		ThrowIfTrue(file.bad(), "SCFileManager::write() failed. The given ofstream is bad.");

		size_t stTypeSize = sizeof(Type);
		file.write((char*)&data, stTypeSize);
		ThrowIfTrue(file.bad(), "SCFileManager::write() failed. The given ofstream is bad.");
	}
}