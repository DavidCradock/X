#include "PCH.h"
#include "utilities.h"
#include "log.h"

namespace X
{
	std::vector<std::string> getFilesInDir(std::string &strDirectory, bool bRecursiveDirs)
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

	// Returns a vector of strings holding each filename stored within the given directory which have the given filename extension
	std::vector<std::string> getFilesInDir(std::string& strDirectory, std::string& ext, bool bRecursiveDirs)
	{
		// Make sure the given extension has the "." character
		ThrowIfTrue(ext.length() < 1, "getFilesInDir() extension has length of zero.");
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
					if (filename.path().extension() == ext)
						filenames.push_back(filename.path().string());
				}
			}
		}
		return filenames;
	}

	std::vector<std::string> getDirsInDir(std::string& strDirectory, bool bRecursiveDirs)
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
}