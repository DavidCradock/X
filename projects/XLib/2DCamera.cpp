#include "PCH.h"
#include "2DCamera.h"

namespace X
{
	C2DCamera::C2DCamera()
	{

	}

	C2DCamera::~C2DCamera()
	{

	}

	glm::vec3 C2DCamera::getPositionGLM(void)
	{
		glm::vec3 vPos;
		vPos.x = (float)mv2rPosition.x;
		vPos.y = (float)mv2rPosition.y;
		vPos.z = 0.0f;
		return vPos;
	}
}