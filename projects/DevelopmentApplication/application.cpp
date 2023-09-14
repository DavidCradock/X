#include "application.h"
#include "resource.h"

namespace X
{
	void CApplication::initOnce(void)
	{
		// Use the resource loading screen
		x->pLoadingScreen->onInit(0);
		x->pLoadingScreen->setFadeOut(0.0f);

		// Set window title bar text and set icon
		x->pWindow->setText("X: DevApp. F1: Toggle fullscreen. F2: Toggle Vsync. F3: Toggle statistics window. F4: Toggle profiling window.");
		x->pWindow->setIcon(IDI_ICON1);

		// Set mouse cursor
		x->pUI->mouseCursorSetToNormal();

		// End of loading screen
		x->pLoadingScreen->onInitEnd();

		// Setup messaging system
		// Create a message service which handles sending and receiving of messages to/from message users
		CMessageService* pMsgService = x->pMessageSystem->serviceAdd("service");	// Create a messaging service
		CMessageUser* pUserDavid = x->pMessageSystem->userAdd("David");				// Add a user
		CMessageUser* pUserMojo = x->pMessageSystem->userAdd("Mojo");				// Add another user
		x->pMessageSystem->subscribeUserToService("David", "service");				// Subscribe "David" user to "service" service.
		CMessage msg("ThisIsMessageContents", EMessageType::UNKNOWN);				// Create a message for some user to send to the service
		pUserMojo->postNewMessage(msg, "service");									// Let user "Mojo" send the message to the "service" service
		pMsgService->dispatch();													// Get the service to dispatch all received messages to all subscribed users
		while (pUserMojo->doesInboxContainMessages())								// Get Mojo user to check to see if their inbox contains any messages
			CMessage message = pUserMojo->getMessageFromInbox();					// If there's a message, remove it from the user's inbox and then we can read the message
		// There won't be any messages as user Mojo is not subscribed to any service.
		if (pUserDavid->doesInboxContainMessages())									// Get David user to check to see if their inbox contains any messages
		{
			std::vector<CMessage> messages = pUserDavid->getMessagesFromInbox();	// Get all messages from the user's inbox, emptying the inbox in the process.
			for (size_t iMessage = 0; iMessage < messages.size(); iMessage++)		// Do stuff with the messages
			{
				CMessage mes = messages[iMessage];
				if (mes.getMessageSender() == "Mojo")
				{
					std::string strContents = mes.getMessageContents();
				}
			}
		}

		// Setup UserInterface theme
//		CUIWindow* pWindow = x->pUI->windowAdd("window1 200x200", false);
//		pWindow->setDimensions(CVector2f(200, 200));
//		pWindow->setPosition(1400-16, 200-30);
//		pWindow->setResizable(true, CVector2f(200, 200), CVector2f(400, 400));
//		pWindow->progressbarAdd("pb1", 0, 0, 200, 30);
//		CUITextEdit* pTextEdit = pWindow->textEditAdd("textedit", 0, 50, 100, 40, "Hello");

		x->pAppMan->debugShowGrid(!x->pAppMan->debugGridShown(), 100, 100);
//		x->pUI->windowGet(x->pUI->getDefaultContainers()->names.profiling)->setPosition(0, 0);

		// Show old GUI statistics
		x->pGUI->getContainer("X:Default:Statistics")->setVisible(true);

		// Create lots of UI elements for stress testing
		for (int i = 0; i < 1; i++)
		{
			std::string str;
			str = "windowSTRESS_TEST_" + std::to_string(i);
			CUIWindow* pWindow = x->pUI->windowAdd(str, false);
			pWindow->setDimensions(850, 480);
			pWindow->setPosition(i * 50, 500 + (i * 50));
			pWindow->textAdd("text_results", 0, 0, 800, 120, "Before optimization in release no debug with vsync off: We get an FPSaverage of 67 fps. In the profiler window...\nx->pUI->render() takes up around 70% of main and a time of around 10 and 11 millseconds.\nx->pUI->update() takes up around 20% of main and a time of around 2.8 and 3.1 millseconds.");
			for (int j = 15; j < 20; j++)
			{
				str = "button_" + std::to_string(j);
				CUIButton* pBut = pWindow->buttonAdd(str, 0, j * 30, 100, 24);

				str = "checkbox_" + std::to_string(j);
				CUICheckbox* pCB = pWindow->checkboxAdd(str, 120, j * 30, 100, 24);

				if (j % 3 == 0)
				{
					str = "linegraph_" + std::to_string(j);
					CUILineGraph* pLG = pWindow->lineGraphAdd(str, 250, j * 40, 100, 100);
				}

				str = "progressbar_" + std::to_string(j);
				CUIProgressbar* pPB = pWindow->progressbarAdd(str, 370, j * 30, 100, 30);

				str = "scrollbar_" + std::to_string(j);
				CUIScrollbar* pSB = pWindow->scrollbarAdd(str, 500, j * 30, 100, 30);
				pSB->setTabPos(randf(0.0f, 1.0f));

				str = "text_" + std::to_string(j);
				pWindow->textAdd(str, 610, j * 30, 90, 30, str);

				str = "textEdit_" + std::to_string(j);
				pWindow->textEditAdd(str, 710, j * 50, 100, 40, str);
			}

			pWindow->colourSelectorAdd("colourSelector_", 0, 140, 256, 256);
		}
		SCWindow::getPointer()->setVsync(false);

		// Real simple as it gets framerate text
		x->pUI->containerAdd("FPSonly")->textAdd("FPS", 0, 0, 150, 30, "FPS");
		x->pUI->containerGet("FPSonly")->setDimensions(150, 40);

		CImage image;
		image.drawCircle(512, CColour(1, 0, 0, 1), CColour(0, 0, 1, 0.15f));
		image.saveAsPNG("CImage_drawCircle.png");

	}

	void CApplication::onStart(void)
	{
	}

	void CApplication::onStop(void)
	{
	}

	bool CApplication::onUpdate(void)
	{

//		static float sfProgress = 0.0f;
//		sfProgress += timer.getSecondsPast();
//		if (sfProgress >= 1.0f)
//			sfProgress = 0.0f;
//		x->pUI->windowGet("window1 200x200")->progressbarGet("pb1")->setProgress(sfProgress);

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

		// Toggle statistics window
		if (x->pInput->key.once(KC_F3))
		{
			CGUIContainer* pStatsCont = x->pGUI->getContainer("X:Default:Statistics");
			pStatsCont->setVisible(!pStatsCont->getVisible());

			CUIWindow* pWindow = x->pUI->windowGet(x->pUI->getDefaultContainers()->names.statistics);
			pWindow->setVisible(!pWindow->getVisible());
		}

		// Toggle profiling window
		if (x->pInput->key.once(KC_F4))
		{
			CUIWindow* pWindow = x->pUI->windowGet(x->pUI->getDefaultContainers()->names.profiling);
			pWindow->setVisible(!pWindow->getVisible());
		}

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