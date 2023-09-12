#include "PCH.h"
#include "UIImage.h"
#include "UIContainer.h"
#include "singletons.h"

namespace X
{
	CUIImage::CUIImage(CUIContainer* pContainer)
	{
		_mpContainer = pContainer;
		_mvDimensions.set(200, 48);
		_mbVisible = true;
		_meResourceType = EResourceType::texture2DFromFile;
	}

	CUIImage::~CUIImage()
	{

	}

	void CUIImage::setDimensions(float fX, float fY)
	{
		_mvDimensions.x = fX;
		_mvDimensions.y = fY;
		_mpContainer->computeScrollbars();
	}

	void CUIImage::setDimensions(int iX, int iY)
	{
		setDimensions(float(iX), float(iY));
	}

	void CUIImage::setDimensions(const CVector2f& vDimensions)
	{
		setDimensions(vDimensions.x, vDimensions.y);
	}

	CVector2f CUIImage::getDimensions(void) const
	{
		return _mvDimensions;
	}

	CVector2f CUIImage::getDimensionsMinimum(void) const
	{
		CVector2f vMinimumDims(1.0f, 1.0f);
		return vMinimumDims;
	}

	void CUIImage::setPosition(float fX, float fY)
	{
		_mvPosition.x = fX;
		_mvPosition.y = fY;
		_mpContainer->computeScrollbars();
	}

	void CUIImage::setPosition(int iX, int iY)
	{
		setPosition(float(iX), float(iY));
	}

	void CUIImage::setPosition(const CVector2f& vPosition)
	{
		setPosition(vPosition.x, vPosition.y);
	}

	CVector2f CUIImage::getPosition(void) const
	{
		return _mvPosition;
	}

	void CUIImage::setVisible(bool bVisible)
	{
		_mbVisible = bVisible;
		_mpContainer->computeScrollbars();
	}

	bool CUIImage::getVisible(void) const
	{
		return _mbVisible;
	}

	void CUIImage::renderNonBG(void)
	{
		if (!_mbVisible)
			return;

		const CUITheme::SSettings* pThemeSettings = _mpContainer->themeGetSettings();

		if (_mstrResourceName.size())
		{
			CVector2f vPos = _mpContainer->getWidgetAreaTLCornerPosition() + _mvPosition + _mpContainer->getWidgetOffset();
			if (EResourceType::texture2DFromFile == _meResourceType)
				x->pResource->getTexture2DFromFile(_mstrResourceName)->renderTo2DQuad((int)vPos.x, (int)vPos.y, (int)_mvDimensions.x, (int)_mvDimensions.y, _mColour);
			else if (EResourceType::texture2DFromImage == _meResourceType)
				x->pResource->getTexture2DFromImage(_mstrResourceName)->renderTo2DQuad((int)vPos.x, (int)vPos.y, (int)_mvDimensions.x, (int)_mvDimensions.y, _mColour);
			else if (EResourceType::framebuffer == _meResourceType)
				x->pResource->getFramebuffer(_mstrResourceName)->renderTo2DQuad((int)vPos.x, (int)vPos.y, (int)_mvDimensions.x, (int)_mvDimensions.y, _mColour);
			else if (EResourceType::depthbuffer == _meResourceType)
				x->pResource->getDepthbuffer(_mstrResourceName)->renderTo2DQuad((int)vPos.x, (int)vPos.y, (int)_mvDimensions.x, (int)_mvDimensions.y, _mColour);
			else if (EResourceType::atlas == _meResourceType)
			{
				if (_mstrImageInAtlasName.size())
					x->pResource->getTexture2DAtlas(_mstrResourceName)->renderAtlasImageTo2dQuad((int)vPos.x, (int)vPos.y, (int)_mvDimensions.x, (int)_mvDimensions.y, _mstrImageInAtlasName, _mColour);
				else
					x->pResource->getTexture2DAtlas(_mstrResourceName)->renderAtlasTo2DQuad((int)vPos.x, (int)vPos.y, (int)_mvDimensions.x, (int)_mvDimensions.y, 0, _mColour);
			}
		}
	}

	/******************************************************************* Widget specific *******************************************************************/

	void CUIImage::setTextureFromFile(const std::string& strResourceName)
	{
		_mstrResourceName = strResourceName;
		_meResourceType = EResourceType::texture2DFromFile;
		ThrowIfFalse(x->pResource->getTexture2DFromFileExists(_mstrResourceName), "CUIImage::setTextureFromFile(\"" + strResourceName + "\") failed. The named texture resource does not exist.");
	}

	void CUIImage::setTextureFromImage(const std::string& strResourceName)
	{
		_mstrResourceName = strResourceName;
		_meResourceType = EResourceType::texture2DFromImage;
		ThrowIfFalse(x->pResource->getTexture2DFromImageExists(_mstrResourceName), "CUIImage::setTextureFromImage(\"" + strResourceName + "\") failed. The named texture resource does not exist.");
	}

	void CUIImage::setTextureDepthbuffer(const std::string& strResourceName)
	{
		_mstrResourceName = strResourceName;
		_meResourceType = EResourceType::depthbuffer;
		ThrowIfFalse(x->pResource->getDepthbufferExists(_mstrResourceName), "CUIImage::setTextureDepthbuffer(\"" + strResourceName + "\") failed. The named texture resource does not exist.");
	}

	void CUIImage::setTextureFramebuffer(const std::string& strResourceName)
	{
		_mstrResourceName = strResourceName;
		_meResourceType = EResourceType::framebuffer;
		ThrowIfFalse(x->pResource->getFramebufferExists(_mstrResourceName), "CUIImage::setTextureFramebuffer(\"" + strResourceName + "\") failed. The named texture resource does not exist.");
	}

	void CUIImage::setTextureAtlas(const std::string& strResourceName, const std::string& strImageInAtlasName)
	{
		_mstrResourceName = strResourceName;
		_mstrImageInAtlasName = strImageInAtlasName;
		_meResourceType = EResourceType::atlas;
		ThrowIfFalse(x->pResource->getTexture2DAtlasExists(_mstrResourceName), "CUIImage::setTextureAtlas(\"" + strResourceName + "\") failed. The named texture resource does not exist.");
		if (_mstrImageInAtlasName.size())
			ThrowIfFalse(x->pResource->getTexture2DAtlas(strResourceName)->getImageNameExists(strImageInAtlasName), "CUIImage::setTextureAtlas(\"" + strResourceName + ", " + strImageInAtlasName + "\") failed. The named atlas exists, but the named image in the atlas does not.");
	}

	void CUIImage::setColour(const CColour& colour)
	{
		_mColour = colour;
	}
}