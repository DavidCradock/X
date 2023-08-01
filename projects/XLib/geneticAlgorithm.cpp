#include "PCH.h"
#include "geneticAlgorithm.h"
#include "utilities.h"
#include "log.h"

namespace X
{
	CGenome::CGenome()
	{
		dFitness = 0;
	}

	CGenome::CGenome(std::vector<double>& vecNeuralNetworkWeights, double dNetworkFitness)
	{
		dFitness = dNetworkFitness;
		vecWeights = vecNeuralNetworkWeights;
	}

	CGenome::CGenome(const CNeuralNetworkFeedForward& neuralNetwork, double dNetworkFitness)
	{
		dFitness = dNetworkFitness;
		vecWeights = neuralNetwork.getNeuronWeights();
	}

	void CGenome::set(std::vector<double>& vecNeuralNetworkWeights, double dNetworkFitness)
	{
		dFitness = dNetworkFitness;
		vecWeights = vecNeuralNetworkWeights;
	}

	void CGenome::set(const CNeuralNetworkFeedForward& neuralNetwork, double dNetworkFitness)
	{
		dFitness = dNetworkFitness;
		vecWeights = neuralNetwork.getNeuronWeights();
	}

	CGenome CGeneticsTraining::rouletteWheelSelection(const std::vector<CGenome>& vecPopulation) const
	{
		// Make sure the vector contains stuff
		ThrowIfFalse(vecPopulation.size(), "CGeneticsTraining::rouletteWheelSelection() failed as the given CGenome vector was empty.");

		// Compute total fitness of all the elements in the vector
		double dTotalFitness = 0;
		for (unsigned int ui = 0; ui < vecPopulation.size(); ui++)
		{
			dTotalFitness += vecPopulation[ui].dFitness;
		}

		// Now compute a random value between 0 and dTotalFitness.
		// Think of this as a portion of the entire wheel.
		double dPortion = randDouble() * dTotalFitness;

		// Now go through the vector of genomes, adding their fitness scores as we go
		// and if the sub total is greater than the above dTotalFitness,
		// we've found our index.
		double dSubTotal = 0;
		CGenome selectedGenome = vecPopulation[0];
		for (unsigned int ui = 0; ui < vecPopulation.size(); ui++)
		{
			dSubTotal += vecPopulation[ui].dFitness;
			if (dSubTotal > dPortion)
			{
				selectedGenome = vecPopulation[ui];
				break;
			}
		}
		return selectedGenome;
	}

	void CGeneticsTraining::computeFitness(const std::vector<double>& vecPopulationFitness, double& dWorstFitness, double& dAverageFitness, double& dBestFitness, double& dTotalFitness) const
	{
		dWorstFitness = 999999999999999;
		dBestFitness = -999999999999999;
		dTotalFitness = 0;
		for (unsigned int ui = 0; ui < vecPopulationFitness.size(); ui++)
		{
			// Total fitness
			dTotalFitness += vecPopulationFitness[ui];

			// Worst fitness
			if (dWorstFitness > vecPopulationFitness[ui])
				dWorstFitness = vecPopulationFitness[ui];

			// Best fitness
			if (dBestFitness < vecPopulationFitness[ui])
				dBestFitness = vecPopulationFitness[ui];
		}

		dAverageFitness = 0;
		if (vecPopulationFitness.size())
			dAverageFitness = dTotalFitness / vecPopulationFitness.size();
	}

	void CGeneticsTraining::computeFitness(const std::vector<CGenome>& vecPopulation, double& dWorstFitness, double& dAverageFitness, double& dBestFitness, double& dTotalFitness) const
	{
		dWorstFitness = 999999999999999;
		dBestFitness = -999999999999999;
		dTotalFitness = 0;
		for (unsigned int ui = 0; ui < vecPopulation.size(); ui++)
		{
			// Total fitness
			dTotalFitness += vecPopulation[ui].dFitness;

			// Worst fitness
			if (dWorstFitness > vecPopulation[ui].dFitness)
				dWorstFitness = vecPopulation[ui].dFitness;

			// Best fitness
			if (dBestFitness < vecPopulation[ui].dFitness)
				dBestFitness = vecPopulation[ui].dFitness;
		}

		dAverageFitness = 0;
		if (vecPopulation.size())
			dAverageFitness = dTotalFitness / vecPopulation.size();
	}

	void CGeneticsTraining::crossover(
		std::vector<double>& vecMumWeights,
		std::vector<double>& vecDadWeights,
		std::vector<double>& vecChildAWeights,
		std::vector<double>& vecChildBWeights,
		double dCrossoverRate) const
	{
		// Error checking to make sure vector sizes are equal
		if (vecMumWeights.size() != vecDadWeights.size())
			ThrowIfTrue(1, "CGeneticsTraining::crossover() failed. The passed parent vectors are not the same size.");

		// Determine whether we should perform the crossover...
		// First by calculating a random value an determining whether it is below the given crossover rate.
		// And second, if both parents are the same
		if (randDouble() > dCrossoverRate || vecMumWeights == vecDadWeights)
		{
			vecChildAWeights = vecMumWeights;
			vecChildBWeights = vecDadWeights;
			return;
		}

		// If we get here, mum and dad are different and a crossover is to be performed.

		// Determine a random cross over point
		int iCrossoverPoint = randInt(0, (int)vecMumWeights.size() - 1);

		// Clear the child weights
		vecChildAWeights.clear();
		vecChildBWeights.clear();

		// Give the children weights from mum and dad, before the crossover point
		for (int i = 0; i < iCrossoverPoint; i++)
		{
			// Child A gets all of mum's weights, before the crossover point
			vecChildAWeights.push_back(vecMumWeights[i]);

			// The other child, all of dad's, before the crossover point
			vecChildBWeights.push_back(vecDadWeights[i]);
		}
		// Give the children weights from dad and mum, after the crossover point
		for (int i = iCrossoverPoint; i < vecMumWeights.size(); i++)
		{
			// Child A gets all of dad's weights, after the crossover point
			vecChildAWeights.push_back(vecDadWeights[i]);

			// The other child, all of mum's, after the crossover point
			vecChildBWeights.push_back(vecMumWeights[i]);
		}
	}

	void CGeneticsTraining::crossoverBetweenNeurons(
		std::vector<double>& vecMumWeights,
		std::vector<double>& vecDadWeights,
		std::vector<double>& vecChildAWeights,
		std::vector<double>& vecChildBWeights,
		const std::vector<int>& vecSplitPoints,
		double dCrossoverRate) const
	{
		// Error checking to make sure vector sizes are equal
		if (vecMumWeights.size() != vecDadWeights.size())
			ThrowIfTrue(1, "CGeneticsTraining::crossoverBetweenNeurons() failed. The passed parent vectors are not the same size.");
		// Error checking to make sure the given split points vector is of adequate size
		ThrowIfTrue(vecSplitPoints.size() < 2, "CGeneticsTraining::crossoverBetweenNeurons() failed. The passed vecSplitPoints vector's size is too small. Must be at least 2.");

		// Determine whether we should perform the crossover...
		// First by calculating a random value an determining whether it is below the given crossover rate.
		// And second, if both parents are the same
		if (randDouble() > dCrossoverRate || vecMumWeights == vecDadWeights)
		{
			// Just pass on the parents as the children.
			vecChildAWeights = vecMumWeights;
			vecChildBWeights = vecDadWeights;
			return;
		}

		// If we get here, mum and dad are different and a crossover is to be performed.

		// Using the vector holding where we are allowed to split the weights, so that they do not split a portion of a single neuron,
		// determine two crossover points
		int iRand1 = randInt(0, (int)vecSplitPoints.size() - 2);
		int iCrossoverPoint1 = vecSplitPoints[iRand1];
		int iRand2 = randInt(iRand1+1, (int)vecSplitPoints.size() - 1);
		int iCrossoverPoint2 = vecSplitPoints[iRand2];

		// Clear the child weights
		vecChildAWeights.clear();
		vecChildBWeights.clear();

		// Give the children weights from mum and dad
		for (int i = 0; i < (int)vecMumWeights.size(); i++)
		{
			// If outside of the crossover points
			if (i < iCrossoverPoint1 || i > iCrossoverPoint2)
			{
				// Child A gets all of mum's weights
				vecChildAWeights.push_back(vecMumWeights[i]);

				// The other child, all of dad's
				vecChildBWeights.push_back(vecDadWeights[i]);
			}
			else
			{
				// Child A gets all of dad's weights
				vecChildAWeights.push_back(vecDadWeights[i]);

				// The other child, all of mum's
				vecChildBWeights.push_back(vecMumWeights[i]);
			}
		}
	}

	void CGeneticsTraining::mutate(std::vector<double>& vecNetworkWeights, double dMutationProbability, double dMutationMaxAmount) const
	{
		ThrowIfFalse(vecNetworkWeights.size(), "CGeneticsTraining::mutate() failed. Given network weights vector of zero size.");
		// Go through each weight and perhaps mutate it
		for (int i = 0; i < (int)vecNetworkWeights.size(); i++)
		{
			if (randDouble() < dMutationProbability)
			{
				vecNetworkWeights[i] += randomClamped() * dMutationMaxAmount;
			}
		}
	}

	std::vector<CGenome> CGeneticsTraining::createNewGeneration(
		const std::vector<CGenome>& genomes,
		const std::vector<int>& vecSplitsPoints,
		double dMutationProbability,
		double dMutationMaxAmount,
		double dCrossoverRate,
		int iNumberOfElite,
		int iNumCopiesOfEachElite) const
	{
		// Error checking
		ThrowIfFalse(genomes.size(), "CGeneticsTraining::createNewGeneration() failed, as it was given a vector of CGenome objects of zero size.");
		ThrowIfTrue(iNumberOfElite < 0 || iNumCopiesOfEachElite < 0, "CGeneticsTraining::createNewGeneration() failed, as it was given values less than zero for iNumberOfElite or iNumCopiesOfEachElite.");

		// The old population of genomes
		std::vector<CGenome> vecOldPop = genomes;

		// Compute worst, average, best and total fitness of the entire population
		double dWorstFitness, dAverageFitness, dBestFitness, dTotalFitness;
		computeFitness(vecOldPop, dWorstFitness, dAverageFitness, dBestFitness, dTotalFitness);

		// Create the new population of genomes
		std::vector<CGenome> vecNewPop;

		// Insert the elite
		insertElite(vecOldPop, vecNewPop, iNumberOfElite, iNumCopiesOfEachElite);

		while (vecNewPop.size() < vecOldPop.size())
		{
			// Use roulette wheel selection to choose a mum and dad
			CGenome mum = rouletteWheelSelection(vecOldPop);
			CGenome dad = rouletteWheelSelection(vecOldPop);

			// Perform some crossover to create the babies
			CGenome baby1;
			CGenome baby2;
			crossoverBetweenNeurons(mum.vecWeights, dad.vecWeights, baby1.vecWeights, baby2.vecWeights, vecSplitsPoints, dCrossoverRate);

			// Now mutate the babies (Yeuck!)
			mutate(baby1.vecWeights, dMutationProbability, dMutationMaxAmount);
			mutate(baby2.vecWeights, dMutationProbability, dMutationMaxAmount);

			// Now add the new babies into the new population of genomes
			vecNewPop.push_back(baby1);

			// We check the new population size as the population size may be an odd number and if we add two
			// babies, we'll create an oversized population.
			if (vecNewPop.size() < vecOldPop.size())
				vecNewPop.push_back(baby2);
		}

		// Error checking
		return vecNewPop;
	}

	void CGeneticsTraining::insertElite(std::vector<CGenome>& vecOldPop, std::vector<CGenome>& vecNewPop, int iNumberOfElite, int iNumCopiesOfEachElite) const
	{
		// Sort the old population by fitness so that the fitest are towards the end of the vector
		std::sort(vecOldPop.begin(), vecOldPop.end());

		// For each number of elite to insert
		for (int i = 0; i < iNumberOfElite; i++)
		{
			// Add X copies of each elite
			for (int j = 0; j < iNumCopiesOfEachElite; j++)
			{
				// Make sure there is room for the elite
				if (vecNewPop.size() < vecOldPop.size())
				{
					vecNewPop.push_back(vecOldPop[vecOldPop.size() - 1 - i]);
				}
				else
				{
					break;
				}
			}
		}
	}
}