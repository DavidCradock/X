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

	void CColour::setFromHSB(float fHueAmount, float fSaturation, float fBrightness)
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

	void CColour::getHSB(float& fHue, float& fSaturation, float& fBrightness) const
	{
		// Get maximum and minimum values of current RGB values
		float cmax = std::max(red, std::max(green, blue));
		float cmin = std::min(red, std::min(green, blue));
		float diff = cmax - cmin; // diff of cmax and cmin.
		fHue = -1, fSaturation = -1;

		// If cmax and cmax are equal then fHue = 0
		if (cmax == cmin)
			fHue = 0;

		// If cmax equal r then compute fHue
		else if (cmax == red)
			fHue = float(fmod(60 * ((green - blue) / diff) + 360, 360));

		// If cmax equal g then compute fHue
		else if (cmax == green)
			fHue = float(fmod(60 * ((blue - red) / diff) + 120, 360));

		// If cmax equal b then compute fHue
		else if (cmax == blue)
			fHue = float(fmod(60 * ((red - green) / diff) + 240, 360));

		fHue /= 360.0f;

		// If cmax equal zero
		if (cmax == 0)
			fSaturation = 0;
		else
			fSaturation = (diff / cmax);// *100;

		// Compute fBrightness
		fBrightness = cmax;// *100;
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