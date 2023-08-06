#include "application.h"
#include "resource.h"

namespace X
{
	void CApplication::initOnce(void)
	{
		// Use the resource loading screen
		SCResourceLoadingScreen* pLS = SCResourceLoadingScreen::getPointer();
		pLS->onInit(0);

		// Set window title bar text and set icon
		SCWindow* pWindow = SCWindow::getPointer();
		pWindow->setText("X: DemoQuadTree. F1: Toggle fullscreen. F2: Toggle Vsync. F3: Toggle statistics window.");
		pWindow->setIcon(IDI_ICON1);
		// Set mouse cursor
		SCInputManager::getPointer()->mouse.setMouseCursorImage("data/X/cursors/new_default.ani");

		// Create quad tree
		CVector2f vQuadTreeMax((float)pWindow->getWidth() - 50.0f, (float)pWindow->getHeight() - 50.0f);
		CVector2f vQuadTreeMin(50, 50);
		_mpQuadTree = new CQuadTree(10, 2);
		ThrowIfFalse(_mpQuadTree, "CApplication::initOnce() failed. Unable to allocate memory for CQuadTree.");

		// Add some entities
		_muiNumEntities = 0;
		for (int i = 0; i < (int)_muiNumEntities; i++)
		{
			std::string strName = "Ent: " + std::to_string(i);
			int iPosX = randInt(0,  50);
			int iPosY = randInt(50, (int)pWindow->getHeight() - 50);
			CVector2f vPos;
			vPos.x = (float)iPosX;
			vPos.y = (float)iPosY;
			_mvecEntityPos.push_back(vPos);
			_mpQuadTree->addEntity(strName, iPosX, iPosY);

			// Compute dir
			vPos.x = randf(-1.0f, 1.0f);
			vPos.y = randf(-1.0f, 1.0f);
			vPos.normalise();
			_mvecEntityDir.push_back(vPos);
		}

		// Range finder defaults
		_mfRange = 200.0f;
		_mvRangePos.set(0, 0);

		// Setup GUI
		SCGUIManager* pGUIMan = SCGUIManager::getPointer();
		CGUIContainer* pCont;
		// Show statistics window and obtain it's dimensions
		pCont = pGUIMan->getContainer("X:Default:Statistics");
		pCont->setVisible(true);
		CVector2f vStatsWindowDims = pCont->getDimensions();

		// Window for application to show various information
		pCont = pGUIMan->addContainer("GUI");
		pCont->setDimensions(450, 670);
		pCont->setPosition(999999.0f, vStatsWindowDims.y + 100.0f);
		std::string strTxt = "Welcome to Demo Quad Tree.\n \n";
		strTxt += "First, the controls...\nThe W, S, A, D keys move the camera.\nThe R and F keys zoom the camera.\n";
		strTxt += "The return key adds entities at the centre of the quad tree and the backspace key removes entities.\n";
		strTxt += "The Y, H, G and J keys move the position of the position we use to get a list of entities within a certain range (They are coloured orange)\n";
		strTxt += "The I and K keys increase/decrease the range from this position.\n \n";
		strTxt += "Details...\n";
		strTxt += "A quad tree gives us fast retrieval of entities which are within a specified ";
		strTxt += "range of a given position aswell as fast retrieval of entities which are within a given area of space.\n";
		strTxt += "This is great for rendering only visible entities and also for getting a list of entities which ";
		strTxt += "can be used to perform collision detection only upon entities which are closeby, instead of having ";
		strTxt += "to perform collision for ALL entities within a world.\n";
		strTxt += "A quad tree begins with an initial root 2D space (node) with which to insert uniquely named entities into, specifying their position.\n";
		strTxt += "The quad tree has a maximum number of entities which can be inserted into a node and once this number ";
		strTxt += "is reached, the node is split into one or more (up to four) child nodes and the entities are removed ";
		strTxt += "from the parent node and into the child nodes.\n";
		strTxt += "The position of entities determines which child nodes need to be created and then the entity inserted into.\n";
		strTxt += "Each of the child nodes have half the dimensions of their parent node.\n";
		strTxt += "This is a recursive process, so smaller and smaller child nodes of child nodes and so on are created until ";
		strTxt += "a \"max node depth\" value is reached, then what happens is the child nodes no longer get any more child nodes ";
		strTxt += "and the entities inserted, ignore the maximum number of entities per node value.\n";
		strTxt += "The max node depth is computed upon the tree's construction based upon it's dimensions and then again if the ";
		strTxt += "root node of the tree gets resized (due to insertion of an entity outside of the root node's region).\n";
		strTxt += "The reason we have a max node depth is because, as the nodes get divided, their dimensions get smaller and ";
		strTxt += "smaller, until they reach a dimension of 1x1 and if they were to become zero dimensions, no entities would ";
		strTxt += "fit in them and the algorithm would enter an infinite recursive loop, forever creating useless zero sized ";
		strTxt += "child nodes and cause a stack overflow.\n";
		strTxt += "The initial region covered by the root node has to be a multiple of 2, otherwise division of the child nodes ";
		strTxt += "may cause certain position to not be covered (For example, if the initial region was (-3, +3), when ";
		strTxt += "creating the child nodes, their dims would be (-1, +1) meaning half the region of the parent node would not ";
		strTxt += "be covered and make it so an entity within the uncovered area would not fit.\n";
		strTxt += "The intial region is set to (-1024, +1024)";

		pCont->addTextScroll("TextScroll", 0, 0, 450, 500, strTxt);
		pCont->addText("CameraPos", 0.0f, 520.0f, "CameraPos: ");
		pCont->addText("NumEntities", 0, 550, "NumEntities: ");
		pCont->addText("NodeDepth", 0, 580, "");
		pCont->addText("RangePos", 0, 610, "");
		pCont->addText("RangeRange", 0, 640, "");

		// Camera initial position
		_mvCameraPosition.set(0.0f, 0.0f, 4000.0f);

		// End of loading screen
		pLS->onInitEnd();
	}

	void CApplication::onStart(void)
	{
	}

	void CApplication::onStop(void)
	{
	}

	bool CApplication::onUpdate(void)
	{
		SCInputManager* pInput = SCInputManager::getPointer();

		// Timer delta
		timer.update();

		// Update some entities by moving them based on their direction
		SCWindow* pWindow = SCWindow::getPointer();
		for (int i = 0; i < (int)_muiNumEntities; i++)
		{
			_mvecEntityPos[i].x += timer.getSecondsPast() * 150.0f * _mvecEntityDir[i].x;
			_mvecEntityPos[i].y += timer.getSecondsPast() * 150.0f * _mvecEntityDir[i].y;
			if (_mvecEntityPos[i].x > 2000.0f)
				_mvecEntityPos[i].x = -2000.0f;
			if (_mvecEntityPos[i].y > 2000.0f)
				_mvecEntityPos[i].y = -2000.0f;
			if (_mvecEntityPos[i].x < -2000.0f)
				_mvecEntityPos[i].x =  2000.0f;
			if (_mvecEntityPos[i].y < -2000.0f)
				_mvecEntityPos[i].y =  2000.0f;

			std::string strName = "Ent: " + std::to_string(i);
			_mpQuadTree->setEntityPosition(strName, (int)_mvecEntityPos[i].x, (int)_mvecEntityPos[i].y);
		}

		// Add entities via enter key
		if (pInput->key.pressed(KC_RETURN))
		{
			// Compute name
			std::string strName = "Ent: " + std::to_string(_muiNumEntities);
			_muiNumEntities++;

			// Compute direction
			CVector2f vDir;
			vDir.x = randf(-1.0f, 1.0f);
			vDir.y = randf(-1.0f, 1.0f);
			vDir.normalise();
			_mvecEntityDir.push_back(vDir);

			// Finally, add position to vector and add enitity to quad tree
			CVector2f v2Pos;
			_mvecEntityPos.push_back(v2Pos);
			_mvecEntityName.push_back(strName);
			_mpQuadTree->addEntity(strName, (int)v2Pos.x, (int)v2Pos.y);
		}

		// Remove entities via the backspace key
		if (pInput->key.pressed(KC_BACK))
		{
			if (_muiNumEntities)
			{
				_muiNumEntities--;
				std::string strName = _mvecEntityName[_muiNumEntities];
				_mvecEntityName.pop_back();
				_mvecEntityDir.pop_back();
				_mvecEntityPos.pop_back();
				_mpQuadTree->removeEntity(strName);
			}
		}

		// Move range finder position
		if (pInput->key.pressed(KC_Y))
			_mvRangePos.y += timer.getSecondsPast() * 750.0f;
		if (pInput->key.pressed(KC_H))
			_mvRangePos.y -= timer.getSecondsPast() * 750.0f;
		if (pInput->key.pressed(KC_G))
			_mvRangePos.x -= timer.getSecondsPast() * 750.0f;
		if (pInput->key.pressed(KC_J))
			_mvRangePos.x += timer.getSecondsPast() * 750.0f;
		// Update range finder range
		if (pInput->key.pressed(KC_I))
			_mfRange += timer.getSecondsPast() * 100.0f;
		if (pInput->key.pressed(KC_K))
		{
			_mfRange -= timer.getSecondsPast() * 100.0f;
			if (_mfRange < 1.0f)
				_mfRange = 1.0f;
		}

		// Obtain all entities found within a certain range of the range finder position and set them to orange
		CColour colWhite(1.0f, 1.0f, 1.0f, 1.0f);
		CColour colOrange(1.0f, 0.75f, 0.0f, 1.0f);
		_mpQuadTree->debugSetAllEntitiesColour(colWhite);
		std::vector<CQuadTreeEntity*> vecEntitiesInRange = _mpQuadTree->getEntitiesWithinRange(int(_mvRangePos.x), int(_mvRangePos.y), int(_mfRange));
		for (int i = 0; i < vecEntitiesInRange.size(); i++)
		{
			vecEntitiesInRange[i]->debugSetColour(colOrange);
		}

		// Debug render the quad tree
		_mpQuadTree->debugRender(_mvCameraPosition, true, true, 8, 6);

		// Render the range finder
		_renderRangeFinder();

		// Update GUI
		SCGUIManager* pGUIMan = SCGUIManager::getPointer();
		CGUIContainer* pCont = pGUIMan->getContainer("GUI");
		std::string strGUITxt = "CameraPos: ";
		StringUtils::appendCVector3f(strGUITxt, _mvCameraPosition, 2, ", ");
		pCont->getText("CameraPos")->setText(strGUITxt);
		strGUITxt = "NumEntities: " + std::to_string(_muiNumEntities);
		pCont->getText("NumEntities")->setText(strGUITxt);
		strGUITxt = "Node Depth cur/max: " + std::to_string(_mpQuadTree->getNodeDepthCurrent()) + " : " + std::to_string(_mpQuadTree->getNodeDepthMax());
		pCont->getText("NodeDepth")->setText(strGUITxt);
		strGUITxt = "RangeFinder Pos: ";
		StringUtils::appendFloat(strGUITxt, _mvRangePos.x, 1);
		strGUITxt += ", ";
		StringUtils::appendFloat(strGUITxt, _mvRangePos.y, 1);
		pCont->getText("RangePos")->setText(strGUITxt);
		strGUITxt = "RangeFinder Range: ";
		StringUtils::appendFloat(strGUITxt, _mfRange, 1);
		pCont->getText("RangeRange")->setText(strGUITxt);

		// Move camera
		if (pInput->key.pressed(KC_W))
			_mvCameraPosition.y += timer.getSecondsPast() * 1000.0f;
		if (pInput->key.pressed(KC_S))
			_mvCameraPosition.y -= timer.getSecondsPast() * 1000.0f;
		if (pInput->key.pressed(KC_A))
			_mvCameraPosition.x -= timer.getSecondsPast() * 1000.0f;
		if (pInput->key.pressed(KC_D))
			_mvCameraPosition.x += timer.getSecondsPast() * 1000.0f;
		if (pInput->key.pressed(KC_R))
			_mvCameraPosition.z -= timer.getSecondsPast() * 1000.0f;
		if (pInput->key.pressed(KC_F))
			_mvCameraPosition.z += timer.getSecondsPast() * 1000.0f;

		// Escape key to exit
		if (pInput->key.pressed(KC_ESCAPE))
			return false;

		// Toggle fullscreen
		if (pInput->key.once(KC_F1))
			SCWindow::getPointer()->toggleFullscreen();

		// Toggle vertical sync
		if (pInput->key.once(KC_F2))
			SCWindow::getPointer()->setVsync(!SCWindow::getPointer()->getVSyncEnabled());

		// Toggle statistics window
		if (pInput->key.once(KC_F3))
		{
			CGUIContainer* pStatsCont = SCGUIManager::getPointer()->getContainer("X:Default:Statistics");
			pStatsCont->setVisible(!pStatsCont->getVisible());
		}
		return true;
	}

	void CApplication::_renderRangeFinder(void)
	{
		// Obtain required resources needed to render the node's as lines.
		SCResourceManager* pRM = SCResourceManager::getPointer();
		CResourceVertexBufferLine* pLine = pRM->getVertexBufferLine("X:default");
		CResourceShader* pShader = pRM->getShader("X:line");
		CResourceTexture2DFromFile* pTexture = pRM->getTexture2DFromFile("X:default_white");

		// Setup orthographic projection matrix
		CMatrix matrixWorld;
		CMatrix matrixView;
		CVector3f vTargetPos = _mvCameraPosition;
		vTargetPos.z -= 100.0f;
		matrixView.setViewLookat(_mvCameraPosition, vTargetPos, CVector3f(0.0f, 1.0f, 0.0f));

		CMatrix matrixProjection;
		matrixProjection.setProjectionPerspective(55.0f, 1.0f, 10000.0f);

		// Bind shader and set uniforms
		pShader->bind();
		pShader->setMat4("matrixWorld", matrixWorld);
		pShader->setMat4("matrixView", matrixView);
		pShader->setMat4("matrixProjection", matrixProjection);

		// Tell OpenGL, for each sampler, to which texture unit it belongs to
		pShader->setInt("texture0_colour", 0);

		// And bind the texture...
		pTexture->bind();

		// Set the line vertex buffer rendering mode
		pLine->setDrawModeAsLineList();
		pLine->removeGeom();

		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Setup vertex we'll be using to render with
		CResourceVertexBufferLine::Vertex vertex;
		vertex.colour.set(1.0f, 0.75f, 0.0f, 1.0f);

		// Add quad representing the range finder
		// Bottom
		vertex.position.set(_mvRangePos.x - _mfRange, _mvRangePos.y - _mfRange, 0.0f);	pLine->addLinePoint(vertex);
		vertex.position.set(_mvRangePos.x + _mfRange, _mvRangePos.y - _mfRange, 0.0f);	pLine->addLinePoint(vertex);
		// Top
		vertex.position.set(_mvRangePos.x - _mfRange, _mvRangePos.y + _mfRange, 0.0f);	pLine->addLinePoint(vertex);
		vertex.position.set(_mvRangePos.x + _mfRange, _mvRangePos.y + _mfRange, 0.0f);	pLine->addLinePoint(vertex);
		// Left
		vertex.position.set(_mvRangePos.x - _mfRange, _mvRangePos.y - _mfRange, 0.0f);	pLine->addLinePoint(vertex);
		vertex.position.set(_mvRangePos.x - _mfRange, _mvRangePos.y + _mfRange, 0.0f);	pLine->addLinePoint(vertex);
		// Right
		vertex.position.set(_mvRangePos.x + _mfRange, _mvRangePos.y - _mfRange, 0.0f);	pLine->addLinePoint(vertex);
		vertex.position.set(_mvRangePos.x + _mfRange, _mvRangePos.y + _mfRange, 0.0f);	pLine->addLinePoint(vertex);

		// Send geometry to be rendered
		pLine->update();
		pLine->render();

		// Cleanup
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		pTexture->unbind();
		pShader->unbind();
	}
}