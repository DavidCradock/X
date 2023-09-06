#pragma once
#include "PCH.h"
#include "resourceTexture2DAtlas.h"
#include "resourceVertexBufferCPT2.h"
#include "UITheme.h"

namespace X
{
	class CUIContainer;

	// A simple image.
	// There's no borders or anything, just a simple texture resource to render.
	// The named texture resource stored in SCResourceManager will be resized to this widget's set dimensions.
	class CUIImage
	{
		friend class CUIContainer;
	public:
		CUIImage(CUIContainer* pContainer);
		~CUIImage();

		// Sets the dimensions of the widget.
		void setDimensions(float fX, float fY);

		// Sets the dimensions of the widget.
		void setDimensions(const CVector2f& vDimensions);

		// Returns the dimensions of the widget.
		CVector2f getDimensions(void) const;

		// Sets the position of this widget in relation to it's container.
		// Both axis should be at least zero. I wish there was an unsigned float, but oh well :)
		void setPosition(float fX, float fY);

		// Sets the position of this widget in relation to it's container.
		// Both axis should be at least zero. I wish there was an unsigned float, but oh well :)
		void setPosition(const CVector2f& vPosition);

		// Returns the position of the widget in relation to it's container.
		CVector2f getPosition(void) const;

		// Sets whether this widget is visible or not.
		void setVisible(bool bVisible);

		// Returns whether this widget is visible or not.
		bool getVisible(void) const;

		// Render this widget
		void render(void);

		/******************************************************************* Widget specific *******************************************************************/

		// Sets the name of the CResourceTexture2DFromFile stored in the SCResourceManager which is used to render this widget.
		// If the resource doesn't exist, an exception occurs.
		void setTexture(const std::string& strNameOfTexture2DFromFileResource);

		// Sets the colour multiplier used when rendering the image.
		// By default, this is CColour(1,1,1,1);
		void setColour(const CColour& colour = CColour(1, 1, 1, 1));
	private:
		// Common amoung widgets
		CVector2f _mvDimensions;			// Dimensions of the widget
		CVector2f _mvPosition;				// Position of the widget in relation to it's container.
		bool _mbVisible;					// Whether this widget is visible or not.
		CUIContainer* _mpContainer;			// The container this widget belongs to. Set in constructor

		// Widget specific
		// The name of the CResourceTexture2DFromFile resource stored in SCResourceManager.
		// Set with setTexture()
		std::string _mstrTextureResourceName;
		CColour _mColour;	// Colour multiplier used when rendering the texture image.
	};
}