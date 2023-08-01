#pragma once
#include "PCH.h"
#include "vector3f.h"
#include "colour.h"

namespace X
{
	class COctTreeNode;

	// An entity which is assigned into an COctTreeNode
	// It contains it's unique name, it's position within the world and the node it belongs to.
	class COctTreeEntity
	{
		friend class COctTree;
		friend class COctTreeNode;
	public:
		// Constructor.
		// strName is the unique name given to this entity.
		// vPosition is this entity's position within the world
		COctTreeEntity(const std::string& strName, const CVector3f& vPosition, COctTreeNode* pNodeOwner, int iUserData = 0, void* pUserData = 0);

		// Set the debug colour of the entity
		void debugSetColour(CColour& colour);

		// Returns position of entity
		CVector3f getPosition(void);

		// Returns name of the entity
		std::string getName(void);

		// Below are members which may be set to store various information.
		// They have nothing to do with the oct tree itself.
		int miUserData;
		void* mpUserData;

	private:
		std::string _mstrName;			// Unique name of this entity
		CVector3f _mvPosition;			// Position of this entity
		COctTreeNode* _mpNodeOwner;		// The node this entity is in
		CColour _mColDebug;				// The colour used when debug rendering this entity
	};
}
