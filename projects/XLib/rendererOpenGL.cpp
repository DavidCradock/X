#include "PCH.h"
#include "rendererOpenGL.h"

namespace X
{
	RendererOpenGL::RendererOpenGL()
	{
	}
	
	RendererOpenGL::~RendererOpenGL()
	{

	}

	void RendererOpenGL::blendDisable(void)
	{
		glDisable(GL_BLEND);
	}

	void RendererOpenGL::blendEnable(void)
	{
		glEnable(GL_BLEND);
	}

	void RendererOpenGL::blendFunction(EBlendFunction blendFunc)
	{
		switch (blendFunc)
		{
		case EBlendFunction::SourceAlpha_One:
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			break;
		case EBlendFunction::SourceAlpha_OneMinusSourceAlpha:
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			break;
		}
	}

	void RendererOpenGL::depthTestDisable(void)
	{
		glDisable(GL_DEPTH_TEST);
	}

	void RendererOpenGL::depthTestEnable(void)
	{
		glEnable(GL_DEPTH_TEST);
	}

	void RendererOpenGL::scissorTest(int iPositionX, int iPositionY, int iWidth, int iHeight)
	{
		glScissor(iPositionX, iPositionY, iWidth, iHeight);
	}

	void RendererOpenGL::scissorTestEnable(void)
	{
		glEnable(GL_SCISSOR_TEST);
	}

	void RendererOpenGL::scissorTestDisable(void)
	{
		glDisable(GL_SCISSOR_TEST);
	}

}