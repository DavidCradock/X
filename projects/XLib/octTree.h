#pragma once
#include "PCH.h"
#include "octTreeNode.h"
#include "SMCamera.h"
#include "frustum.h"

namespace X
{
	// This is a 3D spatial partitioning class.
	// For 2D spatial partitioning, see the CQuadTree class
	// 
	// An oct tree gives us fast retrieval of entities which are within a specified
	// range of a given position aswell as fast retrieval of entities which are within a given area of space.
	// This is great for rendering only visible entities and also for getting a list of entities which
	// can be used to perform collision detection only upon entities which are closeby, instead of having
	// to perform collision for ALL entities within a world.
	// 
	// An oct tree begins with an initial root 3D space (node) with which to insert uniquely named entities 
	// into, specifying their position.
	// The oct tree has a maximum number of entities which can be inserted into a node and once this number
	// is reached, the node is split into one or more (up to eight) child nodes and the entities are removed
	// from the parent node and into the child nodes.
	// The position of entities determines which child nodes need to be created and then the entity inserted into.
	// Each of the child nodes have half the dimensions of their parent node and there are eight of them.
	//
	// This is a recursive process, so smaller and smaller child nodes of child nodes and so on are created until
	// a "max node depth" value is reached, then what happens is the child nodes no longer get any more child nodes
	// and the entities inserted, ignore the maximum number of entities per node value.
	// The max node depth is computed upon the tree's construction based upon it's dimensions and then again if the 
	// root node of the tree gets resized (due to insertion of an entity outside of the root node's region).
	// The reason we have a max node depth is because, as the nodes get divided, their dimensions get smaller and 
	// smaller, until they reach a dimension of 1x1x1 and if they were to become zero dimensions, no entities would
	// fit in them and the algorithm would enter an infinite recursive loop, forever creating useless zero sized
	// child nodes and cause a stack overflow.
	//
	// The intial region is set to -8, +8 along each axis. 
	class COctTree
	{
		friend class COctTreeNode;
		friend class COctTreeEntity;
	public:
		// Constructor
		// iMaxEntitiesPerNode is the maximum number of entities able to be stored
		// within a node before that node will be subdivided again into child nodes.
		// This must be at least 1, otherwise an exception occurs.
		// If we add an entity which is outside of the root node's initial region
		// then the tree is rebuilt.
		// fSizeIncreaseMultiplier is used when the tree is rebuilt. It is the
		// amount to increase the root node's dimensions by until the new entity's
		// position fits. A value of 2 would double the new root node's dimensions
		// each time. It must be at least 2 otherwise an exception occurs.
		COctTree(int iMaxEntitiesPerNode = 10, float fSizeIncreaseMultiplier = 2.0f);

		// Destructor
		// Deletes the root node, which will delete all children and their children and so on.
		~COctTree();

		// Initialise the oct tree using the new given settings.
		// This will free the existing tree and any entities.
		// iMaxEntitiesPerNode is the maximum number of entities able to be stored
		// within a node before that node will be subdivided again into child nodes.
		// This must be at least 1, otherwise an exception occurs.
		// If we add an entity which is outside of the root node's initial region
		// then the tree is rebuilt.
		// fSizeIncreaseMultiplier is used when the tree is rebuilt. It is the
		// amount to increase the root node's dimensions by until the new entity's
		// position fits. A value of 2 would double the new root node's dimensions
		// each time. It must be at least 2 otherwise an exception occurs.
		void init(int iMaxEntitiesPerNode = 10, float fSizeIncreaseMultiplier = 2.0f);

		// Deletes the root node and in turn all of it's children and all entities
		void free(void);

		
		// Debug rendering of the oct tree's nodes
		void debugRender(CSMCamera& camera) const;
		/*
		// For debug rendering, sets the named entity's rendered colour
		// If the named entity doesn't exist, an exception occurs.
		void debugSetEntityColour(const std::string& strName, CColour& colour);

		// For debug rendering, sets all entities' rendered colour to the one given
		void debugSetAllEntitiesColour(CColour& colour);
		*/

		// Add entity to the oct tree.
		// Each entity needs a unique name, if the name given already exists, an exception occurs.
		// If the specified position is outside of the tree's region, the tree is rebuilt
		void addEntity(const std::string& strName, const CVector3f& vPosition, int iUserData = 0, void* pUserData = 0);

		// Removes the named entity from the tree.
		// If the unique name doesn't exist, an exception occurs.
		// To determine whether an entity exists, use getEntityExists()
		void removeEntity(const std::string& strName);

		// Returns whether the named entity exists or not
		bool getEntityExists(const std::string& strName) const;

		// Removes all entities from the tree and depending upon the passed bool, resets the tree to contain
		// just the root node.
		void removeAllEntities(bool bResetTree = false);

		// Set an existing entity's position to the one given, moving to the correct node if needed.
		// If the named entity doesn't exist, an exception occurs
		void setEntityPosition(const std::string& strName, const CVector3f& vNewPosition);

		// Sets the given vector to the named entity's position.
		// If the named entity doesn't exist, an exception occurs
		void getEntityPosition(const std::string& strName, CVector3f &vPosition) const;

		// Returns a vector of COctTreeNodes which holds all nodes which have entities in them
		std::vector<COctTreeNode*> getNodesWithEntities(void) const;

		// Returns a vector of COctTreeNodes which holds all nodes which intersect with the given AABB and have entities
		std::vector<COctTreeNode*> getNodesWithEntitiesWhichIntersect(const CAABB& aabb) const;

		// Returns a vector of COctTreeNodes which holds all nodes which intersect with the given CFrustum and have entities
		std::vector<COctTreeNode*> getNodesWithEntitiesWhichIntersect(const CFrustum& frustum) const;

		// Returns a vector of entities which are within range of the given position.
		// This may return some entities which are outside of the range, as the test to see
		// whether the entities aren't in range isn't 100% accurate.
		std::vector<COctTreeEntity*> getEntitiesWithinRange(const CVector3f& vPosition, float fRange) const;

		// Returns a vector of entities which are within the given CAABB
		// This may return some entities which are outside of the range, as the test to see
		// whether the entities aren't in range isn't 100% accurate.
		std::vector<COctTreeEntity*> getEntitiesWithinAABB(const CAABB& aabb) const;

		// Returns a vector of entities which are within the given CFrustum
		// TODO check this
		std::vector<COctTreeEntity*> getEntitiesWithinFrustum(const CFrustum& frustum) const;

		// Returns current node depth stat
		unsigned int getNodeDepthCurrent(void);

		// Returns current node depth stat
		unsigned int getNodeDepthMax(void) const;

	private:
		// Root node of the tree which holds all child nodes and their entities
		COctTreeNode* _mpRootNode;

		// Maximum number of entities able to be stored within a node before that node will
		// be subdivided again into child nodes.
		// Set during construction
		int _miMaxEntitiesPerNode;

		// When adding a new entity and it's position doesn't fit within the root
		// node's area, the area is increased by this amount until the new
		// entity's position fits. A value of 2 would double the new root node's
		// dimensions each time.
		// Set during construction
		float _mfSizeIncreaseMultiplier;

		// Hashmap holding pointers to each of the added entities
		// This is used for fast retrieval or removal of single entities
		mutable std::map<std::string, COctTreeEntity*> _mmapEntities;

		// Holds the current maximum depth of the nodes.
		// If there are no child nodes, this would be zero.
		// It's used by the debug rendering code to colour the nodes accordingly.
		// It's also used to determine if the tree has reached the maximum child
		// division depth, in which case, additionally added entities no longer
		// subdivide a node into children and instead simply add the new entity
		// into the node, ignoring the _miMaxEntitiesPerNode value.
		unsigned int _muiCurrentMaxNodeDepth;

		// Holds the maximum depth of child nodes.
		// If adding an entity into a node, that node's depth is equal to this value,
		// then _miMaxEntitiesPerNode is ignored and the new entity is simply added
		// to the node, instead of trying to sub-divide the node further.
		// This is to prevent node dimensions of less than 1x1 which would cause
		// infinite sub-division of nodes and cause a stack overflow.
		// This value is computed upon construction and whenever the root node's
		// dimensions are increased.
		unsigned int _muiMaxNodeDepth;

		// Based upon _mpRootNode->_mRegion, computes the maximum allowable node depth
		// and sets _muiMaxNodeDepth.
		void _computeMaxNodeDepth(void);

	};
}
