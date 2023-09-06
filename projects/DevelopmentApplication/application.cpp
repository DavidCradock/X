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
		// Top most container
		CUIContainer* pContainer = x->pUI->containerAdd("Container0");
		pContainer->setDimensions(CVector2f(200, 200));
		pContainer->setPosition(1400, 100);
		CUIButton* pButton = pContainer->buttonAdd("0x0x100x24", 0, 0, 100, 24);
		pContainer->buttonAdd("200x0x100x24", 200, 0, 100, 24);
		pContainer->buttonAdd("0x200x100x24", 0, 200, 100, 24);

		pContainer = x->pUI->containerAdd("Container 1 200x200");
		pContainer->setPosition(1400, 400);
		pContainer->textAdd("text", 0, 0, 200, 200, "Here's some text in a 200x200 CUIText object inside a 200x200 container object.\nLet's have a newline here shall we?");
		pContainer->setDimensions(200, 200);

		CUIWindow* pWindow = x->pUI->windowAdd("window1 200x200", false);
		pWindow->setDimensions(CVector2f(200, 200));
		pWindow->setPosition(1400-16, 800-30);
		pWindow->buttonAdd("buttonC",   50,  100, 100, 24);
		pWindow->buttonAdd("buttonT",   50, 100, 100, 24);
//		pWindow->buttonAdd("buttonB",   50,  300, 100, 24);
		pWindow->buttonAdd("buttonL",  50,  100, 100, 24);
		pWindow->buttonAdd("buttonR",  300,  100, 100, 24);
		pWindow->buttonAdd("button0x0", 0, 0, 100, 24);
		pWindow->buttonAdd("button300x0", 300, 0, 100, 24);
//		pWindow->buttonAdd("button0x300", 0, 300, 100, 24);
		pWindow->textAdd("text0", 0, 30, 100, 20, "Hello World");
		pWindow->textAdd("text1", 0, 60, 200, 200, "This is lots of text.\nRendered on multiple lines.\nThis may cause stuff to happen.");

		pWindow = x->pUI->windowAdd("Window2 200x200 No widgets outside");
		pWindow->setDimensions(200, 200);
		pWindow->setPosition(1400-16, 1200-30);
		CUIText* pText;
//		pText = pWindow->textAdd("textNormal", 0, 0, 200, 30, "Normal0,0,200,30");
//		pText->setBackgroundColour(CColour(0, 0, 0, 0.5f));
		pText = pWindow->textAdd("textCentered", 100, 75, 100, 40, "100,75,180,30");
		pText->setBackgroundColour(CColour(0, 0, 0, 0.5f));
		pText->setCentered(true);
		
		pWindow = x->pUI->windowAdd("Window3 200x200");
		pWindow->setDimensions(200, 200);
		pWindow->setPosition(1700 - 16, 1200 - 30);
		pWindow->textAdd("text0", 0, 0, 200, 30, "Widget to right");
		pWindow->textAdd("text1", 300, 0, 200, 30, "Here I am!");

		pWindow = x->pUI->windowAdd("Window4 200x200");
		pWindow->setDimensions(200, 200);
		pWindow->setPosition(1400 - 16, 1500 - 30);
		pWindow->textAdd("text0", 0, 0, 200, 30, "Widget below");
		pWindow->textAdd("text1", 0, 300, 200, 30, "Here I am!");


		x->pAppMan->debugShowGrid(!x->pAppMan->debugGridShown(), 100, 100);

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