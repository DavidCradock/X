#pragma once
#include "PCH.h"
#include "vector3f.h"

namespace X
{
	namespace StringUtils
	{
		// Converts a string to wide string
		std::wstring stringToWide(const std::string& string);

		// Converts a wide string to a string
		std::string wideToString(const std::wstring& wstring);

		// Returns a vector of strings holding each filename stored within the given directory
		// If bRecursiveDirs is true, then files within sub directories are also added instead of just the ones in the given directory.
		std::vector<std::string> getFilesInDir(const std::string& strDirectory, bool bRecursiveDirs = false);

		// Returns a vector of strings holding each filename stored within the given directory which have the given filename extension
		// If bRecursiveDirs is true, then files within sub directories are also added instead of just the ones in the given directory.
		// The parsed filename extension may or may not have the "." character at the start.
		std::vector<std::string> getFilesInDir(const std::string& strDirectory, const std::string& ext, bool bRecursiveDirs = false);

		// Returns a vector of strings holding each directory stored within the given directory
		// If bRecursiveDirs is true, then directories within sub directories are also added instead of just the ones in the given directory.
		std::vector<std::string> getDirsInDir(const std::string& strDirectory, bool bRecursiveDirs = false);

		// Returns the current directory
		std::string getCurrentDirectory(void);

		// Converts the given string to lowercase
		void stringToLowercase(std::string& str);

		// Takes the given filename and filename extension and adds the given filename extension to the end of the given filename and returns the result.
		// The returned filename is all lowercase.
		// The filename extension may or may not have the "." at the beginning. If it doesn't exist, it is added.
		// If passed strings are of zero length, an exception occurs.
		// Also, if the strFilename already has an extension, it is removed and replaced with the new one.
		std::string addFilenameExtension(const std::string& strFilenameExtension, const std::string& strFilename);

		// Append an integer to given string
		void appendInt(std::string& string, int iInt);

		// Append an integer to given string
		void appendUInt(std::string& string, unsigned int uiInt);

		// Append float to given string with the given number of decimals
		// uiNumDecimalPoints can be from 0 to 9, if outside this range, then 2 is used.
		void appendFloat(std::string& string, float fValue, unsigned int uiNumDecimalPoints = 2);

		// Append double to given string with the given number of decimals
		// uiNumDecimalPoints can be from 0 to 9, if outside this range, then 2 is used.
		void appendDouble(std::string& string, double dValue, unsigned int uiNumDecimalPoints = 2);

		// Appends a CVector3's components to the string, seperated by the specified seperator text
		// If the vector's x,y and z components are 0.0f, 1.0f and 2.0f and the seperator text is ", " then the string would look like...
		// 0.0, 1.0, 2.0
		// uiNumDecimalPoints can be from 0 to 9, if outside this range, then 2 is used.
		void appendCVector3f(std::string& string, const CVector3f& vector, unsigned int uiNumDecimalPoints = 1, const std::string& strSeperatorText = ", ");

		// Given a string, finds each occurrence of strSplitChars and returns a vector of strings which
		// contains strings that are either side of the strSplitChars, with the strSplitChars removed.
		// This is usefull for splitting text into multiple lines, using a specified character sequence used to
		// represent the end of the line of text.
		// For example, if the string is "Line1\nLine2\nLine3" and the strSpliteChars is "\n"
		// then the returned vector would have 3 entries, "Line1" "Line2" and "Line3"
		std::vector<std::string> splitString(const std::string& string, const std::string& strSplitChars = "\n");

		// Returns true if the given string has nothing but digits
		bool representsNumber(const std::string& string);

		// Writes out a string to an already opened ofstream.
		// The writes out the size of the string and then the string itself.
		// It's paired with stringRead()
		// If the file is not open, an exception occurs
		void stringWrite(const std::string& strString, std::ofstream& file);

		// Reads in a string from an already opened ifstream.
		// This reads in the size of the string, resizes it to make room and then loads it in
		// If the file is not open or there was an error during reading, an exception occurs.
		void stringRead(std::string& strString, std::ifstream& file);
	}
}