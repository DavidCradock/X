#pragma once
#include "PCH.h"
#include "timer.h"

namespace X
{

	// A camera which holds view and projection matricies
	// We can simply use the various methods to set the view and projection matrixs, access them directly or we can use a camera mode
	// A camera mode is set with a call to one of the various setMode??() methods and then call update() once per program loop to compute 
	// the view matrix depending upon which mode is currently set
	class CSMCamera
	{
	public:
		CSMCamera();

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
		void setViewAsLookat(glm::vec3 vCameraPosition, glm::vec3 vCameraTargetPosition, glm::vec3 vUpVector = glm::vec3(0.0f,1.0f,0.0f));

		// Multiplies the view and projection matrixs together and returns the result
		glm::mat4 getViewProjectionMatrix(void) const;

		// Sets the camera mode to orbit a point in 3D world space
		// Call update() once per program loop to update the view matrix with this mode in mind.
		void setModeOrbit(
			glm::vec3 vOrbitPoint = glm::vec3(0.0f, 0.0f, 0.0f),
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
			glm::vec3 vInitialPosition = glm::vec3(0.0f, 1.0f, 0.0f),
			float fInitialYawDegrees = 0.0f,
			float fInitialPitchDegrees = 0.0f,
			float fSensitivityX = 0.5f,
			float fSensitivityY = 0.2f,
			float fSensitivityTranslateForward = 1.0f,
			float fSensitivityTranslateStrafe = 1.0f,
			float fSensitivityTranslateUp = 1.0f,
			float fMultiplierShiftKey = 10.0f);

		// Updates this camera
		void update(void);

		glm::mat4 matrixView;
		glm::mat4 matrixProjection;

		enum Mode
		{
			FPS,
			Orbit,
			None
		};
		Mode mMode;	// Which mode is currently set

		// Holds variables used for the orbit mode
		struct ModeOrbit
		{
			glm::vec3 v3OrbitPoint;			// Point in world space that the camera orbits
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
		ModeOrbit modeOrbit;
		
		// Called from update if mode is set to orbit
		void _updateModeOrbit(void);

		// Holds variable used for FPS mode
		struct ModeFPS
		{
			glm::vec3 v3Position;				// Position in world space of the camera
			glm::vec3 v3Forward;				// Forward vector
			glm::vec3 v3Right;					// Right vector
			glm::vec3 v3Up;						// Up vector
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
		ModeFPS modeFPS;

		// Called from update if mode is set to FPS
		void _updateModeFPS(void);
	};
}