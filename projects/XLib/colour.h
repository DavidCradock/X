#pragma once
#include "PCH.h"

namespace X
{
	// Used to represent a colour
	class CColour
	{
	public:
		// Constructor, sets to default values (white with full alpha)
		CColour();

		// Constructor, sets to passed values
		CColour(float fRed, float fGreen, float fBlue, float fAlpha);

		// Sets the colour to the given values.
		// Clamps the values in the range of 0.0f and 1.0f
		void set(float fRed, float fGreen, float fBlue, float fAlpha);

		// Sets this colour to the given hue.
		// Hue value should be between 0 and 1 (Although it is clamped)
		// Saturation value should be between 0 and 1 (Although it is clamped)
		// Brightness value should be between 0 and 1 (Although it is clamped)
		void setHueColour(float fHueAmount, float fSaturation = 1.0f, float fBrightness = 1.0f);

		float red;		// Red colour between 0.0f and 1.0f
		float green;	// Green colour between 0.0f and 1.0f
		float blue;		// Blue colour between 0.0f and 1.0f
		float alpha;	// Alpha colour between 0.0f and 1.0f
	};
}