#include "PCH.h"
#include "UIWindow.h"
#include "UIManager.h"
#include "singletons.h"

namespace X
{
	CUIWindow::CUIWindow()
	{
		_mbInFocus = false;
	}

	CUIWindow::~CUIWindow()
	{

	}

	void CUIWindow::update(float fTimeDeltaSec)
	{
		// Return name of the window, or container which the mouse cursor is over
		std::string strMouseOverWindow = x->pUI->getMouseIsOverWhichWindow();
		bool bAcceptingMouseInput = false;
		// And if that window or container is this object's container, we're accepting mouse input
		if (strMouseOverWindow == _mstrName)
			bAcceptingMouseInput = true;

		CVector2f vMousePos = x->pInput->mouse.getCursorPos();

		// Update all of the widgets
		CUIContainer::update(fTimeDeltaSec);
	}

	void CUIWindow::render(void)
	{
		// Now render the window's borders
		_renderBorders();

		// Render all of the widgets
//		CUIContainer::render();
	}

	void CUIWindow::_renderBorders(void)
	{
		// Get required resources needed to render things
		CResourceVertexBufferCPT2* pVB = x->pResource->getVertexBufferCPT2(x->pResource->defaultRes.vertexbufferCPT2_default);
		CResourceShader* pShader = x->pResource->getShader(x->pResource->defaultRes.shader_ui_window);
		CResourceFramebuffer* pUIFB = x->pResource->getFramebuffer(x->pResource->defaultRes.framebuffer_ui);
		CUITheme* pTheme = SCUIManager::getPointer()->themeGet(_mstrThemename);

		pShader->bind();

		// Setup the projection matrix as orthographic
		CMatrix matProjection;
		matProjection.setProjectionOrthographic(0.0f, (float)pUIFB->getWidth(), 0.0f, (float)pUIFB->getHeight(), -1.0f, 1.0f);
		pShader->setMat4("transform", matProjection);

		// Tell OpenGL, for each sampler, to which texture unit it belongs to
		pShader->setInt("texture0", 0);
		pShader->setFloat("fNormalAmount", pTheme->getSettings()->floats.normalAmount);
		pShader->setFloat("fMouseCursorDistance", pTheme->getSettings()->floats.normalMouseCursorDistance);

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
		const CUITheme::SSettings* pSettings = pTheme->getSettings();
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

		CColour col = pSettings->colours.windowBGNotFocused;
		if (_mbInFocus)
			col = pSettings->colours.windowBGFocused;

		// Top left corner
		CVector2f vPos = _mvPosition;
		CVector2f vDims = idColTL.v2fDimensions;
		pVB->addQuad2D(vPos, vDims, col,
			idColTL.sTexCoords.bottom_left,
			idColTL.sTexCoords.bottom_right,
			idColTL.sTexCoords.top_right,
			idColTL.sTexCoords.top_left,
			idNormTL.sTexCoords.bottom_left,
			idNormTL.sTexCoords.bottom_right,
			idNormTL.sTexCoords.top_right,
			idNormTL.sTexCoords.top_left);

		// Top edge
		vPos.x = _mvPosition.x + idColTL.v2fDimensions.x;
		vPos.y = _mvPosition.y;
		vDims.x = _mvDimensions.x;
		vDims.y = idColT.v2fDimensions.y;
		pVB->addQuad2D(vPos, vDims, col,
			idColT.sTexCoords.bottom_left,
			idColT.sTexCoords.bottom_right,
			idColT.sTexCoords.top_right,
			idColT.sTexCoords.top_left,
			idNormT.sTexCoords.bottom_left,
			idNormT.sTexCoords.bottom_right,
			idNormT.sTexCoords.top_right,
			idNormT.sTexCoords.top_left);

		// Top right corner
		vPos.x = _mvPosition.x + idColTL.v2fDimensions.x + _mvDimensions.x;
		vPos.y = _mvPosition.y;
		vDims = idColTR.v2fDimensions;
		pVB->addQuad2D(vPos, vDims, col,
			idColTR.sTexCoords.bottom_left,
			idColTR.sTexCoords.bottom_right,
			idColTR.sTexCoords.top_right,
			idColTR.sTexCoords.top_left,
			idNormTR.sTexCoords.bottom_left,
			idNormTR.sTexCoords.bottom_right,
			idNormTR.sTexCoords.top_right,
			idNormTR.sTexCoords.top_left);

		// Left edge
		vPos.x = _mvPosition.x;
		vPos.y = _mvPosition.y + idColTL.v2fDimensions.y;
		vDims.x = idColL.v2fDimensions.x;
		vDims.y = _mvDimensions.y;
		pVB->addQuad2D(vPos, vDims, col,
			idColL.sTexCoords.bottom_left,
			idColL.sTexCoords.bottom_right,
			idColL.sTexCoords.top_right,
			idColL.sTexCoords.top_left,
			idNormL.sTexCoords.bottom_left,
			idNormL.sTexCoords.bottom_right,
			idNormL.sTexCoords.top_right,
			idNormL.sTexCoords.top_left);

		// Centre
		vPos.x = _mvPosition.x + idColTL.v2fDimensions.x;
		vPos.y = _mvPosition.y + idColTL.v2fDimensions.y;
		vDims = _mvDimensions;
		pVB->addQuad2D(vPos, vDims, col,
			idColC.sTexCoords.bottom_left,
			idColC.sTexCoords.bottom_right,
			idColC.sTexCoords.top_right,
			idColC.sTexCoords.top_left,
			idNormC.sTexCoords.bottom_left,
			idNormC.sTexCoords.bottom_right,
			idNormC.sTexCoords.top_right,
			idNormC.sTexCoords.top_left);

		// Right edge
		vPos.x = _mvPosition.x + idColTL.v2fDimensions.x + _mvDimensions.x;
		vPos.y = _mvPosition.y + idColTL.v2fDimensions.y;
		vDims.x = idColR.v2fDimensions.x;
		vDims.y = _mvDimensions.y;
		pVB->addQuad2D(vPos, vDims, col,
			idColR.sTexCoords.bottom_left,
			idColR.sTexCoords.bottom_right,
			idColR.sTexCoords.top_right,
			idColR.sTexCoords.top_left,
			idNormR.sTexCoords.bottom_left,
			idNormR.sTexCoords.bottom_right,
			idNormR.sTexCoords.top_right,
			idNormR.sTexCoords.top_left);

		// Bottom left corner
		vPos.x = _mvPosition.x;
		vPos.y = _mvPosition.y + idColTL.v2fDimensions.y + _mvDimensions.y;
		vDims = idColBL.v2fDimensions;
		pVB->addQuad2D(vPos, vDims, col,
			idColBL.sTexCoords.bottom_left,
			idColBL.sTexCoords.bottom_right,
			idColBL.sTexCoords.top_right,
			idColBL.sTexCoords.top_left,
			idNormBL.sTexCoords.bottom_left,
			idNormBL.sTexCoords.bottom_right,
			idNormBL.sTexCoords.top_right,
			idNormBL.sTexCoords.top_left);

		// Bottom edge
		vPos.x = _mvPosition.x + idColBL.v2fDimensions.x;
		vPos.y = _mvPosition.y + idColTL.v2fDimensions.y + _mvDimensions.y;
		vDims.x = _mvDimensions.x;
		vDims.y = idColB.v2fDimensions.y;
		pVB->addQuad2D(vPos, vDims, col,
			idColB.sTexCoords.bottom_left,
			idColB.sTexCoords.bottom_right,
			idColB.sTexCoords.top_right,
			idColB.sTexCoords.top_left,
			idNormB.sTexCoords.bottom_left,
			idNormB.sTexCoords.bottom_right,
			idNormB.sTexCoords.top_right,
			idNormB.sTexCoords.top_left);

		// Bottom right corner
		vPos.x = _mvPosition.x + idColBL.v2fDimensions.x + _mvDimensions.x;
		vPos.y = _mvPosition.y + idColTL.v2fDimensions.y + _mvDimensions.y;
		vDims = idColBR.v2fDimensions;
		pVB->addQuad2D(vPos, vDims, col,
			idColBR.sTexCoords.bottom_left,
			idColBR.sTexCoords.bottom_right,
			idColBR.sTexCoords.top_right,
			idColBR.sTexCoords.top_left,
			idNormBR.sTexCoords.bottom_left,
			idNormBR.sTexCoords.bottom_right,
			idNormBR.sTexCoords.top_right,
			idNormBR.sTexCoords.top_left);

		pVB->update();
		pVB->render();

		pAtlas->unbind();
		pShader->unbind();	// Unbind the GUI shader
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
	}

}