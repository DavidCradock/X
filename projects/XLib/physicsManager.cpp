#include "PCH.h"
#include "physicsManager.h"
#include "logging.h"

namespace X
{
	SCPhysicsManager::SCPhysicsManager()
	{
		SCLog* pLog = SCLog::getPointer();
		pLog->add("SCPhysicsManager::SCPhysicsManager() called.");
	}
}