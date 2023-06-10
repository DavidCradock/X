#pragma once
#include "PCH.h"

namespace X
{

	// A camera which holds view and projection matricies
	// By default, both are set to identity matrixs
	class Camera
	{
	public:
		Camera();

		// Sets the projection matrix of the camera as orthographical
		// The parameters are the dimensions of whatever we're rendering to.
		// Passing the window's dims is a good idea, or perhaps the dims of a framebuffer.
		void setProjectionAsOrthographic(float fWidth, float fHeight);

		// Sets the projection matrix of the camera as perspective
		// The parameters are the dimensions of whatever we're rendering to.
		// Passing the window's dims is a good idea, or perhaps the dims of a framebuffer.
		void setProjectionAsPerspective(float fFOVdegrees, float fWidth, float fHeight, float fZNear, float fZFar);

		// Sets the view matrix as identity
		void setViewAsIdentity(void);

		// Sets the view matrix via lookat
		void setViewAsLookat(glm::vec3 vCameraPosition, glm::vec3 vCameraTargetPosition, glm::vec3 vUpVector = glm::vec3(0.0f,0.0f,1.0f));

		// Multiplies the view and projection matrixs together and returns the result
		glm::mat4 getViewProjectionMatrix(void);

		glm::mat4 matrixView;
		glm::mat4 matrixProjection;

	};
}