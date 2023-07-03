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

	// Returns true if the given disk filename exists.
	bool getFileExists(const std::string& strFilename);

	// Converts the given string to lowercase
	void stringToLowercase(std::string& str);

	// Takes the given filename and filename extension and adds the given filename extension to the end of the given filename and returns the result.
	// The returned filename is all lowercase.
	// The filename extension may or may not have the "." at the beginning. If it doesn't exist, it is added.
	// If passed strings are of zero length, an exception occurs.
	std::string addFilenameExtension(const std::string& strFilenameExtension, const std::string& strFilename);

	// Converts the contents of a file into an array, stored inside a text header file, for inclusion of external files, inside the executable of a program
	bool convertFileToHeader(const std::string &strFilename, const std::string &strArrayName = "data", unsigned int iNumElementsPerRow = 20);

	// Sets the given colour values based on given hue value (between zero and one)
	extern void getHueColour(float fHueAmount, float& fRed, float& fGreen, float& fBlue);

	const float kPi = 3.14159265f;			// Pi constant
	const float k2Pi = kPi * 2.0f;			// 2 * Pi (Radians in a circle)
	const float kPiOver2 = kPi / 2.0f;		// Pi divided by 2 (90 degrees in radians)
	const float k1OverPi = 1.0f / kPi;
	const float k1Over2Pi = 1.0f / k2Pi;
	const float kPiOver180 = kPi / 180.0f;
	const float k180OverPi = 180.0f / kPi;

	// Maximum values for basic data types
	const double  kMaxDouble = (std::numeric_limits<double>::max)();
	const double  kMinDouble = (std::numeric_limits<double>::min)();
	const float   kMaxFloat = (std::numeric_limits<float>::max)();
	const float   kMinFloat = (std::numeric_limits<float>::min)();
	const int     kMaxInt = (std::numeric_limits<int>::max)();

	// Comparing a variable when it becomes invalid always returns false.
	// Nice easy way to check if I've done something stupid with a zero and a division somewhere
	template <typename Type>
	inline bool isNAN(Type type)
	{
		return type != type;
	}

	// Converts degrees to radians
	inline float deg2rad(float fAngleDegrees) { return fAngleDegrees * kPiOver180; }

	// Converts radians to degrees
	inline float rad2deg(float fAngleRadians) { return fAngleRadians * k180OverPi; }

	// Clamps given value within the range of -1 to +1 then performs standard acos function
	inline float acosClamped(float f)
	{
		if (f < -1.0f)
			return kPi;
		if (f > 1.0f)
			return 0.0f;
		return acosf(f);
	}

	// Computes both sin and cos of a scalar (Sometime faster to calculate both at same time)
	inline void sincos(float& outSin, float& outCos, float fScalar)
	{
		outSin = sinf(fScalar);
		outCos = cosf(fScalar);
	}

	// Clamps a value within given range
	template <class type>
	inline void clamp(type& valueToClamp, type valueA, type valueB)
	{
		type min;
		type max;
		if (valueA < valueB)
		{
			min = valueA;
			max = valueB;
		}
		else
		{
			min = valueB;
			max = valueA;
		}
		if (valueToClamp < min)
			valueToClamp = min;
		else if (valueToClamp > max)
			valueToClamp = max;
	}

	// Sets given variable to absolute value
	template <class type>
	inline void absolute(type& value)
	{
		if (value < 0)
			value *= -1;
	}

	// Computes the difference
	template <class type>
	inline type difference(type valueA, type valueB)
	{
		type result = valueA - valueB;
		absolute(result);
		return result;
	}

	// Returns a random integer between x and y
	inline int randInt(int x, int y) { return rand() % (y - x + 1) + x; }

	// Returns a random float between zero and 1
	inline float randFloat() { return float(rand()) / float(RAND_MAX + 1.0f); }

	// Returns a random double between zero and 1
	inline double randDouble() { return double(rand()) / double(RAND_MAX + 1.0); }

	// Returns a random bool
	inline bool randBool()
	{
		if (randInt(0, 1)) return true;
		return false;
	}

	// Returns a random float in the range -1 < n < 1
	inline double randomClamped() { return randFloat() - randFloat(); }

	// Returns a random float within the specified range
	inline float randf(float fMin, float fMax)
	{
		float fZeroToOne = (float)rand() / float(RAND_MAX + 1.0);
		return fMin + (fMax - fMin) * fZeroToOne;
	}

	// Given two vectors, returns the rotation quaternion needed to rotate v1 to match v2
	glm::quat rotationBetweenVectors(glm::vec3 v1, glm::vec3 v2);

	// Holds system memory information which is set by getMemInfo()
	struct SMemInfo
	{
		// Holds process memory usage 
		struct SMemProcess
		{
			unsigned int iPageFaultCount;				// The number of page faults.
			unsigned int iPeakWorkingSetSize;			// The peak working set size. 
			unsigned int iWorkingSetSize;				// The current working set size. 
			unsigned int iQuotaPeakPagedPoolUsage;		// The peak paged pool usage. 
			unsigned int iQuotaPagedPoolUsage;			// The current paged pool usage. 
			unsigned int iQuotaPeakNonPagedPoolUsage;	// The peak nonpaged pool usage. 
			unsigned int iQuotaNonPagedPoolUsage;		// The current nonpaged pool usage. 
			unsigned int iPagefileUsage;				// The current pagefile usage. 
			unsigned int iPeakPagefileUsage;			// The peak pagefile usage.
			unsigned int iPrivateUsage;					// Same as PagefileUsage. The Commit Charge value in bytes for this process. Commit Charge is the total amount of private memory that the memory manager has committed for a running process.
		};

		// Holds OS memory usage
		struct SMemOS
		{
			unsigned int iCommitTotal;			// Total number of pages committed by the system. 
			unsigned int iCommitLimit;			// Current maximum number of page commits that can be performed by the system. This number can change if memory is added or deleted, or if pagefiles have grown, shrunk, or been added. 
			unsigned int iCommitPeak;			// Maximum number of page commit totals that have occurred since the last reboot. 
			unsigned int iPhysicalTotal;		// Total amount, in pages, of physical memory. 
			unsigned int iPhysicalAvailable;	// Amount, in pages, of physical memory available to user processes. 
			unsigned int iSystemCache;			// Total amount, in pages, of system cache memory. 
			unsigned int iKernelTotal;			// Total amount, in pages, of the sum of the paged and nonpaged kernel pools. 
			unsigned int iKernelPaged;			// Total amount, in pages, of the paged kernel pool. 
			unsigned int iKernelNonpaged;		// Total amount, in pages, of the nonpaged kernel pool. 
			unsigned int iPageSize;				// Size, in bytes, of a page. 
			unsigned int iHandleCount;			// Total number of open handles. 
			unsigned int iProcessCount;			// Total number of processes. 
			unsigned int iThreadCount;			// Total number of threads. 
		};
		SMemProcess proc;		// Process information
		SMemOS os;				// Operating system information
	};

	// Places memory information about the process and system into the given SMemInfo structure
	void getMemInfo(SMemInfo& memInfo);
}
