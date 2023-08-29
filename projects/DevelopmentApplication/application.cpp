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
		pWindow->scrollbarAdd("scrollbar", 10, 10, 100, 100, 0.05f);
		pWindow->setDimensions(CVector2f(320, 240));
		pWindow->setPosition(CVector2f((x->pWindow->getDimensions().x / 2) - 160, (x->pWindow->getDimensions().y / 2) - 120));

		pWindow = x->pUI->windowAdd("window2");
		pWindow->setDimensions(CVector2f(640, 480));
		pWindow->setPosition(CVector2f(800, 0));
		CUIButton* pButton = pWindow->buttonAdd("myFirstButton", 0, 0, 128, 24);

		CGUIContainer *pCont = x->pGUI->addContainer("debug");
		pCont->setDimensions(320, 240);

		pCont->addText("text0", 0, 0, "Mouse Over Window: None");
		pCont->addText("text1", 0, 20, "DEBUG text1");

	}

	void CApplication::onStart(void)
	{
	}

	void CApplication::onStop(void)
	{
	}

	bool CApplication::onUpdate(void)
	{
		// Debug text
		CGUIContainer* pCont = x->pGUI->getContainer("debug");
		// Mouse is over which window/container?
		std::string str = x->pUI->getMouseIsOver();
		std::string str2 = "Mouse Over Window : ";
		if (str.size())
			str2 += str;
		else
			str2 += "None";
		pCont->getText("text0")->setText(str2);
		// Mouse is over a titlebar
		CRect rectTitlebarArea = x->pUI->windowGet(0)->getTitlebarArea();
		if (rectTitlebarArea.doesPositionFitWithin(x->pInput->mouse.getCursorPos()))
		{
			pCont->getText("text1")->setText("Mouse IS over titlebar of window0");
		}
		else
		{
			pCont->getText("text1")->setText("Mouse is NOT over titlebar of window0");
		}

		// Timer delta
		timer.update();

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