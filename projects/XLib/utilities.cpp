#include "PCH.h"
#include "utilities.h"
#include "log.h"

namespace X
{
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

	glm::quat rotationBetweenVectors(glm::vec3 v1, glm::vec3 v2)
	{
		v1 = normalize(v1);
		v2 = normalize(v2);

		float cosTheta = dot(v1, v2);
		glm::vec3 rotationAxis;

		// Special case when both vectors are in opposite directions
		// There is no "ideal" rotation axis, so guess one. Any will do as long as it's perpendicular to v1
		if (cosTheta < -1 + 0.001f)
		{	
			rotationAxis = glm::cross(glm::vec3(0.0f, 0.0f, 1.0f), v1);
			if (glm::length2(rotationAxis) < 0.01) // They were parallel
				rotationAxis = glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), v1);
			rotationAxis = normalize(rotationAxis);
			return glm::angleAxis(glm::radians(180.0f), rotationAxis);
		}
		rotationAxis = cross(v1, v2);
		float s = sqrt((1 + cosTheta) * 2);
		float invs = 1 / s;
		return glm::quat(s * 0.5f, rotationAxis.x * invs, rotationAxis.y * invs, rotationAxis.z * invs);
	}

	void getMemInfo(SMemInfo& memInfo)
	{
		SecureZeroMemory(&memInfo, sizeof(SMemInfo));

		HANDLE hAppProcess = GetCurrentProcess();
		if (!hAppProcess)	// Found application process handle?
			return;

		// Retreive and store process mem usage
		PROCESS_MEMORY_COUNTERS_EX pmc;
		if (GetProcessMemoryInfo(hAppProcess, (PPROCESS_MEMORY_COUNTERS)&pmc, sizeof(PROCESS_MEMORY_COUNTERS_EX)))
		{
			memInfo.proc.iPageFaultCount = (unsigned int)pmc.PageFaultCount;
			memInfo.proc.iPeakWorkingSetSize = (unsigned int)pmc.PeakWorkingSetSize;
			memInfo.proc.iWorkingSetSize = (unsigned int)pmc.WorkingSetSize;
			memInfo.proc.iQuotaPeakPagedPoolUsage = (unsigned int)pmc.QuotaPeakPagedPoolUsage;
			memInfo.proc.iQuotaPagedPoolUsage = (unsigned int)pmc.QuotaPagedPoolUsage;
			memInfo.proc.iQuotaPeakNonPagedPoolUsage = (unsigned int)pmc.QuotaPeakNonPagedPoolUsage;
			memInfo.proc.iQuotaNonPagedPoolUsage = (unsigned int)pmc.QuotaNonPagedPoolUsage;
			memInfo.proc.iPagefileUsage = (unsigned int)pmc.PagefileUsage;
			memInfo.proc.iPeakPagefileUsage = (unsigned int)pmc.PeakPagefileUsage;
			memInfo.proc.iPrivateUsage = (unsigned int)pmc.PrivateUsage;
		}

		PERFORMACE_INFORMATION pin;
		if (GetPerformanceInfo(&pin, sizeof(PERFORMACE_INFORMATION)))
		{
			memInfo.os.iCommitLimit = (unsigned int)pin.CommitLimit;
			memInfo.os.iCommitPeak = (unsigned int)pin.CommitPeak;
			memInfo.os.iCommitTotal = (unsigned int)pin.CommitTotal;
			memInfo.os.iHandleCount = (unsigned int)pin.HandleCount;
			memInfo.os.iKernelNonpaged = (unsigned int)pin.KernelNonpaged;
			memInfo.os.iKernelPaged = (unsigned int)pin.KernelPaged;
			memInfo.os.iKernelTotal = (unsigned int)pin.KernelTotal;
			memInfo.os.iPageSize = (unsigned int)pin.PageSize;
			memInfo.os.iPhysicalAvailable = (unsigned int)pin.PhysicalAvailable;
			memInfo.os.iPhysicalTotal = (unsigned int)pin.PhysicalTotal;
			memInfo.os.iProcessCount = (unsigned int)pin.ProcessCount;
			memInfo.os.iSystemCache = (unsigned int)pin.SystemCache;
			memInfo.os.iThreadCount = (unsigned int)pin.ThreadCount;
		}
	}

	int getCPULogicalCores(void)
	{
		SYSTEM_INFO si;
		GetSystemInfo(&si);
		return int(si.dwNumberOfProcessors);
	}


}