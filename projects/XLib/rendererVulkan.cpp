#include "PCH.h"
#include "RendererVulkan.h"

namespace X
{
	RendererVulkan::RendererVulkan()
	{
	}

	RendererVulkan::~RendererVulkan()
	{

	}

	void RendererVulkan::blendDisable(void)
	{
		glDisable(GL_BLEND);
	}

	void RendererVulkan::blendEnable(void)
	{
		glEnable(GL_BLEND);
	}

	void RendererVulkan::blendFunction_SrcAlpha_One(void)
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	}

	void RendererVulkan::blendFunction_SrcAlpha_OneMinusSrcAlpha(void)
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void RendererVulkan::cullFaceAntiClockwise(void)
	{

	}

	void RendererVulkan::cullFaceClockwise(void)
	{

	}

	void RendererVulkan::cullFaceDisable(void)
	{

	}

	void RendererVulkan::cullFaceEnable(void)
	{

	}

	void RendererVulkan::depthTestDisable(void)
	{
		glDisable(GL_DEPTH_TEST);
	}

	void RendererVulkan::depthTestEnable(void)
	{
		glEnable(GL_DEPTH_TEST);
	}

	void RendererVulkan::pixelsRead(int iPositionX, int iPositionY, int iWidth, int iHeight, void* pixels)
	{

	}

	void RendererVulkan::pixelsWrite(int iPositionX, int iPositionY, int iWidth, int iHeight, void* pixels)
	{

	}

	void RendererVulkan::scissorTest(int iPositionX, int iPositionY, int iWidth, int iHeight)
	{
		glScissor(iPositionX, iPositionY, iWidth, iHeight);
	}

	void RendererVulkan::scissorTestEnable(void)
	{
		glEnable(GL_SCISSOR_TEST);
	}

	void RendererVulkan::scissorTestDisable(void)
	{
		glDisable(GL_SCISSOR_TEST);
	}

}