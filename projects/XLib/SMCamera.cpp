#include "PCH.h"
#include "SMCamera.h"
#include "utilities.h"
#include "singletons.h"
#include "input.h"

namespace X
{
	CSMCamera::CSMCamera()
	{
		_mMode = Mode::None;
		setSMFramebufferTarget();	// Default is "X:backbuffer"
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
		_matrixProjection.setProjectionOrthographic(0.0f, fWidth, 0.0f, fHeight, -1.0f, 1.0f);
	}

	void CSMCamera::setProjectionAsPerspective(float fFOVdegrees, float fWidth, float fHeight, float fZNear, float fZFar)
	{
		if (fWidth < 0.0f || fHeight < 0.0f)
		{
			fWidth = (float)x->pWindow->getWidth();
			fHeight = (float)x->pWindow->getHeight();
		}
		_matrixProjection.setProjectionPerspective(fFOVdegrees, fZNear, fZFar, fWidth / fHeight);
	}

	void CSMCamera::setViewAsIdentity(void)
	{
		_matrixView.setIdentity();
	}

	void CSMCamera::setViewAsLookat(CVector3f vCameraPosition, CVector3f vCameraTargetPosition, CVector3f vUpVector)
	{
		_matrixView.setViewLookat(vCameraPosition, vCameraTargetPosition, vUpVector);
	}

	CMatrix CSMCamera::getViewProjectionMatrix(void) const
	{
		return _matrixProjection * _matrixView;
	}

	CMatrix CSMCamera::getViewMatrix(void) const
	{
		return _matrixView;
	}

	CMatrix CSMCamera::getProjectionMatrix(void) const
	{
		return _matrixProjection;
	}

	void CSMCamera::update(void)
	{
		if (Mode::None == _mMode)
		{
			return;
		}
		else if (Mode::Orbit == _mMode)
			_updateModeOrbit();
		else if (Mode::FPS == _mMode)
			_updateModeFPS();
	}

	void CSMCamera::setModeOrbit(CVector3f vOrbitPoint, float fMinDistanceFromPoint, float fCurrentDistanceFromPoint, float fMaxDistanceFromPoint, float fSensitivityX, float fSensitivityY, float fMouseSensitivityWheel, bool bLimitYupToPositive)
	{
		// Set mode of camera
		_mMode = Mode::Orbit;

		// Set variables used by the mode
		_modeOrbit.fAngleUp = 0.0f;
		_modeOrbit.fAngleY = 0.0f;
		_modeOrbit.v3OrbitPoint = vOrbitPoint;
		_modeOrbit.fMinDistanceFromPoint = fMinDistanceFromPoint;
		_modeOrbit.fCurrentDistanceFromPoint = fCurrentDistanceFromPoint;
		_modeOrbit.fMaxDistanceFromPoint = fMaxDistanceFromPoint;
		_modeOrbit.fMouseSensitivityX = fSensitivityX;
		_modeOrbit.fMouseSensitivityY = fSensitivityY;
		_modeOrbit.fMouseSensitivityWheel = fMouseSensitivityWheel;
		_modeOrbit.bLimitYupToPositive = bLimitYupToPositive;
	}

	void CSMCamera::_updateModeOrbit(void)
	{
		// Get input from mouse, multiply by time delta and sensitivity settings of the mode
		_modeOrbit.timer.update();
		float fDeltaSec = _modeOrbit.timer.getSecondsPast();
		float fDeltaX = x->pInput->mouse.deltaX() * _modeOrbit.fMouseSensitivityX;
		float fDeltaY = x->pInput->mouse.deltaY() * _modeOrbit.fMouseSensitivityY;
		float fDeltaWheel = -x->pInput->mouse.deltaZ() * _modeOrbit.fMouseSensitivityWheel;

		// Adjust angles
		_modeOrbit.fAngleY += fDeltaX;
		_modeOrbit.fAngleUp += fDeltaY;
		_modeOrbit.fCurrentDistanceFromPoint += fDeltaWheel;

		// Wrap angles
		while (_modeOrbit.fAngleY > k2Pi)
			_modeOrbit.fAngleY -= k2Pi;
		while (_modeOrbit.fAngleY < 0.0f)
			_modeOrbit.fAngleY += k2Pi;

		// Limit up
		if (_modeOrbit.bLimitYupToPositive)
		{
			if (_modeOrbit.fAngleUp < 0.0f)
				_modeOrbit.fAngleUp = 0.0f;
		}

		// Clamp up
		clamp(_modeOrbit.fAngleUp, -kPiOver2, kPiOver2);

		// Clamp distance from point
		clamp(_modeOrbit.fCurrentDistanceFromPoint, _modeOrbit.fMinDistanceFromPoint, _modeOrbit.fMaxDistanceFromPoint);

		// When fAngleUp is at max height, it'll be 1.57f or kPiOver2
		// When fAngleUp is at min height, it'll be -1.57f or -kPiOver2
		// We have to scale camera position of x and z so that they are at 0 when fAngleUp is at min or max
		float fXZscale = _modeOrbit.fCurrentDistanceFromPoint;
		float fAngleUpabsolute = _modeOrbit.fAngleUp;
		absolute(fAngleUpabsolute);
		float fScale = kPiOver2 - fAngleUpabsolute;	// Will be between 0 and kPiOver2
		fScale /= kPiOver2;
		// Keep fScale above 0.0f
		if (fScale < 0.000001f)
			fScale = 0.000001f;
		fXZscale *= fScale;

		// Compute camera position
		CVector3f v3CameraPosition;
		v3CameraPosition.x = sinf(_modeOrbit.fAngleY) * fXZscale;
		v3CameraPosition.y = sinf(_modeOrbit.fAngleUp) * _modeOrbit.fCurrentDistanceFromPoint;
		v3CameraPosition.z = cosf(_modeOrbit.fAngleY) * fXZscale;

		// Set lookat of view matrix
		setViewAsLookat(v3CameraPosition, _modeOrbit.v3OrbitPoint, CVector3f(0.0f, 1.0f, 0.0f));
	}

	void CSMCamera::setModeFPS(CVector3f vInitialPosition, float fInitialYawDegrees, float fInitialPitchDegrees, float fSensitivityX, float fSensitivityY, float fSensitivityTranslateForward, float fSensitivityTranslateStrafe, float fSensitivityTranslateUp, float fMultiplierShiftKey)
	{
		// Set mode of camera
		_mMode = Mode::FPS;
		_modeFPS.v3Position = vInitialPosition;
		_modeFPS.fMouseSensitivityX = fSensitivityX;
		_modeFPS.fMouseSensitivityY = fSensitivityY;
		_modeFPS.fSensitivityTranslateForward = fSensitivityTranslateForward;
		_modeFPS.fSensitivityTranslateStrafe = fSensitivityTranslateStrafe;
		_modeFPS.fSensitivityTranslateUp = fSensitivityTranslateUp;
		_modeFPS.fMultiplierShiftKey = fMultiplierShiftKey;
		_modeFPS.v3Forward = CVector3f(0.0f, 0.0f, 1.0f);
		_modeFPS.v3Up = CVector3f(0.0f, 1.0f, 0.0f);
		_modeFPS.v3Right = _modeFPS.v3Up.getCross(_modeFPS.v3Forward);
		_modeFPS.v3Right.normalise();
		_modeFPS.v3Up = _modeFPS.v3Forward.getCross(_modeFPS.v3Right);
		_modeFPS.v3Up.normalise();
		_modeFPS.fYaw = fInitialYawDegrees;
		_modeFPS.fPitch = fInitialPitchDegrees;
	}

	void CSMCamera::_updateModeFPS(void)
	{
		// Get input from mouse, multiply by time delta and sensitivity settings of the mode
		_modeFPS.timer.update();
		float fDeltaSec = _modeFPS.timer.getSecondsPast();
		// Rotation with mouse
		float fDeltaX = x->pInput->mouse.deltaX() * _modeFPS.fMouseSensitivityX;
		float fDeltaY = x->pInput->mouse.deltaY() * _modeFPS.fMouseSensitivityY;
		// Translation with WSADRF keys
		CVector3f v3Translation = CVector3f(0.0f, 0.0f, 0.0f);
		float fMultiplierShiftKey = 1.0f;
		if (x->pInput->key.pressed(KC_LSHIFT))	fMultiplierShiftKey = _modeFPS.fMultiplierShiftKey;
		if (x->pInput->key.pressed(KC_S)) v3Translation.z = -_modeFPS.fSensitivityTranslateForward * fDeltaSec * fMultiplierShiftKey;
		if (x->pInput->key.pressed(KC_W)) v3Translation.z = _modeFPS.fSensitivityTranslateForward * fDeltaSec * fMultiplierShiftKey;
		if (x->pInput->key.pressed(KC_A)) v3Translation.x = _modeFPS.fSensitivityTranslateStrafe * fDeltaSec * fMultiplierShiftKey;
		if (x->pInput->key.pressed(KC_D)) v3Translation.x = -_modeFPS.fSensitivityTranslateStrafe * fDeltaSec * fMultiplierShiftKey;
		if (x->pInput->key.pressed(KC_R)) v3Translation.y = _modeFPS.fSensitivityTranslateUp * fDeltaSec * fMultiplierShiftKey;
		if (x->pInput->key.pressed(KC_F)) v3Translation.y = -_modeFPS.fSensitivityTranslateUp * fDeltaSec * fMultiplierShiftKey;
		// Translation now holds the AMOUNT to move forwards/backwards/left/right/up/down

		// Adjust position
		_modeFPS.v3Forward.multiply(v3Translation.z);
		_modeFPS.v3Right.multiply(v3Translation.x);
		_modeFPS.v3Up.multiply(v3Translation.y);
		_modeFPS.v3Position += _modeFPS.v3Forward;
		_modeFPS.v3Position += _modeFPS.v3Right;
		_modeFPS.v3Position += _modeFPS.v3Up;

		// Adjust yaw value and wrap
		_modeFPS.fYaw += fDeltaX;
		while (_modeFPS.fYaw < 0.0f)
			_modeFPS.fYaw += 360.0f;
		while (_modeFPS.fYaw > 360.0f)
			_modeFPS.fYaw -= 360.0f;
		// Adjust pitch and limit
		_modeFPS.fPitch -= fDeltaY;
		while (_modeFPS.fPitch < -89.99f)
			_modeFPS.fPitch = -89.0f;
		while (_modeFPS.fPitch > 89.99f)
			_modeFPS.fPitch = 89.99f;

		// Compute direction vectors based on pitch and yaw
		float fCosRadiansPitch = cos(deg2rad(_modeFPS.fPitch));	// As pitch increases/decreases, x and z need to be reduced in radius 
		_modeFPS.v3Forward.x = cos(deg2rad(_modeFPS.fYaw)) * fCosRadiansPitch;
		_modeFPS.v3Forward.z = sin(deg2rad(_modeFPS.fYaw)) * fCosRadiansPitch;
		_modeFPS.v3Forward.y = sin(deg2rad(_modeFPS.fPitch));

		// Compute up/right vectors
		_modeFPS.v3Up.set(0.0f, 1.0f, 0.0f);
		_modeFPS.v3Right = _modeFPS.v3Up.getCross(_modeFPS.v3Forward);
		_modeFPS.v3Right.normalise();

		// Compute lookat target
		CVector3f v3LookatTarget = _modeFPS.v3Position + _modeFPS.v3Forward;

		// Set lookat of view matrix
		setViewAsLookat(_modeFPS.v3Position, v3LookatTarget, _modeFPS.v3Up);
	}

	CVector3f CSMCamera::getPosition(void) const
	{
		CVector3f vCamPos = _matrixView.getTranslation();
		vCamPos *= -1.0f;
		return vCamPos;
	}
}