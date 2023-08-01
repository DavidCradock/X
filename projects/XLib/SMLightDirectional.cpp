#include "PCH.h"
#include "SMLightDirectional.h"

namespace X
{
	CSMLightDirectional::CSMLightDirectional()
	{
		mvDirection.set(0.0f, -1.0f, 0.0f);
		mvColour.set(1.0f, 1.0f, 1.0f, 1.0f);
	}
}