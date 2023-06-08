#include "PCH.h"
#include "camera.h"
#include "window.h"

namespace X
{
	Camera::Camera()
	{

	}

	void Camera::setProjectionAsOrthographic(float fWidth, float fHeight)
	{
		matrixProjection = glm::ortho(0.0f, fWidth, fHeight, 0.0f, -1.0f, 1.0f);
	}

	void Camera::setViewAsIdentity(void)
	{
		matrixView = glm::mat4();
	}
}