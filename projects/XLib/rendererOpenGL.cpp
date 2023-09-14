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

	void RendererOpenGL::blendFunction_SrcAlpha_One(void)
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	}

	void RendererOpenGL::blendFunction_SrcAlpha_OneMinusSrcAlpha(void)
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void RendererOpenGL::cullFaceAntiClockwise(void)
	{

	}

	void RendererOpenGL::cullFaceClockwise(void)
	{

	}

	void RendererOpenGL::cullFaceDisable(void)
	{

	}

	void RendererOpenGL::cullFaceEnable(void)
	{

	}

	void RendererOpenGL::depthTestDisable(void)
	{
		glDisable(GL_DEPTH_TEST);
	}

	void RendererOpenGL::depthTestEnable(void)
	{
		glEnable(GL_DEPTH_TEST);
	}

	void RendererOpenGL::pixelsRead(int iPositionX, int iPositionY, int iWidth, int iHeight, void* pixels)
	{

	}

	void RendererOpenGL::pixelsWrite(int iPositionX, int iPositionY, int iWidth, int iHeight, void* pixels)
	{

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