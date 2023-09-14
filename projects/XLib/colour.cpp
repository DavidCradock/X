#include "PCH.h"
#include "colour.h"
#include "utilities.h"

namespace X
{
	CColour::CColour()
	{
		set(1.0f, 1.0f, 1.0f, 1.0f);
	}

	CColour::CColour(float fRed, float fGreen, float fBlue, float fAlpha)
	{
		set(fRed, fGreen, fBlue, fAlpha);
	}

	void CColour::set(float fRed, float fGreen, float fBlue, float fAlpha)
	{
		clamp(fRed, 0.0f, 1.0f);
		clamp(fGreen, 0.0f, 1.0f);
		clamp(fBlue, 0.0f, 1.0f);
		clamp(fAlpha, 0.0f, 1.0f);
		red = fRed;
		green = fGreen;
		blue = fBlue;
		alpha = fAlpha;
	}

	void CColour::setHueColour(float fHueAmount, float fSaturation, float fBrightness)
	{
		// Clamp each passed value
		clamp(fHueAmount, 0.0f, 1.0f);
		clamp(fSaturation, 0.0f, 1.0f);
		clamp(fBrightness, 0.0f, 1.0f);

		// Compute colour
		fHueAmount *= 360.0f;
		float fReciprical60 = 1.0f / 60.0f;
		if (fHueAmount <= 60.0f)	// Inc green
		{
			red = 1.0f;
			green = fHueAmount * fReciprical60;
			blue = 0.0f;
		}
		else if (fHueAmount <= 120.0f)	// Dec red
		{
			red = 1.0f - ((fHueAmount - 60.0f) * fReciprical60);
			green = 1.0f;
			blue = 0.0f;
		}
		else if (fHueAmount <= 180.0f)	// Inc blue
		{
			red = 0.0f;
			green = 1.0f;
			blue = (fHueAmount - 120.0f) * fReciprical60;
		}
		else if (fHueAmount <= 240.0f)	// Dec green
		{
			red = 0.0f;
			green = 1.0f - ((fHueAmount - 180.0f) * fReciprical60);
			blue = 1.0f;
		}
		else if (fHueAmount <= 300.0f)	// Inc red
		{
			red = (fHueAmount - 240.0f) * fReciprical60;
			green = 0.0f;
			blue = 1.0f;
		}
		else // dec blue
		{
			red = 1.0f;
			green = 0.0f;
			blue = 1.0f - ((fHueAmount - 300.0f) * fReciprical60);
		}

		// Now RGB is set, apply saturation
		float fSaturationScaleR = 1.0f - red;
		float fSaturationScaleG = 1.0f - green;
		float fSaturationScaleB = 1.0f - blue;
		red += fSaturationScaleR * fSaturation;
		green += fSaturationScaleG * fSaturation;
		blue += fSaturationScaleB * fSaturation;
		clamp(red, 0.0f, 1.0f);
		clamp(green, 0.0f, 1.0f);
		clamp(blue, 0.0f, 1.0f);

		// Now saturation is added, apply brightness
		float fBrightnessInv = 1.0f - fBrightness;
		red -= fBrightnessInv;
		green -= fBrightnessInv;
		blue -= fBrightnessInv;
		clamp(red, 0.0f, 1.0f);
		clamp(green, 0.0f, 1.0f);
		clamp(blue, 0.0f, 1.0f);
	}

	CColour CColour::interpolate(const CColour other, float fValue) const
	{
		clamp(fValue, 0.0f, 1.0f);

		CColour colour;
		float fDiff = red - other.red;		// -1.0f (This is 0.0f and other is 1.0f) to 1.0f (This is 1.0f and other is 0.0f)
		fDiff *= -1.0f;						// 1.0f (This is 0.0f and other is 1.0f) to -1.0f (This is 1.0f and other is 0.0f)
		colour.red = red + (fDiff * fValue);

		fDiff = green - other.green;
		fDiff *= -1.0f;
		colour.green = green + (fDiff * fValue);

		fDiff = blue - other.blue;
		fDiff *= -1.0f;
		colour.blue = blue + (fDiff * fValue);

		fDiff = alpha - other.alpha;
		fDiff *= -1.0f;
		colour.alpha = alpha + (fDiff * fValue);
		return colour;
	}
}