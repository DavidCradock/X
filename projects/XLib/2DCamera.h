#pragma once
#include "PCH.h"
#include "vector2r.h"

namespace X
{
	// A camera has a position inside a C2DWorld and is used to render it's view of the world into a framebuffer
	// which can then be rendered onto the back buffer or rendered inside a CGUIContainer.
	// A camera also has a "zoom/height" value. This scales sprites and gives the impression of zooming in/out to/from the world.
	class C2DCamera
	{
	public:
		C2DCamera();
		~C2DCamera();

		// Returns the camera position in the world
		glm::vec3 getPositionGLM(void) const;

		CVector2r mv2rPosition;	// Position of the camera in the world
		std::string _mstrFramebufferTarget;	// The name of the CFramebuffer resource located in the SCResourceManager which this camera uses to render to.
											// By default, this is set to "X:backbuffer_FB"

		// Sets whether this camera is enabled and rendering to it's framebuffer or not.
		// By default, they are enabled
		void setEnabled(bool bEnabled);

		// Sets whether this camera is enabled and rendering to it's framebuffer or not.
		bool getEnabled(void) const;
	private:
		bool _mbEnabled;					// Whether this camera is enabled and rendering to it's framebuffer  or not
	};
}