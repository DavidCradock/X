#pragma once
#include "PCH.h"
#include "singleton.h"

namespace X
{
	// Physics manager.
	// The physics engine here is a rigid-body, iterative, impulse-based engine.
	class SCPhysicsManager : public CSingleton<SCPhysicsManager>
	{
	public:
		SCPhysicsManager();

	};
}