#include "PCH.h"
#include "2DCamera.h"

namespace X
{
	C2DCamera::C2DCamera()
	{
		_mstrFramebufferTarget = "X:backbuffer";
		_mv2fPosition.setZero();
		_mbEnabled = true;
	}

	C2DCamera::~C2DCamera()
	{

	}

	CVector2f C2DCamera::getPosition(void) const
	{
		return _mv2fPosition;
	}

	void C2DCamera::setPosition(const CVector2f& vPosition)
	{
		_mv2fPosition = vPosition;
	}

	void C2DCamera::setPosition(float fPositionX, float fPositionY)
	{
		_mv2fPosition.x = fPositionX;
		_mv2fPosition.y = fPositionY;
	}

	bool C2DCamera::getEnabled(void) const
	{
		return _mbEnabled;
	}

	void C2DCamera::setEnabled(bool bEnabled)
	{
		_mbEnabled = bEnabled;
	}

	std::string C2DCamera::getFramebufferTargetName(void) const
	{
		return _mstrFramebufferTarget;
	}

	void C2DCamera::setFramebufferTargetName(const std::string& strFramebufferTargetName)
	{
		_mstrFramebufferTarget = strFramebufferTargetName;
	}


}