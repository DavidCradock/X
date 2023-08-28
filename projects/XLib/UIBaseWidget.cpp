#include "PCH.h"
#include "UIBaseWidget.h"
#include "UIContainer.h"

namespace X
{
	CUIBaseWidget::CUIBaseWidget()
	{
		setPosition(CVector2f(0, 0));
		setDimensions(CVector2f(128, 128));
		setVisible(true);
	}

	void CUIBaseWidget::setPosition(const CVector2f& vPosition)
	{
		_mvPosition = vPosition;
	}

	CVector2f CUIBaseWidget::getPosition(void) const
	{
		return _mvPosition;
	}

	void CUIBaseWidget::setDimensions(const CVector2f& vDimensions)
	{
		_mvDimensions = vDimensions;
	}

	CVector2f CUIBaseWidget::getDimensions(void) const
	{
		return _mvDimensions;
	}

	void CUIBaseWidget::setVisible(bool bVisible)
	{
		_mbVisible = bVisible;
	}

	bool CUIBaseWidget::getVisible(void) const
	{
		return _mbVisible;
	}

	void CUIBaseWidget::_addGridGeometry(CUITheme::SImageType imageType, CResourceTexture2DAtlas* pAtlas, CUIContainer* pContainer, bool bContainerIsWindow, CUITheme* pTheme, CResourceVertexBufferCPT2* pVB)
	{
		CImageAtlasDetails idColC = pAtlas->getImageDetails(imageType.colour.centre);
		CImageAtlasDetails idNormC = pAtlas->getImageDetails(imageType.normal.centre);
		CImageAtlasDetails idColBL = pAtlas->getImageDetails(imageType.colour.cornerBL);
		CImageAtlasDetails idNormBL = pAtlas->getImageDetails(imageType.normal.cornerBL);
		CImageAtlasDetails idColBR = pAtlas->getImageDetails(imageType.colour.cornerBR);
		CImageAtlasDetails idNormBR = pAtlas->getImageDetails(imageType.normal.cornerBR);
		CImageAtlasDetails idColTL = pAtlas->getImageDetails(imageType.colour.cornerTL);
		CImageAtlasDetails idNormTL = pAtlas->getImageDetails(imageType.normal.cornerTL);
		CImageAtlasDetails idColTR = pAtlas->getImageDetails(imageType.colour.cornerTR);
		CImageAtlasDetails idNormTR = pAtlas->getImageDetails(imageType.normal.cornerTR);
		CImageAtlasDetails idColB = pAtlas->getImageDetails(imageType.colour.edgeB);
		CImageAtlasDetails idNormB = pAtlas->getImageDetails(imageType.normal.edgeB);
		CImageAtlasDetails idColL = pAtlas->getImageDetails(imageType.colour.edgeL);
		CImageAtlasDetails idNormL = pAtlas->getImageDetails(imageType.normal.edgeL);
		CImageAtlasDetails idColR = pAtlas->getImageDetails(imageType.colour.edgeR);
		CImageAtlasDetails idNormR = pAtlas->getImageDetails(imageType.normal.edgeR);
		CImageAtlasDetails idColT = pAtlas->getImageDetails(imageType.colour.edgeT);
		CImageAtlasDetails idNormT = pAtlas->getImageDetails(imageType.normal.edgeT);

		const CUITheme::SSettings* pThemeSettings = pTheme->getSettings();

		// The set position of a container requires no offset for the widget's position
		CVector2f vPos = _mvPosition;
		// Regardless of whether the container is a window or not, we still need to offset by the container's position...
		vPos += pContainer->getPosition();
		// The set position of a window however is the top left position of the window along with it's borders and therefore the widget's position must be offset by this.
		if (bContainerIsWindow)
			vPos += pAtlas->getImageDetails(pThemeSettings->images.windowBG.colour.cornerTL).v2fDimensions;

		CVector2f vDims;

		// Top left corner
		vDims = idColTL.v2fDimensions;
		CVector2f vCellPos = vPos;
		pVB->addQuad2D(vCellPos, vDims, pThemeSettings->colours.scrollbarBG,
			idColTL.sTexCoords.bottom_left,
			idColTL.sTexCoords.bottom_right,
			idColTL.sTexCoords.top_right,
			idColTL.sTexCoords.top_left,
			idNormTL.sTexCoords.bottom_left,
			idNormTL.sTexCoords.bottom_right,
			idNormTL.sTexCoords.top_right,
			idNormTL.sTexCoords.bottom_left);

		// Top right corner
		vDims = idColTR.v2fDimensions;
		vCellPos = vPos;
		vCellPos.x += _mvDimensions.x - vDims.x;
		pVB->addQuad2D(vCellPos, vDims, pThemeSettings->colours.scrollbarBG,
			idColTR.sTexCoords.bottom_left,
			idColTR.sTexCoords.bottom_right,
			idColTR.sTexCoords.top_right,
			idColTR.sTexCoords.top_left,
			idNormTR.sTexCoords.bottom_left,
			idNormTR.sTexCoords.bottom_right,
			idNormTR.sTexCoords.top_right,
			idNormTR.sTexCoords.bottom_left);

	}
}