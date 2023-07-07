#pragma once
#include "PCH.h"
#include "vector2r.h"

namespace X
{
	class C2DCamera
	{
	public:
		C2DCamera();
		~C2DCamera();

		// Returns the camera position in the world
		glm::vec3 getPositionGLM(void);

		CVector2r mv2rPosition;	// Position of the camera in the world
	};
}