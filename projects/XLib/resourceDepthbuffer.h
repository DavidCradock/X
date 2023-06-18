#pragma once
#include "PCH.h"
#include "resourceBase.h"

namespace X
{
	class ResourceDepthbuffer : public ResourceBase
	{
	public:
		ResourceDepthbuffer(unsigned int iWidth, unsigned int iHeight);
		~ResourceDepthbuffer();

		// Create all OpenGL context dependent objects when an OpenGL context exists.
		void onGLContextCreated(void);

		// Free all OpenGL context dependent objects before the OpenGL context is destroyed.
		void onGLContextToBeDestroyed(void);

		// Bind this framebuffer object so that all render operations use this as the target.
		void bindAsRenderTarget(void);

		// Unbind the framebuffer object so that all render operations use the window's backbuffer as the target.
		void unbindAsRenderTarget(void);

		// Bind this framebuffer's attached colour texture to a texture unit, to be used as a normal 2D texture.
		// uiTextureUnit should be between 0-7
		void bindAsTexture(unsigned int uiTextureUnit);

		// Unbinds texturing for the specified texture unit
		// uiTextureUnit should be between 0-7
		void unbindTexture(unsigned int uiTextureUnit = 0);

		unsigned int _muiWidth;
		unsigned int _muiHeight;
		unsigned int _muiFramebufferID;	// OpenGL ID for the framebuffer
		unsigned int _muiTextureID;		// OpenGL ID for the texture attached to the framebuffer
	};
}