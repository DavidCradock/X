#pragma once
#include "PCH.h"
#include "singleton.h"

namespace X
{
	// The SCSettings class which holds all global settings.
	// These settings are things which are usually accessed from a program's settings interface and include such things as..
	// Shadow map resolution, backbuffer resolution, global audio volumn and more.
	class SCSettings : public CSingleton<SCSettings>
	{
	public:

		// Default constructor which sets, settings to their defaults.
		SCSettings();

		// Attempts to load all the settings from a file.
		// If the file couldn't be found, all the settings are set to default values, the settings are saved and false is returned, else true
		bool load(const std::string& strFilename = "settings.cfg");

		// Saves all settings to a file.
		void save(const std::string& strFilename = "settings.cfg");

		// Sets the multiplier for the back buffer framebuffer which many things are rendered to.
		// This, by default is set to 1.0.
		// A value of 1.0f is 100 percent of the window dims.
		// Setting this smaller will reduce GPU usage.
		// Setting this greater will increase quality.
		// The given value is clamped between 0.1 - 4.0
		// This also resizes the framebuffer used by the backbuffer.
		// This re-saves the settings file.
		void setBackbufferScale(float fBackbufferScale);

		// Returns the currently set back buffer scaling value.
		// See setBackbufferScaling for more information.
		float getBackbufferScale(void) const;

		// Returns whether the application's window should be created fullscreen or not.
		bool getWindowFullscreen(void) const;

		// Sets whether the application's window should be created fullscreen or not.
		// This re-saves the settings file.
		void setWindowFullscreen(bool bFullscreen);

		// Returns whether the application's window should set VSync on or off.
		bool getWindowVSync(void) const;

		// Sets whether the application's window should set VSync on or off.
		// This re-saves the settings file.
		void setWindowVSync(bool bFullscreen);

	private:

		// Sets all settings to default values
		void _setAllToDefault(void);

		// Multiplies the dimensions of the application's window and is used to set the scale of the
		// backbuffer frame buffer which alot of things are rendered to.
		float _mfBackbufferScale;

		// Whether the application window should be created windowed or fullscreen
		bool _mbWindowFullscreen;

		// Whether the application window should set VSync to enabled or not
		bool _mbWindowVSync;

	};
}