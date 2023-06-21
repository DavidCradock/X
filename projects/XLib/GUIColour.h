#pragma once
#include "PCH.h"

namespace X
{
	// Used throughout the GUI code to represent a colour
	class GUIColour
	{
	public:
		// Constructor, sets to default values
		GUIColour();

		// Sets the colour to the given values.
		// Clamps the values in the range of 0.0f and 1.0f
		void set(float fRed, float fGreen, float fBlue, float fAlpha);

		float red;		// Red colour between 0.0f and 1.0f
		float green;	// Green colour between 0.0f and 1.0f
		float blue;		// Blue colour between 0.0f and 1.0f
		float alpha;	// Alpha colour between 0.0f and 1.0f
	};
}