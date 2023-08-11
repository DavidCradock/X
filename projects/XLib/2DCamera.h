#pragma once
#include "PCH.h"
#include "vector2f.h"

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
		CVector2f getPosition(void) const;

		// Sets the camera position in the world
		void setPosition(const CVector2f& vPosition);

		// Sets the camera position in the world
		void setPosition(float fPositionX, float fPositionY);

		// Sets whether this camera is enabled and rendering to it's framebuffer or not.
		bool getEnabled(void) const;

		// Sets whether this camera is enabled and rendering to it's framebuffer or not.
		// By default, they are enabled
		void setEnabled(bool bEnabled);

		// Returns a std::string holding the name of the framebuffer located in SCResourceManager which the camera uses to render to.
		// By default, this is set to "X:backbuffer"
		std::string getFramebufferTargetName(void) const;

		// Sets the name of the framebuffer located in SCResourceManager which the camera uses to render to.
		// By default, this is set to "X:backbuffer"
		void setFramebufferTargetName(const std::string& strFramebufferTargetName);

	private:
		bool _mbEnabled;					// Whether this camera is enabled and rendering to it's framebuffer or not

		// The name of the CFramebuffer resource located in the SCResourceManager which this camera uses to render to.
		// By default, this is set to "X:backbuffer"
		std::string _mstrFramebufferTarget;
		
		CVector2f _mv2fPosition;	// Position of the camera in the world
	};
}