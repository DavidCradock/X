#include "PCH.h"
#include "camera.h"
#include "window.h"
#include "input.h"

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

	void Camera::update(float fDeltaTimeSeconds)
	{
		// Get keyboard input for WSADRF and shift keys and set movement amount.0
		InputManager* pInput = InputManager::getPointer();
		float fMoveForward = 0.0f;
		float fMoveRight = 0.0f;
		float fMoveUp = 0.0f;
		if (pInput->key.pressed(KC_W))
			fMoveForward += 1.0f * fDeltaTimeSeconds;
		if (pInput->key.pressed(KC_S))
			fMoveForward -= 1.0f * fDeltaTimeSeconds;
		if (pInput->key.pressed(KC_D))
			fMoveRight += 1.0f * fDeltaTimeSeconds;
		if (pInput->key.pressed(KC_A))
			fMoveRight -= 1.0f * fDeltaTimeSeconds;
		if (pInput->key.pressed(KC_R))
			fMoveUp += 1.0f * fDeltaTimeSeconds;
		if (pInput->key.pressed(KC_F))
			fMoveUp -= 1.0f * fDeltaTimeSeconds;
		if (pInput->key.pressed(KC_LSHIFT))
		{
			fMoveForward *= 10.0f;
			fMoveRight *= 10.0f;
			fMoveUp *= 10.0f;
		}

	}
}