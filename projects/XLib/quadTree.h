#pragma once
#include "PCH.h"
#include "quadTreeNode.h"

namespace X
{
	// This is a 2D spatial partitioning class.
	// For 3D spatial partitioning, see the COctTree class
	// 
	// A quad tree gives us fast retrieval of entities which are within a specified
	// range of a given position aswell as fast retrieval of entities which are within a given area of space.
	// This is great for rendering only visible entities and also for getting a list of entities which
	// can be used to perform collision detection only upon entities which are closeby, instead of having
	// to perform collision for ALL entities within a world.
	// 
	// A quad tree begins with an initial root 2D space (node) with which to insert uniquely named entities 
	// into, specifying their position.
	// The quad tree has a maximum number of entities which can be inserted into a node and once this number
	// is reached, the node is split into one or more (up to four) child nodes and the entities are removed
	// from the parent node and into the child nodes.
	// The position of entities determines which child nodes need to be created and then the entity inserted into.
	// Each of the child nodes have half the dimensions of their parent node and there are four of them.
	//
	// This is a recursive process, so smaller and smaller child nodes of child nodes and so on are created until
	// a "max node depth" value is reached, then what happens is the child nodes no longer get any more child nodes
	// and the entities inserted, ignore the maximum number of entities per node value.
	// The max node depth is computed upon the tree's construction based upon it's dimensions and then again if the 
	// root node of the tree gets resized (due to insertion of an entity outside of the root node's region).
	// The reason we have a max node depth is because, as the nodes get divided, their dimensions get smaller and 
	// smaller, until they reach a dimension of 1x1 and if they were to become zero dimensions, no entities would
	// fit in them and the algorithm would enter an infinite recursive loop, forever creating useless zero sized
	// child nodes and cause a stack overflow.
	//
	// The initial region covered by the root node has to be a multiple of 2, otherwise division of the child nodes
	// may cause certain position to not be covered (For example, if the initial region was (3, 3) in dimensions, when
	// creating the child nodes, their dims would be (1, 1) meaning half the region of the parent node would not
	// be covered and make it so an entity within the uncovered area would not fit.
	// The intial region is set to -1024, +1024 along each axis.
	class CQuadTree
	{
		friend class CQuadTreeNode;
		friend class CQuadTreeEntity;
	public:
		// Constructor
		// iMaxEntitiesPerNode is the maximum number of entities able to be stored
		// within a node before that node will be subdivided again into child nodes.
		// This must be at least 1, otherwise an exception occurs.
		// If we add an entity which is outside of the root node's initial region of
		// -1024 to 1024, then the tree is rebuilt.
		// iRectSizeIncreaseMultiplier is used when the tree is rebuilt. It is the
		// amount to increase the root node's dimensions by until the new entity's
		// position fits. A value of 2 would double the new root node's dimensions
		// each time. It must be at least 2 otherwise an exception occurs.
		CQuadTree(int iMaxEntitiesPerNode = 10, int iRectSizeIncreaseMultiplier = 2);

		// Destructor
		// Deletes the root node, which will delete all children and their children and so on.
		~CQuadTree();

		// Initialise the quad tree using the new given settings.
		// This will free the existing tree and any entities.
		// iMaxEntitiesPerNode is the maximum number of entities able to be stored
		// within a node before that node will be subdivided again into child nodes.
		// This must be at least 1, otherwise an exception occurs.
		// If we add an entity which is outside of the root nodes initial region of
		// -1024 to 1024, then the tree is rebuilt.
		// fRectSizeIncreaseMultiplier is used when the tree is rebuilt. It is the
		// amount to increase the root node's dimensions by until the new entity's
		// position fits. A value of 2 would double the new root node's dimensions
		// each time. It must be at least 2 otherwise an exception occurs.
		void init(int iMaxEntitiesPerNode = 10, int iRectSizeIncreaseMultiplier = 2);

		// Deletes the root node and in turn all of it's children and all entities
		void free(void);

		// Debug rendering of the oct tree's nodes and entities
		void debugRender(const CVector3f& vCameraPosition = CVector3f(0.0f, 0.0f, 0.0f), bool bRenderNodes = true, bool bRenderEntities = true, int iEntityCircleRadius = 8, unsigned int uiEntityCircleNumSegments = 6) const;

		// For debug rendering, sets the named entity's rendered colour
		// If the named entity doesn't exist, an exception occurs.
		void debugSetEntityColour(const std::string& strName, CColour& colour);

		// For debug rendering, sets all entities' rendered colour to the one given
		void debugSetAllEntitiesColour(CColour& colour);

		// Add entity to the quad tree.
		// Each entity needs a unique name, if the name given already exists, an exception occurs.
		// If the specified position is outside of the tree's region, the tree is rebuilt
		void addEntity(const std::string& strName, int iPosX, int iPosY, int iUserData = 0, void *pUserData = 0);

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
		void setEntityPosition(const std::string& strName, int iNewPosX, int iNewPosY);

		// Sets the given ints to the named entity's position.
		// If the named entity doesn't exist, an exception occurs
		void getEntityPosition(const std::string& strName, int &iPosX, int &iPosY) const;

		// Returns a vector of CQuadTreeNodes which holds all nodes which have entities in them
		std::vector<CQuadTreeNode*> getNodesWithEntities(void) const;

		// Returns a vector of CQuadTreeNodes which holds all nodes which intersect with the given rect and have entities
		std::vector<CQuadTreeNode*> getNodesWithEntitiesWhichIntersect(const CRect& rect) const;

		// Returns a vector of entities which are within range of the given position.
		// This may return some entities which are outside of the range, as the test to see
		// whether the entities aren't in range isn't 100% accurate.
		std::vector<CQuadTreeEntity*> getEntitiesWithinRange(int iPosX, int iPosY, int iRange) const;

		// Returns a vector of entities which are within the given CRect
		// This may return some entities which are outside of the range, as the test to see
		// whether the entities aren't in range isn't 100% accurate.
		std::vector<CQuadTreeEntity*> getEntitiesWithinRect(const CRect& rect) const;

		// Returns current node depth stat
		unsigned int getNodeDepthCurrent(void);

		// Returns current node depth stat
		unsigned int getNodeDepthMax(void) const;

	private:
		// Root node of the tree which holds all child nodes and their entities
		CQuadTreeNode* _mpRootNode;

		// Maximum number of entities able to be stored within a node before that node will
		// be subdivided again into child nodes.
		// Set during construction
		int _miMaxEntitiesPerNode;

		// When adding a new entity and it's position doesn't fit within the root
		// node's rect area, the area is increased by this amount until the new
		// entity's position fits. A value of 2 would double the new root node's
		// dimensions each time.
		// Set during construction
		int _miRectSizeIncreaseMultiplier;

		// Hashmap holding pointers to each of the added entities
		// This is used for fast retrieval or removal of single entities
		mutable std::map<std::string, CQuadTreeEntity*> _mmapEntities;

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

		// Based upon _mpRootNode->_mRectRegion, computes the maximum allowable node depth
		// and sets _muiMaxNodeDepth.
		void _computeMaxNodeDepth(void);

	};
}
