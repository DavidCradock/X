#include "PCH.h"
#include "2DCamera.h"

namespace X
{
	C2DCamera::C2DCamera()
	{
		_mstrFramebufferTarget = "X:backbuffer_FB";
		mv2rPosition.setZero();
		_mbEnabled = true;
	}

	C2DCamera::~C2DCamera()
	{

	}

	glm::vec3 C2DCamera::getPositionGLM(void) const
	{
		glm::vec3 vPos;
		vPos.x = (float)mv2rPosition.x;
		vPos.y = (float)mv2rPosition.y;
		vPos.z = 0.0f;
		return vPos;
	}

	void C2DCamera::setEnabled(bool bEnabled)
	{
		_mbEnabled = bEnabled;
	}

	bool C2DCamera::getEnabled(void) const
	{
		return _mbEnabled;
	}
}