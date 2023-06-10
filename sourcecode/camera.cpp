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

	void Camera::setProjectionAsPerspective(float fFOVdegrees, float fWidth, float fHeight, float fZNear, float fZFar)
	{
		matrixProjection = glm::perspective(glm::radians(fFOVdegrees), fWidth / fHeight, fZNear, fZFar);
	}
	void Camera::setViewAsIdentity(void)
	{
		matrixView = glm::mat4();
	}

	void Camera::setViewAsLookat(glm::vec3 vCameraPosition, glm::vec3 vCameraTargetPosition, glm::vec3 vUpVector)
	{
		matrixView = glm::lookAt(vCameraPosition, vCameraTargetPosition, vUpVector);
	}

	glm::mat4 Camera::getViewProjectionMatrix(void)
	{
		return matrixProjection * matrixView;	// Other way around?
	}
}