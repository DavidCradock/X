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
		x->pWindow->setText("X: DevApp. F1: Toggle fullscreen. F2: Toggle Vsync. F3: Toggle statistics window.");
		x->pWindow->setIcon(IDI_ICON1);

		// Set mouse cursor
		SCInputManager::getPointer()->mouse.setMouseCursorImage("data/X/cursors/new_default.ani");

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
		CUIWindow* pWindow = x->pUI->windowAdd("window1", false);
		pWindow->setDimensions(CVector2f(200, 200));
//		pWindow->scrollbarAdd("scrollbar", 220, 0, 100, 24, 0.05f);
		pWindow->setPosition(1400-16, 800-30);
		pWindow->buttonAdd("buttonC",   50,  100, 100, 24);
		pWindow->buttonAdd("buttonT",   50, 100, 100, 24);
		pWindow->buttonAdd("buttonB",   50,  300, 100, 24);
		pWindow->buttonAdd("buttonL",  50,  100, 100, 24);
		pWindow->buttonAdd("buttonR",  300,  100, 100, 24);
		pWindow->buttonAdd("button0x0", 0, 0, 100, 24);
		pWindow->buttonAdd("button300x0", 300, 0, 100, 24);
		pWindow->buttonAdd("button0x300", 0, 300, 100, 24);
		x->pUI->windowAdd("Window2");

//		pWindow = x->pUI->windowAdd("window2 300x300");
//		pWindow->setDimensions(CVector2f(300, 300));
//		pWindow->setPosition(CVector2f(400-16, 400-30));
//		CUIButton* pButton = pWindow->buttonAdd("100x24", 0, 0, 100, 24);
		//pButton
//		CUIScrollbar* pScrollbar = pWindow->scrollbarAdd("100x24", 0, 0, 100, 24);

		// Add container for debugging
//		CUIContainer* pContainer = x->pUI->containerAdd("container0 100x100x100x100");
//		pContainer->setDimensions(CVector2f(100, 100));
//		pContainer->setPosition(100, 100);
//		pButton = pContainer->buttonAdd("0x0x100x100", 0, 0, 100, 100);

//		x->pAppMan->debugShowGrid(!x->pAppMan->debugGridShown(), 100, 100);

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
		}
		return true;
	}

}