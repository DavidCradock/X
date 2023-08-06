#pragma once
#include "PCH.h"

namespace X
{
	// Represents an entity
	class CNNEntity
	{
	public:
		CNNEntity();

		// Update the neural network and update the entity
		void update(float fDeltaSeconds);

		CNeuralNetworkFeedForward nn;
		CVector2f vPos;				// Position in the world
		CVector2f vDir;				// Lookat direction
		double dFitness;			// Each time an entity gets towards food, it's fitness increases
		CSMEntityTriangle* pEntity;	// Holds pointer to entity
		double dMaxTurnRate;		// Maximum value to clamp the outputs of the NN to, set in constructor
		double dMaxSpeed;			// Maximum value to clamp the outputs of the NN to, set in constructor
	};

	// A state of the neural network demo application
	class CDemoNeuralNetsState3 : public CFiniteStateBase
	{
		// Initialise
		void onEnter(void);

		// Cleanup
		void onExit(void);

		// State's main loop
		void onActive(CFiniteStateMachine* pFSM);
	private:
		// Render everything
		void _render(void);

		CTimer _mTimer;
		CSceneManagerSimple _mSM;

		// Holds each entity
		std::vector<CNNEntity> mvecEntities;

		float _mfTimeUntilNewGeneration;	// Counts down to zero, when a new generation will be created.
		bool _mbPerformNextGenCountdown;	// If true, countdown to next gen happens
		int _miGenerationNumber;			// Which generation we're up to.
		bool _mbTurboMode;					// Whether turbo mode is on or off
	};
}
