#include "PCH.h"
#include "UITooltip.h"
#include "singletons.h"

namespace X
{
	CUITooltip::CUITooltip(CUIContainer* pContainer)
	{
		_mpContainer = pContainer;
		_mbEnabled = false;
		_mfTooltipDelay = 0.0f;
		_mColour.set(1.0f, 1.0f, 1.0f, 0.0f);
		_mvDimensions.set(200, 100);
	}

	CUITooltip::~CUITooltip()
	{

	}

	void CUITooltip::render(void)
	{
//		if (_mColour.alpha <= 0.0f)
//			return;

		CResourceVertexBufferCPT2* pVB = x->pResource->getVertexBufferCPT2(x->pResource->defaultRes.vertexbufferCPT2_default);
		CResourceShader* pShader = x->pResource->getShader(x->pResource->defaultRes.shader_ui);
		CResourceFramebuffer* pUIFB = x->pResource->getFramebuffer(x->pResource->defaultRes.framebuffer_ui);
		CUITheme* pTheme = _mpContainer->themeGet();
		const CUITheme::SSettings* pSettings = pTheme->getSettings();

		pShader->bind();

		// Setup the projection matrix as orthographic
		CMatrix matProjection;
		matProjection.setProjectionOrthographic(0.0f, (float)pUIFB->getWidth(), 0.0f, (float)pUIFB->getHeight(), -1.0f, 1.0f);
		pShader->setMat4("transform", matProjection);

		// Tell OpenGL, for each sampler, to which texture unit it belongs to
		pShader->setInt("texture0", 0);
		pShader->setFloat("fMouseCursorDistance", pSettings->floats.normalMouseCursorDistance);
		pShader->setVec4("v4AmbientLight", pSettings->colours.ambientLight);
		pShader->setVec4("v4MouseLight", pSettings->colours.mouseLight);

		// Set mouse position, inverting Y position
		CVector2f vMousePos = x->pInput->mouse.getCursorPos();
		vMousePos.y = float(x->pWindow->getHeight()) - vMousePos.y;
		pShader->setVec2("v2MousePos", vMousePos);

		x->pRenderer->blendEnable();
		x->pRenderer->depthTestDisable();
		x->pRenderer->cullFaceDisable();
		x->pRenderer->blendFunction_SrcAlpha_OneMinusSrcAlpha();

		// Get textures and bind them
		CResourceTexture2DAtlas* pAtlas = pTheme->getTextureAtlas();
		pAtlas->bindAtlas(0, 0);
		pVB->removeGeom();

		// Get image details for the 3x3 cell images
		CImageAtlasDetails* idColC = pAtlas->getImageDetailsPointer(pSettings->images.tooltipBG.colour.centre);
		CImageAtlasDetails* idColBL = pAtlas->getImageDetailsPointer(pSettings->images.tooltipBG.colour.cornerBL);
		CImageAtlasDetails* idColBR = pAtlas->getImageDetailsPointer(pSettings->images.tooltipBG.colour.cornerBR);
		CImageAtlasDetails* idColTL = pAtlas->getImageDetailsPointer(pSettings->images.tooltipBG.colour.cornerTL);
		CImageAtlasDetails* idColTR = pAtlas->getImageDetailsPointer(pSettings->images.tooltipBG.colour.cornerTR);
		CImageAtlasDetails* idColB = pAtlas->getImageDetailsPointer(pSettings->images.tooltipBG.colour.edgeB);
		CImageAtlasDetails* idColL = pAtlas->getImageDetailsPointer(pSettings->images.tooltipBG.colour.edgeL);
		CImageAtlasDetails* idColR = pAtlas->getImageDetailsPointer(pSettings->images.tooltipBG.colour.edgeR);
		CImageAtlasDetails* idColT = pAtlas->getImageDetailsPointer(pSettings->images.tooltipBG.colour.edgeT);
		CImageAtlasDetails* idNormC = pAtlas->getImageDetailsPointer(pSettings->images.tooltipBG.normal.centre);
		CImageAtlasDetails* idNormBL = pAtlas->getImageDetailsPointer(pSettings->images.tooltipBG.normal.cornerBL);
		CImageAtlasDetails* idNormBR = pAtlas->getImageDetailsPointer(pSettings->images.tooltipBG.normal.cornerBR);
		CImageAtlasDetails* idNormTL = pAtlas->getImageDetailsPointer(pSettings->images.tooltipBG.normal.cornerTL);
		CImageAtlasDetails* idNormTR = pAtlas->getImageDetailsPointer(pSettings->images.tooltipBG.normal.cornerTR);
		CImageAtlasDetails* idNormB = pAtlas->getImageDetailsPointer(pSettings->images.tooltipBG.normal.edgeB);
		CImageAtlasDetails* idNormL = pAtlas->getImageDetailsPointer(pSettings->images.tooltipBG.normal.edgeL);
		CImageAtlasDetails* idNormR = pAtlas->getImageDetailsPointer(pSettings->images.tooltipBG.normal.edgeR);
		CImageAtlasDetails* idNormT = pAtlas->getImageDetailsPointer(pSettings->images.tooltipBG.normal.edgeT);

		CColour col = pSettings->colours.tooltipBG *_mColour;
		col = _mColour;

		// Top left corner
		CVector2f vPos = _mvPosition;
		CVector2f vDims = idColTL->vDims;
		pVB->addQuad2D(vPos, vDims, col,
			idColTL->sTexCoords.vBL,
			idColTL->sTexCoords.vBR,
			idColTL->sTexCoords.vTR,
			idColTL->sTexCoords.vTL,
			idNormTL->sTexCoords.vBL,
			idNormTL->sTexCoords.vBR,
			idNormTL->sTexCoords.vTR,
			idNormTL->sTexCoords.vTL);

		// Top edge
		vPos.x = _mvPosition.x + idColTL->vDims.x;
		vPos.y = _mvPosition.y;
		vDims.x = _mvDimensions.x;
		vDims.y = idColT->vDims.y;
		pVB->addQuad2D(vPos, vDims, col,
			idColT->sTexCoords.vBL,
			idColT->sTexCoords.vBR,
			idColT->sTexCoords.vTR,
			idColT->sTexCoords.vTL,
			idNormT->sTexCoords.vBL,
			idNormT->sTexCoords.vBR,
			idNormT->sTexCoords.vTR,
			idNormT->sTexCoords.vTL);

		// Top right corner
		vPos.x = _mvPosition.x + idColTL->vDims.x + _mvDimensions.x;
		vPos.y = _mvPosition.y;
		vDims = idColTR->vDims;
		pVB->addQuad2D(vPos, vDims, col,
			idColTR->sTexCoords.vBL,
			idColTR->sTexCoords.vBR,
			idColTR->sTexCoords.vTR,
			idColTR->sTexCoords.vTL,
			idNormTR->sTexCoords.vBL,
			idNormTR->sTexCoords.vBR,
			idNormTR->sTexCoords.vTR,
			idNormTR->sTexCoords.vTL);

		// Left edge
		vPos.x = _mvPosition.x;
		vPos.y = _mvPosition.y + idColTL->vDims.y;
		vDims.x = idColL->vDims.x;
		vDims.y = _mvDimensions.y;
		pVB->addQuad2D(vPos, vDims, col,
			idColL->sTexCoords.vBL,
			idColL->sTexCoords.vBR,
			idColL->sTexCoords.vTR,
			idColL->sTexCoords.vTL,
			idNormL->sTexCoords.vBL,
			idNormL->sTexCoords.vBR,
			idNormL->sTexCoords.vTR,
			idNormL->sTexCoords.vTL);

		// Centre
		vPos.x = _mvPosition.x + idColTL->vDims.x;
		vPos.y = _mvPosition.y + idColTL->vDims.y;
		vDims = _mvDimensions;
		pVB->addQuad2D(vPos, vDims, col,
			idColC->sTexCoords.vBL,
			idColC->sTexCoords.vBR,
			idColC->sTexCoords.vTR,
			idColC->sTexCoords.vTL,
			idNormC->sTexCoords.vBL,
			idNormC->sTexCoords.vBR,
			idNormC->sTexCoords.vTR,
			idNormC->sTexCoords.vTL);

		// Right edge
		vPos.x = _mvPosition.x + idColTL->vDims.x + _mvDimensions.x;
		vPos.y = _mvPosition.y + idColTL->vDims.y;
		vDims.x = idColR->vDims.x;
		vDims.y = _mvDimensions.y;
		pVB->addQuad2D(vPos, vDims, col,
			idColR->sTexCoords.vBL,
			idColR->sTexCoords.vBR,
			idColR->sTexCoords.vTR,
			idColR->sTexCoords.vTL,
			idNormR->sTexCoords.vBL,
			idNormR->sTexCoords.vBR,
			idNormR->sTexCoords.vTR,
			idNormR->sTexCoords.vTL);

		// Bottom left corner
		vPos.x = _mvPosition.x;
		vPos.y = _mvPosition.y + idColTL->vDims.y + _mvDimensions.y;
		vDims = idColBL->vDims;
		pVB->addQuad2D(vPos, vDims, col,
			idColBL->sTexCoords.vBL,
			idColBL->sTexCoords.vBR,
			idColBL->sTexCoords.vTR,
			idColBL->sTexCoords.vTL,
			idNormBL->sTexCoords.vBL,
			idNormBL->sTexCoords.vBR,
			idNormBL->sTexCoords.vTR,
			idNormBL->sTexCoords.vTL);

		// Bottom edge
		vPos.x = _mvPosition.x + idColBL->vDims.x;
		vPos.y = _mvPosition.y + idColTL->vDims.y + _mvDimensions.y;
		vDims.x = _mvDimensions.x;
		vDims.y = idColB->vDims.y;
		pVB->addQuad2D(vPos, vDims, col,
			idColB->sTexCoords.vBL,
			idColB->sTexCoords.vBR,
			idColB->sTexCoords.vTR,
			idColB->sTexCoords.vTL,
			idNormB->sTexCoords.vBL,
			idNormB->sTexCoords.vBR,
			idNormB->sTexCoords.vTR,
			idNormB->sTexCoords.vTL);

		// Bottom right corner
		vPos.x = _mvPosition.x + idColBL->vDims.x + _mvDimensions.x;
		vPos.y = _mvPosition.y + idColTL->vDims.y + _mvDimensions.y;
		vDims = idColBR->vDims;
		pVB->addQuad2D(vPos, vDims, col,
			idColBR->sTexCoords.vBL,
			idColBR->sTexCoords.vBR,
			idColBR->sTexCoords.vTR,
			idColBR->sTexCoords.vTL,
			idNormBR->sTexCoords.vBL,
			idNormBR->sTexCoords.vBR,
			idNormBR->sTexCoords.vTR,
			idNormBR->sTexCoords.vTL);

		pVB->update();
		pVB->render();

		pAtlas->unbind();
		pShader->unbind();	// Unbind the GUI shader
		x->pRenderer->blendDisable();
		x->pRenderer->depthTestEnable();
		x->pRenderer->cullFaceEnable();

		{
			auto it = _mmapTexts.begin();
			while (it != _mmapTexts.end())
			{
				it->second->renderNonBG();
				it++;
			}
		}
	}

	void CUITooltip::update(const CVector2f& vWidgetPosition, const CVector2f& vWidgetDims, float fTimeDeltaSec)
	{
//		if (!_mbEnabled)
//			return;

		// Determine whether the mouse cursor is over this widget's container or not.
		bool bContainerHasMouseOver = false;
		std::string strMouseIsOver = x->pUI->getMouseIsOverWhichContainer();
		if (strMouseIsOver == _mpContainer->getName())
			bContainerHasMouseOver = true;

		CVector2f vMousePos = x->pInput->mouse.getCursorPos();

		// Determine whether the mouse cursor is over this widget or not
		bool bMouseOver = false;
		if (bContainerHasMouseOver)
		{
			// Compute a CRect which represents this widget's actual screen area
			CRect rctWidget;	// This will hold actual screen position for the widget's four corners.
			CVector2f vWidgetAreaTLPos = _mpContainer->getWidgetAreaTLCornerPosition() + _mpContainer->getWidgetOffset();
			rctWidget.miMinX = int(vWidgetAreaTLPos.x + vWidgetPosition.x);
			rctWidget.miMinY = int(vWidgetAreaTLPos.y + vWidgetPosition.y);
			rctWidget.miMaxX = rctWidget.miMinX + int(vWidgetDims.x);
			rctWidget.miMaxY = rctWidget.miMinY + int(vWidgetDims.y);

			// Determine whether the mouse cursor is over the widget or not
			if (rctWidget.doesPositionFitWithin(vMousePos))
				bMouseOver = true;
		}

		// Deal with fading in and out
		bool bFadeIn = false;
		if (bMouseOver)
		{
			_mfTooltipDelay += fTimeDeltaSec;
			if (_mfTooltipDelay >= x->pSettings->getUITooltipDelaySeconds())
			{
				_mfTooltipDelay = x->pSettings->getUITooltipDelaySeconds();
				
			}
			bFadeIn = true;
		}
		else
			_mfTooltipDelay -= fTimeDeltaSec;

		const CUITheme::SSettings* pThemeSettings = _mpContainer->themeGetSettings();
		if (bFadeIn)
			_mColour.alpha += fTimeDeltaSec * pThemeSettings->floats.tooltipFadeSpeed;
		else
			_mColour.alpha -= fTimeDeltaSec * pThemeSettings->floats.tooltipFadeSpeed;
		clamp(_mColour.alpha, 0.2f, 1.0f);
//		_mColour.alpha = 1.0f;
		// Set top left corner position of the tooltip
		_mvPosition = vMousePos + pThemeSettings->vectors.tooltipOffsetFromCursor;
//		_mvPosition.setZero();
		


	}

	void CUITooltip::setEnabled(bool bEnabled)
	{
		_mbEnabled = bEnabled;
	}

	bool CUITooltip::getEnabled(void) const
	{
		return _mbEnabled;
	}

	/************************************************************************************************************************************************************/
	/* CUIText */
	/************************************************************************************************************************************************************/

	CUIText* CUITooltip::textAdd(const std::string& strName, float fPosX, float fPosY, float fWidth, float fHeight, const std::string& strText)
	{
		// Throw exception if named object already exists
		ThrowIfTrue(_mmapTexts.find(strName) != _mmapTexts.end(), "CUITooltip::textAdd(\"" + strName + "\") failed. Named object already exists.");

		// Create new object
		CUIText* pNewObject = new CUIText(_mpContainer, strName);
		ThrowIfFalse(pNewObject, "CUITooltip::textAdd() failed to allocate memory for new object.");

		_mmapTexts[strName] = pNewObject;

		// Set settings for new object
		pNewObject->setDimensions(CVector2f(fWidth, fHeight));
		pNewObject->setPosition(CVector2f(fPosX, fPosY));
		pNewObject->setText(strText);

		_computeDimensions();

		return pNewObject;
	}

	CUIText* CUITooltip::textAdd(const std::string& strName, int iPosX, int iPosY, int iWidth, int iHeight, const std::string& strText)
	{
		return textAdd(strName, float(iPosX), float(iPosY), float(iWidth), float(iHeight), strText);
	}

	CUIText* CUITooltip::textGet(const std::string& strName)
	{
		auto it = _mmapTexts.find(strName);
		ThrowIfTrue(_mmapTexts.end() == it, "CUITooltip::textGet(\"" + strName + "\") failed. Named object doesn't exist.");
		return it->second;
	}

	void CUITooltip::textRemove(const std::string& strName)
	{
		auto it = _mmapTexts.find(strName);
		if (_mmapTexts.end() == it)
			return;
		delete it->second;
		_mmapTexts.erase(it);
	}

	void CUITooltip::textRemoveAll(void)
	{
		auto it = _mmapTexts.begin();
		while (it != _mmapTexts.end())
		{
			delete it->second;
			it++;
		}
		_mmapTexts.clear();
	}

	void CUITooltip::_computeDimensions(void)
	{
		_mvDimensions.setZero();

		{
			auto it = _mmapTexts.begin();
			CVector2f vBR;
			while (it != _mmapTexts.end())
			{
				vBR = it->second->getPosition() + it->second->getDimensions();
				if (_mvDimensions.x < vBR.x)
					_mvDimensions.x = vBR.x;
				if (_mvDimensions.y < vBR.y)
					_mvDimensions.y = vBR.y;
				it++;
			}
		}



	}
}