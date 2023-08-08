#include "PCH.h"
#include "2DParticleAffector.h"

namespace X
{
	C2DParticleAffector::C2DParticleAffector()
	{
		_mType = Type::directional;
		_mvForce.set(0.0f, -50.0f);
		_mbActive = true;
		_mfDrag = 0.01f;
	}

	void C2DParticleAffector::setType(Type type)
	{
		_mType = type;
	}

	C2DParticleAffector::Type C2DParticleAffector::getType(void) const
	{
		return _mType;
	}

	void C2DParticleAffector::setPosition(const CVector2f& vPosition)
	{
		_mvPosition = vPosition;
	}

	void C2DParticleAffector::setForce(const CVector2f& vForce)
	{
		_mvForce = vForce;
	}

	void C2DParticleAffector::setActive(bool bActive)
	{
		_mbActive = bActive;
	}

	bool C2DParticleAffector::getActive(void) const
	{
		return _mbActive;
	}

	void C2DParticleAffector::setDrag(float fDrag)
	{
		_mfDrag = fDrag;
	}
}