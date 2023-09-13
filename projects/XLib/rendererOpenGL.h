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

		void scissorTestGet(int &positionX, int &positionY, int &width, int &height) const;
		void scissorTestSet(int positionX, int positionY, int width, int height);
		void scissorTestDisable(void);
		void scissorTestEnable(void);

	private:

		bool blendEnabled;

		bool scissorTestEnabled;
		int scissorTestPositionX;
		int scissorTestPositionY;
		int scissorTestWidth;
		int scissorTestHeight;
	};
}