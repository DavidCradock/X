#include "PCH.h"
#include "UIWindow.h"
#include "UIManager.h"
#include "singletons.h"

namespace X
{
	CUIWindow::CUIWindow()
	{
		_mbInFocus = false;
		_mbVisible = true;
		_mbContainerIsWindow = true;
	}

	CUIWindow::~CUIWindow()
	{

	}

	void CUIWindow::update(float fTimeDeltaSec)
	{
		// Update all of the widgets
		CUIContainer::update(fTimeDeltaSec);
	}

	void CUIWindow::render(void)
	{
		if (!_mbVisible)
			return;

		_renderBorders();

		// Render all of the widgets
		CUIContainer::render();

		// Render the title bar text
		CUITheme* pTheme = x->pUI->themeGet(_mstrThemename);
		const CUITheme::SSettings* pSettings = pTheme->getSettings();
		CResourceFont* pFont = x->pResource->getFont(pSettings->fonts.windowTitlebar);
		CImageAtlasDetails idTL = pTheme->getTextureAtlas()->getImageDetails(pSettings->images.windowBG.colour.cornerTL);
		float fTextHeight = pFont->getTextHeight();
		CColour colour;
		if (_mbInFocus)
			colour = pSettings->colours.windowTitlebarTextFocused;
		else
			colour = pSettings->colours.windowTitlebarTextNotFocused;
		CVector2f vTextPos = _mvPosition;
		vTextPos.x += idTL.vDims.x;
		vTextPos.y += idTL.vDims.y * 0.5f;
		vTextPos.y -= fTextHeight * 0.5f;
		pFont->print(_mstrTitlebarText, (int)vTextPos.x, (int)vTextPos.y, x->pWindow->getWidth(), x->pWindow->getHeight(), 1.0f, colour);
	}

	void CUIWindow::_renderBorders(void)
	{
		// Get required resources needed to render things
		CResourceVertexBufferCPT2* pVB = x->pResource->getVertexBufferCPT2(x->pResource->defaultRes.vertexbufferCPT2_default);
		CResourceShader* pShader = x->pResource->getShader(x->pResource->defaultRes.shader_ui);
		CResourceFramebuffer* pUIFB = x->pResource->getFramebuffer(x->pResource->defaultRes.framebuffer_ui);
		CUITheme* pTheme = SCUIManager::getPointer()->themeGet(_mstrThemename);
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

		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		glCullFace(GL_BACK);
//		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Get textures and bind them
		CResourceTexture2DAtlas* pAtlas = pTheme->getTextureAtlas();
		pAtlas->bindAtlas(0, 0);
		pVB->removeGeom();

		// Get image details for the 3x3 cell images
		CImageAtlasDetails idColC = pAtlas->getImageDetails(pSettings->images.windowBG.colour.centre);
		CImageAtlasDetails idColBL = pAtlas->getImageDetails(pSettings->images.windowBG.colour.cornerBL);
		CImageAtlasDetails idColBR = pAtlas->getImageDetails(pSettings->images.windowBG.colour.cornerBR);
		CImageAtlasDetails idColTL = pAtlas->getImageDetails(pSettings->images.windowBG.colour.cornerTL);
		CImageAtlasDetails idColTR = pAtlas->getImageDetails(pSettings->images.windowBG.colour.cornerTR);
		CImageAtlasDetails idColB = pAtlas->getImageDetails(pSettings->images.windowBG.colour.edgeB);
		CImageAtlasDetails idColL = pAtlas->getImageDetails(pSettings->images.windowBG.colour.edgeL);
		CImageAtlasDetails idColR = pAtlas->getImageDetails(pSettings->images.windowBG.colour.edgeR);
		CImageAtlasDetails idColT = pAtlas->getImageDetails(pSettings->images.windowBG.colour.edgeT);
		CImageAtlasDetails idNormC = pAtlas->getImageDetails(pSettings->images.windowBG.normal.centre);
		CImageAtlasDetails idNormBL = pAtlas->getImageDetails(pSettings->images.windowBG.normal.cornerBL);
		CImageAtlasDetails idNormBR = pAtlas->getImageDetails(pSettings->images.windowBG.normal.cornerBR);
		CImageAtlasDetails idNormTL = pAtlas->getImageDetails(pSettings->images.windowBG.normal.cornerTL);
		CImageAtlasDetails idNormTR = pAtlas->getImageDetails(pSettings->images.windowBG.normal.cornerTR);
		CImageAtlasDetails idNormB = pAtlas->getImageDetails(pSettings->images.windowBG.normal.edgeB);
		CImageAtlasDetails idNormL = pAtlas->getImageDetails(pSettings->images.windowBG.normal.edgeL);
		CImageAtlasDetails idNormR = pAtlas->getImageDetails(pSettings->images.windowBG.normal.edgeR);
		CImageAtlasDetails idNormT = pAtlas->getImageDetails(pSettings->images.windowBG.normal.edgeT);

		// When rendering these borders, we need to add the additional space for the scrollbars.
		// The widgets' area remains the same.
		// The scrollbars are rendered to the right and below the widget area, so we need to increase
		// the dimensions of the centre cell of the window and move the right and bottom borders along
		// to make room for those scrollbars.
		// Let's store the width and height of those scroll bars and use them as an offset and use it below
		// to position everything correctly...
		CVector2f vScrollbarOffset;
		vScrollbarOffset.x = pSettings->floats.windowScrollbarVerticalWidth;
		vScrollbarOffset.y = pSettings->floats.windowScrollbarHorizontalHeight;

		CColour col = pSettings->colours.windowBGNotFocused;
		if (_mbInFocus)
			col = pSettings->colours.windowBGFocused;

		// Top left corner
		CVector2f vPos = _mvPosition;
		CVector2f vDims = idColTL.vDims;
		pVB->addQuad2D(vPos, vDims, col,
			idColTL.sTexCoords.vBL,
			idColTL.sTexCoords.vBR,
			idColTL.sTexCoords.vTR,
			idColTL.sTexCoords.vTL,
			idNormTL.sTexCoords.vBL,
			idNormTL.sTexCoords.vBR,
			idNormTL.sTexCoords.vTR,
			idNormTL.sTexCoords.vTL);

		// Top edge
		vPos.x = _mvPosition.x + idColTL.vDims.x;
		vPos.y = _mvPosition.y;
		vDims.x = _mvContainersWidgetAreaDimensions.x + vScrollbarOffset.x;
		vDims.y = idColT.vDims.y;
		pVB->addQuad2D(vPos, vDims, col,
			idColT.sTexCoords.vBL,
			idColT.sTexCoords.vBR,
			idColT.sTexCoords.vTR,
			idColT.sTexCoords.vTL,
			idNormT.sTexCoords.vBL,
			idNormT.sTexCoords.vBR,
			idNormT.sTexCoords.vTR,
			idNormT.sTexCoords.vTL);

		// Top right corner
		vPos.x = _mvPosition.x + idColTL.vDims.x + _mvContainersWidgetAreaDimensions.x + vScrollbarOffset.x;
		vPos.y = _mvPosition.y;
		vDims = idColTR.vDims;
		pVB->addQuad2D(vPos, vDims, col,
			idColTR.sTexCoords.vBL,
			idColTR.sTexCoords.vBR,
			idColTR.sTexCoords.vTR,
			idColTR.sTexCoords.vTL,
			idNormTR.sTexCoords.vBL,
			idNormTR.sTexCoords.vBR,
			idNormTR.sTexCoords.vTR,
			idNormTR.sTexCoords.vTL);

		// Left edge
		vPos.x = _mvPosition.x;
		vPos.y = _mvPosition.y + idColTL.vDims.y;
		vDims.x = idColL.vDims.x;
		vDims.y = _mvContainersWidgetAreaDimensions.y + vScrollbarOffset.y;
		pVB->addQuad2D(vPos, vDims, col,
			idColL.sTexCoords.vBL,
			idColL.sTexCoords.vBR,
			idColL.sTexCoords.vTR,
			idColL.sTexCoords.vTL,
			idNormL.sTexCoords.vBL,
			idNormL.sTexCoords.vBR,
			idNormL.sTexCoords.vTR,
			idNormL.sTexCoords.vTL);

		// Centre
		vPos.x = _mvPosition.x + idColTL.vDims.x;
		vPos.y = _mvPosition.y + idColTL.vDims.y;
		vDims = _mvContainersWidgetAreaDimensions;
		vDims += vScrollbarOffset;
		pVB->addQuad2D(vPos, vDims, col,
			idColC.sTexCoords.vBL,
			idColC.sTexCoords.vBR,
			idColC.sTexCoords.vTR,
			idColC.sTexCoords.vTL,
			idNormC.sTexCoords.vBL,
			idNormC.sTexCoords.vBR,
			idNormC.sTexCoords.vTR,
			idNormC.sTexCoords.vTL);

		// Right edge
		vPos.x = _mvPosition.x + idColTL.vDims.x + _mvContainersWidgetAreaDimensions.x + vScrollbarOffset.x;
		vPos.y = _mvPosition.y + idColTL.vDims.y;
		vDims.x = idColR.vDims.x;
		vDims.y = _mvContainersWidgetAreaDimensions.y + vScrollbarOffset.y;
		pVB->addQuad2D(vPos, vDims, col,
			idColR.sTexCoords.vBL,
			idColR.sTexCoords.vBR,
			idColR.sTexCoords.vTR,
			idColR.sTexCoords.vTL,
			idNormR.sTexCoords.vBL,
			idNormR.sTexCoords.vBR,
			idNormR.sTexCoords.vTR,
			idNormR.sTexCoords.vTL);

		// Bottom left corner
		vPos.x = _mvPosition.x;
		vPos.y = _mvPosition.y + idColTL.vDims.y + _mvContainersWidgetAreaDimensions.y + vScrollbarOffset.y;
		vDims = idColBL.vDims;
		pVB->addQuad2D(vPos, vDims, col,
			idColBL.sTexCoords.vBL,
			idColBL.sTexCoords.vBR,
			idColBL.sTexCoords.vTR,
			idColBL.sTexCoords.vTL,
			idNormBL.sTexCoords.vBL,
			idNormBL.sTexCoords.vBR,
			idNormBL.sTexCoords.vTR,
			idNormBL.sTexCoords.vTL);

		// Bottom edge
		vPos.x = _mvPosition.x + idColBL.vDims.x;
		vPos.y = _mvPosition.y + idColTL.vDims.y + _mvContainersWidgetAreaDimensions.y + vScrollbarOffset.y;
		vDims.x = _mvContainersWidgetAreaDimensions.x + vScrollbarOffset.x;
		vDims.y = idColB.vDims.y;
		pVB->addQuad2D(vPos, vDims, col,
			idColB.sTexCoords.vBL,
			idColB.sTexCoords.vBR,
			idColB.sTexCoords.vTR,
			idColB.sTexCoords.vTL,
			idNormB.sTexCoords.vBL,
			idNormB.sTexCoords.vBR,
			idNormB.sTexCoords.vTR,
			idNormB.sTexCoords.vTL);

		// Bottom right corner
		vPos.x = _mvPosition.x + idColBL.vDims.x + _mvContainersWidgetAreaDimensions.x + vScrollbarOffset.x;
		vPos.y = _mvPosition.y + idColTL.vDims.y + _mvContainersWidgetAreaDimensions.y + vScrollbarOffset.y;
		vDims = idColBR.vDims;
		pVB->addQuad2D(vPos, vDims, col,
			idColBR.sTexCoords.vBL,
			idColBR.sTexCoords.vBR,
			idColBR.sTexCoords.vTR,
			idColBR.sTexCoords.vTL,
			idNormBR.sTexCoords.vBL,
			idNormBR.sTexCoords.vBR,
			idNormBR.sTexCoords.vTR,
			idNormBR.sTexCoords.vTL);

		pVB->update();
		pVB->render();

		pAtlas->unbind();
		pShader->unbind();	// Unbind the GUI shader
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
	}

	CVector2f CUIWindow::getDimsIncludingTheme(void)
	{
		CUITheme* pTheme = themeGet();
		const CUITheme::SSettings* pSettings = pTheme->getSettings();
		CResourceTexture2DAtlas* pAtlas = pTheme->getTextureAtlas();
		CImageAtlasDetails idTL = pAtlas->getImageDetails(pSettings->images.windowBG.colour.cornerTL);
		CImageAtlasDetails idBR = pAtlas->getImageDetails(pSettings->images.windowBG.colour.cornerBR);
		CVector2f vDims = _mvContainersWidgetAreaDimensions;
		vDims.x += idTL.vDims.x + idBR.vDims.x;
		vDims.y += idTL.vDims.y + idBR.vDims.y;
		return vDims;
	}

	CRect CUIWindow::getTitlebarArea(void)
	{
		CUITheme* pTheme = themeGet();
		const CUITheme::SSettings* pSettings = pTheme->getSettings();
		CResourceTexture2DAtlas* pAtlas = pTheme->getTextureAtlas();
		CImageAtlasDetails idTL = pAtlas->getImageDetails(pSettings->images.windowBG.colour.cornerTL);
		CImageAtlasDetails idTR = pAtlas->getImageDetails(pSettings->images.windowBG.colour.cornerTR);
		CRect area(
			int(_mvPosition.x),
			int(_mvPosition.y),
			int(_mvPosition.x + _mvContainersWidgetAreaDimensions.x + idTL.vDims.x + idTR.vDims.x),
			int(_mvPosition.y + idTL.vDims.y));
		return area;
	}

	std::string CUIWindow::getTitlebarText(void)
	{
		return _mstrTitlebarText;
	}

	void CUIWindow::setTitlebarText(const std::string& strTitlebarText)
	{
		_mstrTitlebarText = strTitlebarText;
	}
}