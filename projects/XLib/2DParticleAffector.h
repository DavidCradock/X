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
		friend class C2DParticleSystem;
	public:
		// Constructor, sets to default (directional, applying a force downwards, simulating gravity)
		C2DParticleAffector();

		// What type this particle affector is
		enum Type
		{
			attractor,		// Attractor force applied to all particles. Uses vPosition and force
			directional,	// Directional force applied to all particles. Uses vForce.
			drag			// Air resistance/drag
		};

		// Sets the type of this affector
		void setType(Type type);

		// Gets the type of this affector
		Type getType(void) const;

		// Depending upon the type, if applicable, set's the position of this affector
		void setPosition(const CVector2f& vPosition);

		// Depending upon the type, if applicable, set's the force of this affector
		void setForce(const CVector2f& vForce);

		// Sets whether this affector is active or not
		void setActive(bool bActive);

		// Gets whether this affector is active or not
		bool getActive(void) const;

		// If this affector is set to the type of drag, set's the drag value
		void setDrag(float fDrag);

	private:
		
		Type _mType;

		CVector2f _mvPosition;	// Position of affector
		CVector2f _mvForce;		// Force to apply to the particles
		float _mfDrag;
		bool _mbActive;			// Whether this affector is active or not
	};
}