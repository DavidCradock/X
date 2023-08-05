#pragma once
#include "PCH.h"
#include "vector2f.h"

namespace X
{
	// A particle affector used with the C2DParticleSystem class
	// These affect all particles within the system in certain ways, typically adjusting their velocity.
	// A common affector is gravity, where it applies a constant for downwards upon the particles to simulate gravity.
	// Affectors, as well as affecting all particles globally, can be assigned to particles by their particle type so that they
	// affect other particles too.
	// By default, the affector is set to type directional with a force being applied downwards to simulate gravity.
	class C2DParticleAffector
	{
	public:
		// Constructor, sets to default (directional, applying a force downwards, simulating gravity)
		C2DParticleAffector();

		// What type this particle affector is
		enum Type
		{
			attractor,		// Attractor force applied to all particles. Uses vPosition, ???
			directional,	// Directional force applied to all particles. Uses vForce.
			drag			// Air resistance/drag
		};
		Type _mType;

		CVector2f vPosition;	// Position of affector
		CVector2f vForce;		// Force to apply to the particles
		bool bActive;			// Whether this affector is active or not
	};
}