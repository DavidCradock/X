#include "PCH.h"
#include "SMCamera.h"
#include "window.h"
#include "input.h"
#include "utilities.h"
#include "resourceManager.h"	// For debug text
#include "log.h"

namespace X
{
	CSMCamera::CSMCamera()
	{
		mMode = Mode::None;
		setSMFramebufferTarget();	// Default is "X:backbuffer_FB"
		_mbActive = true;
		setFOV(60.0f);
		setZNearFar(1.0f, 10000.0f);
	}

	void CSMCamera::setSMFramebufferTarget(const std::string& strFramebufferResourceName)
	{
		_mstrFramebufferTargetName = strFramebufferResourceName;
	}

	std::string CSMCamera::getSMFramebufferTarget(void) const
	{
		return _mstrFramebufferTargetName;
	}

	void CSMCamera::setActive(bool bActive)
	{
		_mbActive = bActive;
	}

	bool CSMCamera::getActive(void) const
	{
		return _mbActive;
	}

	void CSMCamera::setFOV(float fFOVdegrees)
	{
		_mfFOVdegrees = fFOVdegrees;
	}

	float CSMCamera::getFOV(void) const
	{
		return _mfFOVdegrees;
	}

	void CSMCamera::setZNearFar(float fNearDistance, float fFarDistance)
	{
		_mfZNear = fNearDistance;
		_mfZFar = fFarDistance;
	}

	float CSMCamera::getZNear(void) const
	{
		return _mfZNear;
	}

	float CSMCamera::getZFar(void) const
	{
		return _mfZFar;
	}

	void CSMCamera::setProjectionAsOrthographic(float fWidth, float fHeight)
	{
		matrixProjection.setProjectionOrthographic(0.0f, fWidth, 0.0f, fHeight, -1.0f, 1.0f);
	}

	void CSMCamera::setProjectionAsPerspective(float fFOVdegrees, float fWidth, float fHeight, float fZNear, float fZFar)
	{
		if (fWidth < 0.0f || fHeight < 0.0f)
		{
			SCWindow* pWindow = SCWindow::getPointer();
			fWidth = (float)pWindow->getWidth();
			fHeight = (float)pWindow->getHeight();
		}
		matrixProjection.setProjectionPerspective(fFOVdegrees, fZNear, fZFar, fWidth / fHeight);
	}

	void CSMCamera::setViewAsIdentity(void)
	{
		matrixView.setIdentity();
	}

	void CSMCamera::setViewAsLookat(CVector3f vCameraPosition, CVector3f vCameraTargetPosition, CVector3f vUpVector)
	{
		matrixView.setViewLookat(vCameraPosition, vCameraTargetPosition, vUpVector);
	}

	CMatrix CSMCamera::getViewProjectionMatrix(void) const
	{
		return matrixProjection * matrixView;
	}

	CMatrix CSMCamera::getViewMatrix(void) const
	{
		return matrixView;
	}

	CMatrix CSMCamera::getProjectionMatrix(void) const
	{
		return matrixProjection;
	}

	void CSMCamera::update(void)
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

	void CSMCamera::setModeOrbit(CVector3f vOrbitPoint, float fMinDistanceFromPoint, float fCurrentDistanceFromPoint, float fMaxDistanceFromPoint, float fSensitivityX, float fSensitivityY, float fMouseSensitivityWheel, bool bLimitYupToPositive)
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

	void CSMCamera::_updateModeOrbit(void)
	{
		// Get input from mouse, multiply by time delta and sensitivity settings of the mode
		SCInputManager* pInput = SCInputManager::getPointer();
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
		CVector3f v3CameraPosition;
		v3CameraPosition.x = sinf(modeOrbit.fAngleY) * fXZscale;
		v3CameraPosition.y = sinf(modeOrbit.fAngleUp) * modeOrbit.fCurrentDistanceFromPoint;
		v3CameraPosition.z = cosf(modeOrbit.fAngleY) * fXZscale;

		// Set lookat of view matrix
		setViewAsLookat(v3CameraPosition, modeOrbit.v3OrbitPoint, CVector3f(0.0f, 1.0f, 0.0f));
	}

	void CSMCamera::setModeFPS(CVector3f vInitialPosition, float fInitialYawDegrees, float fInitialPitchDegrees, float fSensitivityX, float fSensitivityY, float fSensitivityTranslateForward, float fSensitivityTranslateStrafe, float fSensitivityTranslateUp, float fMultiplierShiftKey)
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
		modeFPS.v3Forward = CVector3f(0.0f, 0.0f, 1.0f);
		modeFPS.v3Up = CVector3f(0.0f, 1.0f, 0.0f);
		modeFPS.v3Right = modeFPS.v3Up.getCross(modeFPS.v3Forward);
		modeFPS.v3Right.normalise();
		modeFPS.v3Up = modeFPS.v3Forward.getCross(modeFPS.v3Right);
		modeFPS.v3Up.normalise();
		modeFPS.fYaw = fInitialYawDegrees;
		modeFPS.fPitch = fInitialPitchDegrees;
	}

	void CSMCamera::_updateModeFPS(void)
	{
		// Get input from mouse, multiply by time delta and sensitivity settings of the mode
		SCInputManager* pInput = SCInputManager::getPointer();
		modeFPS.timer.update();
		float fDeltaSec = modeFPS.timer.getSecondsPast();
		// Rotation with mouse
		float fDeltaX = pInput->mouse.deltaX() * modeFPS.fMouseSensitivityX;
		float fDeltaY = pInput->mouse.deltaY() * modeFPS.fMouseSensitivityY;
		// Translation with WSADRF keys
		CVector3f v3Translation = CVector3f(0.0f, 0.0f, 0.0f);
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
		modeFPS.v3Forward.multiply(v3Translation.z);
		modeFPS.v3Right.multiply(v3Translation.x);
		modeFPS.v3Up.multiply(v3Translation.y);
		modeFPS.v3Position += modeFPS.v3Forward;
		modeFPS.v3Position += modeFPS.v3Right;
		modeFPS.v3Position += modeFPS.v3Up;

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
		float fCosRadiansPitch = cos(deg2rad(modeFPS.fPitch));	// As pitch increases/decreases, x and z need to be reduced in radius 
		modeFPS.v3Forward.x = cos(deg2rad(modeFPS.fYaw)) * fCosRadiansPitch;
		modeFPS.v3Forward.z = sin(deg2rad(modeFPS.fYaw)) * fCosRadiansPitch;
		modeFPS.v3Forward.y = sin(deg2rad(modeFPS.fPitch));

		// Compute up/right vectors
		modeFPS.v3Up.set(0.0f, 1.0f, 0.0f);
		modeFPS.v3Right = modeFPS.v3Up.getCross(modeFPS.v3Forward);
		modeFPS.v3Right.normalise();

		// Compute lookat target
		CVector3f v3LookatTarget = modeFPS.v3Position + modeFPS.v3Forward;

		// Set lookat of view matrix
		setViewAsLookat(modeFPS.v3Position, v3LookatTarget, modeFPS.v3Up);
	}

	CVector3f CSMCamera::getPosition(void) const
	{
		CVector3f vCamPos = matrixView.getTranslation();
		vCamPos *= -1.0f;
		return vCamPos;
	}
}