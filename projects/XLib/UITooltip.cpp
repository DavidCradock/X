#include "PCH.h"
#include "UITooltip.h"
#include "singletons.h"
#include "UIManager.h"
#include "input.h"
#include "renderer.h"
#include "settings.h"

namespace X
{
	CUITooltip::CUITooltip(CUIContainer* pContainer)
	{
		_mpContainer = pContainer;
		_mbEnabled = false;
		_mfTooltipDelay = 0.0f;
		_mColour.set(1.0f, 1.0f, 1.0f, 0.0f);
		_mvDimensions.set(200, 100);
		setText("Tooltip text goes here.", false);
	}

	CUITooltip::~CUITooltip()
	{

	}

	void CUITooltip::render(void)
	{
		if (_mColour.alpha <= 0.0f || !_mbEnabled)
			return;

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

		CVector2f vTooltipPos;
		vTooltipPos.x = _mvPosition.x;
		vTooltipPos.y = _mvPosition.y;
		CVector2f vUIFBDims = x->pResource->getUIFramebufferDims();
		// If goes off right edge of render target
		if (vTooltipPos.x + _mvDimensions.x > vUIFBDims.x)
		{
			// Position so the tooltip's right most edge is to the left of the mouse cursor
			vTooltipPos.x = vUIFBDims.x - _mvDimensions.x;
		}
		// If goes off left edge of render target
		else if (vTooltipPos.x < 0)
		{
			vTooltipPos.x = 0;
			// If the tooltip is large, it may be rendered underneath the cursor which we don't want, 
			// move it down so it's not rendered underneath the cursor
//			vTooltipPosition.y += vTexDimsPoint3.y * 2.0f;
//			vTooltipPos.y += x->pWindow->getMouseCursorDimensions().y;
		}
		// If goes off bottom edge of render target
		if (vTooltipPos.y + _mvDimensions.y > vUIFBDims.y)
		{
			// Position so the tooltip's bottom most edge is above the mouse cursor
			vTooltipPos.y = vUIFBDims.y - _mvDimensions.y;
		}
		// If goes off top edge of render target
		else if (vTooltipPos.y < 0)
		{
			vTooltipPos.y = 0;
		}

		// Top left corner
		CVector2f vPos = vTooltipPos;
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
		vPos.x = vTooltipPos.x + idColTL->vDims.x;
		vPos.y = vTooltipPos.y;
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
		vPos.x = vTooltipPos.x + idColTL->vDims.x + _mvDimensions.x;
		vPos.y = vTooltipPos.y;
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
		vPos.x = vTooltipPos.x;
		vPos.y = vTooltipPos.y + idColTL->vDims.y;
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
		vPos.x = vTooltipPos.x + idColTL->vDims.x;
		vPos.y = vTooltipPos.y + idColTL->vDims.y;
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
		vPos.x = vTooltipPos.x + idColTL->vDims.x + _mvDimensions.x;
		vPos.y = vTooltipPos.y + idColTL->vDims.y;
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
		vPos.x = vTooltipPos.x;
		vPos.y = vTooltipPos.y + idColTL->vDims.y + _mvDimensions.y;
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
		vPos.x = vTooltipPos.x + idColBL->vDims.x;
		vPos.y = vTooltipPos.y + idColTL->vDims.y + _mvDimensions.y;
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
		vPos.x = vTooltipPos.x + idColBL->vDims.x + _mvDimensions.x;
		vPos.y = vTooltipPos.y + idColTL->vDims.y + _mvDimensions.y;
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

		// Render tooltip text to tooltip framebuffer
		CResourceFramebuffer* pUITTFB = x->pResource->getFramebuffer(x->pResource->defaultRes.framebuffer_uiTooltip);
		CResourceFont* pFont = x->pResource->getFont(pSettings->fonts.tooltipText);
		CColour textColour = pSettings->colours.tooltipText;
		textColour.alpha = col.alpha;
		std::vector<std::string> vstrLines;
		int iTotalRenderedHeight;
		// Compute total height
		pFont->printInRectNewline(
			false,
			_mstrText,
			0,//vTooltipPos.x,
			0,//vTooltipPos.y,
			_miTextMaxWidth,
			(int)vUIFBDims.y,
			vstrLines,
			iTotalRenderedHeight);

		pUITTFB->resize(_miTextMaxWidth, iTotalRenderedHeight);
		pUITTFB->bindAsRenderTarget(true, false, CColour(0.0f, 0.0f, 0.0f, 0.0f));

		pFont->printInRectNewline(
			true,
			_mstrText,
			0,
			0,
			_miTextMaxWidth,
			iTotalRenderedHeight,
			vstrLines,
			iTotalRenderedHeight);

		pUIFB->bindAsRenderTarget(false, false);

		x->pRenderer->blendEnable();
		pUITTFB->renderTo2DQuad(int(vTooltipPos.x + idColTL->vDims.x), int(vTooltipPos.y + idColTL->vDims.y), _miTextMaxWidth, iTotalRenderedHeight, textColour);
		x->pRenderer->blendDisable();
	}

	void CUITooltip::update(const CVector2f& vWidgetPosition, const CVector2f& vWidgetDims, float fTimeDeltaSec)
	{
		if (!_mbEnabled)
			return;

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
			CVector2f vWidgetAreaTLPos = vWidgetPosition;// _mpContainer->getWidgetAreaTLCornerPosition() + _mpContainer->getWidgetOffset();
			rctWidget.miMinX = int(vWidgetPosition.x);
			rctWidget.miMinY = int(vWidgetPosition.y);
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
				bFadeIn = true;
			}
		}
		else
		{
			_mfTooltipDelay -= fTimeDeltaSec;
			if (_mfTooltipDelay < 0)
				_mfTooltipDelay = 0;
		}

		const CUITheme::SSettings* pThemeSettings = _mpContainer->themeGetSettings();
		if (bFadeIn)
			_mColour.alpha += fTimeDeltaSec * pThemeSettings->floats.tooltipFadeSpeed;
		else
			_mColour.alpha -= fTimeDeltaSec * pThemeSettings->floats.tooltipFadeSpeed;
		clamp(_mColour.alpha, 0.0f, 1.0f);

		// Set top left corner position of the tooltip
		_mvPosition = vMousePos + pThemeSettings->vectors.tooltipOffsetFromCursor;

	}

	void CUITooltip::setEnabled(bool bEnabled)
	{
		_mbEnabled = bEnabled;
		if (!_mbEnabled)
			reset();
	}

	bool CUITooltip::getEnabled(void) const
	{
		return _mbEnabled;
	}

	void CUITooltip::reset(void)
	{
		_mColour.alpha = 0.0f;
		_mfTooltipDelay = 0.0f;
	}

	void CUITooltip::setText(const std::string& strText, bool bEnable, int iMaxWidth)
	{
		_mstrText = strText;
		_miTextMaxWidth = iMaxWidth;
		if (bEnable)
			setEnabled(true);
		_computeDimensions();
	}

	void CUITooltip::_computeDimensions(void)
	{
		const CUITheme::SSettings* pThemeSettings = _mpContainer->themeGetSettings();
		CResourceFont* pFont = x->pResource->getFont(pThemeSettings->fonts.tooltipText);
		
		std::vector<std::string> vstrLines;
		int iTotalRenderedHeight;
		pFont->printInRectNewline(
			false,					// Render?
			_mstrText,
			0,						// PosX
			0,						// PosY
			_miTextMaxWidth,		// Render target width
			1000,					// Render target height
			vstrLines,
			iTotalRenderedHeight);

		int iMaxWidth = 0;
		for (int i = 0; i < vstrLines.size(); i++)
		{
			int iWidth = (int)pFont->getTextWidth(vstrLines[i]);
			if (iWidth > iMaxWidth)
				iMaxWidth = iWidth;
		}
		_mvDimensions.y = float(iTotalRenderedHeight);
		_mvDimensions.x = float(iMaxWidth);
	}
}