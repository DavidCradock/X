#include "PCH.h"
#include "utilities.h"
#include "log.h"

namespace X
{
	unsigned int getNumberOfCPUs(void)
	{
//		SYSTEM_INFO si;
//		GetSystemInfo(&si);
//		return unsigned int(si.dwNumberOfProcessors);
		return std::thread::hardware_concurrency();
	}

	std::vector<std::string> getFilesInDir(const std::string &strDirectory, bool bRecursiveDirs)
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
		ThrowIfTrue(0 == ext.length(), "getFilesInDir() extension has length of zero.");
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

	bool getFileExists(const std::string& strFilename)
	{
		FILE* f;
		if (fopen_s(&f, strFilename.c_str(), "rb"))
			return false;
		fclose(f);
		return true;
	}

	bool convertFileToHeader(const std::string &strFilename, const std::string &strArrayName, unsigned int uiNumElementsPerRow)
	{
		FILE* fs = NULL;  // Source file
		FILE* fd = NULL;  // Destination file

		std::string strFilenameInput = strFilename;
		std::string strFilenameOutput = strFilename;
		strFilenameOutput.append(".h");

		// Open source
		fopen_s(&fs, strFilename.c_str(), "rb");
		if (fs == NULL)
			return false;

		// Open destination
		fopen_s(&fd, strFilenameOutput.c_str(), "wb");
		if (fd == NULL)
		{
			fclose(fs);
			return false;
		}

		// Write comment giving name of binary file that this data came from
		fprintf(fd, "// Following data taken from file \"%s\"\n", strFilename.c_str());

		// Get length of data
		fseek(fs, 0, SEEK_END);
		int length = ftell(fs);
		fseek(fs, 0, SEEK_SET);

		// Write #define for length of array
		std::string strArrayNameUppercase = strArrayName;
		std::transform(strArrayNameUppercase.begin(), strArrayNameUppercase.end(), strArrayNameUppercase.begin(), ::toupper);
		strArrayNameUppercase.append("_SIZE");
		fprintf(fd, "#define %s %d\n", strArrayNameUppercase.c_str(), length);

		// Write start of array
		fprintf(fd, "unsigned char %s[] =\n", strArrayName.c_str());
		fprintf(fd, "{\n ");

		// Write out data from source binary file
		while (feof(fs) == false)
		{
			unsigned char tmp;
			for (int e = 0; e < (int)uiNumElementsPerRow; e++)
			{
				fread(&tmp, 1, sizeof(unsigned char), fs);
				if (feof(fs) == false)
					fprintf(fd, "0x%02x,", tmp);
			}

			if (feof(fs)) // Reached end of file
			{
				fseek(fd, -1, SEEK_CUR);  // Remove last comma
				fprintf(fd, "};");
			}
			fprintf(fd, "\n ");
		}
		fprintf(fd, "\n");

		fclose(fs);
		fclose(fd);
		return true;
	}

	void getHueColour(float fHueAmount, float& fRed, float& fGreen, float& fBlue)
	{
		clamp(fHueAmount, 0.0f, 1.0f);
		fHueAmount *= 360.0f;
		if (fHueAmount <= 60.0f)	// Inc green
		{
			fRed = 1.0f;
			fGreen = fHueAmount / 60.0f;
			fBlue = 0.0f;
		}
		else if (fHueAmount <= 120.0f)	// Dec red
		{
			fRed = 1.0f - ((fHueAmount - 60.0f) / 60.0f);
			fGreen = 1.0f;
			fBlue = 0.0f;
		}
		else if (fHueAmount <= 180.0f)	// Inc blue
		{
			fRed = 0.0f;
			fGreen = 1.0f;
			fBlue = (fHueAmount - 120.0f) / 60.0f;
		}
		else if (fHueAmount <= 240.0f)	// Dec green
		{
			fRed = 0.0f;
			fGreen = 1.0f - ((fHueAmount - 180.0f) / 60.0f);
			fBlue = 1.0f;
		}
		else if (fHueAmount <= 300.0f)	// Inc red
		{
			fRed = (fHueAmount - 240.0f) / 60.0f;
			fGreen = 0.0f;
			fBlue = 1.0f;
		}
		else // dec blue
		{
			fRed = 1.0f;
			fGreen = 0.0f;
			fBlue = 1.0f - ((fHueAmount - 300.0f) / 60.0f);
		}
	}
}