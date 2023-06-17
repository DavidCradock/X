#include "PCH.h"
#include "SMCamera.h"
#include "window.h"
#include "input.h"
#include "utilities.h"
//#include "resourceManager.h"	// For debug text

namespace X
{
	SMCamera::SMCamera()
	{
		mMode = Mode::None;
	}

	void SMCamera::setProjectionAsOrthographic(float fWidth, float fHeight)
	{
		matrixProjection = glm::ortho(0.0f, fWidth, fHeight, 0.0f, -1.0f, 1.0f);
	}

	void SMCamera::setProjectionAsPerspective(float fFOVdegrees, float fWidth, float fHeight, float fZNear, float fZFar)
	{
		if (fWidth < 0.0f || fHeight < 0.0f)
		{
			Window* pWindow = Window::getPointer();
			fWidth = (float)pWindow->getWidth();
			fHeight = (float)pWindow->getHeight();
		}
		matrixProjection = glm::perspective(glm::radians(fFOVdegrees), fWidth / fHeight, fZNear, fZFar);
	}

	void SMCamera::setViewAsIdentity(void)
	{
		matrixView = glm::mat4();
	}

	void SMCamera::setViewAsLookat(glm::vec3 vCameraPosition, glm::vec3 vCameraTargetPosition, glm::vec3 vUpVector)
	{
		matrixView = glm::lookAt(vCameraPosition, vCameraTargetPosition, vUpVector);
	}

	glm::mat4 SMCamera::getViewProjectionMatrix(void)
	{
		return matrixProjection * matrixView;	// Other way around?
	}

	void SMCamera::setModeOrbit(glm::vec3 vOrbitPoint, float fMinDistanceFromPoint, float fCurrentDistanceFromPoint, float fMaxDistanceFromPoint, float fSensitivityX, float fSensitivityY, float fMouseSensitivityWheel)
	{
		// Set mode of camera
		mMode = Mode::Orbit;

		// Set variables used by the mode
		modeOrbit.fAngleUp = 0.0f;
		modeOrbit.fAngleY = 0.0f;
		modeOrbit.v3OrbitPoint = vOrbitPoint;
		modeOrbit.fMinDistanceFromPoint = fMinDistanceFromPoint;
		modeOrbit.fCurrentDistanceFromPoint = fCurrentDistanceFromPoint;
		modeOrbit.fMaxDistanceFromPoint = fMaxDistanceFromPoint;
		modeOrbit.fMouseSensitivityX = fSensitivityX;
		modeOrbit.fMouseSensitivityY = fSensitivityY;
		modeOrbit.fMouseSensitivityWheel = fMouseSensitivityWheel;
	}

	void SMCamera::update(void)
	{
		if (Mode::None == mMode)
		{
			return;
		}
		else if (Mode::Orbit == mMode)
		{
			// Get input from mouse, multiply by time delta and sensitivity settings of the mode
			InputManager* pInput = InputManager::getPointer();
			modeOrbit.timer.update();
			float fDeltaSec = modeOrbit.timer.getSecondsPast();
			float fDeltaX = pInput->mouse.deltaX() * modeOrbit.fMouseSensitivityX;
			float fDeltaY = pInput->mouse.deltaY() * modeOrbit.fMouseSensitivityY;
			float fDeltaWheel = -pInput->mouse.deltaZ() * modeOrbit.fMouseSensitivityWheel;

			// Adjust angles
			modeOrbit.fAngleY += fDeltaX;
			modeOrbit.fAngleUp += fDeltaY;
			modeOrbit.fCurrentDistanceFromPoint += fDeltaWheel;

			// Wrap angles
			while (modeOrbit.fAngleY > k2Pi)
				modeOrbit.fAngleY -= k2Pi;
			while (modeOrbit.fAngleY < 0.0f)
				modeOrbit.fAngleY += k2Pi;
			
			// Clamp up
			clamp(modeOrbit.fAngleUp, -kPiOver2, kPiOver2);

			// Clamp distance from point
			clamp(modeOrbit.fCurrentDistanceFromPoint, modeOrbit.fMinDistanceFromPoint, modeOrbit.fMaxDistanceFromPoint);

			// When fAngleUp is at max height, it'll be 1.57f or kPiOver2
			// When fAngleUp is at min height, it'll be -1.57f or -kPiOver2
			// We have to scale camera position of x and z so that they are at 0 when fAngleUp is at min or max
			float fXZscale = modeOrbit.fCurrentDistanceFromPoint;
			float fAngleUpabsolute = modeOrbit.fAngleUp;
			absolute(fAngleUpabsolute);
			float fScale = kPiOver2 - fAngleUpabsolute;	// Will be between 0 and kPiOver2
			fScale /= kPiOver2;
			// Keep fScale above 0.0f
			if (fScale < 0.000001f)
				fScale = 0.000001f;
			fXZscale *= fScale;

			// Compute camera position
			glm::vec3 v3CameraPosition;
			v3CameraPosition.x = sinf(modeOrbit.fAngleY) * fXZscale;
			v3CameraPosition.y = sinf(modeOrbit.fAngleUp) * modeOrbit.fCurrentDistanceFromPoint;
			v3CameraPosition.z = cosf(modeOrbit.fAngleY) * fXZscale;

			// Set lookat of view matrix
			setViewAsLookat(v3CameraPosition, modeOrbit.v3OrbitPoint, glm::vec3(0.0f, 1.0f, 0.0f));

			// Debug text
			/*
			ResourceManager* pRM = ResourceManager::getPointer();
			std::string strDebug = "Camera mode orbit: Position: ";
			strDebug += std::format("{:.2f}", v3CameraPosition.x);
			strDebug += ", " + std::format("{:.2f}", v3CameraPosition.y);
			strDebug += ", " + std::format("{:.2f}", v3CameraPosition.z);
			pRM->getFont("arial_26")->print(strDebug, 0, 50, Window::getPointer()->getWidth(), Window::getPointer()->getHeight(), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

			strDebug = "Camera mode orbit: fAngleY: " + std::format("{:.2f}", modeOrbit.fAngleY);
			pRM->getFont("arial_26")->print(strDebug, 0, 80, Window::getPointer()->getWidth(), Window::getPointer()->getHeight(), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

			strDebug = "Camera mode orbit: fAngleUp: " + std::format("{:.2f}", modeOrbit.fAngleUp);
			pRM->getFont("arial_26")->print(strDebug, 0, 110, Window::getPointer()->getWidth(), Window::getPointer()->getHeight(), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

			strDebug = "Camera mode orbit: fXZscale: " + std::format("{:.2f}", fXZscale);
			pRM->getFont("arial_26")->print(strDebug, 0, 140, Window::getPointer()->getWidth(), Window::getPointer()->getHeight(), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

			strDebug = "Camera mode orbit: fScale: " + std::format("{:.2f}", fScale);
			pRM->getFont("arial_26")->print(strDebug, 0, 170, Window::getPointer()->getWidth(), Window::getPointer()->getHeight(), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
			*/
		}

		/*
		// Get keyboard input for WSADRF and shift keys and set movement amount.0
		
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
		*/
	}
}