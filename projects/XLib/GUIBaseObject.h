#pragma once
#include "PCH.h"

namespace X
{
	// Base class which most GUI classes derive from
	class GUIBaseObject
	{
	public:
		GUIBaseObject();

		int miPositionX;	// Top left position of object along X axis
		int miPositionY;	// Top left position of object along Y axis
		int miWidth;		// Width of the object along X axis
		int miHeight;		// Height of the object along Y axis

	};
}