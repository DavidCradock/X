#include "PCH.h"
#include "utilities.h"

namespace X
{
	std::vector<std::string> getFilesInDir(std::string &strDirectory)
	{
		std::vector<std::string> filenames;
		std::string strFilename;
		for (const auto& filename : std::filesystem::directory_iterator(strDirectory))
		{
			if (filename.is_regular_file())
				filenames.push_back(filename.path().string());
		}
		return filenames;
	}

	std::vector<std::string> getDirsInDir(std::string& strDirectory)
	{
		std::vector<std::string> filenames;
		std::string strFilename;
		for (const auto& filename : std::filesystem::directory_iterator(strDirectory))
		{
			if (filename.is_directory())
				filenames.push_back(filename.path().string());
		}
		return filenames;
	}

	std::string getCurrentDirectory(void)
	{
		return std::filesystem::current_path().string();
	}
}