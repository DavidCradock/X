#include "PCH.h"
#include "state3.h"
#include "GUIstateSelectionUpdate.h"

namespace X
{
	CNNEntity::CNNEntity()
	{
		// Create neural network of this entity.
		nn.create(1/*Num inputs*/, 2/*num outputs*/, 1/*num layers*/, 10/*num neurons per layer*/);
		vPos.setZero();
		vDir.set(0, 1);
		dMaxTurnRate = kPiOver2 * 0.1;
		dMaxSpeed = 0.5f;
		dFitness = 0;
	}

	void CNNEntity::update(float fDeltaSeconds)
	{
		// Compute the inputs
		std::vector<double> inputs;

		// Give brain it's inputs
		inputs.push_back(0.0f);

		// Update the network
		std::vector<double> outputs = nn.update(inputs);

		// Now we have the outputs from the network, let's apply them

		// Compute rotation from NN
		double dRotation = outputs[0] - outputs[1];

		// Update the entity
		pEntity->setWorldPosition(CVector3f(vPos.x, vPos.y, 0.0f));
		CVector2f vInvDir = vDir;
		vInvDir.y *= -1.0f;
		vInvDir *= -1.0f;
		float fAngleRadians = vInvDir.getAngleRadiansSigned(CVector2f(0.0f, 1.0f));
		pEntity->setRotation(CVector3f(0.0f, 0.0f, 1.0f), fAngleRadians);
	}

	void CState3::onEnter(void)
	{
		// Create materials
		CSMMaterial* pMat;
		// Critter material
		pMat = _mSM.addMaterial("entity");
//		pMat->mstrTextureNameDiffuse = "critter_diffuse";
		// Background material
		pMat = _mSM.addMaterial("background");
		pMat->setAmbientStrength(0.25f);
		pMat->setSpecularStrength(1.0f);

		// Entities
		CSMEntityTriangle* pEnt;
		// Background
		_mSM.addEntityTriangle("background", "background", "background");

		// Set camera position
		CSMCamera* pCamera = _mSM.getCamera(0);
		pCamera->setProjectionAsPerspective(70.0f);
		pCamera->setViewAsLookat(CVector3f(25.0f, -25.0f, 45.0f), CVector3f(0.0f, 0.0f, 0.0f), CVector3f(0.0f, 0.0f, 1.0f));

		// Set light position
		CVector3f vLightDir(0.5f, -0.5f, -1.0f);
		vLightDir.normalise();
		_mSM.setDirectionalLightDir(vLightDir);

		// Entities
		int iNumberOfEntities = 100;
		for (int i = 0; i < iNumberOfEntities; i++)
		{
			// Compute initial position of entity
			CVector2f vEntityPos;
			vEntityPos.x += randf(-25.0f, 25.0f);
			vEntityPos.y += randf(-25.0f, 25.0f);

			// Create creature entity
			std::string strName = std::to_string(i) + "_ent";
			pEnt = _mSM.addEntityTriangle(strName, "critter", "entity");
			pEnt->setWorldPosition(CVector3f(vEntityPos.x, vEntityPos.y, 0.3f));

			// Create creature object
			CNNEntity entity;
			// As we're not deleting the entity, store it's pointer so we can update the sprite's position
			entity.pEntity = pEnt;
			entity.vPos = vEntityPos;
			mvecEntities.push_back(entity);
		}

		// Set time until a new generation is created
		_mfTimeUntilNewGeneration = 30.0f;
		_mbPerformNextGenCountdown = true;
		_miGenerationNumber = 1;

		// Set turbo mode to initially off
		_mbTurboMode = false;

		// Setup the GUI for this state

		// Create colours used for best/average/worst text and line graphs.
		CColour colBest(1.0f, 1.0f, 0.0f, 1.0f);
		CColour colAverage(1.0f, 0.5f, 0.0f, 1.0f);
		CColour colWorst(1.0f, 0.0f, 0.0f, 1.0f);

		SCGUIManager* pGUI = SCGUIManager::getPointer();
		CGUIContainer* pCont = pGUI->addContainer("state3");
		pCont->setDimensions(650, 300);
		pCont->setPosition(99999, 99999);
		// Add text scroll describing this state.
		std::string strTxt = "State3\n \n";
		strTxt += "This state shows training of a neural network where???.\n \n";
		strTxt += "Press the space bar to toggle proceeding to the next generation.\n";
		strTxt += "Press the return key to toggle turbo training speed.\n \n";
		pCont->addTextScroll("state3TextScroll", 310, 0, 340, 300, strTxt);
		pCont->addText("best_fitness", 0, 0, "Best Fitness: 0")->setColour(false, colBest);
		pCont->addText("aver_fitness", 0, 30, "Average Fitness: 0")->setColour(false, colAverage);
		pCont->addText("worst_fitness", 0, 60, "Worst Fitness: 0")->setColour(false, colWorst);
		pCont->addText("total_fitness", 0, 90, "Total Fitness: 0");
		pCont->addText("next_gen_time", 0, 120, "Time until next gen: ");
		pCont->addText("generation", 0, 150, "Generation: 1");
		// Add line graph
		CGUILineGraph* pLineGraph = pCont->addLineGraph("lineGraph", 0, 180, 300, 120);
		pLineGraph->addDataset("best", colBest)->addValue(0);
		pLineGraph->addDataset("avr", colAverage)->addValue(0);
		pLineGraph->addDataset("worst", colWorst)->addValue(0);
		// Text to show most recent value of line graph
		pCont->addText("linegraph_best_fitness", 270, 190, "0")->setColour(false, colBest);
		pCont->addText("linegraph_aver_fitness", 270, 230, "0")->setColour(false, colAverage);
		pCont->addText("linegraph_worst_fitness", 270, 270, "0")->setColour(false, colWorst);
		// Add turbo mode text
		pCont->addText("turbo", 200, 0, "Turbo: off");

		// Reset timer
		_mTimer.update();

	}

	void CState3::onExit(void)
	{
		// Remove GUI objects
		SCGUIManager* pGUI = SCGUIManager::getPointer();
		pGUI->removeContainer("state3");

		// Remove created resources
		SCResourceManager* pRM = SCResourceManager::getPointer();

		// Remove all materials
		_mSM.removeAllMaterials();

		// Remove all entities
		_mSM.removeAllEnititiesTriangle();

		// Clear entities
		mvecEntities.clear();
	}

	void CState3::onActive(CFiniteStateMachine* pFSM)
	{
		_mTimer.update();

		// Render everything
		_render();

		SCGUIManager* pGUI = SCGUIManager::getPointer();
		CGUIContainer* pCont = pGUI->getContainer("state3");
		std::string strTxt;

		// Compute current worst, average and best fitness of population
		std::vector<double> vecPopFitness;
		for (int i = 0; i < mvecEntities.size(); i++)
		{
			vecPopFitness.push_back(mvecEntities[i].dFitness);
		}
		CGeneticsTraining genTraining;
		double dWorstFitness, dAverageFitness, dBestFitness, dTotalFitness;
		genTraining.computeFitness(vecPopFitness, dWorstFitness, dAverageFitness, dBestFitness, dTotalFitness);

		// Create a new generation?
		if (_mbPerformNextGenCountdown)
			_mfTimeUntilNewGeneration -= _mTimer.getSecondsPast();
		bool bUpdateGUIDataSet = false;	// Used to update the GUI line graph data sets if a new generation was created
		if (_mfTimeUntilNewGeneration <= 0.0f)
		{
			bUpdateGUIDataSet = true;

			// Yes, let's
			_mfTimeUntilNewGeneration = 30.0f;
			_miGenerationNumber++;
			strTxt = "Generation: " + std::to_string(_miGenerationNumber);
			pCont->getText("generation")->setText(strTxt);

			// OK, here we generate an entirely new generation based on the current one's fitness scores
			// First create a vector of CGenome objects which the genetic algorithm training class can use to generate a new population
			std::vector<CGenome> vecOldPopulation;
			for (int i = 0; i < mvecEntities.size(); i++)
			{
				CGenome genome(mvecEntities[i].nn, mvecEntities[i].dFitness);
				vecOldPopulation.push_back(genome);
			}
			// Now we have a vector of CGenomes holding the old population's NNs and their fitness scores, use the
			// CGeneticsTraining class to create a new vector of CGenomes with the new population
			// We need to compute split points from a neural network
			std::vector<int> vecSplitsPoints = mvecEntities[0].nn.calculateSplitPoints();
			CGeneticsTraining genTrainer;
			std::vector<CGenome> vecNewPopulation = genTrainer.createNewGeneration(
				vecOldPopulation,
				vecSplitsPoints,
				0.001/*mutation probability*/,
				0.1/*mutation amount*/,
				0.1/*crossover rate*/);

			// Now we have the new generation of weights for the new population, place them into each of the existing networks
			SCWindow* pWindow = SCWindow::getPointer();
			CVector2f vNewEntityPos;
			for (int i = 0; i < mvecEntities.size(); i++)
			{
				mvecEntities[i].dFitness = 0;	// Also reset their fitness
				mvecEntities[i].nn.replaceWeights(vecNewPopulation[i].vecWeights);	// Replace the weights of the neural network.

				// Give the entity a new random position and direction
				vNewEntityPos.x = randf(-25.0f, 25.0f);
				vNewEntityPos.y = randf(-25.0f, 25.0f);
				mvecEntities[i].vPos = vNewEntityPos;
				mvecEntities[i].vDir.set(0, 1);
				mvecEntities[i].pEntity->setWorldPosition(CVector3f(vNewEntityPos.x, vNewEntityPos.y, 0.3f));
			}
		}	// Done creating a new generation

		// Now update GUI line graph with fitness values
		if (bUpdateGUIDataSet)
		{
			CGUILineGraph* pLineGraph = pCont->getLineGraph("lineGraph");
			CGUILineGraphDataSet* pDS = pLineGraph->getDataset("best");
			pDS->addValue(float(dBestFitness));
			pDS = pLineGraph->getDataset("worst");
			pDS->addValue(float(dWorstFitness));
			pDS = pLineGraph->getDataset("avr");
			pDS->addValue(float(dAverageFitness));

			// Update GUI text showing most recent line graph values
			strTxt.clear();	StringUtils::appendDouble(strTxt, dBestFitness, 1);		pCont->getText("linegraph_best_fitness")->setText(strTxt);
			strTxt.clear();	StringUtils::appendDouble(strTxt, dAverageFitness, 1);	pCont->getText("linegraph_aver_fitness")->setText(strTxt);
			strTxt.clear();	StringUtils::appendDouble(strTxt, dWorstFitness, 1);		pCont->getText("linegraph_worst_fitness")->setText(strTxt);
		}

		strTxt = "Worst Fitness: ";		StringUtils::appendDouble(strTxt, dWorstFitness, 1);		pCont->getText("worst_fitness")->setText(strTxt);
		strTxt = "Average Fitness: ";	StringUtils::appendDouble(strTxt, dAverageFitness, 1);	pCont->getText("aver_fitness")->setText(strTxt);
		strTxt = "Best Fitness : ";		StringUtils::appendDouble(strTxt, dBestFitness, 1);		pCont->getText("best_fitness")->setText(strTxt);
		strTxt = "Total Fitness : ";	StringUtils::appendDouble(strTxt, dTotalFitness, 1);		pCont->getText("total_fitness")->setText(strTxt);
		strTxt = "Time until next gen: ";
		StringUtils::appendFloat(strTxt, _mfTimeUntilNewGeneration, 1);
		if (!_mbPerformNextGenCountdown)
			strTxt += " PAUSED.";
		pCont->getText("next_gen_time")->setText(strTxt);

		SCInputManager* pInput = SCInputManager::getPointer();

		// Pause/resume next generation
		if (pInput->key.once(KC_SPACE))
			_mbPerformNextGenCountdown = !_mbPerformNextGenCountdown;

		// Turbo mode toggle
		if (pInput->key.once(KC_RETURN))
		{
			_mbTurboMode = !_mbTurboMode;
			CGUIText* pTurboTxt = pCont->getText("turbo");
			if (_mbTurboMode)
			{
				pTurboTxt->setText("Turbo: ON");
				pTurboTxt->setColour(false, CColour(1.0f, 1.0f, 1.0f, 1.0f));
			}
			else
			{
				pTurboTxt->setText("Turbo: off");
				pTurboTxt->setColour(false, CColour(0.8f, 0.8f, 0.8f, 0.5f));
			}
		}

		// Here's were we update all the entities and their neural networks.
		// We deal with "turbo mode" here.
		// Compute maximum amount of time that's allowed to pass before we need to update the screen
		if (_mbTurboMode)
		{
			SCWindow* pWindow = SCWindow::getPointer();
			float fTimeUntilUpdateScreen = 1.0f / (float)pWindow->getRefreshRate();
			float fTimePassed = 0.0f;
			float fTimePerUpdate = 1.0f / 30.0f;
			CTimer timerTurbo;
			timerTurbo.update();

			while (fTimePassed < fTimeUntilUpdateScreen)
			{
				timerTurbo.update();
				fTimePassed += timerTurbo.getSecondsPast();

				// Update each entity
				for (int i = 0; i < mvecEntities.size(); i++)
				{
					mvecEntities[i].update(fTimePerUpdate);
				}

				// Deal with time before new generation
				_mfTimeUntilNewGeneration -= fTimePerUpdate;
				if (_mfTimeUntilNewGeneration <= 0.0f)
					break;
			}
		}
		else // Normal mode, just update the entities as normal
		{
			// Update each entity
			for (int i = 0; i < mvecEntities.size(); i++)
			{
				mvecEntities[i].update(_mTimer.getSecondsPast());
			}
		}

		// Deal with state switching from the buttons being clicked.
		GUIstateSelectionUpdate(pFSM);
	}

	void CState3::_render(void)
	{
		// Update shadows
		CVector3f vLightDir(0.5f, -0.5f, -1.0f);
		vLightDir.normalise();
		CVector3f vLightPos(vLightDir.x * -25.0f, vLightDir.y * -25.0f, vLightDir.z * -25.0f);
		_mSM.setShadowMatrixManual(
			60.0f, 60.0f, // Projection dims
			1.0f, 40.0f,	// Near and far
			vLightPos);	// Position

		// Render the 3D scene manager
		_mSM.render();

		// Render shadow map
		glEnable(GL_BLEND);
		SCResourceManager::getPointer()->getDepthbuffer("X:shadows")->renderTo2DQuad(SCWindow::getPointer()->getWidth() - 512, 0, 512, 512, CColour(1, 1, 1, 0.75f));
		glDisable(GL_BLEND);
	}
}