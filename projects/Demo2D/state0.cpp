#include "PCH.h"
#include "state0.h"

namespace X
{
	void CState0::onEnter(void)
	{
		SC2DRenderer* p2DRenderer = SC2DRenderer::getPointer();		// Obtain pointer to 2D renderer
		C2DWorld* pWorld = p2DRenderer->addWorld("Scene0World");	// Add a new world to contain everything
		C2DLayer* pLayer = pWorld->addLayer("Scene0Layer");			// Add a new layer to the world
		C2DCamera* pCamera = pWorld->addCamera("Scene0Camera");		// Add a new camera to the world
		SCWindow* pWindow = SCWindow::getPointer();
		SCResourceManager* pRM = SCResourceManager::getPointer();
		SCGUIManager* pGUI = SCGUIManager::getPointer();

		// Get filenames of each image for the top down creature and it's rotations
		std::vector<std::string> vstrImageFilenames = StringUtils::getFilesInDir("data/Demo2D/images/creature_top_down/");

		// Add 1000 entities
		for (int i = 0; i < 1000; i++)
		{
			// Create unique name for the entity
			std::string strEntityName = "Entity: " + std::to_string(i);

			// Add entity to the layer
			C2DEntity* pEntity = pLayer->addEntity(strEntityName, "Scene0Creatures");

			// Set which images the entity will be using
			pEntity->setImagesMultiple(vstrImageFilenames);

			// Set random position
			CVector2f vPos(randf(0, (float)pWindow->getWidth()), randf(0, (float)pWindow->getHeight()));
			pEntity->setPosition(vPos);

			// Set scale
			pEntity->setScale(0.5, 0.5);

			// Set random initial rotation
			mfEntityBackgroundDegrees[i] = randf(0, 360.0f);

			// Set entity rotation direction
			if (randf(0, 1) < 0.5f)
				mfEntityRotDir[i] = -1.0f;
			else
				mfEntityRotDir[i] = 1.0f;
		}

		// Create state window
		CGUIContainer* pCont = pGUI->addContainer("state0");
		pCont->setDimensions(640, 640);
		pCont->setPosition(0, 0);
		pCont->mstrTitleText = "State zero.";

		// Add text scroll
		std::string strTxt;
		strTxt += "Welcome to the Demo2D application.\n";
		strTxt += "The app has a number of states, each showing an example of a particular part of the 2D rendering code. These states can be switched between with the buttons found in the \"State Selection\" window, found in the lower right of the screen.\n \n";
		strTxt += "Welcome to state 0!\n";
		strTxt += "This state shows a single world and a single camera with one layer\n";
		strTxt += "The layer has 1000 CEntity objects rotating around, flipping between their many images stored in their texture atlas.\n";
		strTxt += "Controls...\n";
		strTxt += "The cursor keys move the camera.\n \n";
		strTxt += "Details...\n";
		strTxt += "To get anything rendered, we need to use the singleton class SC2DRenderer.\n";
		strTxt += "We use this to render everything and to add worlds.\n";
		strTxt += "A C2DWorld holds one or more C2DCamera objects which render the world to their set framebuffer object and a camera has a position, so we can render a world to multiple framebuffers at different locations.\n";
		strTxt += "A C2DWorld holds all layers which hold all the renderable entity types we have to play with. Layers have a Z order and can be positioned above or underneath other layers to help with the rendering order of each layers' entities.\n";
		strTxt += "Each layer can have added to it, one or more entities of various types...\n \n";
		strTxt += "CEntity is a simple entity which can be positioned and scaled and have multiple images used to render itself with. These images are stored in the SCResourceManager with it's addTexture2DAtlas() method which accepts a vector of image file names to insert into a single texture.\n";
		strTxt += "These images can be used to represent an entity's rotation, or an animation sequence.\n \n";
		strTxt += "CEntityLine entities use a CResourceLine object to render lines. These lines can be cubes, circles or simple lines and unlike the previously mentioned entities, can actually be rotated. They are generally used for debug rendering.\n \n";
		strTxt += "CEntityRot entities, unlike CEntities, do not (Although can do) use multiple images stored in an atlas to represent their rotations, but instead, have their vertices rotated on the GPU via a world matrix prior to being sent to the graphics device. This allows for more flexibility (Being able to be rotated), at the expense of less efficiency (No batching).\n";
		pCont->addTextScroll("TextScroll", 0, 0, 640, 640, strTxt);
	}

	void CState0::onExit(void)
	{
		SC2DRenderer* p2D = SC2DRenderer::getPointer();
		SCGUIManager* pGUI = SCGUIManager::getPointer();
		p2D->removeWorld("Scene0World");
		pGUI->removeContainer("state0");
	}

	void CState0::onActive(CFiniteStateMachine* pFSM)
	{
		// Timer delta
		timer.update();

		SC2DRenderer* p2D = SC2DRenderer::getPointer();
		SCGUIManager* pGUI = SCGUIManager::getPointer();
		SCInputManager* pInput = SCInputManager::getPointer();

		// Rotate background entities
		C2DWorld* pWorld = p2D->getWorld("Scene0World");
		C2DLayer* pLayer = pWorld->getLayer("Scene0Layer");
		for (int i = 0; i < 1000; i++)
		{
			std::string strEntityName = "Entity: " + std::to_string(i);
			C2DEntity* pEntity = pLayer->getEntity(strEntityName);
			mfEntityBackgroundDegrees[i] += timer.getSecondsPast() * 45.0f * mfEntityRotDir[i];
			while (mfEntityBackgroundDegrees[i] >= 360)
				mfEntityBackgroundDegrees[i] -= 360;
			while (mfEntityBackgroundDegrees[i] < 0.0f)
				mfEntityBackgroundDegrees[i] += 360;
			pEntity->setFrameBasedOnAngle(mfEntityBackgroundDegrees[i]);
		}

		// Now move camera
		C2DCamera* pCamera = pWorld->getCamera(0);
		CVector2f vCamPos = pCamera->getPosition();
		if (pInput->key.pressed(KC_UP))
			vCamPos.y -= timer.getSecondsPast() * 150.0f;
		if (pInput->key.pressed(KC_DOWN))
			vCamPos.y += timer.getSecondsPast() * 150.0f;
		if (pInput->key.pressed(KC_LEFT))
			vCamPos.x -= timer.getSecondsPast() * 150.0f;
		if (pInput->key.pressed(KC_RIGHT))
			vCamPos.x += timer.getSecondsPast() * 150.0f;
		pCamera->setPosition(vCamPos);
	}
}