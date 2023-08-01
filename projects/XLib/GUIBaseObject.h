#pragma once
#include "PCH.h"
#include "colour.h"
#include "vector2f.h"

namespace X
{
	// Base class which most GUI classes derive from
	class CGUIBaseObject
	{
	public:
		CGUIBaseObject();

		// Set position of object
		void setPosition(float fPosX, float fPosY);
		
		// Set position to centre of window
		void setPositionCentreWindow(void);

		// Set dimensions of object
		void setDimensions(float fWidth, float fHeight);

		// Gets dimensions of object as a vector
		CVector2f getDimensions(void) const;

		// Renders the four edges, four corners and centre quad which make up an object's background
		void renderBackground(
			void* pParentContainer,							// Pointer to the object's parent container
			const std::string& strFramebufferToSampleFrom,	// The framebuffer to use as background when sampling (Given to SCGUIManager::render() and passed down)
			const std::string& strObjectColourTextureName,	// The name of the object's background colour texture
			const std::string& strObjectNormalTextureName,	// The name of the object's background normal texture
			CColour& colour									// Colour
			);		

		float mfPositionX;	// Top left position of object along X axis
		float mfPositionY;	// Top left position of object along Y axis
		float mfWidth;		// Width of the object along X axis
		float mfHeight;		// Height of the object along Y axis

		// Four vectors containing texture coordinates for a quad
		struct TexCoordsQuad
		{
			CVector2f BL;	// Bottom left texture coordinate
			CVector2f BR;	// Bottom right texture coordinate
			CVector2f TL;	// Top left texture coordinate
			CVector2f TR;	// Top right texture coordinate
		};

		// Most GUI objects need texture coordinates for each of the nine "components" taken from the theme's texture maps
		// For example, a container has nine images inside a texture representing the four corners, the four edges and the centre.
		// These are computed inside CGUIBaseObject()
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