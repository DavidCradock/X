#pragma once
#include "PCH.h"

namespace X
{
	// Represents some food
	class CFood3
	{
	public:
		CVector2f vPos;				// Position of food
		CSMEntityVertexBuffer* pEntity;	// Holds pointer to entity
	};

	// Represents an entity looking for food
	class CCritter3
	{
	public:
		CCritter3();

		// Update the neural network and update the entity
		void update(std::vector<CFood3>& vecFood, float fDeltaSeconds);

		CNeuralNetworkFeedForward nn;
		CVector2f vPos;		// Position in the world
		CVector2f vDir;		// Lookat direction
		double dFitness;	// Each time an entity gets towards food, it's fitness increases
		CSMEntityVertexBuffer* pEntity;	// Holds pointer to entity
		double dMaxTurnRate;	// Maximum value to clamp the outputs of the NN to, set in constructor
		double dMaxSpeed;		// Maximum value to clamp the outputs of the NN to, set in constructor
	};

	// A state of the neural network demo application
	class CDemoNeuralNetsState2 : public CFiniteStateBase
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

		// Holds each entity looking for food
		std::vector<CCritter3> mvecCritters;

		// Holds each piece of food
		std::vector<CFood3> mvecFood;

		float _mfTimeUntilNewGeneration;	// Counts down to zero, when a new generation will be created.
		bool _mbPerformNextGenCountdown;	// If true, countdown to next gen happens
		int _miGenerationNumber;
		bool _mbTurboMode;	// Whether turbo mode is on or off
	};
}
