#pragma once
#include "PCH.h"
#include "GUIBaseObject.h"
#include "colour.h"

namespace X
{
	class CGUITooltip;

	class CGUIImage : public CGUIBaseObject
	{
		friend class CGUIContainer;
		friend class CGUISubContainer;
		friend class CGUITooltip;
	public:
		CGUIImage();
		~CGUIImage();

		// Called from CGUIContainer to render this object
		void render(void* pParentContainer, const std::string& strFramebufferToSampleFrom);

		// Called from CGUIContainer to update this object
		void update(void* pParentContainer, bool bParentContainerAcceptingMouseClicks);

		CColour mColour;

		// The tooltip for this object.
		// By default, it is disabled. Use this object to enable and setup the tooltip
		CGUITooltip* mpTooltip;
	private:
		std::string _mstrTextureResourceName;	// Name of the texture stored in SCResourceManager will be either the name of a CTexture2DFromImage, or CTexture2DFromFile
		bool _mbImageIsFromFile;				// If this is true, then _mstrTextureResourceName points to a CTexture2DFromImage, else CTexture2DFromFile
	};
}