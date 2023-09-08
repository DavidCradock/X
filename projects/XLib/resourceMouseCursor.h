#pragma once
#include "PCH.h"
#include "resourceBase.h"

namespace X
{
	// A Windows mouse cursor resource.
	// The name given to the constructor is the filename of either an ".ani" or ".cur" file.
	// However, you can pass in a string with zero length and when calling set(), the operating system's default cursor is set.
	// The cursor is only loaded when the application's OpenGL context is created.
	// This isn't needed, but it keeps consistency with all the other resource types.
	// To actually set the mouse cursor, call set()
	class CResourceMouseCursor : public CResourceBase
	{
	public:
		// Constructor accepting the filename holding the cursor's image data and the name of the resource
		CResourceMouseCursor(const std::string& strFilename, const std::string& strResourceName);
		~CResourceMouseCursor();

		// Create all OpenGL context dependent objects when an OpenGL context exists.
		void onGLContextCreated(void);

		// Free all OpenGL context dependent objects before the OpenGL context is destroyed.
		void onGLContextToBeDestroyed(void);

		// Sets the mouse cursor resource to the operating system's mouse cursor.
		void set(void);

	private:
		// Filename of the ".ani" or ".cur" file.
		std::string _mstrFilename;

		// Holds the resource name given to the constructor
		std::string _mstrResourceName;

		// Mouse cursor handle holding the loaded cursor
		HCURSOR _mhCursor;
	};
}