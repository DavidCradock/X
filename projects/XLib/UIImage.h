#pragma once
#include "PCH.h"
#include "resourceTexture2DAtlas.h"
#include "resourceVertexBufferCPT2.h"
#include "UITheme.h"

namespace X
{
	class CUIContainer;
	class CUITooltip;

	// A simple image.
	// There are no borders or anything, just a simple texture resource to render.
	// The named texture resource stored in SCResourceManager will be resized to this widget's set dimensions.
	// That doesn't stop you from setting this widget's dimension though :)
	class CUIImage
	{
		friend class CUIContainer;
	public:
		CUIImage(CUIContainer* pContainer);
		~CUIImage();

		// Sets the dimensions of the widget.
		void setDimensions(float fX, float fY);

		// Sets the dimensions of the widget.
		void setDimensions(int iX, int iY);

		// Sets the dimensions of the widget.
		void setDimensions(const CVector2f& vDimensions);

		// Returns the dimensions of the widget.
		CVector2f getDimensions(void) const;

		// Returns the minimum dimensions of this widget based upon the container's currently set theme.
		// As this widget has no background, this returns a vector of (1, 1)
		CVector2f getDimensionsMinimum(void) const;

		// Sets the position of this widget in relation to it's container.
		// Both axis should be at least zero.
		void setPosition(float fX, float fY);

		// Sets the position of this widget in relation to it's container.
		// Both axis should be at least zero.
		void setPosition(int iX, int iY);

		// Sets the position of this widget in relation to it's container.
		// Both axis should be at least zero.
		void setPosition(const CVector2f& vPosition);

		// Returns the position of the widget in relation to it's container.
		CVector2f getPosition(void) const;

		// Sets whether this widget is visible or not.
		void setVisible(bool bVisible);

		// Returns whether this widget is visible or not.
		bool getVisible(void) const;

		// Render this widget's non-background items
		void renderNonBG(void);

		// Render this widget's tooltip
		void renderTooltip(void);

		// Update this widget and it's tooltip
		void update(float fTimeDeltaSec);

		// Sets the name of the CResourceTexture2DFromFile stored in the SCResourceManager which is used to render this widget.
		// If the resource doesn't exist, an exception occurs.
		void setTextureFromFile(const std::string& strResourceName);

		// Sets the name of the CResourceTexture2DFromImage stored in the SCResourceManager which is used to render this widget.
		// If the resource doesn't exist, an exception occurs.
		void setTextureFromImage(const std::string& strResourceName);

		// Sets the name of the CResourceDepthbuffer stored in the SCResourceManager which is used to render this widget.
		// If the resource doesn't exist, an exception occurs.
		void setTextureDepthbuffer(const std::string& strResourceName);

		// Sets the name of the CResourceFramebuffer stored in the SCResourceManager which is used to render this widget.
		// If the resource doesn't exist, an exception occurs.
		void setTextureFramebuffer(const std::string& strResourceName);

		// Sets the name of the CResourceTexture2DAtlas stored in SCResourceManager which is used to render this widget.
		// If the resource doesn't exist, an exception occurs.
		// If strImageInAtlasName is of zero length, then the whole atlas texture is rendered instead of just an image inside of it
		// If strImageInAtlasName is not of 0 length and the image name in the atlas doesn't exist, an exception occurs.
		void setTextureAtlas(const std::string& strResourceName, const std::string& strImageInAtlasName);

		// Sets the colour multiplier used when rendering the image.
		// By default, this is CColour(1,1,1,1);
		void setColour(const CColour& colour = CColour(1, 1, 1, 1));

		// Tooltip object for this widget.
		// Access and use this object to setup and enable the tooltip.
		CUITooltip* pTooltip;
	private:
		// Common amoung widgets
		CVector2f _mvDimensions;			// Dimensions of the widget
		CVector2f _mvPosition;				// Position of the widget in relation to it's container.
		bool _mbVisible;					// Whether this widget is visible or not.
		CUIContainer* _mpContainer;			// The container this widget belongs to. Set in constructor

		// Widget specific
		// The name of the resource stored in SCResourceManager.
		// Set with setTexture???()
		std::string _mstrResourceName;
		enum EResourceType
		{
			depthbuffer,
			framebuffer,
			texture2DFromFile,
			texture2DFromImage,
			atlas
		};
		// Type of resource which _strResourceName is for.
		// Set depending upon last called setTexture???? methods
		EResourceType _meResourceType;

		// If _mstrResourceName represents an atlas resource and _meResourceType specifies this,
		// then this holds the name of the image stored in the atlas to use when rendering this widget.
		std::string _mstrImageInAtlasName;

		CColour _mColour;	// Colour multiplier used when rendering the texture image.
	};
}