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

		// Interpolates between this colour and the one given by the given float and returns the new colour.
		// If the float is 0.0f, the returned colour will be this colour.
		// If the float is 0.5f, the returned colour will be half way bewteen this colour and the one given.
		// If the float is 1.0f, the returned colour will be the one given.
		CColour interpolate(const CColour other, float fValue) const;

		bool operator == (const CColour& col)
		{
			if (red == col.red && green == col.green && blue == col.blue && alpha == col.alpha)
				return true;
			return false;
		}

		float red;		// Red colour between 0.0f and 1.0f
		float green;	// Green colour between 0.0f and 1.0f
		float blue;		// Blue colour between 0.0f and 1.0f
		float alpha;	// Alpha colour between 0.0f and 1.0f
	};
}