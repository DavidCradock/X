#pragma once
#include "PCH.h"

namespace X
{
	// A file archive is a file holding files.
	// A file archive can be an uncompressed ".arc" file.
	// It's usefull for hiding files from the end user, although nothing, even with encryption is entirely hidden.
	class CFileArchive
	{
	public:
		// Constructor
		// Initially a file archive is in an un-opened state.
		// Use open() to open a file archive file ready for reading/writing
		CFileArchive();

		// Destructor
		// Closes file handle if open
		~CFileArchive();

		// Opens an ".arc" file ready for use.
		// If the file doesn't exist, it is created


	private:
		// This is stored at the end of the archive file.
		// It stores information needed to determine whether the file is infact an .arc file, what version it is and where the file information is located.
		struct SFooter
		{
			std::string strArcFileID;	// Should be "X::ARCFILE"
			size_t stVersionNumber;		// 1, 2 etc
			size_t stFileInfoPosition;	// Position in file where file information begins
			size_t stFileInfoLength;	// Length in file of the file information
		};

		// This holds each file stored inside the archive's information.
		struct SFileInfov1
		{
			size_t stFilenameLength;
			std::string strFilename;
			size_t stDataPosition;
			size_t stDataLength;
		};
	};
}