#include "PCH.h"
#include "UIBaseWidget.h"
#include "UIContainer.h"

namespace X
{
	CUIBaseWidget::CUIBaseWidget()
	{
		setPosition(CVector2f(0, 0));
		setDimensions(CVector2f(128, 128));
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
			idColTL.sTexCoords.vBL,
			idColTL.sTexCoords.vBR,
			idColTL.sTexCoords.vTR,
			idColTL.sTexCoords.vTL,
			idNormTL.sTexCoords.vBL,
			idNormTL.sTexCoords.vBR,
			idNormTL.sTexCoords.vTR,
			idNormTL.sTexCoords.vTL);

		// Top edge
		vCellPos = vPos;
		vCellPos.x += idColTL.v2fDimensions.x;
		vDims.x = _mvDimensions.x - idColTR.v2fDimensions.x;
		vDims.y = idColT.v2fDimensions.y;
		pVB->addQuad2D(vCellPos, vDims, pThemeSettings->colours.scrollbarBG,
			idColT.sTexCoords.vBL,
			idColT.sTexCoords.vBR,
			idColT.sTexCoords.vTR,
			idColT.sTexCoords.vTL,
			idNormT.sTexCoords.vBL,
			idNormT.sTexCoords.vBR,
			idNormT.sTexCoords.vTR,
			idNormT.sTexCoords.vTL);

		// Top right corner
		vCellPos = vPos;
		vCellPos.x += _mvDimensions.x - idColTR.v2fDimensions.x;
		vDims = idColTR.v2fDimensions;
		pVB->addQuad2D(vCellPos, vDims, pThemeSettings->colours.scrollbarBG,
			idColTR.sTexCoords.vBL,
			idColTR.sTexCoords.vBR,
			idColTR.sTexCoords.vTR,
			idColTR.sTexCoords.vTL,
			idNormTR.sTexCoords.vBL,
			idNormTR.sTexCoords.vBR,
			idNormTR.sTexCoords.vTR,
			idNormTR.sTexCoords.vTL);

		// Left edge
		vCellPos = vPos;
		vCellPos.y += idColTL.v2fDimensions.y;
		vDims.x = idColL.v2fDimensions.x;
		vDims.y = _mvDimensions.y - idColTL.v2fDimensions.y - idColBL.v2fDimensions.y;
		pVB->addQuad2D(vCellPos, vDims, pThemeSettings->colours.scrollbarBG,
			idColL.sTexCoords.vBL,
			idColL.sTexCoords.vBR,
			idColL.sTexCoords.vTR,
			idColL.sTexCoords.vTL,
			idNormL.sTexCoords.vBL,
			idNormL.sTexCoords.vBR,
			idNormL.sTexCoords.vTR,
			idNormL.sTexCoords.vTL);

		// Centre
		vCellPos = vPos;
		vCellPos.x += idColL.v2fDimensions.x;
		vCellPos.y += idColTL.v2fDimensions.y;
		vDims.x = _mvDimensions.x - idColL.v2fDimensions.x - idColR.v2fDimensions.x;
		vDims.y = _mvDimensions.y - idColTL.v2fDimensions.y - idColBL.v2fDimensions.y;
		pVB->addQuad2D(vCellPos, vDims, pThemeSettings->colours.scrollbarBG,
			idColC.sTexCoords.vBL,
			idColC.sTexCoords.vBR,
			idColC.sTexCoords.vTR,
			idColC.sTexCoords.vTL,
			idNormC.sTexCoords.vBL,
			idNormC.sTexCoords.vBR,
			idNormC.sTexCoords.vTR,
			idNormC.sTexCoords.vTL);

		// Right edge
		vCellPos = vPos;
		vCellPos.x += _mvDimensions.x - idColR.v2fDimensions.x;
		vCellPos.y += idColTL.v2fDimensions.y;
		vDims.x = idColR.v2fDimensions.x;
		vDims.y = _mvDimensions.y - idColL.v2fDimensions.y - idColR.v2fDimensions.y;
		pVB->addQuad2D(vCellPos, vDims, pThemeSettings->colours.scrollbarBG,
			idColR.sTexCoords.vBL,
			idColR.sTexCoords.vBR,
			idColR.sTexCoords.vTR,
			idColR.sTexCoords.vTL,
			idNormR.sTexCoords.vBL,
			idNormR.sTexCoords.vBR,
			idNormR.sTexCoords.vTR,
			idNormR.sTexCoords.vTL);

		// Bottom left corner
		vCellPos = vPos;
		vCellPos.y += _mvDimensions.y - idColBL.v2fDimensions.y;
		vDims = idColBL.v2fDimensions;
		pVB->addQuad2D(vCellPos, vDims, pThemeSettings->colours.scrollbarBG,
			idColBL.sTexCoords.vBL,
			idColBL.sTexCoords.vBR,
			idColBL.sTexCoords.vTR,
			idColBL.sTexCoords.vTL,
			idNormBL.sTexCoords.vBL,
			idNormBL.sTexCoords.vBR,
			idNormBL.sTexCoords.vTR,
			idNormBL.sTexCoords.vTL);

		// Bottom edge
		vCellPos = vPos;
		vCellPos.x += idColBL.v2fDimensions.x;
		vCellPos.y += _mvDimensions.y - idColB.v2fDimensions.y;
		vDims.x = _mvDimensions.x - idColBL.v2fDimensions.x - idColBR.v2fDimensions.x;
		vDims.y = idColB.v2fDimensions.y;
		pVB->addQuad2D(vCellPos, vDims, pThemeSettings->colours.scrollbarBG,
			idColB.sTexCoords.vBL,
			idColB.sTexCoords.vBR,
			idColB.sTexCoords.vTR,
			idColB.sTexCoords.vTL,
			idNormB.sTexCoords.vBL,
			idNormB.sTexCoords.vBR,
			idNormB.sTexCoords.vTR,
			idNormB.sTexCoords.vTL);

		// Bottom right corner
		vCellPos = vPos;
		vCellPos.x += _mvDimensions.x - idColBR.v2fDimensions.x;
		vCellPos.y += _mvDimensions.y - idColBR.v2fDimensions.y;
		vDims = idColBR.v2fDimensions;
		pVB->addQuad2D(vCellPos, vDims, pThemeSettings->colours.scrollbarBG,
			idColBR.sTexCoords.vBL,
			idColBR.sTexCoords.vBR,
			idColBR.sTexCoords.vTR,
			idColBR.sTexCoords.vTL,
			idNormBR.sTexCoords.vBL,
			idNormBR.sTexCoords.vBR,
			idNormBR.sTexCoords.vTR,
			idNormBR.sTexCoords.vTL);
	}
}