#include "PCH.h"
#include "stringUtils.h"
#include "logging.h"

namespace X
{
	namespace StringUtils
	{

		std::wstring stringToWide(const std::string& string)
		{
			if (string.empty())
				return std::wstring();
			int iSize = MultiByteToWideChar(CP_UTF8, 0, &string[0], (int)string.size(), NULL, 0);
			std::wstring wstrOut(iSize, 0);
			MultiByteToWideChar(CP_UTF8, 0, &string[0], (int)string.size(), &wstrOut[0], iSize);
			return wstrOut;
		}

		std::string wideToString(const std::wstring& wstring)
		{
			if (wstring.empty())
				return std::string();
			int iSize = WideCharToMultiByte(CP_UTF8, 0, &wstring[0], (int)wstring.size(), NULL, 0, NULL, NULL);
			std::string strOut(iSize, 0);
			WideCharToMultiByte(CP_UTF8, 0, &wstring[0], (int)wstring.size(), &strOut[0], iSize, NULL, NULL);
			return strOut;
		}

		std::vector<std::string> getFilesInDir(const std::string& strDirectory, bool bRecursiveDirs)
		{
			std::vector<std::string> filenames;
			if (bRecursiveDirs)
			{
				for (const auto& filename : std::filesystem::recursive_directory_iterator(strDirectory))
				{
					if (filename.is_regular_file())
						filenames.push_back(filename.path().string());
				}
			}
			else
			{
				for (const auto& filename : std::filesystem::directory_iterator(strDirectory))
				{
					if (filename.is_regular_file())
						filenames.push_back(filename.path().string());
				}
			}
			return filenames;
		}

		std::vector<std::string> getFilesInDir(const std::string& strDirectory, const std::string& ext, bool bRecursiveDirs)
		{
			// Make sure the given extension has the "." character
			ThrowIfTrue(0 == ext.length(), "CStringHelper::getFilesInDir() extension has length of zero.");
			std::string strExt;
			if (ext.c_str()[0] != '.')
				strExt.append(".");
			strExt.append(ext);

			std::vector<std::string> filenames;
			if (bRecursiveDirs)
			{
				for (const auto& filename : std::filesystem::recursive_directory_iterator(strDirectory))
				{
					if (filename.is_regular_file())
					{
						if (filename.path().extension() == strExt)
							filenames.push_back(filename.path().string());
					}
				}
			}
			else
			{
				for (const auto& filename : std::filesystem::directory_iterator(strDirectory))
				{
					if (filename.is_regular_file())
					{
						if (filename.path().extension() == strExt)
							filenames.push_back(filename.path().string());
					}
				}
			}
			return filenames;
		}

		std::vector<std::string> getDirsInDir(const std::string& strDirectory, bool bRecursiveDirs)
		{
			std::vector<std::string> dirnames;
			if (bRecursiveDirs)
			{
				for (const auto& dirname : std::filesystem::recursive_directory_iterator(strDirectory))
				{
					if (dirname.is_directory())
						dirnames.push_back(dirname.path().string());
				}
			}
			else
			{
				for (const auto& dirname : std::filesystem::directory_iterator(strDirectory))
				{
					if (dirname.is_directory())
						dirnames.push_back(dirname.path().string());
				}
			}
			return dirnames;
		}

		std::string getCurrentDirectory(void)
		{
			return std::filesystem::current_path().string();
		}

		void stringToLowercase(std::string& str)
		{
			std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return std::tolower(c); });
		}

		std::string addFilenameExtension(const std::string& strFilenameExtension, const std::string& strFilename)
		{
			// Make sure valid values given.
			ThrowIfTrue(0 == strFilenameExtension.length(), "CStringHelper::addFilenameExtension() failed. Given extension name of zero length.");
			ThrowIfTrue(0 == strFilename.length(), "CStringHelper::addFilenameExtension() failed. Given file name of zero length.");

			// Append "." to extension if needed
			std::string strExt = strFilenameExtension;
			if (strExt.c_str()[0] != '.')
			{
				std::string::iterator itBegin = strExt.begin();
				strExt.insert(itBegin, '.');
			}
			std::string strFile = strFilename;
			// Find last position of "." in given file name and remove everything after it
			auto const pos = strFile.find_last_of('.');
			if (pos != std::string::npos)	// If "." found
			{
				// Remove "." and all following text
				strFile.erase(pos, strFile.length() - pos);
			}
			// Append extension to filename
			strFile.append(strExt);

			// Make all lowercase
			stringToLowercase(strFile);
			return strFile;
		}

		void appendInt(std::string& string, int iInt)
		{
			string += std::to_string(iInt);
		}

		void appendUInt(std::string& string, unsigned int uiInt)
		{
			string += std::to_string(uiInt);
		}

		void appendFloat(std::string& string, float fValue, unsigned int uiNumDecimalPoints)
		{
			switch (uiNumDecimalPoints)
			{
			case 0:	string += std::format("{:.0f}", fValue);	break;
			case 1:	string += std::format("{:.1f}", fValue);	break;
			case 2:	string += std::format("{:.2f}", fValue);	break;
			case 3:	string += std::format("{:.3f}", fValue);	break;
			case 4:	string += std::format("{:.4f}", fValue);	break;
			case 5:	string += std::format("{:.5f}", fValue);	break;
			case 6:	string += std::format("{:.6f}", fValue);	break;
			case 7:	string += std::format("{:.7f}", fValue);	break;
			case 8:	string += std::format("{:.8f}", fValue);	break;
			case 9:	string += std::format("{:.9f}", fValue);	break;
			default:string += std::format("{:.2f}", fValue);
			}
		}

		void appendDouble(std::string& string, double dValue, unsigned int uiNumDecimalPoints)
		{
			switch (uiNumDecimalPoints)
			{
			case 0:	string += std::format("{:.0f}", dValue);	break;
			case 1:	string += std::format("{:.1f}", dValue);	break;
			case 2:	string += std::format("{:.2f}", dValue);	break;
			case 3:	string += std::format("{:.3f}", dValue);	break;
			case 4:	string += std::format("{:.4f}", dValue);	break;
			case 5:	string += std::format("{:.5f}", dValue);	break;
			case 6:	string += std::format("{:.6f}", dValue);	break;
			case 7:	string += std::format("{:.7f}", dValue);	break;
			case 8:	string += std::format("{:.8f}", dValue);	break;
			case 9:	string += std::format("{:.9f}", dValue);	break;
			default:string += std::format("{:.2f}", dValue);
			}
		}

		void appendCVector3f(std::string& string, const CVector3f& vector, unsigned int uiNumDecimalPoints, const std::string& strSeperatorText)
		{
			std::string strTmp;
			appendFloat(strTmp, vector.x, uiNumDecimalPoints);
			string += strTmp;
			string += strSeperatorText;

			strTmp.clear();
			appendFloat(strTmp, vector.y, uiNumDecimalPoints);
			string += strTmp;
			string += strSeperatorText;

			strTmp.clear();
			appendFloat(strTmp, vector.z, uiNumDecimalPoints);
			string += strTmp;
		}

		std::vector<std::string> splitString(const std::string& string, const std::string& strSplitChars)
		{
			std::vector<std::string> out;

			// If no strSplitChars found, simply add the entire string and return the result
			size_t pos = string.find(strSplitChars, 0);
			if (std::string::npos == pos)
			{
				out.push_back(string);
				return out;
			}

			// If we get here, strSplitChars has been found in the string
			std::string strLine;
			std::string strAll = string;
			while (std::string::npos != pos)
			{
				// Copy character upto the position of the found strSplitChars into strLine
				strLine.assign(strAll, 0, pos);

				// Add the line to the output
				out.push_back(strLine);

				// Reset strLine
				strLine.clear();

				// Remove all characters including the strSplitChars from strAll
				strAll.erase(0, pos + strSplitChars.length());

				// Find next position of strSplitChars in strAll
				pos = strAll.find(strSplitChars, 0);
			}
			// If strAll still contains characters, add them to the vector
			if (strAll.length())
			{
				out.push_back(strAll);
			}

			return out;
		}

		bool representsNumber(const std::string& string)
		{
			return (std::all_of(string.begin(), string.end(), ::isdigit));
		}

		void stringWrite(const std::string& strString, std::ofstream& file)
		{
			ThrowIfFalse(file.is_open(), "StringUtils::stringWrite() failed. The given ofstream is not open.");
			size_t size = strString.size();
			file.write((char*)&size, sizeof(size));
			file.write(strString.c_str(), size);
			ThrowIfFalse(file.good(), "StringUtils::stringWrite() failed. The ofstream is not good.");
		}

		void stringRead(std::string& strString, std::ifstream& file)
		{
			ThrowIfFalse(file.is_open(), "StringUtils::stringRead() failed. The given ofstream is not open.");
			strString.clear();
			size_t size;
			file.read((char*)&size, sizeof(size));
			strString.resize(size);
			//file.read(&strString[0], size);
			file.read((char*)strString.c_str(), size);
			ThrowIfFalse(file.good(), "StringUtils::stringRead() failed. The ifstream is not good.");
		}

		std::string blenderAnimFilename(const std::string& strBasename, const std::string& strExtension, int iFrameNumber)
		{
			ThrowIfTrue(iFrameNumber < 0 || iFrameNumber > 9999, "StringUtils::blenderAnimFilename() given invalid iFrameNumber.");
			std::string output;
			output = strBasename;
			if (iFrameNumber < 10)
			{
				output += "000";
				output += std::to_string(iFrameNumber);
			}
			else if (iFrameNumber < 100)
			{
				output += "00";
				output += std::to_string(iFrameNumber);
			}
			else if (iFrameNumber < 1000)
			{
				output += "0";
				output += std::to_string(iFrameNumber);
			}
			else
			{
				output += std::to_string(iFrameNumber);
			}

			// Append "." to extension if needed
			std::string strExt = strExtension;
			if (strExt.c_str()[0] != '.')
			{
				std::string::iterator itBegin = strExt.begin();
				strExt.insert(itBegin, '.');
			}
			output += strExt;
			return output;
		}

		float stringToFloat(const std::string& string)
		{
			float strfloat = std::stof(string);
			return strfloat;
		}
	}

}