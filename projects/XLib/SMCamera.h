#pragma once
#include "PCH.h"
#include "timer.h"
#include "matrix.h"
#include "vector3f.h"

namespace X
{
	// A camera which holds view and projection matrices
	// We can use the various methods to set the view and projection matrices, access them directly or we can use a camera mode
	// A camera mode is set with a call to one of the various setMode??() methods and then call update() once per program loop to compute 
	// the view matrix depending upon which mode is currently set
	class CSMCamera
	{
	public:
		CSMCamera();
		
		// Sets the framebuffer the scene manager renders this camera to.
		// The framebuffer must exist in the SCResourceManager otherwise an exception occurs during rendering
		// By default, all cameras render to the framebuffer named "X:backbuffer".
		void setSMFramebufferTarget(const std::string& strFramebufferResourceName = "X:backbuffer");
		
		// Returns the name of the framebuffer the scene manager renders this camera to.
		// By default, all cameras render to the framebuffer named "X:backbuffer".
		std::string getSMFramebufferTarget(void) const;

		// Sets whether this camera is active or not.
		// By default, all cameras are active.
		// If inactive, they are not rendered to their set framebuffer target.
		void setActive(bool bActive);

		// Returns whether this camera is active or not.
		bool getActive(void) const;

		// Sets the FOV in degress which this camera will be set to use by the scene manager
		// The default is 60.0f
		void setFOV(float fFOVdegrees);

		// Returns the FOV in degrees which this camera will be set to use by the scene manager
		float getFOV(void) const;

		// Sets the near and far clipping plane distances this camera will be set to use by the scene manager
		// The default is 1.0f and 10000.0f
		void setZNearFar(float fNearDistance, float fFarDistance);

		// Gets the near clipping plane distance this camera is set to use by the scene manager
		float getZNear(void) const;

		// Gets the far clipping plane distance this camera is set to use by the scene manager
		float getZFar(void) const;

		// Sets the projection matrix of the camera as orthogonal
		// The parameters are the dimensions of whatever we're rendering to.
		// Passing the window's dims is a good idea, or perhaps the dims of a framebuffer.
		void setProjectionAsOrthographic(float fWidth, float fHeight);

		// Sets the projection matrix of the camera as perspective
		// The parameters are the dimensions of whatever we're rendering to.
		// Passing the window's dims is a good idea, or perhaps the dims of a framebuffer.
		// Passing a value of less than zero to the fWidth and fHeight params will set the dims to SCWindow::getWidth() and SCWindow::getHeight().
		void setProjectionAsPerspective(float fFOVdegrees, float fWidth = -1.0f, float fHeight = -1.0f, float fZNear = 1.0f, float fZFar = 10000.0f);

		// Sets the view matrix as identity
		void setViewAsIdentity(void);

		// Sets the view matrix via lookat
		void setViewAsLookat(CVector3f vCameraPosition, CVector3f vCameraTargetPosition, CVector3f vUpVector = CVector3f(0.0f, 1.0f, 0.0f));

		// Multiplies the view and projection matrixs together and returns the result
		CMatrix getViewProjectionMatrix(void) const;

		// Returns the camera's view matrix
		CMatrix getViewMatrix(void) const;

		// Returns the camera's projection matrix
		CMatrix getProjectionMatrix(void) const;

		// Updates this camera
		// If one of the camera modes are set with a call to setModeOrbit() or setModeFPS(), then the variables of those
		// modes need to be updated (Checking input from keyboard etc)
		// This needs to be called if using one of those modes.
		void update(void);

		// Returns the current world position of the camera
		CVector3f getPosition(void) const;

		// Sets the camera mode to orbit a point in 3D world space
		// Call update() once per program loop to update the view matrix with this mode in mind.
		void setModeOrbit(
			CVector3f vOrbitPoint = CVector3f(0.0f, 0.0f, 0.0f),
			float fMinDistanceFromPoint = 1.5f,
			float fCurrentDistanceFromPoint = 10.0f,
			float fMaxDistanceFromPoint = 100.0f,
			float fSensitivityX = 0.005f,
			float fSensitivityY = 0.005f,
			float fMouseSensitivityWheel = 0.005f,
			bool bLimitYupToPositive = false);

		// Sets the camera mode to FPS
		// Call update() once per program loop to update the view matrix with this mode in mind.
		void setModeFPS(
			CVector3f vInitialPosition = CVector3f(0.0f, 1.0f, 0.0f),
			float fInitialYawDegrees = 0.0f,
			float fInitialPitchDegrees = 0.0f,
			float fSensitivityX = 0.5f,
			float fSensitivityY = 0.2f,
			float fSensitivityTranslateForward = 1.0f,
			float fSensitivityTranslateStrafe = 1.0f,
			float fSensitivityTranslateUp = 1.0f,
			float fMultiplierShiftKey = 10.0f);

	private:
		enum Mode
		{
			FPS,
			Orbit,
			None
		};
		Mode _mMode;	// Which mode is currently set

		// Holds variables used for the orbit mode
		struct ModeOrbit
		{
			CVector3f v3OrbitPoint;			// Point in world space that the camera orbits
			float fMinDistanceFromPoint;	// Minimum distance the camera can be from the point
			float fMaxDistanceFromPoint;	// Maximum distance the camera can be from the point
			float fMouseSensitivityX;		// Sensitivity of mouse along it's X axis
			float fMouseSensitivityY;		// Sensitivity of mouse along it's Y axis
			float fMouseSensitivityWheel;	// Sensitivity of mouse for mouse wheel

			// Variables/objects used to compute view matrix
			float fCurrentDistanceFromPoint;	// Current distance the camera is from the point
			CTimer timer;						// Timer used for time delta
			float fAngleY;						// Angle around Y axis
			float fAngleUp;						// Angle up/down
			bool bLimitYupToPositive;			// Does what it says on the tin
		};
		ModeOrbit _modeOrbit;
		
		// Called from update if mode is set to orbit
		void _updateModeOrbit(void);

		// Holds variable used for FPS mode
		struct ModeFPS
		{
			CVector3f v3Position;				// Position in world space of the camera
			CVector3f v3Forward;				// Forward vector
			CVector3f v3Right;					// Right vector
			CVector3f v3Up;						// Up vector
			float fMouseSensitivityX;			// Sensitivity of mouse along it's X axis
			float fMouseSensitivityY;			// Sensitivity of mouse along it's Y axis
			float fSensitivityTranslateForward;	// Movement speed for WS keys
			float fSensitivityTranslateStrafe;	// Movement speed for AD keys
			float fSensitivityTranslateUp;		// Movement speed for RF keys
			float fMultiplierShiftKey;			// Multiplier of movement speed when shift key is held down
			CTimer timer;						// Timer used for time delta
			// Variables used to calculate pitch/yaw
			float fYaw;							// Rotation around Y axis
			float fPitch;						// Rotation around right vector
		};
		ModeFPS _modeFPS;

		// Called from update if mode is set to FPS
		void _updateModeFPS(void);

		CMatrix _matrixView;
		CMatrix _matrixProjection;

		std::string _mstrFramebufferTargetName;		// Name of the framebuffer stored in SCResourceManager which this camera uses to render to.
		bool _mbActive;								// Whether this camera is active or not and thereby rendered or not.
		float _mfFOVdegrees;						// The field of view, used by the scene manager when setting this camera's perspective matrix.
		float _mfZNear, _mfZFar;					// The near and far Z clipping plane distances used by the scene manager when setting this camera's perspective matrix.
	};
}