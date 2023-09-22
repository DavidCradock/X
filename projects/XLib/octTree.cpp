#include "PCH.h"
#include "octTree.h"
#include "singletons.h"
#include "logging.h"

namespace X
{
	COctTree::COctTree(int iMaxEntitiesPerNode, float fSizeIncreaseMultiplier)
	{
		_mpRootNode = 0;
		init(iMaxEntitiesPerNode, fSizeIncreaseMultiplier);
	}

	void COctTree::init(int iMaxEntitiesPerNode, float fSizeIncreaseMultiplier)
	{
		free();

		CAABB aabbInitialRootNodeRegion(CVector3f(-8, -8, -8), CVector3f(8, 8, 8));

		// Attempt to create root node, passing 0 as the pointer to the parent node
		_mpRootNode = new COctTreeNode(aabbInitialRootNodeRegion, 0, this);
		ThrowIfFalse(_mpRootNode, "COctTree::init() failed. Unable to allocate memory for root node.");

		// Make sure valid values were given
		ThrowIfTrue(iMaxEntitiesPerNode < 1, "COctTree::init() failed. Given invalid number for iMaxEntitiesPerNode. Must be at least one.");
		ThrowIfTrue(fSizeIncreaseMultiplier < 2, "COctTree::init() failed. Given invalid number for fSizeIncreaseMultiplier. Must be at least 2.0f.");

		// Store settings
		_miMaxEntitiesPerNode = iMaxEntitiesPerNode;
		_mfSizeIncreaseMultiplier = fSizeIncreaseMultiplier;

		// Current maximum node depth
		_muiCurrentMaxNodeDepth = 0;

		// Compute maximum node depth before node division is forbidden.
		_computeMaxNodeDepth();
	}

	COctTree::~COctTree()
	{
		free();
	}

	void COctTree::free(void)
	{
		// Delete root node, which will delete all children and their children and so on.
		// Although this obviously removes the entities from the nodes, because the nodes themselves
		// no longer exist, this does NOT delete the entity pointers. They are stored in this object's
		// _mmapEntities hashmap.
		if (_mpRootNode)
		{
			delete _mpRootNode;
			_mpRootNode = 0;
		}

		// Delete all entities
		std::map<std::string, COctTreeEntity*>::iterator it = _mmapEntities.begin();
		while (it != _mmapEntities.end())
		{
			delete it->second;
			it++;
		}
		_mmapEntities.clear();
	}

	void COctTree::debugRender(CSMCamera& camera) const
	{
		// Obtain required resources needed to render the node's as lines.
		CResourceVertexBufferLine* pLine = x->pResource->getVertexBufferLine("X:default");
		CResourceShader* pShader = x->pResource->getShader("X:VBCPT");
		CResourceTexture2DFromFile* pTexture = x->pResource->getTexture2DFromFile("X:default_white");

		CMatrix matrixWorld;

		// Bind shader and set uniforms
		pShader->bind();
		pShader->setMat4("matrixWorld", matrixWorld);
		pShader->setMat4("matrixView", camera.getViewMatrix());
		pShader->setMat4("matrixProjection", camera.getProjectionMatrix());

		// Tell OpenGL, for each sampler, to which texture unit it belongs to
		pShader->setInt("texture0", 0);

		// And bind the texture...
		pTexture->bind();

		// Set the line vertex buffer rendering mode
		pLine->setDrawModeAsLineList();
		pLine->removeGeom();

		glEnable(GL_BLEND);
//		glDisable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Call the root node's debugRender method which will render itself and it's children recursively
		_mpRootNode->debugRenderNodes(pLine, CColour(1.0f, 1.0f, 1.0f, 1.0f));

		// Send geometry to be rendered
		pLine->update();
		pLine->render();

		// Cleanup
		glDisable(GL_BLEND);
//		glEnable(GL_DEPTH_TEST);
		pTexture->unbind();
		pShader->unbind();
	}
	/*
	void COctTree::debugSetEntityColour(const std::string& strName, CColour& colour)
	{
		// Find the entity
		std::map<std::string, COctTreeEntity*>::iterator it = _mmapEntities.find(strName);
		ThrowIfTrue(_mmapEntities.end() == it, "COctTree::debugSetEntityColour() failed. The entity name of " + strName + " doesn't exist.");
		it->second->_mColDebug = colour;
	}

	void COctTree::debugSetAllEntitiesColour(CColour& colour)
	{
		std::map<std::string, COctTreeEntity*>::iterator it = _mmapEntities.begin();
		while (it != _mmapEntities.end())
		{
			it->second->_mColDebug = colour;
			it++;
		}
	}
	*/
	void COctTree::addEntity(const std::string& strName, const CVector3f& vPosition, int iUserData, void* pUserData)
	{
		// Make sure the entity doesn't already exist by checking the hashmap
		std::map<std::string, COctTreeEntity*>::iterator it = _mmapEntities.find(strName);
		ThrowIfTrue(_mmapEntities.end() != it, "COctTree::addEntity() failed. The entity name of " + strName + " already exists.");

		// Create new entity, setting it's owner to 0
		COctTreeEntity* pEntity = new COctTreeEntity(strName, vPosition, 0, iUserData, pUserData);
		ThrowIfFalse(pEntity, "COctTree::addEntity() failed to allocate memory for new entity.");

		// Add entity to hashmap for fast lookup
		_mmapEntities[strName] = pEntity;

		// Determine whether the given position of the entity fits in the root node
		if (!_mpRootNode->_mRegion.getPointIsInside(vPosition))
		{
			// The position of the new entity doesn't fit within the root node's area
			// We're going to have to recreate the entire tree

			// Delete root node, which will delete all children and their children and so on.
			// This does not delete the entities (As they're stored in the _mmapEntities hashmap)
			// But before deleting the root node, get it's currently set region which we will multiply to get
			// the root node's new dimensions
			CAABB aabbOldRootNodeRegion = _mpRootNode->_mRegion;
			delete _mpRootNode;

			// Inscrease the old root node's region size until the new entity's position fits within
			bool bNewPositionFits = false;
			while (!bNewPositionFits)
			{
				aabbOldRootNodeRegion.resizeArea(_mfSizeIncreaseMultiplier);
				bNewPositionFits = aabbOldRootNodeRegion.getPointIsInside(vPosition);
			}

			// Now re-create the root node, passing 0 as the pointer to the parent node and the new region size
			_mpRootNode = new COctTreeNode(aabbOldRootNodeRegion, 0, this);
			ThrowIfFalse(_mpRootNode, "COctTree::addEntity() failed. Unable to allocate memory for new root node.");

			// Now re-insert all entities stored in the hash map back into the tree
			// The new entity has already been added to the hash map, so no need to insert it seperately
			it = _mmapEntities.begin();
			while (it != _mmapEntities.end())
			{
				_mpRootNode->addEntity(it->second);
				it++;
			}
		}
		else  // The entity position does fit inside the area of this node, simply add it
		{
			_mpRootNode->addEntity(pEntity);
		}
	}

	void COctTree::removeEntity(const std::string& strName)
	{
		// Make sure the entity exists by checking the hashmap
		std::map<std::string, COctTreeEntity*>::iterator it = _mmapEntities.find(strName);
		ThrowIfTrue(_mmapEntities.end() == it, "COctTree::removeEntity() failed. The entity name of " + strName + " doesn't exist.");

		// Get the node the entity is stored in to remove the entity from itself.
		COctTreeNode* pNodeContainingRemovedEntity = it->second->_mpNodeOwner;
		pNodeContainingRemovedEntity->removeEntity(it->second);

		// Remove the entity from the hashmap and delete it
		COctTreeEntity* pEntity = it->second;
		_mmapEntities.erase(it);
		delete pEntity;

		// Now check to see if the node is now empty and if so, remove it
		if (!pNodeContainingRemovedEntity->hasEntitiesInThisAndAllChildren())
		{
			// Is the node which held the removed entity, the root node
			bool bNodeIsRoot = _mpRootNode == pNodeContainingRemovedEntity;

			if (!bNodeIsRoot)  // The node wasn't the root node and therefore pNodeParent will contain a valid pointer to it's parent node.
			{
				// Store pointer to the parent
				COctTreeNode* pNodeParent = pNodeContainingRemovedEntity->_mpParentNode;

				// We're done, except...
				// We need to traverse up to the root node and delete any NOW empty nodes, which may 
				// have only existed to hold the now deleted node.
				// We also have to remove any parent node's _mpChildNode[8] pointers if we delete a node.
				while (pNodeParent)
				{
					// Go through each of the eight possible child nodes of the parent
					for (int i = 0; i < 8; i++)
					{
						if (pNodeParent->_mpChildNode[i])	// There is a child at this array position
						{
							if (!pNodeParent->_mpChildNode[i]->hasEntitiesInThisAndAllChildren())	// Node is empty
							{
								delete pNodeParent->_mpChildNode[i];
								pNodeParent->_mpChildNode[i] = 0;
							}
						}
					}
					pNodeParent = pNodeParent->_mpParentNode;
				}

				// This will delete the node and all it's children.
				// No need to delete, as it's deleted above when deleting children.
				// If we attempted to delete again, all hell breaks loose!
//				delete pNodeContainingRemovedEntity;
			}
		}
		// The node which contained the entity has other entities, leave it alone.
	}

	bool COctTree::getEntityExists(const std::string& strName) const
	{
		// Check the hashmap
		std::map<std::string, COctTreeEntity*>::iterator it = _mmapEntities.find(strName);
		return(_mmapEntities.end() != it);
	}

	void COctTree::removeAllEntities(bool bResetTree)
	{
		// Go through each entity, asking each node which it's in to remove itself
		std::map<std::string, COctTreeEntity*>::iterator it = _mmapEntities.begin();
		while (it != _mmapEntities.end())
		{
			// Get the node the entity is stored in to remove the entity from itself.
			it->second->_mpNodeOwner->removeEntity(it->second);

			// Remove the entity from the hashmap and delete it
			COctTreeEntity* pEntity = it->second;
			_mmapEntities.erase(it);
			delete pEntity;
			it = _mmapEntities.begin();
		}

		// Now all entities are removed and deleted, reset the tree if bResetTree desires it
		if (bResetTree)
		{
			// Get the root node's AABB, so we can re-create it
			CAABB aabbRootNode = _mpRootNode->_mRegion;
			delete _mpRootNode;
			_mpRootNode = new COctTreeNode(aabbRootNode, 0, this);

			// Reset current max node depth
			_muiCurrentMaxNodeDepth = 0;

			// Compute new maximum node depth
			_computeMaxNodeDepth();
		}
	}

	void COctTree::setEntityPosition(const std::string& strName, const CVector3f& vNewPosition)
	{
		// First make sure the named entity exists
		std::map<std::string, COctTreeEntity*>::iterator it = _mmapEntities.find(strName);
		ThrowIfTrue(it == _mmapEntities.end(), "COctTree::setEntityPosition() failed. The named entity of " + strName + " doesn't exist.");

		// First check to see if the new entity position still fits within it's current node
		// If it does, we simply update the position
		if (it->second->_mpNodeOwner->_mRegion.getPointIsInside(vNewPosition))
		{
			it->second->_mvPosition = vNewPosition;
			return;
		}

		// If we get here, the new position doesn't fit within the entity's current node

		// Remove the entity from the tree and then re-insert it
		int iUserData = it->second->miUserData;
		void* pUserData = it->second->mpUserData;
		removeEntity(strName);
		addEntity(strName, vNewPosition, iUserData, pUserData);
	}

	void COctTree::getEntityPosition(const std::string& strName, CVector3f& vPosition) const
	{
		// First make sure the named entity exists
		std::map<std::string, COctTreeEntity*>::iterator it = _mmapEntities.find(strName);
		ThrowIfTrue(it == _mmapEntities.end(), "COctTree::getEntityPosition() failed. The named entity of " + strName + " doesn't exist.");
		vPosition = it->second->_mvPosition;
	}

	void COctTree::_computeMaxNodeDepth(void)
	{
		// Obtain smallest dimension of root node
		CVector3f vRootNodeDims = _mpRootNode->_mRegion.getDimensions();
		float fSmallestDim = vRootNodeDims.x;
		if (vRootNodeDims.y < fSmallestDim)
			fSmallestDim = vRootNodeDims.y;
		if (vRootNodeDims.z < fSmallestDim)
			fSmallestDim = vRootNodeDims.z;

		// Now compute max node depth
		_muiMaxNodeDepth = 0;
		while (fSmallestDim > 2)
		{
			fSmallestDim *= 0.5f;
			_muiMaxNodeDepth++;
		}
	}

	std::vector<COctTreeNode*> COctTree::getNodesWithEntities(void) const
	{
		std::vector<COctTreeNode*> vResult;
		_mpRootNode->getNodesWithEntities(vResult);
		return vResult;
	}

	std::vector<COctTreeNode*> COctTree::getNodesWithEntitiesWhichIntersect(const CAABB& aabb) const
	{
		std::vector<COctTreeNode*> vResult;
		_mpRootNode->getNodesWithEntitiesWhichIntersect(vResult, aabb);
		return vResult;
	}

	std::vector<COctTreeNode*> COctTree::getNodesWithEntitiesWhichIntersect(const CFrustum& frustum) const
	{
		std::vector<COctTreeNode*> vResult;
		_mpRootNode->getNodesWithEntitiesWhichIntersect(vResult, frustum);
		return vResult;
	}

	std::vector<COctTreeEntity*> COctTree::getEntitiesWithinRange(const CVector3f& vPosition, float fRange) const
	{
		// Create an AABB which covers the maximum range from the given position
		CAABB aabb;
		float fRangeTimesTwo = fRange * 2.0f;
		aabb.setPosDims(vPosition, CVector3f(fRangeTimesTwo, fRangeTimesTwo, fRangeTimesTwo));

		// Go through the nodes and get a vector of nodes which intersect the AABB and have entities in them
		std::vector<COctTreeNode*> vNodes = getNodesWithEntitiesWhichIntersect(aabb);
		std::vector<COctTreeEntity*> vResult;
		for (unsigned int ui = 0; ui < vNodes.size(); ui++)
		{
			// Go through each entity within each node and add them to the resulting vector
			std::map<std::string, COctTreeEntity*>::iterator it = vNodes[ui]->_mmapEntities.begin();
			while (it != vNodes[ui]->_mmapEntities.end())
			{
				vResult.push_back(it->second);
				it++;
			}
		}
		return vResult;
	}

	std::vector<COctTreeEntity*> COctTree::getEntitiesWithinAABB(const CAABB& aabb) const
	{
		// Go through the nodes and get a vector of the nodes which intersect the AABB and have entities in them
		std::vector<COctTreeNode*> vNodes = getNodesWithEntitiesWhichIntersect(aabb);
		std::vector<COctTreeEntity*> vResult;
		for (unsigned int ui = 0; ui < vNodes.size(); ui++)
		{
			// Go through each entity within each node and add them to the resulting vector
			std::map<std::string, COctTreeEntity*>::iterator it = vNodes[ui]->_mmapEntities.begin();
			while (it != vNodes[ui]->_mmapEntities.end())
			{
				vResult.push_back(it->second);
				it++;
			}
		}
		return vResult;
	}

	std::vector<COctTreeEntity*> COctTree::getEntitiesWithinFrustum(const CFrustum& frustum) const
	{
		// Go through the nodes and get a vector of the nodes which intersect the frustum and have entities in them
		std::vector<COctTreeNode*> vNodes = getNodesWithEntitiesWhichIntersect(frustum);
		std::vector<COctTreeEntity*> vResult;
		for (unsigned int ui = 0; ui < vNodes.size(); ui++)
		{
			// Go through each entity within each node and add them to the resulting vector
			std::map<std::string, COctTreeEntity*>::iterator it = vNodes[ui]->_mmapEntities.begin();
			while (it != vNodes[ui]->_mmapEntities.end())
			{
				vResult.push_back(it->second);
				it++;
			}
		}
		return vResult;
	}

	unsigned int COctTree::getNodeDepthCurrent(void)
	{
		// We have to recompute this, so go through all nodes, get their depth and compare
		_muiCurrentMaxNodeDepth = 0;
		_mpRootNode->getMaxNodeDepth(_muiCurrentMaxNodeDepth);
		return _muiCurrentMaxNodeDepth;
	}

	unsigned int COctTree::getNodeDepthMax(void) const
	{
		return _muiMaxNodeDepth;
	}

}