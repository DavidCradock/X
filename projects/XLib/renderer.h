#pragma once
#include "PCH.h"

namespace X
{
	class Renderer
	{
	public:
		virtual void blendDisable(void) = 0;
		virtual void blendEnable(void) = 0;
		
		virtual void scissorTestGet(int& positionX, int& positionY, int& width, int& height) const = 0;
		virtual void scissorTestSet(int positionX, int positionY, int width, int height) = 0;
		virtual void scissorTestDisable(void) = 0;
		virtual void scissorTestEnable(void) = 0;
		

	};
}