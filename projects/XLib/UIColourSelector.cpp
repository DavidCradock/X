#include "PCH.h"
#include "UIColourSelector.h"
#include "UIContainer.h"
#include "singletons.h"

namespace X
{
	CUIColourSelector::CUIColourSelector(CUIContainer* pContainer, const std::string& strWidgetName)
	{
		_mpContainer = pContainer;
		_mvDimensions = getDimensionsMinimum();
		_mbVisible = true;
		_miBrightnessWidth = 20;
		_mfBrightness = 1.0f;

		_mstrResourceNameWheel = "CUIColourSelectorImageWheel_" + pContainer->getName() + "_" + strWidgetName;
		CImage image;
		image.drawColourWheel(4, unsigned char(_mfBrightness * 255.0f));
		x->pResource->addTexture2DFromImage(_mstrResourceNameWheel, image);

		_mstrResourceNameBrightness = "CUIColourSelectorImageBrightness_" + pContainer->getName() + "_" + strWidgetName;
		image.drawGradient(_miBrightnessWidth, unsigned int(_mvDimensions.y), 4, CColour(0, 0, 0, 1), CColour(1, 1, 1, 1));
		x->pResource->addTexture2DFromImage(_mstrResourceNameBrightness, image);
	}

	CUIColourSelector::~CUIColourSelector()
	{
		x->pResource->removeTexture2DFromImage(_mstrResourceNameWheel);
		x->pResource->removeTexture2DFromImage(_mstrResourceNameBrightness);
	}

	void CUIColourSelector::setDimensions(float fX, float fY)
	{
		_mvDimensions.x = fX;
		_mvDimensions.y = fY;

		_updateTextures();

		_mpContainer->computeScrollbars();
	}

	void CUIColourSelector::setDimensions(int iX, int iY)
	{
		setDimensions(float(iX), float(iY));
	}

	void CUIColourSelector::setDimensions(const CVector2f& vDimensions)
	{
		setDimensions(vDimensions.x, vDimensions.y);
	}

	CVector2f CUIColourSelector::getDimensions(void) const
	{
		return _mvDimensions;
	}

	CVector2f CUIColourSelector::getDimensionsMinimum(void) const
	{
		CVector2f vMinimumDims(64.0f, 64.0f);
		return vMinimumDims;
	}

	void CUIColourSelector::setPosition(float fX, float fY)
	{
		_mvPosition.x = fX;
		_mvPosition.y = fY;
		_mpContainer->computeScrollbars();
	}

	void CUIColourSelector::setPosition(int iX, int iY)
	{
		setPosition(float(iX), float(iY));
	}

	void CUIColourSelector::setPosition(const CVector2f& vPosition)
	{
		setPosition(vPosition.x, vPosition.y);
	}

	CVector2f CUIColourSelector::getPosition(void) const
	{
		return _mvPosition;
	}

	void CUIColourSelector::setVisible(bool bVisible)
	{
		_mbVisible = bVisible;
		_mpContainer->computeScrollbars();
	}

	bool CUIColourSelector::getVisible(void) const
	{
		return _mbVisible;
	}

	void CUIColourSelector::renderNonBG(void)
	{
		if (!_mbVisible)
			return;

//		const CUITheme::SSettings* pThemeSettings = _mpContainer->themeGetSettings();

		if (_mstrResourceNameWheel.size())
		{
			CVector2f vPos = _mpContainer->getWidgetAreaTLCornerPosition() + _mvPosition + _mpContainer->getWidgetOffset();
			x->pRenderer->blendEnable();

			x->pResource->getTexture2DFromImage(_mstrResourceNameWheel)->renderTo2DQuad(
				(int)vPos.x,
				(int)vPos.y,
				(int)_mvDimensions.x,
				(int)_mvDimensions.y, CColour());

			x->pResource->getTexture2DFromImage(_mstrResourceNameBrightness)->renderTo2DQuad(
				(int)vPos.x + _mvDimensions.x,
				(int)vPos.y,
				(int)_miBrightnessWidth,
				(int)_mvDimensions.y, CColour());

			x->pRenderer->blendDisable();
		}
	}

	void CUIColourSelector::update(float fTimeDeltaSec)
	{

	}

	/******************************************************************* Widget specific *******************************************************************/

	void CUIColourSelector::setBrightness(float fBrightness)
	{
		clamp(fBrightness, 0.0f, 1.0f);
		if (_mfBrightness == fBrightness)
			return;

		_mfBrightness = fBrightness;

		_updateTextures();
	}

	void CUIColourSelector::_updateTextures(void)
	{
		CResourceTexture2DFromImage* pImage = x->pResource->getTexture2DFromImage(_mstrResourceNameWheel);
		CImage image;
		image.drawColourWheel(unsigned int(_mvDimensions.x - _miBrightnessWidth), unsigned char(_mfBrightness * 255.0f));
		pImage->update(image);

		pImage = x->pResource->getTexture2DFromImage(_mstrResourceNameBrightness);
		image.drawGradient(_miBrightnessWidth, unsigned int(_mvDimensions.y), 4, CColour(0, 0, 0, 1), CColour(1, 1, 1, 1));
		pImage->update(image);
	}
}