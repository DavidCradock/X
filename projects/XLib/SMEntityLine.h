#pragma once
#include "PCH.h"
#include "SMEntityBase.h"

namespace X
{
	// This is an entity which is inserted into the various scene managers.
	// It's derived from the SceneManagerEntityBase class which holds
	// an entity's position, rotation and scale within the scene manager. Along with various helper methods to move and rotate the thing.
	// This entity holds the name of a vertex buffer line and texture names, each located inside the SCResourceManager class.
	// We have a default texture map stored in the resource manager which are used if the texture string is not set.
	// They are added from the SCApplicationManager::addDefaultResources() method.
	// It is named "X:default_white"
	// This is usefull if we do not need to use them.
	class CSMEntityLine : public CSMEntityBase
	{
	public:
		CSMEntityLine();

		std::string mstrLineName;		// Name of the line resource in the resource manager used to render this entity
		std::string mstrTextureName;	// Name of the texture in the resource manager used to render this entity for colour
		
	};
}