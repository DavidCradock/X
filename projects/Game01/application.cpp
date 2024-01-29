#include "application.h"
#include "resource.h"

namespace X
{
	void CApplication::initOnce(void)
	{
		// Use the resource loading screen
//		x->pLoadingScreen->onInit(0);
//		x->pLoadingScreen->setFadeOut(0.0f);

		// Set window title bar text and set icon
		x->pWindow->setText("X: Game01. F1: Toggle fullscreen. F2: Toggle Vsync. F3: Toggle defaults window.");
		x->pWindow->setIcon(IDI_ICON1);

		// Set mouse cursor
		x->pUI->mouseCursorSetToNormal();

		// End of loading screen
//		x->pLoadingScreen->onInitEnd();

		

		// Real simple as it gets framerate text
		timer.setAveragedFPSRate(1.0f);
		x->pUI->containerAdd("FPSonly")->textAdd("FPS", 0, 0, 150, 30, "FPS");
		x->pUI->containerGet("FPSonly")->setDimensions(150, 40);

		// Show statistics
//		x->pUI->windowGet(x->pUI->getDefaultContainers()->names.statistics)->setVisible(true);

		// UI Theme editor default window
//		CUIWindow* pWindow = x->pUI->windowGet(x->pUI->getDefaultContainers()->names.uiThemeEditor);
//		pWindow->setVisible(true);
//		pWindow->setPositionCentre();
	}

	void CApplication::onStart(void)
	{
	}

	void CApplication::onStop(void)
	{
	}

	bool CApplication::onUpdate(void)
	{
		// Timer delta
		timer.update();

		static float sfUpdate = 1.0f;
		sfUpdate -= timer.getSecondsPast();
		if (sfUpdate <= 0.0f)
		{
			sfUpdate = 1.0f;
			std::string strTxt = "FPS: ";
			StringUtils::appendFloat(strTxt, timer.getFPSAveraged(), 1);
			x->pUI->containerGet("FPSonly")->textGet("FPS")->setText(strTxt);
		}

		// G key to toggle debug grid
		if (x->pInput->key.once(KC_G))
		{
			CVector2f vDebugGridSpacing = x->pAppMan->debugGridSpacing();
			x->pAppMan->debugShowGrid(!x->pAppMan->debugGridShown(), (int)vDebugGridSpacing.x, (int)vDebugGridSpacing.y);
		}
		if (x->pInput->key.repeat(KC_EQUALS))
		{
			CVector2f vDebugGridSpacing = x->pAppMan->debugGridSpacing();
			x->pAppMan->debugShowGrid(x->pAppMan->debugGridShown(), (int)vDebugGridSpacing.x + 1, (int)vDebugGridSpacing.y + 1);
		}
		if (x->pInput->key.repeat(KC_MINUS))
		{
			CVector2f vDebugGridSpacing = x->pAppMan->debugGridSpacing();
			x->pAppMan->debugShowGrid(x->pAppMan->debugGridShown(), (int)vDebugGridSpacing.x - 1, (int)vDebugGridSpacing.y - 1);
		}
		// Escape key to exit
		if (x->pInput->key.pressed(KC_ESCAPE))
			return false;

		// Toggle fullscreen
		if (x->pInput->key.once(KC_F1))
			SCWindow::getPointer()->toggleFullscreen();

		// Toggle vertical sync
		if (x->pInput->key.once(KC_F2))
			SCWindow::getPointer()->setVsync(!SCWindow::getPointer()->getVSyncEnabled());

		// Toggle defaults window
		if (x->pInput->key.once(KC_F3))
		{
//			CGUIContainer* pStatsCont = x->pGUI->getContainer("X:Default:Statistics");
//			pStatsCont->setVisible(!pStatsCont->getVisible());

			CUIWindow* pWindow = x->pUI->windowGet(x->pUI->getDefaultContainers()->names.defaults);
			pWindow->setVisible(!pWindow->getVisible());
		}

		// Toggle profiling window
//		if (x->pInput->key.once(KC_F4))
//		{
//			CUIWindow* pWindow = x->pUI->windowGet(x->pUI->getDefaultContainers()->names.profiling);
//			pWindow->setVisible(!pWindow->getVisible());
//		}

		return true;
	}

	void CApplication::onFrameEnd(void)
	{
		if (x->pInput->key.once(KC_F9))
		{
			CResourceFramebuffer* pFB = x->pResource->getFramebuffer(x->pResource->defaultRes.framebuffer_ui);
			pFB->bindAsRenderTarget(false);
			CImage img;
			img.createBlank(x->pWindow->getWidth(), x->pWindow->getHeight(), 4);
			img.fillFromOpenGL();
			img.saveAsPNG("glReadPixels_output.png");
			pFB->unbindAsRenderTarget();
		}
	}

}