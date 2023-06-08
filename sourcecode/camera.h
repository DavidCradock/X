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
		// Passing the window's dims is a good example.
		void setProjectionAsOrthographic(float fWidth, float fHeight);

		// Sets the view matrix as identity
		void setViewAsIdentity(void);

		// Sets the view matrix via lookat
		//void setViewAsLookat(glm::vec3 &vCameraPosition, glm::vec3 &vCameraTargetPosition)
		glm::mat4 matrixView;
		glm::mat4 matrixProjection;

	};
}