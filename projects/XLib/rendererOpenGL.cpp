#include "PCH.h"
#include "rendererOpenGL.h"

namespace X
{
	RendererOpenGL::RendererOpenGL()
	{
		blendEnabled = false;

		scissorTestEnabled = false;
		scissorTestPositionX = 0;
		scissorTestPositionY = 0;
		scissorTestWidth = 0;
		scissorTestHeight = 0;
	}
	
	RendererOpenGL::~RendererOpenGL()
	{

	}

	void RendererOpenGL::blendDisable(void)
	{
		blendEnabled = false;
		glDisable(GL_BLEND);
	}

	void RendererOpenGL::blendEnable(void)
	{
		blendEnabled = true;
		glEnable(GL_BLEND);
	}

	void RendererOpenGL::scissorTestGet(int &positionX, int &positionY, int &width, int &height) const
	{
		positionX = scissorTestPositionX;
		positionY = scissorTestPositionY;
		width = scissorTestWidth;
		height = scissorTestHeight;
	}

	void RendererOpenGL::scissorTestSet(int positionX, int positionY, int width, int height)
	{
		scissorTestPositionX = positionX;
		scissorTestPositionY = positionY;
		scissorTestWidth = width;
		scissorTestHeight = height;
		glScissor((int)scissorTestPositionX, (int)scissorTestPositionY, (int)scissorTestWidth, (int)scissorTestHeight);
	}

	void RendererOpenGL::scissorTestEnable(void)
	{
		scissorTestEnabled = true;
		glEnable(GL_SCISSOR_TEST);
	}

	void RendererOpenGL::scissorTestDisable(void)
	{
		scissorTestEnabled = false;
		glDisable(GL_SCISSOR_TEST);
	}

}