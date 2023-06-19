#include "PCH.h"
#include "SMCamera.h"
#include "window.h"
#include "input.h"
#include "utilities.h"
#include "resourceManager.h"	// For debug text

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

	void SMCamera::update(void)
	{
		if (Mode::None == mMode)
		{
			return;
		}
		else if (Mode::Orbit == mMode)
			_updateModeOrbit();
		else if (Mode::FPS == mMode)
			_updateModeFPS();
	}

	void SMCamera::setModeOrbit(glm::vec3 vOrbitPoint, float fMinDistanceFromPoint, float fCurrentDistanceFromPoint, float fMaxDistanceFromPoint, float fSensitivityX, float fSensitivityY, float fMouseSensitivityWheel, bool bLimitYupToPositive)
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
		modeOrbit.bLimitYupToPositive = bLimitYupToPositive;
	}

	void SMCamera::_updateModeOrbit(void)
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

		// Limit up
		if (modeOrbit.bLimitYupToPositive)
		{
			if (modeOrbit.fAngleUp < 0.0f)
				modeOrbit.fAngleUp = 0.0f;
		}

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
		pRM->getFont("arial_26")->print(strDebug, 0, 50, Window::getPointer()->getWidth(), Window::getPointer()->getHeight(), 1.0f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

		strDebug = "Camera mode orbit: fAngleY: " + std::format("{:.2f}", modeOrbit.fAngleY);
		pRM->getFont("arial_26")->print(strDebug, 0, 80, Window::getPointer()->getWidth(), Window::getPointer()->getHeight(), 1.0f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

		strDebug = "Camera mode orbit: fAngleUp: " + std::format("{:.2f}", modeOrbit.fAngleUp);
		pRM->getFont("arial_26")->print(strDebug, 0, 110, Window::getPointer()->getWidth(), Window::getPointer()->getHeight(), 1.0f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

		strDebug = "Camera mode orbit: fXZscale: " + std::format("{:.2f}", fXZscale);
		pRM->getFont("arial_26")->print(strDebug, 0, 140, Window::getPointer()->getWidth(), Window::getPointer()->getHeight(), 1.0f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

		strDebug = "Camera mode orbit: fScale: " + std::format("{:.2f}", fScale);
		pRM->getFont("arial_26")->print(strDebug, 0, 170, Window::getPointer()->getWidth(), Window::getPointer()->getHeight(), 1.0f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		*/
	}

	void SMCamera::setModeFPS(glm::vec3 vInitialPosition, float fInitialYawDegrees, float fInitialPitchDegrees, float fSensitivityX, float fSensitivityY, float fSensitivityTranslateForward, float fSensitivityTranslateStrafe, float fSensitivityTranslateUp, float fMultiplierShiftKey)
	{
		// Set mode of camera
		mMode = Mode::FPS;
		modeFPS.v3Position = vInitialPosition;
		modeFPS.fMouseSensitivityX = fSensitivityX;
		modeFPS.fMouseSensitivityY = fSensitivityY;
		modeFPS.fSensitivityTranslateForward = fSensitivityTranslateForward;
		modeFPS.fSensitivityTranslateStrafe = fSensitivityTranslateStrafe;
		modeFPS.fSensitivityTranslateUp = fSensitivityTranslateUp;
		modeFPS.fMultiplierShiftKey = fMultiplierShiftKey;
		modeFPS.v3Forward = glm::vec3(0.0f, 0.0f, 1.0f);
		modeFPS.v3Up = glm::vec3(0.0f, 1.0f, 0.0f);
		modeFPS.v3Right = glm::normalize(glm::cross(modeFPS.v3Up, modeFPS.v3Forward));
		modeFPS.v3Up = glm::normalize(glm::cross(modeFPS.v3Forward, modeFPS.v3Right));
		modeFPS.fYaw = fInitialYawDegrees;
		modeFPS.fPitch = fInitialPitchDegrees;
	}

	void SMCamera::_updateModeFPS(void)
	{
		// Get input from mouse, multiply by time delta and sensitivity settings of the mode
		InputManager* pInput = InputManager::getPointer();
		modeFPS.timer.update();
		float fDeltaSec = modeFPS.timer.getSecondsPast();
		// Rotation with mouse
		float fDeltaX = pInput->mouse.deltaX() * modeFPS.fMouseSensitivityX;
		float fDeltaY = pInput->mouse.deltaY() * modeFPS.fMouseSensitivityY;
		// Translation with WSADRF keys
		glm::vec3 v3Translation = glm::vec3(0.0f, 0.0f, 0.0f);
		float fMultiplierShiftKey = 1.0f;
		if (pInput->key.pressed(KC_LSHIFT))	fMultiplierShiftKey = modeFPS.fMultiplierShiftKey;
		if (pInput->key.pressed(KC_S)) v3Translation.z = -modeFPS.fSensitivityTranslateForward * fDeltaSec * fMultiplierShiftKey;
		if (pInput->key.pressed(KC_W)) v3Translation.z = modeFPS.fSensitivityTranslateForward * fDeltaSec * fMultiplierShiftKey;
		if (pInput->key.pressed(KC_A)) v3Translation.x = modeFPS.fSensitivityTranslateStrafe * fDeltaSec * fMultiplierShiftKey;
		if (pInput->key.pressed(KC_D)) v3Translation.x = -modeFPS.fSensitivityTranslateStrafe * fDeltaSec * fMultiplierShiftKey;
		if (pInput->key.pressed(KC_R)) v3Translation.y = modeFPS.fSensitivityTranslateUp * fDeltaSec * fMultiplierShiftKey;
		if (pInput->key.pressed(KC_F)) v3Translation.y = -modeFPS.fSensitivityTranslateUp * fDeltaSec * fMultiplierShiftKey;
		// Translation now holds the AMOUNT to move forwards/backwards/left/right/up/down

		// Adjust position
		modeFPS.v3Position += v3Translation.z/*Amount to move forward/back*/ * modeFPS.v3Forward;
		modeFPS.v3Position += v3Translation.x/*Amount to move left/right*/ * modeFPS.v3Right;
		modeFPS.v3Position += v3Translation.y/*Amount to move up/down*/ * modeFPS.v3Up;

		// Adjust yaw value and wrap
		modeFPS.fYaw += fDeltaX;
		while (modeFPS.fYaw < 0.0f)
			modeFPS.fYaw += 360.0f;
		while (modeFPS.fYaw > 360.0f)
			modeFPS.fYaw -= 360.0f;
		// Adjust pitch and limit
		modeFPS.fPitch -= fDeltaY;
		while (modeFPS.fPitch < -89.99f)
			modeFPS.fPitch = -89.0f;
		while (modeFPS.fPitch > 89.99f)
			modeFPS.fPitch = 89.99f;

		// Compute direction vectors based on pitch and yaw
		float fCosRadiansPitch = cos(glm::radians(modeFPS.fPitch));	// As pitch increases/decreases, x and z need to be reduced in radius 
		modeFPS.v3Forward.x = cos(glm::radians(modeFPS.fYaw)) * fCosRadiansPitch;
		modeFPS.v3Forward.z = sin(glm::radians(modeFPS.fYaw)) * fCosRadiansPitch;
		modeFPS.v3Forward.y = sin(glm::radians(modeFPS.fPitch));

		// Compute up/right vectors
		modeFPS.v3Up = glm::vec3(0.0f, 1.0f, 0.0f);
		modeFPS.v3Right = glm::normalize(glm::cross(modeFPS.v3Up, modeFPS.v3Forward));

		// Compute lookat target
		glm::vec3 v3LookatTarget = modeFPS.v3Position + modeFPS.v3Forward;

		// Set lookat of view matrix
		setViewAsLookat(modeFPS.v3Position, v3LookatTarget, modeFPS.v3Up);

		// Debug text
		/*
		ResourceManager* pRM = ResourceManager::getPointer();
		ResourceFont* pFont = pRM->getFont("arial_26");
		int iYpos = (int)pFont->getTextHeight();

		// Position
		std::string strDebug = "Camera mode FPS: Position: " + std::format("{:.2f}", modeFPS.v3Position.x) += ", " + std::format("{:.2f}", modeFPS.v3Position.y) += ", " + std::format("{:.2f}", modeFPS.v3Position.z);
		pFont->print(strDebug, 0, iYpos, Window::getPointer()->getWidth(), Window::getPointer()->getHeight(), 1.0f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));		iYpos += (int)pFont->getTextHeight();
		
		// modeFPS.v3Forward
		strDebug = "modeFPS.v3Forward: " + std::format("{:.2f}", modeFPS.v3Forward.x) += ", " + std::format("{:.2f}", modeFPS.v3Forward.y) += ", " + std::format("{:.2f}", modeFPS.v3Forward.z);
		pFont->print(strDebug, 0, iYpos, Window::getPointer()->getWidth(), Window::getPointer()->getHeight(), 1.0f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));		iYpos += (int)pFont->getTextHeight();

		// modeFPS.v3Right
		strDebug = "modeFPS.v3Right: " + std::format("{:.2f}", modeFPS.v3Right.x) += ", " + std::format("{:.2f}", modeFPS.v3Right.y) += ", " + std::format("{:.2f}", modeFPS.v3Right.z);
		pFont->print(strDebug, 0, iYpos, Window::getPointer()->getWidth(), Window::getPointer()->getHeight(), 1.0f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));		iYpos += (int)pFont->getTextHeight();

		// modeFPS.v3Up
		strDebug = "modeFPS.v3Up: " + std::format("{:.2f}", modeFPS.v3Up.x) += ", " + std::format("{:.2f}", modeFPS.v3Up.y) += ", " + std::format("{:.2f}", modeFPS.v3Up.z);
		pFont->print(strDebug, 0, iYpos, Window::getPointer()->getWidth(), Window::getPointer()->getHeight(), 1.0f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));		iYpos += (int)pFont->getTextHeight();

		// modeFPS.fPitch and modeFPS.fYaw
		strDebug = "modeFPS.fPitch: " + std::format("{:.2f}", modeFPS.fPitch) += " modeFPS.fYaw: " + std::format("{:.2f}", modeFPS.fYaw);
		pFont->print(strDebug, 0, iYpos, Window::getPointer()->getWidth(), Window::getPointer()->getHeight(), 1.0f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));		iYpos += (int)pFont->getTextHeight();
		*/
	}
}