#pragma once
#include "PCH.h"
#include "resourceBase.h"

namespace X
{
	class ResourceFramebuffer : public ResourceBase
	{
	public:
		ResourceFramebuffer(unsigned int iWidth, unsigned int iHeight);
		~ResourceFramebuffer();

		// Create all OpenGL context dependent objects when an OpenGL context exists.
		void onGLContextCreated(void);

		// Free all OpenGL context dependent objects before the OpenGL context is destroyed.
		void onGLContextToBeDestroyed(void);

		// Bind this framebuffer object so that all render operations use this as the target.
		// Also clears the render target by default and sets viewport to match render target
		void bindAsRenderTarget(bool bClearbuffer = true);

		// Unbind the framebuffer object so that all render operations use the window's backbuffer as the target.
		// Also calls glViewport to reset to the window's dimensions
		void unbindAsRenderTarget(void);

		// Bind this framebuffer's attached colour texture to a texture unit, to be used as a normal 2D texture.
		// uiTextureUnit should be between 0-7
		void bindAsTexture(unsigned int uiTextureUnit);

		// Unbinds texturing for the specified texture unit
		// uiTextureUnit should be between 0-7
		void unbindTexture(unsigned int uiTextureUnit = 0);

		// Returns width of the frame buffer
		unsigned int getWidth(void);

		// Returns height of the frame buffer
		unsigned int getHeight(void);

		// Sets frame buffer to new dimensions
		void resize(unsigned int uiNewWidth, unsigned int uiNewHeight);

		// Renders the contents of this buffer to the backbuffer
		void renderToBackbuffer(unsigned int uiPosX, unsigned int uiPosY, unsigned int uiWidth, unsigned int uiHeight);
	private:
		unsigned int _muiWidth;
		unsigned int _muiHeight;
		unsigned int _muiFramebufferID;	// OpenGL ID for the framebuffer
		unsigned int _muiTextureID;		// OpenGL ID for the texture attached to the framebuffer
		unsigned int _muiRenderbufferID;// OpenGL ID for the renderbuffer
	};
}