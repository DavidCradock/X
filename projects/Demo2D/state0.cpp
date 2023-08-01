#include "PCH.h"
#include "state0.h"

namespace X
{
	void CState0::onEnter(void)
	{
		SC2DRenderer* p2DRenderer = SC2DRenderer::getPointer();		// Obtain pointer to 2D renderer
		C2DWorld* pWorld = p2DRenderer->addWorld("Scene0World");		// Add a new world to contain everything
		C2DLayer* pLayer = pWorld->addLayer("Scene0Layer");				// Add a new layer to the world
		C2DCamera* pCamera = pWorld->addCamera("Scene0Camera");			// Add a new camera to the world
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

		// Now add controllable complex entity on a new layer above the layer with 1000 entities
		pLayer = pWorld->addLayer("Layer2");

		// Add complex entity to the layer
		C2DEntityComplex* pEntityComplex = pLayer->addEntityComplex("EntityControllable");

		// Add a layer to the complex entity
		C2DLayerComplex* pLayerComplex = pEntityComplex->addLayer("ParentLayer");

		// Setup the texture atlas the "PARENT" entity of the complex entity will use
		vstrImageFilenames = StringUtils::getFilesInDir("data/Demo2D/images/spaceship_hull/");
		C2DEntity* pEntity = pLayerComplex->addEntity("PARENT", "Scene0SpaceshipHull");
		pEntity->setImagesMultiple(vstrImageFilenames);
		mvEntityDir.set(0.0f, 1.0f);
		mvEntityPos.set((pWindow->getDimensions().x * 0.5f), (pWindow->getDimensions().y * 0.5f));
		mfEntityVel = 0.0f;

		// Now set up child entities of the parent entity (The gun turrets)
		// Setup the texture atlas the gun turret entities of the complex entity will use
		vstrImageFilenames = StringUtils::getFilesInDir("data/Demo2D/images/spaceship_turret/");

		// Create a new layer in the complex entity for the gun turrets
		pLayerComplex = pEntityComplex->addLayer("TurretLayer");
		// Create a new child entity in the new layer
		pEntity = pLayerComplex->addEntity("Gun1", "Scene0SpaceshipTurrets");
		// Set the atlas the gun entity will use
		pEntity->setImagesMultiple(vstrImageFilenames);
		// Scale it down a bit
		pEntity->setScale(0.5f, 0.5f);
		// Set position offset from the "PARENT" entity
		pEntity->setPosition(CVector2f(0, 50));

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
		strTxt += "This state shows a single world and a single camera with two layers (Well, three, but hold on...)\n";
		strTxt += "The lowest layer, the one that's rendered first and is therefore in the background, has 1000 CEntity objects rotating around, flipping between their many images stored in their texture atlas.\n";
		strTxt += "The next layer, has a CEntityComplex object, the controllable space ship, which itself has 2 layers, the lowest have the space ship hull and the layer above that, the gun turret which is a child of the \"PARENT\" space ship hull entity.\n \n";
		strTxt += "Controls...\n";
		strTxt += "The cursor keys move the camera.\n";
		strTxt += "The WSAD keys move and rotate entity.\n \n";
		strTxt += "Details...\n";
		strTxt += "To get anything rendered, we need to use the singleton class SC2DRenderer.\n";
		strTxt += "We use this to render everything and to add worlds.\n";
		strTxt += "A C2DWorld holds one or more C2DCamera objects which render the world to their set framebuffer object and a camera has a position, so we can render a world to multiple framebuffers at different locations.\n";
		strTxt += "A C2DWorld holds all layers which hold all the renderable entity types we have to play with. Layers have a Z order and can be positioned above or underneath other layers to help with the rendering order of each layers' entities.\n";
		strTxt += "Each layer can have added to it, one or more entities of various types...\n \n";
		strTxt += "CEntity is a simple entity which can be positioned and scaled and have multiple images used to render itself with. These images are stored in the SCResourceManager with it's addTexture2DAtlas() method which accepts a vector of image file names to insert into a single texture.\n";
		strTxt += "These images can be used to represent an entity's rotation, or an animation sequence.\n \n";
		strTxt += "CEntityComplex is just like a CEntity, except it can have child entities which can be set to \"rotate\" along with it's \"PARENT\" entity, using a position offset from the parent entity.\n \n";
		strTxt += "CEntityLine entities use a CResourceLine object to render lines. These lines can be cubes, circles or simple lines and unlike the previously mentioned entities, can actually be rotated. They are generally used for debug rendering.\n \n";
		strTxt += "CEntityRot entities, unlike CEntities, do not (Although can do) use multiple images stored in an atlas to represent their rotations, but instead, have their vertices rotated on the CPU via a CMatrix object prior to being sent to the graphics device. This allows for more flexibility (Being able to be rotated), at the expense of more CPU cycles needed to perform the rotation.\n";
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

		// Now deal with controllable entity
		pLayer = pWorld->getLayer("Layer2");
		C2DEntityComplex* pEntityComplex = pLayer->getEntityComplex("EntityControllable");
		pEntityComplex->setFrameBasedOnDirection(mvEntityDir);
		// Set rotation of entity
		pEntityComplex->setRotation(mvEntityDir);

		if (pInput->key.pressed(KC_D))
			mvEntityDir.rotate(timer.getSecondsPast() * 180.0f);
		if (pInput->key.pressed(KC_A))
			mvEntityDir.rotate(timer.getSecondsPast() * -180.0f);
		if (pInput->key.pressed(KC_W))
		{
			mfEntityVel += timer.getSecondsPast() * 1.5f;
			if (mfEntityVel > 2.0f)
				mfEntityVel = 2.0f;
		}
		if (pInput->key.pressed(KC_S))
		{
			mfEntityVel -= timer.getSecondsPast() * 1.5f;
		}
		// Reduce velocity
		mfEntityVel -= timer.getSecondsPast() * 0.85f;
		if (mfEntityVel < 0.0f)
			mfEntityVel = 0.0f;
		// Compute position offset of entity based upon velocity.
		CVector2f vOffset = mvEntityDir * timer.getSecondsPast() * 250.0f * mfEntityVel;
		vOffset.y *= -1.0f;							// Invert Y for correct direction
		mvEntityPos += vOffset;						// Adjust position
		pEntityComplex->setPosition(mvEntityPos);	// Set position

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