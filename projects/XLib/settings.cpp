#include "PCH.h"
#include "settings.h"
#include "utilities.h"
#include "log.h"
#include "stringUtils.h"
#include "singletons.h"

namespace X
{
	SCSettings::SCSettings()
	{
		SCLog* pLog = SCLog::getPointer();
		pLog->add("SCSettings::SCSettings() called.");

		// Attempt to load from file, if fail, set defaults.
		load();
	}

	bool SCSettings::load(const std::string& strFilename)
	{
		SCLog* pLog = SCLog::getPointer();

		// Open file in text mode
		std::ifstream file;
		file.open(strFilename, std::ofstream::in);
		if (!file.is_open())
		{
			pLog->add("SCSettings::load(" + strFilename + ") failed to load from file.");
			_setAllToDefault();
			return false;
		}

		// Filename
		std::string strWord;
		std::string strLine;
		char space;
		getline(file, strLine);

		// _mfBackbufferScale
		file >> strWord;	space = file.get();	// Setting name, then space
		//getline(file, strLine);
		file >> _mfBackbufferScale;

		// _mbWindowFullscreen
		file >> strWord;	space = file.get();	// Setting name, then space
		file >> _mbWindowFullscreen;

		// _mbWindowVSync
		file >> strWord;	space = file.get();	// Setting name, then space
		file >> _mbWindowVSync;





		// If an error occurred
		if (file.fail())
		{
			_setAllToDefault();
			file.close();
			save();
			return false;
		}
		
		file.close();
		return true;
	}

	void SCSettings::save(const std::string& strFilename)
	{
		// Open file in text mode, deleting contents if existed before
		std::ofstream file;
		file.open(strFilename, std::ofstream::out | std::ofstream::trunc);
		ThrowIfFalse(file.is_open(), "SCSettings::saveToFile(" + strFilename + ") failed to open file for writing.");

		std::string str;

		str = strFilename + "\n";
		file << str;

		// _mfBackbufferScale
		str = "BackbufferScale: " + std::to_string(_mfBackbufferScale) + "\n";
		file << str;

		// _mbWindowFullscreen
		str = "WindowFullscreen: " + std::to_string(_mbWindowFullscreen) + "\n";
		file << str;

		// _mbWindowVSync
		str = "WindowVSync: " + std::to_string(_mbWindowVSync) + "\n";
		file << str;
		


		// Make sure there were no errors
		ThrowIfTrue(file.fail(), "SCSettings::save(" + strFilename + ") failed whilst saving file.");
		file.close();
	}

	void SCSettings::_setAllToDefault(void)
	{
		SCLog* pLog = SCLog::getPointer();
		pLog->add("SCSettings::_setAllToDefault() called.");

		_mfBackbufferScale = 1.0f;
		_mbWindowFullscreen = false;
		_mbWindowVSync = true;
	}

	void SCSettings::setBackbufferScale(float fBackbufferScale)
	{
		clamp(fBackbufferScale, 0.01f, 4.0f);
		_mfBackbufferScale = fBackbufferScale;
		save();
	}

	float SCSettings::getBackbufferScale(void) const
	{
		return _mfBackbufferScale;
	}

	bool SCSettings::getWindowFullscreen(void) const
	{
		return _mbWindowFullscreen;
	}

	void SCSettings::setWindowFullscreen(bool bFullscreen)
	{
		_mbWindowFullscreen = bFullscreen;
		save();
	}

	bool SCSettings::getWindowVSync(void) const
	{
		return _mbWindowVSync;
	}

	void SCSettings::setWindowVSync(bool bFullscreen)
	{
		_mbWindowVSync = bFullscreen;
		save();
	}
}