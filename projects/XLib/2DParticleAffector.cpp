#include "PCH.h"
#include "2DParticleAffector.h"

namespace X
{
	C2DParticleAffector::C2DParticleAffector()
	{
		_mType = Type::directional;
		vForce.set(0.0f, -1.0f);
		bActive = true;
	}
}