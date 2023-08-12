#pragma once
#include "PCH.h"

namespace X
{
	// Represents some food
	class CFood2
	{
	public:
		CVector2f vPos;		// Position of food
		C2DEntity* pEntity;	// Holds pointer to entity sprite
	};

	class CCritterEvil;

	// Represents an entity looking for food
	class CCritter2
	{
	public:
		CCritter2();

		// Update the neural network and update the entity
		void update(std::vector<CFood2>& vecFood, std::vector<CCritterEvil>& vecEvil, float fDeltaSeconds);

		CNeuralNetworkFeedForward nn;
		CVector2f vPos;		// Position in the world
		CVector2f vDir;		// Lookat direction
		double dFitness;	// Each time an entity gets towards food, it's fitness increases
		C2DEntity* pEntity;	// Holds pointer to entity sprite
		double dMaxTurnRate;	// Maximum value to clamp the outputs of the NN to, set in constructor
		double dMaxSpeed;		// Maximum value to clamp the outputs of the NN to, set in constructor
	};

	// Represents an evil entity looking for food(The other critters)
	class CCritterEvil
	{
	public:
		CCritterEvil();

		// Update the neural network and update the entity
		void update(std::vector<CFood2>& vecFood, std::vector<CCritter2>& mvecCritters, float fDeltaSeconds);

		CNeuralNetworkFeedForward nn;
		CVector2f vPos;		// Position in the world
		CVector2f vDir;		// Lookat direction
		double dFitness;	// Each time an entity gets towards food, it's fitness increases
		C2DEntity* pEntity;	// Holds pointer to entity sprite
		double dMaxTurnRate;	// Maximum value to clamp the outputs of the NN to, set in constructor
		double dMaxSpeed;		// Maximum value to clamp the outputs of the NN to, set in constructor
	};

	// Second state of the neural network demo application
	class CDemoNeuralNetsState1 : public CFiniteStateBase
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
		std::vector<std::string> mvecstrImageNamesEntityEvil;	// Image names for the entities (Evil)

		// Holds each entity looking for food
		std::vector<CCritter2> mvecCritters;

		// Holds each piece of food
		std::vector<CFood2> mvecFood;

		// Holds each evil entity, also looking for food (The other critters)
		std::vector<CCritterEvil> mvecEvil;

		float _mfTimeUntilNewGeneration;	// Counts down to zero, when a new generation will be created.
		bool _mbPerformNextGenCountdown;	// If true, countdown to next gen happens

		int _miGenerationNumber;

		bool _mbTurboMode;	// Whether turbo mode is on or off
	};
}
