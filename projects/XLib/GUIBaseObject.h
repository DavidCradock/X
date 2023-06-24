#pragma once
#include "PCH.h"

namespace X
{
	// Base class which most GUI classes derive from
	class GUIBaseObject
	{
	public:
		GUIBaseObject();

		float mfPositionX;	// Top left position of object along X axis
		float mfPositionY;	// Top left position of object along Y axis
		float mfWidth;		// Width of the object along X axis
		float mfHeight;		// Height of the object along Y axis

		// Four vectors containing texture coordinates for a quad
		struct TexCoordsQuad
		{
			glm::vec2 BL;	// Bottom left texture coordinate
			glm::vec2 BR;	// Bottom right texture coordinate
			glm::vec2 TL;	// Top left texture coordinate
			glm::vec2 TR;	// Top right texture coordinate
		};

		// Most GUI objects need texture coordinates for each of the nine "components" taken from the theme's texture maps
		// For example, a container has nine images inside a texture representing the four corners, the four edges and the centre.
		// These are computed inside GUIBaseObject()
		struct TextureCoordinates
		{
			TexCoordsQuad centre;
			TexCoordsQuad topLeft;
			TexCoordsQuad topRight;
			TexCoordsQuad bottomLeft;
			TexCoordsQuad bottomRight;
			TexCoordsQuad top;
			TexCoordsQuad bottom;
			TexCoordsQuad left;
			TexCoordsQuad right;
		};
		TextureCoordinates mTC;
	};
}