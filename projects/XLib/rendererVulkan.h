#pragma once
#include "PCH.h"
#include "renderer.h"

namespace X
{
	class RendererVulkan : public Renderer
	{
	public:
		RendererVulkan();
		~RendererVulkan();

		void blendDisable(void);
		void blendEnable(void);
		void blendFunction_SrcAlpha_One(void);
		void blendFunction_SrcAlpha_OneMinusSrcAlpha(void);

		void cullFaceAntiClockwise(void);
		void cullFaceClockwise(void);
		void cullFaceDisable(void);
		void cullFaceEnable(void);

		void depthTestDisable(void);
		void depthTestEnable(void);

		void pixelsRead(int iPositionX, int iPositionY, int iWidth, int iHeight, void* pixels);
		void pixelsWrite(int iPositionX, int iPositionY, int iWidth, int iHeight, void* pixels);

		void scissorTest(int iPositionX, int iPositionY, int iWidth, int iHeight);
		void scissorTestDisable(void);
		void scissorTestEnable(void);

	private:

	};
}