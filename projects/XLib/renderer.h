#pragma once
#include "PCH.h"

namespace X
{
	class Renderer
	{
	public:

		enum EBlendFunction
		{
			SourceAlpha_One,
			SourceAlpha_OneMinusSourceAlpha
		};

		virtual void blendDisable(void) = 0;
		virtual void blendEnable(void) = 0;
		virtual void blendFunction(EBlendFunction blendFunc) = 0;
		
		virtual void depthTestDisable(void) = 0;
		virtual void depthTestEnable(void) = 0;
		
		virtual void scissorTest(int positionX, int positionY, int width, int height) = 0;
		virtual void scissorTestDisable(void) = 0;
		virtual void scissorTestEnable(void) = 0;
		

	};
}