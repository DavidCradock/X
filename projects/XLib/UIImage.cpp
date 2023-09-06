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

	void CUIImage::setDimensions(const CVector2f& vDimensions)
	{
		_mvDimensions = vDimensions;
		_mpContainer->computeScrollbars();
	}

	CVector2f CUIImage::getDimensions(void) const
	{
		return _mvDimensions;
	}

	void CUIImage::setPosition(float fX, float fY)
	{
		_mvPosition.x = fX;
		_mvPosition.y = fY;
		_mpContainer->computeScrollbars();
	}

	void CUIImage::setPosition(const CVector2f& vPosition)
	{
		_mvPosition = vPosition;
		_mpContainer->computeScrollbars();
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

	void CUIImage::render(void)
	{
		const CUITheme::SSettings* pThemeSettings = _mpContainer->themeGetSettings();

		if (_mstrTextureResourceName.size())
		{
			CVector2f vPos = _mpContainer->getWidgetAreaTLCornerPosition() + _mvPosition + _mpContainer->getWidgetOffset();
			x->pResource->getTexture2DFromFile(_mstrTextureResourceName)->renderTo2DQuad(vPos.x, vPos.y, _mvDimensions.x, _mvDimensions.y, _mColour);
		}
	}

	/******************************************************************* Widget specific *******************************************************************/

	void CUIImage::setTexture(const std::string& strNameOfTexture2DFromFileResource)
	{
		_mstrTextureResourceName = strNameOfTexture2DFromFileResource;
		ThrowIfFalse(x->pResource->getTexture2DFromFileExists(_mstrTextureResourceName), "CUIImage::setTexture(\"" + strNameOfTexture2DFromFileResource + "\") failed. The named texture resource does not exist.");
	}

	void CUIImage::setColour(const CColour& colour)
	{
		_mColour = colour;
	}
}