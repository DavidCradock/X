#include "PCH.h"
#include "GUIBaseObject.h"

namespace X
{
	GUIBaseObject::GUIBaseObject()
	{
		mfPositionX = mfPositionY = 0.0f;
		mfWidth = mfHeight = 128.0f;

		// Compute texture coordinates for the nine components of a GUI object/widget
		// Compute texture coordinates for each of the 9 components
		float point3 = 0.3333333f;
		float point6 = 0.6666666f;

		// Centre quad
		mTC.centre.BL.x = point3;
		mTC.centre.TR.x = point6;
		mTC.centre.BR.x = point6;
		mTC.centre.TL.x = point3;
		mTC.centre.BL.y = point3;
		mTC.centre.TR.y = point6;
		mTC.centre.BR.y = point3;
		mTC.centre.TL.y = point6;

		// Top left corner
		mTC.topLeft.BL.x = 0.0f;
		mTC.topLeft.TR.x = point3;
		mTC.topLeft.BR.x = point3;
		mTC.topLeft.TL.x = 0.0f;
		mTC.topLeft.BL.y = point3;
		mTC.topLeft.TR.y = 0.0f;
		mTC.topLeft.BR.y = point3;
		mTC.topLeft.TL.y = 0.0f;

		// Top right corner
		mTC.topRight.BL.x = point6;
		mTC.topRight.TR.x = 1.0f;
		mTC.topRight.BR.x = 1.0f;
		mTC.topRight.TL.x = point6;
		mTC.topRight.BL.y = point3;
		mTC.topRight.TR.y = 0.0f;
		mTC.topRight.BR.y = point3;
		mTC.topRight.TL.y = 0.0f;

		// Bottom left corner
		mTC.bottomLeft.BL.x = 0.0f;
		mTC.bottomLeft.TR.x = point3;
		mTC.bottomLeft.BR.x = point3;
		mTC.bottomLeft.TL.x = 0.0f;
		mTC.bottomLeft.BL.y = 1.0f;
		mTC.bottomLeft.TR.y = point6;
		mTC.bottomLeft.BR.y = 1.0f;
		mTC.bottomLeft.TL.y = point6;

		// Bottom right corner
		mTC.bottomRight.BL.x = point6;
		mTC.bottomRight.TR.x = 1.0f;
		mTC.bottomRight.BR.x = 1.0f;
		mTC.bottomRight.TL.x = point6;
		mTC.bottomRight.BL.y = 1.0f;
		mTC.bottomRight.TR.y = point6;
		mTC.bottomRight.BR.y = 1.0f;
		mTC.bottomRight.TL.y = point6;

		// Top edge
		mTC.top.BL.x = point3;
		mTC.top.TR.x = point6;
		mTC.top.BR.x = point6;
		mTC.top.TL.x = point3;
		mTC.top.BL.y = point3;
		mTC.top.TR.y = 0.0f;
		mTC.top.BR.y = point3;
		mTC.top.TL.y = 0.0f;

		// Bottom edge
		mTC.bottom.BL.x = point3;
		mTC.bottom.TR.x = point6;
		mTC.bottom.BR.x = point6;
		mTC.bottom.TL.x = point3;
		mTC.bottom.BL.y = 1.0f;
		mTC.bottom.TR.y = point6;
		mTC.bottom.BR.y = 1.0f;
		mTC.bottom.TL.y = point6;

		// Left edge
		mTC.left.BL.x = 0.0f;
		mTC.left.TR.x = point3;
		mTC.left.BR.x = point3;
		mTC.left.TL.x = 0.0f;
		mTC.left.BL.y = point3;
		mTC.left.TR.y = point6;
		mTC.left.BR.y = point3;
		mTC.left.TL.y = point6;

		// Right edge
		mTC.right.BL.x = point6;
		mTC.right.TR.x = 1.0f;
		mTC.right.BR.x = 1.0f;
		mTC.right.TL.x = point6;
		mTC.right.BL.y = point3;
		mTC.right.TR.y = point6;
		mTC.right.BR.y = point3;
		mTC.right.TL.y = point6;
	}

	void GUIBaseObject::setPosition(float fPosX, float fPosY)
	{
		mfPositionX = fPosX;
		mfPositionY = fPosY;
	}

	void GUIBaseObject::setDimensions(float fWidth, float fHeight)
	{
		mfWidth = fWidth;
		mfHeight = fHeight;
	}
}