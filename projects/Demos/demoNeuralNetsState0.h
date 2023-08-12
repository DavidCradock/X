#pragma once
#include "PCH.h"

namespace X
{
	// Represents some food
	class CFood
	{
	public:
		CVector2f vPos;		// Position of food
		C2DEntity* pEntity;	// Holds pointer to entity sprite
	};

	// Represents an entity looking for food
	class CCritter
	{
	public:
		CCritter();

		// Update the neural network and update the entity
		void update(std::vector<CFood>& vecFood, float fDeltaSeconds);

		CNeuralNetworkFeedForward nn;
		CVector2f vPos;		// Position in the world
		CVector2f vDir;		// Lookat direction
		double dFitness;	// Each time an entity gets towards food, it's fitness increases
		C2DEntity* pEntity;	// Holds pointer to entity sprite
		double dMaxTurnRate;	// Maximum value to clamp the outputs of the NN to, set in constructor
		double dMaxSpeed;		// Maximum value to clamp the outputs of the NN to, set in constructor
	};

	

	// First state of the neural network demo application
	class CDemoNeuralNetsState0 : public CFiniteStateBase
	{
		// Initialise
		void onEnter(void);

		// Cleanup
		void onExit(void);

		// State's main loop
		void onActive(CFiniteStateMachine* pFSM);

		// Set to true if there has been a fullscreen toggle.
		// Check this in onActive to reposition various things such as GUI containers
		bool bFullscreenToggle;
	private:
		CTimer mTimer;

		// Holds image names of each image in texture atlases
		std::vector<std::string> mvecstrImageNamesEntity;		// Image names for the entities
		std::vector<std::string> mvecstrImageNamesFood;			// Image names for the food
		std::vector<std::string> mvecstrImageNamesTiles;		// Image names for the floor tiles

		// Holds each entity looking for food
		std::vector<CCritter> mvecCritters;

		// Holds each piece of food
		std::vector<CFood> mvecFood;

		float _mfTimeUntilNewGeneration;	// Counts down to zero, when a new generation will be created.
		bool _mbPerformNextGenCountdown;	// If true, countdown to next gen happens

		int _miGenerationNumber;

		bool _mbTurboMode;	// Whether turbo mode is on or off
	};
}
