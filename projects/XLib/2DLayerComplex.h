#pragma once
#include "PCH.h"
#include "2DEntity.h"

namespace X
{
	// This is a layer for the complex entity class
	// A layer stores entities and each layer is ordered by rendering order (in the render method) so that we can
	// have some control over which entities get rendered on top of others.
	class C2DLayerComplex
	{
	public:
		friend class SC2DRenderer;
		friend class C2DEntityComplex;
		C2DLayerComplex();
		~C2DLayerComplex();

		// Add a new named object
		// If the object name already exists, an exception occurs
		// strResourceTexture2DAtlasName is the name of the CResourceTexture2DAtlas added to SCResourceManager which contains this entity's image data.
		// If the name of the CResourceTexture2DAtlas doesn't exist, an exception occurs.
		// Returns a pointer to the newly added object
		C2DEntity* addEntity(const std::string& strUniqueName, const std::string& strResourceTexture2DAtlasName);

		// Returns true if an object exists, else false
		bool getEntityExists(const std::string& strUniqueName) const;

		// Returns a pointer to a previously added named object if it exists, else an exception occurs
		C2DEntity* getEntity(const std::string& strUniqueName) const;

		// Returns a pointer to a previously added named object if it exists, else an exception occurs
		C2DEntity* getEntity(unsigned int uiIndex) const;

		// Removes an object
		// If the object's name given doesn't exist, an exception occurs
		void removeEntity(const std::string& strUniqueName);

		// Removes an object
		// If an invalid index is given, an exception occurs
		void removeEntity(unsigned int uiIndex);

		// Removes all objects
		void removeAllEntities(void);

		// Returns the total number of added objects
		unsigned int getNumEntities(void) const;

	private:
		mutable std::map<std::string, C2DEntity*> _mmapEntities;					// Each named entity
	};
}