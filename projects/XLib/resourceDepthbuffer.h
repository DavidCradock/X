#pragma once
#include "PCH.h"
#include "resourceBase.h"
#include "vector2f.h"
#include "colour.h"

namespace X
{
	class CResourceDepthbuffer : public CResourceBase
	{
	public:
		CResourceDepthbuffer(unsigned int iWidth, unsigned int iHeight);
		~CResourceDepthbuffer();

		// Create all OpenGL context dependent objects when an OpenGL context exists.
		void onGLContextCreated(void);

		// Free all OpenGL context dependent objects before the OpenGL context is destroyed.
		void onGLContextToBeDestroyed(void);

		// Bind this framebuffer object so that all render operations use this as the target.
		// Also clears the render target by default and sets viewport to match render target
		void bindAsRenderTarget(bool bClearbuffer = true) const;

		// Unbind the framebuffer object so that all render operations use the window's backbuffer as the target.
		// Also calls glViewport to reset to the window's dimensions
		void unbindAsRenderTarget(void) const;

		// Bind this framebuffer's attached colour texture to a texture unit, to be used as a normal 2D texture.
		// uiTextureUnit should be between 0-7
		void bindAsTexture(unsigned int uiTextureUnit) const;

		// Unbinds texturing for the specified texture unit
		// uiTextureUnit should be between 0-7
		void unbindTexture(unsigned int uiTextureUnit = 0) const;

		// Returns width of the depth buffer
		unsigned int getWidth(void) const;

		// Returns height of the depth buffer
		unsigned int getHeight(void) const;

		// Returns dimensions of the depth buffer
		CVector2f getDimensions(void) const;

		// Sets depth buffer to new dimensions
		void resize(unsigned int uiNewWidth, unsigned int uiNewHeight);

		// Renders the contents of this buffer to a 2D quad with specified position and dimensions
		void renderTo2DQuad(int iPosX, int iPosY, int iWidth, int iHeight, const CColour& colour = CColour(1.0f, 1.0f, 1.0f, 1.0f));
	private:
		unsigned int _muiWidth;
		unsigned int _muiHeight;
		unsigned int _muiFramebufferID;	// OpenGL ID for the framebuffer
		unsigned int _muiTextureID;		// OpenGL ID for the texture attached to the framebuffer
	};
}