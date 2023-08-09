#include "PCH.h"
#include "demoNeuralNetsState1.h"
#include "demoNeuralNetsGUIstateSelectionUpdate.h"

namespace X
{
	CCritter2::CCritter2()
	{
		// Create neural network of this entity.
		nn.create(2/*Num inputs*/, 2/*num outputs*/, 1/*num layers*/, 10/*num neurons per layer*/);
		vPos.setZero();
		vDir.set(0, 1);
		dMaxTurnRate = kPiOver2 * 0.5;
		dMaxSpeed = 50;
		dFitness = 0;
	}

	void CCritter2::update(std::vector<CFood2>& vecFood, std::vector<CCritterEvil>& vecEvil, float fDeltaSeconds)
	{
		// Compute the inputs
		std::vector<double> inputs;

		// Get direction vector to closest food
		// First, get closest food in the given food vector
		CVector2f vVectorToFood;
		float fVectorMagnitude;
		float fMinMag = 99999;
		int iClosestFoodIndex = 0;
		for (int i = 0; i < vecFood.size(); i++)
		{
			vVectorToFood = vecFood[i].vPos - vPos;
			fVectorMagnitude = vVectorToFood.getMagnitude();
			if (fMinMag > fVectorMagnitude)
			{
				fMinMag = fVectorMagnitude;
				iClosestFoodIndex = i;
			}
		}

		// Get direction vector to closest evil critter
		CVector2f vVectorToEvil;
		fMinMag = 99999;
		int iClosestEvilIndex = 0;
		for (int i = 0; i < vecEvil.size(); i++)
		{
			vVectorToEvil = vecEvil[i].vPos - vPos;
			fVectorMagnitude = vVectorToEvil.getMagnitude();
			if (fMinMag > fVectorMagnitude)
			{
				fMinMag = fVectorMagnitude;
				iClosestEvilIndex = i;
			}
		}

		// Now we have index to closest food.
		// Get direction vector to it
		CVector2f vDirToFood = vecFood[iClosestFoodIndex].vPos - vPos;
		// Distance to food...
		float fDistanceToFood = vDirToFood.getMagnitude();
		// Now normalize the direction to the food
		vDirToFood.normalise();

		// Now we have index to closest evil critter.
		// Get direction vector to it
		CVector2f vDirToEvil = vecEvil[iClosestEvilIndex].vPos - vPos;
		// Distance to evil...
		float fDistanceToEvil = vDirToEvil.getMagnitude();
		// Now normalize the direction to the evil critter
		vDirToEvil.normalise();

		// If we're really close to the food, consider it eaten!
		// Increase this entity's fitness and re-position the food
		if (fDistanceToFood < 5.0f)
		{
			dFitness += 0.5f;
			CVector2f vNewFoodPos = x->pWindow->getDimensions();
			vNewFoodPos *= 0.5f;
			vNewFoodPos.x += randf( 800.0f, 1000.0f);
			vNewFoodPos.y += randf(-1000.0f, 1000.0f);
			vecFood[iClosestFoodIndex].vPos = vNewFoodPos;
			// Update the sprite position
			vecFood[iClosestFoodIndex].pEntity->setPosition(vNewFoodPos);
		}

		// Give brain it's inputs
		double dAngleTowardsFood = vDir.getDot(vDirToFood);
		dAngleTowardsFood *= vDir.getSign(vDirToFood);
		inputs.push_back(dAngleTowardsFood);

		double dAngleTowardsEvil = vDir.getDot(vDirToEvil);
		dAngleTowardsEvil *= vDir.getSign(vDirToEvil);
		inputs.push_back(dAngleTowardsEvil);

		// Update the network
		std::vector<double> outputs = nn.update(inputs);

		// Now we have the outputs from the network, let's apply them to the rotation of the entity
		// and the speed.
		// Compute rotation from NN
		double dRotation = outputs[0] - outputs[1];
		// Clamp rotation to max turn rate;
		clamp(dRotation, -dMaxTurnRate, dMaxTurnRate);
		// Compute speed from NN
		double dSpeed = outputs[0] + outputs[1];
		// Clamp speed to max speed
		double dZero = 0;
		clamp(dSpeed, dZero, dMaxSpeed);

		// Now that we know the amount to rotate and move, apply this to the entity
		vDir.rotate(rad2deg((float)dRotation) * fDeltaSeconds * 70.0f);
		vPos -= vDir * float(dSpeed) * fDeltaSeconds * 200.0f;

		// Update the sprite
		pEntity->setPosition(vPos);
		CVector2f vInvDir = vDir;
		vInvDir.y *= -1.0f;
		vInvDir *= -1.0f;
		pEntity->setFrameBasedOnDirection(vInvDir);
	}

	CCritterEvil::CCritterEvil()
	{
		// Create neural network of this entity.
		nn.create(2/*Num inputs*/, 2/*num outputs*/, 1/*num layers*/, 10/*num neurons per layer*/);
		vPos.setZero();
		vDir.set(0, 1);
		dMaxTurnRate = kPiOver2 * 0.5;
		dMaxSpeed = 50;
		dFitness = 0;
	}

	void CCritterEvil::update(std::vector<CFood2>& vecFood, std::vector<CCritter2>& mvecCritters, float fDeltaSeconds)
	{
		// Compute the inputs
		std::vector<double> inputs;

		// Get direction vector to closest food
		// First, get closest food in the given food vector
		CVector2f vVectorToFood;
		float fVectorMagnitude;
		float fMinMag = 99999;
		int iClosestFoodIndex = 0;
		for (int i = 0; i < vecFood.size(); i++)
		{
			vVectorToFood = vecFood[i].vPos - vPos;
			fVectorMagnitude = vVectorToFood.getMagnitude();
			if (fMinMag > fVectorMagnitude)
			{
				fMinMag = fVectorMagnitude;
				iClosestFoodIndex = i;
			}
		}

		// Now we have index to closest food.
		// Get direction vector to it
		CVector2f vDirToFood = vecFood[iClosestFoodIndex].vPos - vPos;
		// Distance to food...
		float fDistanceToFood = vDirToFood.getMagnitude();
		// Now normalize the direction to the food
		vDirToFood.normalise();

		// If we're really close to the food, consider it eaten!
		// Increase this entity's fitness and re-position the food
//		if (fDistanceToFood < 5.0f)
//		{
//			dFitness += 0.1f;
//			CVector2f vNewFoodPos = x->pWindow->getDimensions();
//			vNewFoodPos *= 0.5f;
//			vNewFoodPos.x += randf(-1000.0f, 1000.0f);
//			vNewFoodPos.y += randf(-1000.0f, 1000.0f);
//			vecFood[iClosestFoodIndex].vPos = vNewFoodPos;
//			// Update the sprite position
//			vecFood[iClosestFoodIndex].pEntity->setPosition(vNewFoodPos);
//		}

		// Give brain it's inputs
		// Give it the angle between it's current direction and the direction to the food.
		double dAngleTowardsFood = vDir.getDot(vDirToFood);
		dAngleTowardsFood *= vDir.getSign(vDirToFood);
		inputs.push_back(dAngleTowardsFood);

		// Now do the same for critters, instead of food
		// Get direction vector to closest critter
		// First, get closest critter in the given critter vector
		CVector2f vVectorToCritter;
		fMinMag = 99999;
		int iClosestCritterIndex = 0;
		for (int i = 0; i < mvecCritters.size(); i++)
		{
			vVectorToCritter = mvecCritters[i].vPos - vPos;
			fVectorMagnitude = vVectorToCritter.getMagnitude();
			if (fMinMag > fVectorMagnitude)
			{
				fMinMag = fVectorMagnitude;
				iClosestCritterIndex = i;
			}
		}
		// Now we have index to closest critter.
		// Get direction vector to it
		CVector2f vDirToCritter = mvecCritters[iClosestCritterIndex].vPos - vPos;
		// Distance to critter...
		float fDistanceToCritter = vDirToCritter.getMagnitude();
		// Now normalize the direction to the food
		vDirToCritter.normalise();

		// If we're really close to the critter, consider it eaten!
		// Increase this entity's fitness and re-position the critter
		if (fDistanceToCritter < 10.0f)
		{
			dFitness += 0.2f;	// Increase this critter's fitness
			mvecCritters[iClosestCritterIndex].dFitness -= 0.2f;	// Decrease eaten critter's fitness

			CVector2f vNewPos = x->pWindow->getDimensions();
			vNewPos *= 0.5f;
			vNewPos.x += randf(-1000.0f, -1000.0f);
			vNewPos.y += randf(-1000.0f, 1000.0f);
			mvecCritters[iClosestCritterIndex].vPos = vNewPos;
			// Update the sprite position
			mvecCritters[iClosestCritterIndex].pEntity->setPosition(vNewPos);
		}

		// Give brain it's inputs
		// Give it the angle between it's current direction and the direction to the food.
		dAngleTowardsFood = vDir.getDot(vDirToCritter);
		dAngleTowardsFood *= vDir.getSign(vDirToCritter);
		inputs.push_back(dAngleTowardsFood);

		// Update the network
		std::vector<double> outputs = nn.update(inputs);

		// Now we have the outputs from the network, let's apply them to the rotation of the entity
		// and the speed.
		// Compute rotation from NN
		double dRotation = outputs[0] - outputs[1];
		// Clamp rotation to max turn rate;
		clamp(dRotation, -dMaxTurnRate, dMaxTurnRate);
		// Compute speed from NN
		double dSpeed = outputs[0] + outputs[1];
		// Clamp speed to max speed
		double dZero = 0;
		clamp(dSpeed, dZero, dMaxSpeed);

		// Now that we know the amount to rotate and move, apply this to the entity
		vDir.rotate(rad2deg((float)dRotation) * fDeltaSeconds * 70.0f);
		vPos -= vDir * float(dSpeed) * fDeltaSeconds * 200.0f;

		// Update the sprite
		pEntity->setPosition(vPos);
		CVector2f vInvDir = vDir;
		vInvDir.y *= -1.0f;
		vInvDir *= -1.0f;
		pEntity->setFrameBasedOnDirection(vInvDir);
	}

	void CDemoNeuralNetsState1::onEnter(void)
	{
		// Setup 2D rendering stuff
		C2DWorld* pWorld = x->p2dRenderer->addWorld("world");
		C2DLayer* pLayer = pWorld->addLayer("floor");
		pLayer = pWorld->addLayer("food");
		pLayer = pWorld->addLayer("entities");
		pLayer = pWorld->addLayer("evil");

		C2DCamera* pCamera = pWorld->addCamera("camera");

		// Get filenames for each of the images for the various texture atlases
		mvecstrImageNamesEntity = StringUtils::getFilesInDir("data/Demos/DemoNeuralNets/images/creature_top_down/");
		mvecstrImageNamesFood = StringUtils::getFilesInDir("data/Demos/DemoNeuralNets/images/food/");
		mvecstrImageNamesTiles = StringUtils::getFilesInDir("data/Demos/DemoNeuralNets/images/floor_tiles/");
		mvecstrImageNamesEntityEvil = StringUtils::getFilesInDir("data/Demos/DemoNeuralNets/images/creature_top_down_evil/");

		// Floor tiles
		int iCnt = 0;
		pLayer = pWorld->getLayer("floor");
		for (int ix = 0; ix < (int)x->pWindow->getWidth() + 1024; ix += 256)
		{
			for (int y = 0; y < (int)x->pWindow->getHeight() + 1024; y += 256)
			{
				std::string strName = std::to_string(iCnt) + "_floorTile"; iCnt++;
				C2DEntity* pEntity = pLayer->addEntity(strName, "tiles");
				pEntity->setImagesMultiple(mvecstrImageNamesTiles);
				pEntity->setPosition(CVector2f(float(ix), float(y)));
				pEntity->setFrameBasedOnAngle(randf(0, 360));
			}
		}

		// Food
		pLayer = pWorld->getLayer("food");
		for (int i = 0; i < 100; i++)
		{
			// Compute intial position of food
			CVector2f vFoodPos = x->pWindow->getDimensions();
			vFoodPos *= 0.5f;
			vFoodPos.x += randf(-1000.0f, 1000.0f);
			vFoodPos.y += randf(-1000.0f, 1000.0f);

			// Create food sprite
			std::string strName = std::to_string(i) + "_entFood";
			C2DEntity* pEntity = pLayer->addEntity(strName, "food");
			pEntity->setImagesMultiple(mvecstrImageNamesFood);
			pEntity->setPosition(vFoodPos);
			pEntity->setScale(0.5f, 0.5f);
			pEntity->setFrameBasedOnAngle(randf(0, 360));	// Set random image

			// Create food object
			CFood2 food;
			food.vPos = vFoodPos;
			// As we're not deleting any sprites, store it's pointer in the food object
			food.pEntity = pEntity;
			mvecFood.push_back(food);
		}

		// Creature top down
		int iNumberOfCritters = 100;
		pLayer = pWorld->getLayer("entities");
		for (int i = 0; i < iNumberOfCritters; i++)
		{
			// Compute initial position of entity
			CVector2f vEntityPos;
			vEntityPos = x->pWindow->getDimensions();
			vEntityPos *= 0.5f;
			vEntityPos.x += randf(-500.0f, 500.0f);
			vEntityPos.y += randf(-500.0f, 500.0f);

			// Create creature sprite
			std::string strName = std::to_string(i) + "_ent";
			C2DEntity* pEntity = pLayer->addEntity(strName, "creature_top_down");
			pEntity->setImagesMultiple(mvecstrImageNamesEntity);
			pEntity->setPosition(vEntityPos);
			pEntity->setScale(0.25f, 0.25f);

			// Create creature object
			CCritter2 entity;
			// As we're not deleting the sprite, store it's pointer so we can update the sprite's position
			entity.pEntity = pEntity;
			entity.vPos = vEntityPos;
			mvecCritters.push_back(entity);
		}

		// Creature top down
		int iNumberOfEvilCritters = 50;
		pLayer = pWorld->getLayer("evil");
		for (int i = 0; i < iNumberOfEvilCritters; i++)
		{
			// Compute initial position of entity
			CVector2f vEntityPos;
			vEntityPos = x->pWindow->getDimensions();
			vEntityPos *= 0.5f;
			vEntityPos.x += randf(-500.0f, 500.0f);
			vEntityPos.y += randf(-500.0f, 500.0f);

			// Create creature sprite
			std::string strName = std::to_string(i) + "_entEvil";
			C2DEntity* pEntity = pLayer->addEntity(strName, "creature_top_down_evil");
			pEntity->setImagesMultiple(mvecstrImageNamesEntityEvil);
			pEntity->setPosition(vEntityPos);
			pEntity->setScale(0.25f, 0.25f);

			// Create creature object
			CCritterEvil evil;
			// As we're not deleting the sprite, store it's pointer so we can update the sprite's position
			evil.pEntity = pEntity;
			evil.vPos = vEntityPos;
			mvecEvil.push_back(evil);
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

		CColour colEvilBest(1.0f, 1.0f, 0.0f, 0.5f);
		CColour colEvilAverage(1.0f, 0.5f, 0.0f, 0.5f);
		CColour colEvilWorst(1.0f, 0.0f, 0.0f, 0.5f);

		CGUIContainer* pCont = x->pGUI->addContainer("state1");
		pCont->setDimensions(950, 300);
		pCont->setPosition(999999, float(x->pWindow->getHeight()) - 380);
		// Add text scroll describing this state.
		std::string strTxt = "State1\n \n";
		strTxt += "This has not one, but two populations training at the same time. The good old green critters hunt for food as normal";
		strTxt += " and the second generation (the red ones), instead of searching for food, search for the green critters and treat those as food instead.\n \n";
		strTxt += "Press the space bar to toggle proceeding to the next generation.\n";
		strTxt += "Press the return key to toggle turbo training speed.\n \n";
		strTxt += "Information...\n";
		strTxt += "Apart from the above changes, everything is the same as in the previous state where each network is given a direction vector to it's food.\n";
		pCont->addTextScroll("state1TextScroll", 610, 0, 340, 300, strTxt);

		pCont->addText("best_fitness", 0, 0, "Best Fitness: 0")->setColour(false, colBest);
		pCont->addText("aver_fitness", 0, 30, "Average Fitness: 0")->setColour(false, colAverage);
		pCont->addText("worst_fitness", 0, 60, "Worst Fitness: 0")->setColour(false, colWorst);
		pCont->addText("total_fitness", 0, 90, "Total Fitness: 0");

		pCont->addText("best_fitness_evil", 300, 0, "Best Fitness: 0")->setColour(false, colEvilBest);
		pCont->addText("aver_fitness_evil", 300, 30, "Average Fitness: 0")->setColour(false, colEvilAverage);
		pCont->addText("worst_fitness_evil", 300, 60, "Worst Fitness: 0")->setColour(false, colEvilWorst);
		pCont->addText("total_fitness_evil", 300, 90, "Total Fitness: 0");

		pCont->addText("next_gen_time", 0, 120, "Time until next gen: ");
		pCont->addText("generation", 0, 150, "Generation: 1");

		// Add line graph for critters
		CGUILineGraph* pLineGraph = pCont->addLineGraph("lineGraph", 0, 180, 300, 120);
		pLineGraph->addDataset("best", colBest)->addValue(0);
		pLineGraph->addDataset("avr", colAverage)->addValue(0);
		pLineGraph->addDataset("worst", colWorst)->addValue(0);

		// Text to show most recent value of line graph
		pCont->addText("linegraph_best_fitness", 270, 190, "0")->setColour(false, colBest);
		pCont->addText("linegraph_aver_fitness", 270, 230, "0")->setColour(false, colAverage);
		pCont->addText("linegraph_worst_fitness", 270, 270, "0")->setColour(false, colWorst);

		// Add line graph for evil critters
		pLineGraph = pCont->addLineGraph("lineGraphEvil", 300, 180, 300, 120);
		pLineGraph->addDataset("best", colBest)->addValue(0);
		pLineGraph->addDataset("avr", colAverage)->addValue(0);
		pLineGraph->addDataset("worst", colWorst)->addValue(0);

		// Text to show most recent value of line graph
		pCont->addText("linegraph_best_fitness_evil", 570, 190, "0")->setColour(false, colEvilBest);
		pCont->addText("linegraph_aver_fitness_evil", 570, 230, "0")->setColour(false, colEvilAverage);
		pCont->addText("linegraph_worst_fitness_evil", 570, 270, "0")->setColour(false, colEvilWorst);

		// Add turbo mode text
		pCont->addText("turbo", 200, 0, "Turbo: off");

		// Add an image of each critter next to the line graphs
		pCont->addImageAnimated("critter", 200, 50, mvecstrImageNamesEntity, 50, 50);
		pCont->addImageAnimated("critterEvil", 500, 50, mvecstrImageNamesEntityEvil, 50, 50);

		// Reset timer
		mTimer.update();
	}

	void CDemoNeuralNetsState1::onExit(void)
	{
		// Remove GUI objects
		x->pGUI->removeContainer("state1");

		// Remove 2D rendering stuff
		x->p2dRenderer->removeWorld("world");

		// Clear food and entities
		mvecFood.clear();
		mvecCritters.clear();
		mvecEvil.clear();
	}

	void CDemoNeuralNetsState1::onActive(CFiniteStateMachine* pFSM)
	{
		mTimer.update();

		CGUIContainer* pCont = x->pGUI->getContainer("state1");
		std::string strTxt;

		// Compute current worst, average and best fitness of populations
		// Critters
		std::vector<double> vecPopFitness;
		for (int i = 0; i < mvecCritters.size(); i++)
		{
			vecPopFitness.push_back(mvecCritters[i].dFitness);
		}
		// Evil critters
		std::vector<double> vecPopFitnessEvil;
		for (int i = 0; i < mvecEvil.size(); i++)
		{
			vecPopFitnessEvil.push_back(mvecEvil[i].dFitness);
		}

		// Compute fitness
		CGeneticsTraining genTraining;
		double dWorstFitness, dAverageFitness, dBestFitness, dTotalFitness;
		double dWorstFitnessEvil, dAverageFitnessEvil, dBestFitnessEvil, dTotalFitnessEvil;
		genTraining.computeFitness(vecPopFitness, dWorstFitness, dAverageFitness, dBestFitness, dTotalFitness);
		genTraining.computeFitness(vecPopFitnessEvil, dWorstFitnessEvil, dAverageFitnessEvil, dBestFitnessEvil, dTotalFitnessEvil);

		// Create a new generation?
		if (_mbPerformNextGenCountdown)
			_mfTimeUntilNewGeneration -= mTimer.getSecondsPast();
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
			// Critters
			std::vector<CGenome> vecOldPopulation;
			for (int i = 0; i < mvecCritters.size(); i++)
			{
				CGenome genome(mvecCritters[i].nn, mvecCritters[i].dFitness);
				vecOldPopulation.push_back(genome);
			}
			// Evil
			std::vector<CGenome> vecOldPopulationEvil;
			for (int i = 0; i < mvecEvil.size(); i++)
			{
				CGenome genome(mvecEvil[i].nn, mvecEvil[i].dFitness);
				vecOldPopulationEvil.push_back(genome);
			}

			// Now we have a vector of CGenomes holding the old population's NNs and their fitness scores, use the
			// CGeneticsTraining class to create a new vector of CGenomes with the new population
			// We need to compute split points from a neural network
			// Critters
			std::vector<int> vecSplitsPoints = mvecCritters[0].nn.calculateSplitPoints();
			CGeneticsTraining genTrainer;
			std::vector<CGenome> vecNewPopulation = genTrainer.createNewGeneration(
				vecOldPopulation,
				vecSplitsPoints,
				0.001/*mutation probability*/,
				0.1/*mutation amount*/,
				0.1/*crossover rate*/);
			// Evil
			std::vector<int> vecSplitsPointsEvil = mvecEvil[0].nn.calculateSplitPoints();
			std::vector<CGenome> vecNewPopulationEvil = genTrainer.createNewGeneration(
				vecOldPopulationEvil,
				vecSplitsPointsEvil,
				0.001/*mutation probability*/,
				0.1/*mutation amount*/,
				0.1/*crossover rate*/);

			// Now we have the new generation of weights for the new population, place them into each of the existing networks
			CVector2f vNewEntityPos;
			// Critters
			for (int i = 0; i < mvecCritters.size(); i++)
			{
				mvecCritters[i].dFitness = 0;	// Also reset their fitness
				mvecCritters[i].nn.replaceWeights(vecNewPopulation[i].vecWeights);	// Replace the weights of the neural network.

				// Give the entity a new random position and direction
				vNewEntityPos = x->pWindow->getDimensions();
				vNewEntityPos *= 0.5f;
				vNewEntityPos.x += randf(-500.0f, 500.0f);
				vNewEntityPos.y += randf(-500.0f, 500.0f);
				mvecCritters[i].vPos = vNewEntityPos;
				mvecCritters[i].vDir.set(0, 1);
			}
			// Evil
			for (int i = 0; i < mvecEvil.size(); i++)
			{
				mvecEvil[i].dFitness = 0;	// Also reset their fitness
				mvecEvil[i].nn.replaceWeights(vecNewPopulationEvil[i].vecWeights);	// Replace the weights of the neural network.

				// Give the entity a new random position and direction
				vNewEntityPos = x->pWindow->getDimensions();
				vNewEntityPos *= 0.5f;
				vNewEntityPos.x += randf(-500.0f, 500.0f);
				vNewEntityPos.y += randf(-500.0f, 500.0f);
				mvecEvil[i].vPos = vNewEntityPos;
				mvecEvil[i].vDir.set(0, 1);
			}
		}	// Done creating a new generation

		// Now update GUI line graph with fitness values
		if (bUpdateGUIDataSet)
		{
			// Critters
			CGUILineGraph* pLineGraph = pCont->getLineGraph("lineGraph");
			CGUILineGraphDataSet* pDS;;
			pDS = pLineGraph->getDataset("best");
			float fTemp = (float)dBestFitness;	if (fTemp < 0)	fTemp = 0;	pDS->addValue(float(fTemp));
			pDS = pLineGraph->getDataset("worst");
			fTemp = (float)dWorstFitness;	if (fTemp < 0)	fTemp = 0;	pDS->addValue(float(fTemp));
			pDS = pLineGraph->getDataset("avr");
			fTemp = (float)dAverageFitness;	if (fTemp < 0)	fTemp = 0;	pDS->addValue(float(fTemp));			// Critters evil
			pLineGraph = pCont->getLineGraph("lineGraphEvil");
			pDS = pLineGraph->getDataset("best");
			fTemp = (float)dBestFitnessEvil;	if (fTemp < 0)	fTemp = 0;	pDS->addValue(float(fTemp));
			pDS = pLineGraph->getDataset("worst");
			fTemp = (float)dWorstFitnessEvil;	if (fTemp < 0)	fTemp = 0;	pDS->addValue(float(fTemp));
			pDS = pLineGraph->getDataset("avr");
			fTemp = (float)dAverageFitnessEvil;	if (fTemp < 0)	fTemp = 0;	pDS->addValue(float(fTemp));

			// Update GUI text showing most recent line graph values
			// Critters
			strTxt.clear();	StringUtils::appendDouble(strTxt, dBestFitness, 1);		pCont->getText("linegraph_best_fitness")->setText(strTxt);
			strTxt.clear();	StringUtils::appendDouble(strTxt, dAverageFitness, 1);	pCont->getText("linegraph_aver_fitness")->setText(strTxt);
			strTxt.clear();	StringUtils::appendDouble(strTxt, dWorstFitness, 1);		pCont->getText("linegraph_worst_fitness")->setText(strTxt);
			// Critters evil
			strTxt.clear();	StringUtils::appendDouble(strTxt, dBestFitnessEvil, 1);		pCont->getText("linegraph_best_fitness_evil")->setText(strTxt);
			strTxt.clear();	StringUtils::appendDouble(strTxt, dAverageFitnessEvil, 1);	pCont->getText("linegraph_aver_fitness_evil")->setText(strTxt);
			strTxt.clear();	StringUtils::appendDouble(strTxt, dWorstFitnessEvil, 1);		pCont->getText("linegraph_worst_fitness_evil")->setText(strTxt);
		}

		// Critters
		strTxt = "Worst Fitness: ";		StringUtils::appendDouble(strTxt, dWorstFitness, 1);		pCont->getText("worst_fitness")->setText(strTxt);
		strTxt = "Average Fitness: ";	StringUtils::appendDouble(strTxt, dAverageFitness, 1);		pCont->getText("aver_fitness")->setText(strTxt);
		strTxt = "Best Fitness : ";		StringUtils::appendDouble(strTxt, dBestFitness, 1);			pCont->getText("best_fitness")->setText(strTxt);
		strTxt = "Total Fitness : ";	StringUtils::appendDouble(strTxt, dTotalFitness, 1);		pCont->getText("total_fitness")->setText(strTxt);
		// Evil critters
		strTxt = "Worst Fitness: ";		StringUtils::appendDouble(strTxt, dWorstFitnessEvil, 1);	pCont->getText("worst_fitness_evil")->setText(strTxt);
		strTxt = "Average Fitness: ";	StringUtils::appendDouble(strTxt, dAverageFitnessEvil, 1);	pCont->getText("aver_fitness_evil")->setText(strTxt);
		strTxt = "Best Fitness : ";		StringUtils::appendDouble(strTxt, dBestFitnessEvil, 1);		pCont->getText("best_fitness_evil")->setText(strTxt);
		strTxt = "Total Fitness : ";	StringUtils::appendDouble(strTxt, dTotalFitnessEvil, 1);	pCont->getText("total_fitness_evil")->setText(strTxt);

		strTxt = "Time until next gen: ";
		StringUtils::appendFloat(strTxt, _mfTimeUntilNewGeneration, 1);
		if (!_mbPerformNextGenCountdown)
			strTxt += " PAUSED.";
		pCont->getText("next_gen_time")->setText(strTxt);

		C2DWorld* pWorld = x->p2dRenderer->getWorld("world");
		C2DCamera* pCamera = pWorld->getCamera("camera");

		// Now move camera
		CVector2f vCamPos = pCamera->getPosition();
		if (x->pInput->key.pressed(KC_UP))
			vCamPos.y -= mTimer.getSecondsPast() * 150.0f;
		if (x->pInput->key.pressed(KC_DOWN))
			vCamPos.y += mTimer.getSecondsPast() * 150.0f;
		if (x->pInput->key.pressed(KC_LEFT))
			vCamPos.x -= mTimer.getSecondsPast() * 150.0f;
		if (x->pInput->key.pressed(KC_RIGHT))
			vCamPos.x += mTimer.getSecondsPast() * 150.0f;
		pCamera->setPosition(vCamPos);

		// Pause/resume next generation
		if (x->pInput->key.once(KC_SPACE))
			_mbPerformNextGenCountdown = !_mbPerformNextGenCountdown;

		// Turbo mode toggle
		if (x->pInput->key.once(KC_RETURN))
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
			float fTimeUntilUpdateScreen = 1.0f / (float)x->pWindow->getRefreshRate();
			float fTimePassed = 0.0f;
			float fTimePerUpdate = 1.0f / 30.0f;
			CTimer timerTurbo;
			timerTurbo.update();

			while (fTimePassed < fTimeUntilUpdateScreen)
			{
				timerTurbo.update();
				fTimePassed += timerTurbo.getSecondsPast();

				// Update each entity
				// Critters
				for (int i = 0; i < mvecCritters.size(); i++)
				{
					mvecCritters[i].update(mvecFood, mvecEvil, fTimePerUpdate);
				}
				// Critters evil
				for (int i = 0; i < mvecEvil.size(); i++)
				{
					mvecEvil[i].update(mvecFood, mvecCritters, fTimePerUpdate);
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
			// Critters
			for (int i = 0; i < mvecCritters.size(); i++)
			{
				mvecCritters[i].update(mvecFood, mvecEvil, mTimer.getSecondsPast());
			}
			// Critters evil
			for (int i = 0; i < mvecEvil.size(); i++)
			{
				mvecEvil[i].update(mvecFood, mvecCritters, mTimer.getSecondsPast());
			}
		}

		// Deal with state switching from the buttons being clicked.
		DemoNNGUIstateSelectionUpdate(pFSM);
	}
}