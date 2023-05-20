#pragma once
#include "PCH.h"

namespace X
{
	// Due to floating point errors, we use this to determine whether
	// two floating points are roughly equal
	const float Epsilon = 0.000001f;
	inline bool areFloatsEqual(const float& A, const float& B)
	{
		return (fabs(A - B) < Epsilon);
	}

	// Converts a string to wide string
	inline std::wstring StringToWString(const std::string& str)
	{
		WCHAR buffer[512];
		MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, buffer, 512);
		return std::wstring(buffer);
	}

	// Convertes a wide string to a string
	inline std::string WStringToString(const std::wstring& wstr)
	{
		// Setup converter
//		using convert_type = std::codecvt_utf8<wchar_t>;
//		std::wstring_convert<convert_type, wchar_t> converter;
//		std::string str = converter.to_bytes(wstr);
//		return str;
		CHAR buffer[512];
		WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, buffer, 512, NULL, NULL);
		return std::string(buffer);
	}

	// Returns a vector of strings holding each filename stored within the given directory
	// If bRecursiveDirs is true, then files within sub directories are also added instead of just the ones in the given directory.
	std::vector<std::string> getFilesInDir(const std::string &strDirectory, bool bRecursiveDirs = false);

	// Returns a vector of strings holding each filename stored within the given directory which have the given filename extension
	// If bRecursiveDirs is true, then files within sub directories are also added instead of just the ones in the given directory.
	// The parsed filename extension may or may not have the "." character at the start.
	std::vector<std::string> getFilesInDir(const std::string &strDirectory, const std::string& ext, bool bRecursiveDirs = false);

	// Returns a vector of strings holding each directory stored within the given directory
	// If bRecursiveDirs is true, then directories within sub directories are also added instead of just the ones in the given directory.
	std::vector<std::string> getDirsInDir(const std::string &strDirectory, bool bRecursiveDirs = false);

	// Returns the current directory
	std::string getCurrentDirectory(void);


}
