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
	class C2DParticleAffector
	{
	public:
		// What type this particle affector is
		enum Type
		{
			attractor,
			gravity,
			repellor
		};
		Type _mType;

		CVector2f vPosition;	// Position of affector
	};
}