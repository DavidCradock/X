#pragma once
#include "PCH.h"
#include "resourceBase.h"
#include "image.h"
#include "colour.h"
#include "vector2f.h"

namespace X
{
	class CResourceFramebuffer : public CResourceBase
	{
	public:
		CResourceFramebuffer(unsigned int iWidth, unsigned int iHeight);
		~CResourceFramebuffer();

		// Create all OpenGL context dependent objects when an OpenGL context exists.
		void onGLContextCreated(void);

		// Free all OpenGL context dependent objects before the OpenGL context is destroyed.
		void onGLContextToBeDestroyed(void);

		// Bind this framebuffer object so that all render operations use this as the target.
		// Also clears the render target by default and sets viewport to match render target
		// May also detect current dimensions and resize to the window's dims.
		void bindAsRenderTarget(bool bClearbuffer = true, bool bResizeToWindowDimensions = false, const CColour& backgroundClearColour = CColour(0,0,0,0));

		// Unbind the framebuffer object so that all render operations use the window's backbuffer as the target.
		// Also calls glViewport to reset to the window's dimensions
		void unbindAsRenderTarget(void) const;

		// Bind this framebuffer's attached colour texture to a texture unit, to be used as a normal 2D texture.
		// uiTextureUnit should be between 0-7
		void bindAsTexture(unsigned int uiTextureUnit) const;

		// Unbinds texturing for the specified texture unit
		// uiTextureUnit should be between 0-7
		void unbindTexture(unsigned int uiTextureUnit = 0) const;

		// Returns width of the frame buffer
		unsigned int getWidth(void) const;

		// Returns height of the frame buffer
		unsigned int getHeight(void) const;

		// Returns dimensions of the frame buffer
		CVector2f getDimensions(void) const;

		// Sets frame buffer to new dimensions
		// If the new dimensions are the same as the current ones, this simply returns
		void resize(unsigned int uiNewWidth, unsigned int uiNewHeight);

		// Renders the contents of this buffer to a 2D quad with specified position and dimensions
		void renderTo2DQuad(int iPosX, int iPosY, int iWidth, int iHeight, CColour colour = CColour());

		bool mbNeedsUpdating;	// This is set to true when onGLContextToBeDestroyed() has been called
								// Set the top of CGUITextScroll::render() for where and how this is used.
								// It's a flag that's only used by the GUI objects which need to know if the FB needs updating again.
	private:
		unsigned int _muiWidth;
		unsigned int _muiHeight;
		unsigned int _muiFramebufferID;	// OpenGL ID for the framebuffer
		unsigned int _muiTextureID;		// OpenGL ID for the texture attached to the framebuffer
		unsigned int _muiRenderbufferID;// OpenGL ID for the renderbuffer
	};
}