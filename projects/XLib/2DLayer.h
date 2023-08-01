#pragma once
#include "PCH.h"
#include "2DEntity.h"
#include "2DEntityComplex.h"

namespace X
{
	// A layer stores entities and each layer is ordered by rendering order (in the C2DWorld objects) so that we can
	// have some control over which entities get rendered on top of others.
	class C2DLayer
	{
	public:
		friend class SC2DRenderer;
		
		C2DLayer();
		~C2DLayer();

		// Add a new named object
		// NOTE: Entities using different atlases, should NOT be placed on the same layer as this increases texture bindings.
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

		// Add a new named object
		// If the object name already exists, an exception occurs
		// Returns a pointer to the newly added object
		C2DEntityComplex* addEntityComplex(const std::string& strUniqueName);

		// Returns true if an object exists, else false
		bool getEntityComplexExists(const std::string& strUniqueName) const;

		// Returns a pointer to a previously added named object if it exists, else an exception occurs
		C2DEntityComplex* getEntityComplex(const std::string& strUniqueName) const;

		// Returns a pointer to a previously added named object if it exists, else an exception occurs
		C2DEntityComplex* getEntityComplex(unsigned int uiIndex) const;

		// Removes an object
		// If the object's name given doesn't exist, an exception occurs
		void removeEntityComplex(const std::string& strUniqueName);

		// Removes an object
		// If an invalid index is given, an exception occurs
		void removeEntityComplex(unsigned int uiIndex);

		// Removes all objects
		void removeAllEntityComplexs(void);

		// Returns the total number of added objects
		int getNumEntityComplexs(void) const;

		// Sets whether this layer is rendered or not.
		// By default, they are visible
		void setVisible(bool bVisible);

		// Returns whether this layer is currently set to be visible and rendered or not
		bool getVisible(void) const;
	private:
		mutable std::map<std::string, C2DEntity*> _mmapEntities;				// Each named entity
		mutable std::map<std::string, C2DEntityComplex*> _mmapEntityComplexs;	// Each named entity complex
		bool _mbVisible;														// Whether this layer is visible and rendered or not

		// This is the quad tree for this layer
		// Entities, when created are given a pointer to their layer so they can 
		// modify their positions in this quad tree
		//CQuadTree _mQuadTree;
	};
}