#include "application.h"

namespace X
{
	void Application::initOnce(void)
	{
/*
		Image img;
		Image normal;
		img.load("data/X/gui/default/progressBarBGheight.png");
		img.normalmap(normal, 0.5f);
		normal.saveAsPNG("data/X/gui/default//progressBarBGnormal.png");

		img.load("data/X/gui/default/progressBarFillerheight.png");
		img.normalmap(normal, 0.5f);
		normal.saveAsPNG("data/X/gui/default/progressBarFillernormal.png");
*/	
//		ResourceManager::getPointer()->buildFontFiles("data/x/fonts/UltimateSerial-Medium-Regular", "UltimateSerial-Medium-Regular.ttf", 20, true, false, false, false, false);

		GUITheme theme;
		theme.save("data/X/GUI/default");
		GUITheme theme2;
		theme2.load("data/X/GUI/default");

		Window* pWindow = Window::getPointer();
		pWindow->getMaxTextureSize();
		GUIManager* pGUI = GUIManager::getPointer();

		// Small container window with close button
		GUIContainer* pContainer1 = pGUI->addContainer("Container1");
		pContainer1->mfPositionX = 412.0f;
		pContainer1->mfPositionY = 412.0f;
		pContainer1->addButton("Close", 0, 0, 64, 16, "Close");
		
		// Container Preview
		GUIContainer* pContainer2 = pGUI->addContainer("ContainerPreview");
		pContainer2->setDimensions(1024, 768);
		// Centre position of container
		pContainer2->setPosition((float)pWindow->getWidth() / 2 - 512, (float)pWindow->getHeight() / 2 - (768 / 2));
	
		// Add each widget and static text naming the widget type
		// Text edit
		pContainer2->addText("Text0", 0, 10, "GUITextEdit:");
		GUITextEdit* pTextEdit = pContainer2->addTextEdit("TextEdit", 140, 0, 100, 40, "TextEdit");
		pTextEdit->setMaxChars(16);
		pTextEdit->setIntegerInputOnly(false);
		// Button
		pContainer2->addText("Text1", 0, 60, "GUIButton:");
		GUIButton* pButton = pContainer2->addButton("Button", 140, 60, 100, 30, "A button.");
		// Line graph
		pContainer2->addText("Text2", 0, 100, "GUILineGraph:");
		GUIColour col;
		GUILineGraph* pLineGraph = pContainer2->addLineGraph("FPS", 140, 90, 200, 50);
		pLineGraph->addDataset("FPS", col);
		// Slider
		pContainer2->addText("Text3", 0, 160, "GUISlider H:");
		pContainer2->addSlider("SliderH", 140, 160, 200, 30, 0.1f);
		// Progress bar
		pContainer2->addText("Text4", 0, 200, "GUIProgress H:");
		pContainer2->addProgressBar("ProgressH", 140, 200, 200, 30);
		// Image
		pContainer2->addImage("Image", 140, 240, "data/DevApp/textures/image_test.png", -1, -1);


		ResourceManager* pRM = ResourceManager::getPointer();
		timer.setAveragedFPSRate(1);	// Once every X seconds

		// Load some stuff in and setup simple scene manager
		_initSceneManager();

		// Create some fonts using the resource manager
//		pRM->buildFontFiles("data/DevApp/fonts/tahoma", "tahoma", 200, true, false, false, false, false);

		// Add some fonts
		pRM->addFont("arial_26", "data/DevApp/fonts/arial_26");
//		pRM->addFont("tahoma_200", "data/DevApp/fonts/tahoma_200");

		// Temporary debug triangle 2D quad for showing depth buffer from shadowmap
//		pRM->addTriangle("DEBUG_QUAD");
	}

	void Application::onStart(void)
	{
	}

	void Application::onStop(void)
	{

	}

	bool Application::onUpdate(void)
	{
		// Get pointers to needed managers
		InputManager* pInputManager = InputManager::getPointer();
		ResourceManager* pRM = ResourceManager::getPointer();

		// Timer delta
		timer.update();

		// Update and render scene manager to a framebuffer
//		mSceneManagerSimple.mCamera.update();
		mSceneManagerSimple.render();

		// Render the debug shadow map onto the screen
//		ResourceDepthbuffer* pDepthbuffer = pRM->getDepthbuffer("X:shadows");
//		pDepthbuffer->renderToBackbuffer(pWindow->getWidth() - 512, 0, 512, 512);

		float fInc = timer.getSecondsPast();

		// GUI button clicks
		GUIManager* pGUIMan = GUIManager::getPointer();
		GUIContainer* pCont = pGUIMan->getContainer("Container1");
		GUIButton* pButton = pCont->getButton("Close");
		if (pButton->getClicked())
			pCont->setVisible(false);

		// Update FPS linegraph
		static float fTimeToAddNewValue = 0.0f;
		fTimeToAddNewValue += fInc;
		if (fTimeToAddNewValue >= 0.1f)
		{
			fTimeToAddNewValue = 0.0f;
			pCont = pGUIMan->getContainer("ContainerPreview");
			GUILineGraph* pLineGraph = pCont->getLineGraph("FPS");
			GUILineGraphDataSet* pDataSet = pLineGraph->getDataset("FPS");
			pDataSet->addValue(timer.getFPS());
			while (pDataSet->getNumValues() > 200)
				pDataSet->removeValue();
		}

		// Update progress bars
		pCont = pGUIMan->getContainer("ContainerPreview");
		GUISlider* pSlider = pCont->getSlider("SliderH");
		pCont->getProgressBar("ProgressH")->setProgress(pSlider->getTabPos());

		// Update line entity
		SMEntityLine* pEntityLine = mSceneManagerSimple.getEntityLine("line");
		// Translation
		if (pInputManager->key.pressed(KC_W))	pEntityLine->translateWorld(glm::vec3(0.0f, 0.0f, 1.0f * fInc));
		if (pInputManager->key.pressed(KC_S))	pEntityLine->translateWorld(glm::vec3(0.0f, 0.0f, -1.0f * fInc));
		if (pInputManager->key.pressed(KC_A))	pEntityLine->translateWorld(glm::vec3(-1.0f * fInc, 0.0f, 0.0f));
		if (pInputManager->key.pressed(KC_D))	pEntityLine->translateWorld(glm::vec3(1.0f * fInc, 0.0f, 0.0f));
		if (pInputManager->key.pressed(KC_R))	pEntityLine->translateWorld(glm::vec3(0.0f, 1.0f * fInc, 0.0f));
		if (pInputManager->key.pressed(KC_F))	pEntityLine->translateWorld(glm::vec3(0.0f, -1.0f * fInc, 0.0f));
		// Scale
		if (pInputManager->key.pressed(KC_T))
		{
			glm::vec3 v3EntityScale = pEntityLine->getScale();
			v3EntityScale.x += fInc;
			v3EntityScale.y += fInc;
			v3EntityScale.z += fInc;
			pEntityLine->setScale(v3EntityScale);
		}
		if (pInputManager->key.pressed(KC_G))
		{
			glm::vec3 v3EntityScale = pEntityLine->getScale();
			v3EntityScale.x -= fInc;
			v3EntityScale.y -= fInc;
			v3EntityScale.z -= fInc;
			pEntityLine->setScale(v3EntityScale);
		}
		// Rotation
		if (pInputManager->key.pressed(KC_Y))
		{

		}
		if (pInputManager->key.pressed(KC_H))
		{

		}
		if (pInputManager->key.pressed(KC_U))
		{

		}
		if (pInputManager->key.pressed(KC_J))
		{

		}
		if (pInputManager->key.pressed(KC_I))
		{

		}
		if (pInputManager->key.pressed(KC_K))
		{

		}

		// Update entity rotation
		for (int i = 0; i < 100; ++i)
		{
			// Temp rotation
			_mvEntityRot[i].x += float(i) * timer.getSecondsPast();
			_mvEntityRot[i].y += float(i) * timer.getSecondsPast();
			_mvEntityRot[i].z += float(i) * timer.getSecondsPast();

			std::string strEntity = "entity_" + std::to_string(i);
			SMEntityTriangle* pEntity = mSceneManagerSimple.getEntityTriangle(strEntity);
			pEntity->setRotation(_mvEntityRot[i].x, _mvEntityRot[i].y, _mvEntityRot[i].z);
		}

		// Render some text
		std::string strTXT = "FPS: ";
		strTXT += std::format("{:.2f}", timer.getFPSAveraged());
		ResourceFont* pFont = pRM->getFont("arial_26");
		int iYpos = 0;
		pFont->print(strTXT, 0, iYpos, Window::getPointer()->getWidth(), Window::getPointer()->getHeight(), 1.0f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		iYpos += (int)pFont->getTextHeight();

		// Render some debug text
		// Position
//		glm::vec3 vPos = pEntityLine->getWorldPosition();
//		strTXT = "Line entity getWorldPosition: AD: " + std::format("{:.4f}", vPos.x) + ", RF: " + std::format("{:.4f}", vPos.y) + ", WS: " + std::format("{:.4f}", vPos.z);
//		pFont->print(strTXT, 0, iYpos, Window::getPointer()->getWidth(), Window::getPointer()->getHeight(), 1.0f);
//		iYpos += (int)pFont->getTextHeight();

		// Scale
//		glm::vec3 vScale = pEntityLine->getScale();
//		strTXT = "Line entity getScale: TG: " + std::format("{:.4f}", vScale.x) + ", " + std::format("{:.4f}", vScale.y) + ", " + std::format("{:.4f}", vScale.z);
//		pFont->print(strTXT, 0, iYpos, Window::getPointer()->getWidth(), Window::getPointer()->getHeight(), 1.0f);
//		iYpos += (int)pFont->getTextHeight();

		// Rotation
//		strTXT = "Line entity rotation YHUJIK";
//		pFont->print(strTXT, 0, iYpos, Window::getPointer()->getWidth(), Window::getPointer()->getHeight(), 1.0f);
//		iYpos += (int)pFont->getTextHeight();

		/*		// Render more text with various scaling values
				float fYpos = 30.0f;
				for (float fScale = 0.1f; fScale < 1.0f; fScale += 0.1f)
				{
					std::string strText = "Text scale: " + std::format("{:.1f}", fScale) + " hmm, OK.";
					pRM->getFont("tahoma_200")->print(strText, 0, fYpos, Window::getPointer()->getWidth(), Window::getPointer()->getHeight(), fScale, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
					fYpos += 100.0f * fScale;
				}
				for (float fScale = 1.0f; fScale < 3.5f; fScale += 0.5f)
				{
					std::string strText = "Text scale: " + std::format("{:.1f}", fScale) + " hmm, OK.";
					pRM->getFont("tahoma_200")->print(strText, 0, fYpos, Window::getPointer()->getWidth(), Window::getPointer()->getHeight(), fScale, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
					fYpos += 100.0f * fScale;
				}
		*/

		// Toggle shadows
		if (pInputManager->key.once(KC_F3))
			mSceneManagerSimple.setShadowsEnabled(!mSceneManagerSimple.getShadowsEnabled());
		// Escape key to exit
		if (pInputManager->key.pressed(KC_ESCAPE))
			return false;
		// Toggle fullscreen
		if (pInputManager->key.once(KC_F1))
		{
			Window::getPointer()->toggleFullscreen();
		}
		// Toggle vertical sync
		if (pInputManager->key.once(KC_F2))
			Window::getPointer()->setVsync(!Window::getPointer()->getVSyncEnabled());
		return true;
	}

	void Application::_initSceneManager(void)
	{
		// Set camera mode
//		mSceneManagerSimple.mCamera.setModeOrbit(
//			glm::vec3(0.0f, 0.0f, 0.0f),	// Orbit point
//			1.5f,							// Min distance from point
//			10.0f,							// Current distance from point
//			100.0f,							// Max distance from point
//			0.005f,							// Sensitivity X
//			0.005f,							// Sensitivity Y
//			0.005f,							// Sensitivity wheel
//			true);							// Limit Y angle to > 0.0f

		mSceneManagerSimple.mCamera.setModeFPS(
		glm::vec3(0.0f, 5.0f, -5.0f),	// Initial position
		90.0f,	// Initial yaw in degrees
		-25.0f,	// Initial pitch in degrees
		0.5f,	// Mouse X sensitivity
		0.2f,	// Mouse Y sensitivity
		1.0f,	// WS sensitivity
		1.0f,	// AD sensitivity
		1.0f,	// RF sensitivity
		10.0f);	// Shift key multiplier

		// Create needed triangles
		ResourceManager* pRM = ResourceManager::getPointer();
		// Cube
		ResourceTriangle* pTri = pRM->addTriangle("cube");
		pTri->addFromFile("data/DevApp/geometry/cube.geom", true);
		// Point lights to show where they are
		pTri = pRM->addTriangle("icosphere_radius_0.01");
		pTri->addFromFile("data/DevApp/geometry/icosphere_radius_0.01.geom", true);
		// Ground plane
		pTri = pRM->addTriangle("groundplane");
		pTri->addFromFile("data/DevApp/geometry/ground_plane_200x200x1.geom", true);

		// Load in a textures
		// Cubes
		pRM->addTexture2D("data/DevApp/textures/cube_BaseColor.png", "data/DevApp/textures/cube_BaseColor.png", true);
		pRM->addTexture2D("data/DevApp/textures/cube_Roughness.png", "data/DevApp/textures/cube_Roughness.png", true);
		pRM->addTexture2D("data/DevApp/textures/cube_Normal.png", "data/DevApp/textures/cube_Normal.png", true);
		// Point light entities
		pRM->addTexture2D("data/DevApp/textures/groundplane.png", "data/DevApp/textures/groundplane.png", true);

		// Create materials
		mSceneManagerSimple.addMaterial("mat_cubes", 0.05f, "data/DevApp/textures/cube_BaseColor.png", "data/DevApp/textures/cube_Roughness.png", 0.75f, "data/DevApp/textures/cube_Normal.png", "X:default_emission");
		mSceneManagerSimple.addMaterial("mat_white", 0.05f, "X:default_white", "X:default_white", 0.5f, "X:default_normal", "X:default_white");
		mSceneManagerSimple.addMaterial(
			"mat_groundplane",						// Material name
			0.25f,									// Ambient strength
			"data/DevApp/textures/groundplane.png",	// Diffuse texture
			"X:default_roughness",					// Roughness texture (grey)
			1.0f,									// Specular strength
			"X:default_normal",						// Normal texture (float)
			"X:default_emission");					// Emission texture (black)

		SMEntityTriangle* pEntity = mSceneManagerSimple.addEntityTriangle("centre", "cube", "mat_cubes");
		pEntity->setWorldPosition(glm::vec3(0.0f, 0.0f, 0.0f));

		for (int i = 0; i < 100; ++i)
		{
			// Temp rotation
			_mvEntityRot[i].x += float(i) * 1.0f;
			_mvEntityRot[i].y += float(i) * 1.0f;
			_mvEntityRot[i].z += float(i) * 1.0f;

			std::string strEntity = "entity_" + std::to_string(i);
			pEntity = mSceneManagerSimple.addEntityTriangle(strEntity, "cube", "mat_cubes");
			pEntity->setWorldPosition(glm::vec3(randf(-19.0f, 19.0f), 0.5f, randf(-19.0f, 19.0f)));
			pEntity->setRotation(_mvEntityRot[i].x, _mvEntityRot[i].y, _mvEntityRot[i].z);
		}
		// Ground plane
		pEntity = mSceneManagerSimple.addEntityTriangle("groundplane", "groundplane", "mat_groundplane");

		// Directional light settings
		mSceneManagerSimple.setDirectionalLightColour(glm::vec3(1, 1, 1));
		mSceneManagerSimple.setDirectionalLightDir(glm::vec3(1, -1, 1));
		mSceneManagerSimple.setDirectionalLightProjection();

		// Set point light settings
		mSceneManagerSimple.miNumPointLights = 0;
		mSceneManagerSimple.mvLightPoint[0].mvPosition = glm::vec3(2.0f, 2.0f, 10.0f);
		mSceneManagerSimple.mvLightPoint[0].mvColour = glm::vec3(0.7f, 0.7f, 0.5f);
		mSceneManagerSimple.mvLightPoint[1].mvPosition = glm::vec3(-2.0f, 2.0f, 2.0f);
		mSceneManagerSimple.mvLightPoint[1].mvColour = glm::vec3(0.0f, 1.0f, 0.0f);
		mSceneManagerSimple.mvLightPoint[2].mvPosition = glm::vec3(2.0f, -2.0f, 2.0f);
		mSceneManagerSimple.mvLightPoint[2].mvColour = glm::vec3(0.0f, 0.0f, 1.0f);
		mSceneManagerSimple.mvLightPoint[3].mvPosition = glm::vec3(2.0f, 2.0f, -2.0f);
		mSceneManagerSimple.mvLightPoint[3].mvColour = glm::vec3(1.0f, 1.0f, 1.0f);
		// Add icosphere_radius_0.01.geom to position of each light
		for (int i = 0; i < mSceneManagerSimple.miNumPointLights; i++)
		{
			std::string strEntity = "point_light_" + std::to_string(i);
			pEntity = mSceneManagerSimple.addEntityTriangle(strEntity, "icosphere_radius_0.01", "mat_white");
			pEntity->setWorldPosition(mSceneManagerSimple.mvLightPoint[i].mvPosition);
		}

		// Now we're done with adding triangle entities.
		// Let's add some line entities
		ResourceLine* pLine = pRM->addLine("line");
		pLine->setDrawModeAsLineList();
		ResourceLine::Vertex lineVertex;
		// + Y
		lineVertex.colour = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
		lineVertex.position = glm::vec3(0.0f, 0.0f, 0.0f);		pLine->addLinePoint(lineVertex);
		lineVertex.position = glm::vec3(0.0f, 1.0f, 0.0f);		pLine->addLinePoint(lineVertex);
		// + X
		lineVertex.colour = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		lineVertex.position = glm::vec3(0.0f, 0.0f, 0.0f);		pLine->addLinePoint(lineVertex);
		lineVertex.position = glm::vec3(1.0f, 0.0f, 0.0f);		pLine->addLinePoint(lineVertex);
		// + Z
		lineVertex.colour = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
		lineVertex.position = glm::vec3(0.0f, 0.0f, 0.0f);		pLine->addLinePoint(lineVertex);
		lineVertex.position = glm::vec3(0.0f, 0.0f, 1.0f);		pLine->addLinePoint(lineVertex);
		pLine->update();

		SMEntityLine* pEntityLine = mSceneManagerSimple.addEntityLine("line", "line", "X:default_white");
		pEntityLine->setWorldPosition(glm::vec3(0.0f, 1.0f, 0.0f));
	}
}