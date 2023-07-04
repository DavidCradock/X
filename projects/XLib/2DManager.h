#pragma once
#include "PCH.h"
#include "singleton.h"

namespace X
{
	class SC2DManager : public CSingleton<SC2DManager>
	{
	public:
		SC2DManager();
		~SC2DManager();

	};
}