#include "PCH.h"
#include "GUIColour.h"
#include "utilities.h"

namespace X
{
	GUIColour::GUIColour()
	{
		set(1.0f, 1.0f, 1.0f, 1.0f);
	}

	void GUIColour::set(float fRed, float fGreen, float fBlue, float fAlpha)
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

	glm::vec4 GUIColour::get(void)
	{
		glm::vec4 col;
		col.r = red;
		col.g = green;
		col.b = blue;
		col.a = alpha;
		return col;
	}
}