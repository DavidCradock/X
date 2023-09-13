#pragma once
#include "PCH.h"
#include "renderer.h"

namespace X
{
	class RendererOpenGL : public Renderer
	{
	public:
		RendererOpenGL();
		~RendererOpenGL();

		void blendDisable(void);
		void blendEnable(void);
		void blendFunction(EBlendFunction blendFunc);

		void depthTestDisable(void);
		void depthTestEnable(void);

		void scissorTest(int iPositionX, int iPositionY, int iWidth, int iHeight);
		void scissorTestDisable(void);
		void scissorTestEnable(void);

	private:

	};
}