#include "PCH.h"
#include "quadTree.h"
#include "log.h"

namespace X
{
	CQuadTree::CQuadTree(int iMaxEntitiesPerNode, int iRectSizeIncreaseMultiplier)
	{
		_mpRootNode = 0;

		init(iMaxEntitiesPerNode, iRectSizeIncreaseMultiplier);
	}

	void CQuadTree::init(int iMaxEntitiesPerNode, int iRectSizeIncreaseMultiplier)
	{
		free();

		CRect rctInitialRootNodeRegion(-1024, -1024, 1024, 1024);

		// Attempt to create root node, passing 0 as the pointer to the parent node
		_mpRootNode = new CQuadTreeNode(rctInitialRootNodeRegion, 0, this);
		ThrowIfFalse(_mpRootNode, "CQuadTree::init() failed. Unable to allocate memory for root node.");

		// Make sure valid values were given
		ThrowIfTrue(iMaxEntitiesPerNode < 1, "CQuadTree::init() failed. Given invalid number for iMaxEntitiesPerNode. Must be at least one.");
		ThrowIfTrue(iRectSizeIncreaseMultiplier < 2, "CQuadTree::init() failed. Given invalid number for iRectSizeIncreaseMultiplier. Must be at least 2.");

		// Store settings
		_miMaxEntitiesPerNode = iMaxEntitiesPerNode;
		_miRectSizeIncreaseMultiplier = iRectSizeIncreaseMultiplier;

		// Current maximum node depth
		_muiCurrentMaxNodeDepth = 0;

		// Compute maximum node depth before node division is forbidden.
		_computeMaxNodeDepth();
	}

	CQuadTree::~CQuadTree()
	{
		free();
	}

	void CQuadTree::free(void)
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
		std::map<std::string, CQuadTreeEntity*>::iterator it = _mmapEntities.begin();
		while (it != _mmapEntities.end())
		{
			delete it->second;
			it++;
		}
		_mmapEntities.clear();
	}

	void CQuadTree::debugRender(const CVector3f& vCameraPosition, bool bRenderNodes, bool bRenderEntities, int iEntityCircleRadius, unsigned int uiEntityCircleNumSegments) const
	{
		if (!bRenderNodes && !bRenderEntities)
			return;

		// Obtain required resources needed to render the node's as lines.
		SCResourceManager* pRM = SCResourceManager::getPointer();
		CResourceLine* pLine = pRM->getLine("X:default");
		CResourceShader* pShader = pRM->getShader("X:line");
		CResourceTexture2DFromFile* pTexture = pRM->getTexture2DFromFile("X:default_white");

		// Setup orthographic projection matrix
		CMatrix matrixWorld;
		CMatrix matrixView;
		CVector3f vTargetPos = vCameraPosition;
		vTargetPos.z -= 100.0f;
		matrixView.setViewLookat(vCameraPosition, vTargetPos, CVector3f(0.0f, 1.0f, 0.0f));

		CMatrix matrixProjection;
		matrixProjection.setProjectionPerspective(55.0f, 1.0f, 10000.0f);

		// Bind shader and set uniforms
		pShader->bind();
		pShader->setMat4("matrixWorld", matrixWorld);
		pShader->setMat4("matrixView", matrixView);
		pShader->setMat4("matrixProjection", matrixProjection);

		// Tell OpenGL, for each sampler, to which texture unit it belongs to
		pShader->setInt("texture0_colour", 0);

		// And bind the texture...
		pTexture->bind();

		// Set the line vertex buffer rendering mode
		pLine->setDrawModeAsLineList();
		pLine->removeGeom();

		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Setup vertex we'll be using to render with
		CResourceLine::Vertex vertex;
		vertex.colour.set(1.0f, 1.0f, 1.0f, 1.0f);

		// Call the root node's debugRender method which will render itself and it's children recursively
		if (bRenderNodes)
			_mpRootNode->debugRenderNodes(pLine, vertex, CColour(1.0f, 1.0f, 1.0f, 1.0f));
		if (bRenderEntities)
			_mpRootNode->debugRenderEntities(pLine, iEntityCircleRadius, uiEntityCircleNumSegments);

		// Send geometry to be rendered
		pLine->update();
		pLine->draw();

		// Cleanup
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		pTexture->unbind();
		pShader->unbind();
	}

	void CQuadTree::debugSetEntityColour(const std::string& strName, CColour& colour)
	{
		// Find the entity
		std::map<std::string, CQuadTreeEntity*>::iterator it = _mmapEntities.find(strName);
		ThrowIfTrue(_mmapEntities.end() == it, "CQuadTree::debugSetEntityColour() failed. The entity name of " + strName + " doesn't exist.");
		it->second->_mColDebug = colour;
	}

	void CQuadTree::debugSetAllEntitiesColour(CColour& colour)
	{
		std::map<std::string, CQuadTreeEntity*>::iterator it = _mmapEntities.begin();
		while (it != _mmapEntities.end())
		{
			it->second->_mColDebug = colour;
			it++;
		}
	}

	void CQuadTree::addEntity(const std::string& strName, int iPosX, int iPosY, int iUserData, void* pUserData)
	{
		// Make sure the entity doesn't already exist by checking the hashmap
		std::map<std::string, CQuadTreeEntity*>::iterator it = _mmapEntities.find(strName);
		ThrowIfTrue(_mmapEntities.end() != it, "CQuadTree::addEntity() failed. The entity name of " + strName + " already exists.");

		// Create new entity, setting it's owner to 0
		CQuadTreeEntity* pEntity = new CQuadTreeEntity(strName, iPosX, iPosY, 0, iUserData, pUserData);
		ThrowIfFalse(pEntity, "CQuadTree::addEntity() failed to allocate memory for new entity.");

		// Add entity to hashmap for fast lookup
		_mmapEntities[strName] = pEntity;

		// Determine whether the given position of the entity fits in the root node
		if (!_mpRootNode->_mRectRegion.doesPositionFitWithin(iPosX, iPosY))
		{
			// The position of the new entity doesn't fit within the root node's area
			// We're going to have to recreate the entire tree

			// Delete root node, which will delete all children and their children and so on.
			// This does not delete the entities (As they're stored in the _mmapEntities hashmap)
			// But before deleting the root node, get it's currently set region which we will multiply to get
			// the root node's new dimensions
			CRect rectOldRootNodeRegion = _mpRootNode->_mRectRegion;
			delete _mpRootNode;

			// Inscrease the old root node's region size until the new entity's position fits within
			bool bNewPositionFits = false;
			while (!bNewPositionFits)
			{
				rectOldRootNodeRegion.resizeArea(_miRectSizeIncreaseMultiplier);
				bNewPositionFits = rectOldRootNodeRegion.doesPositionFitWithin(iPosX, iPosY);
			}

			// Now re-create the root node, passing 0 as the pointer to the parent node and the new region size
			_mpRootNode = new CQuadTreeNode(rectOldRootNodeRegion, 0, this);
			ThrowIfFalse(_mpRootNode, "CQuadTree::addEntity() failed. Unable to allocate memory for new root node.");

			// Now re-insert all entities stored in the hash map back into the tree
			// The new entity has already been added to the hash map, so no need to insert it seperately
			it = _mmapEntities.begin();
			while (it != _mmapEntities.end())
			{
				_mpRootNode->addEntity(it->second);
				it++;
			}
		}
		else  // The entity position does fit inside the rect of this node, simply add it
		{
			_mpRootNode->addEntity(pEntity);
		}
	}

	void CQuadTree::removeEntity(const std::string& strName)
	{
		// Make sure the entity exists by checking the hashmap
		std::map<std::string, CQuadTreeEntity*>::iterator it = _mmapEntities.find(strName);
		ThrowIfTrue(_mmapEntities.end() == it, "CQuadTree::removeEntity() failed. The entity name of " + strName + " doesn't exist.");

		// Get the node the entity is stored in to remove the entity from itself.
		CQuadTreeNode* pNodeContainingRemovedEntity = it->second->_mpNodeOwner;
		pNodeContainingRemovedEntity->removeEntity(it->second);

		// Remove the entity from the hashmap and delete it
		CQuadTreeEntity* pEntity = it->second;
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
				CQuadTreeNode* pNodeParent = pNodeContainingRemovedEntity->_mpParentNode;

				// We're done, except...
				// We need to traverse up to the root node and delete any NOW empty nodes, which may 
				// have only existed to hold the now deleted node.
				// We also have to remove any parent node's _mpChildNode[4] pointers if we delete a node.
				while (pNodeParent)
				{
					// Go through each of the four possible child nodes of the parent
					for (int i = 0; i < 4; i++)
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

	bool CQuadTree::getEntityExists(const std::string& strName) const
	{
		// Check the hashmap
		std::map<std::string, CQuadTreeEntity*>::iterator it = _mmapEntities.find(strName);
		return(_mmapEntities.end() != it);
	}

	void CQuadTree::removeAllEntities(bool bResetTree)
	{
		// Go through each entity, asking each node which it's in to remove itself
		std::map<std::string, CQuadTreeEntity*>::iterator it = _mmapEntities.begin();
		while (it != _mmapEntities.end())
		{
			// Get the node the entity is stored in to remove the entity from itself.
			it->second->_mpNodeOwner->removeEntity(it->second);

			// Remove the entity from the hashmap and delete it
			CQuadTreeEntity* pEntity = it->second;
			_mmapEntities.erase(it);
			delete pEntity;
			it = _mmapEntities.begin();
		}

		// Now all entities are removed and deleted, reset the tree if bResetTree desires it
		if (bResetTree)
		{
			// Get the root node's rect, so we can re-create it
			CRect rectRootNode = _mpRootNode->_mRectRegion;
			delete _mpRootNode;
			_mpRootNode = new CQuadTreeNode(rectRootNode, 0, this);

			// Reset current max node depth
			_muiCurrentMaxNodeDepth = 0;

			// Compute new maximum node depth
			_computeMaxNodeDepth();
		}
	}

	void CQuadTree::setEntityPosition(const std::string& strName, int iNewPosX, int iNewPosY)
	{
		// First make sure the named entity exists
		std::map<std::string, CQuadTreeEntity*>::iterator it = _mmapEntities.find(strName);
		ThrowIfTrue(it == _mmapEntities.end(), "CQuadTree::setEntityPosition() failed. The named entity of " + strName + " doesn't exist.");

		// First check to see if the new entity position still fits within it's current node
		// If it does, we simply update the position
		if (it->second->_mpNodeOwner->_mRectRegion.doesPositionFitWithin(iNewPosX, iNewPosY))
		{
			it->second->_miPosX = iNewPosX;
			it->second->_miPosY = iNewPosY;
			return;
		}

		// If we get here, the new position doesn't fit within the entity's current node

		// Remove the entity from the tree and then re-insert it
		int iUserData = it->second->miUserData;
		void* pUserData = it->second->mpUserData;
		removeEntity(strName);
		addEntity(strName, iNewPosX, iNewPosY, iUserData, pUserData);
	}

	void CQuadTree::getEntityPosition(const std::string& strName, int& iPosX, int& iPosY) const
	{
		// First make sure the named entity exists
		std::map<std::string, CQuadTreeEntity*>::iterator it = _mmapEntities.find(strName);
		ThrowIfTrue(it == _mmapEntities.end(), "CQuadTree::getEntityPosition() failed. The named entity of " + strName + " doesn't exist.");
		iPosX = it->second->_miPosX;
		iPosY = it->second->_miPosY;
	}

	void CQuadTree::_computeMaxNodeDepth(void)
	{
		// Obtain smallest dimension of root node
		int iDimOfRootX = _mpRootNode->_mRectRegion.miMaxX - _mpRootNode->_mRectRegion.miMinX;
		int iDimOfRootY = _mpRootNode->_mRectRegion.miMaxY - _mpRootNode->_mRectRegion.miMinY;
		int iSmallestDim = iDimOfRootX;
		if (iDimOfRootY < iDimOfRootX)
			iSmallestDim = iDimOfRootY;
		// Now compute max node depth
		_muiMaxNodeDepth = 0;
		while (iSmallestDim > 2)
		{
			iSmallestDim /= 2;
			_muiMaxNodeDepth++;
		}
	}

	std::vector<CQuadTreeNode*> CQuadTree::getNodesWithEntities(void) const
	{
		std::vector<CQuadTreeNode*> vResult;
		_mpRootNode->getNodesWithEntities(vResult);
		return vResult;
	}

	std::vector<CQuadTreeNode*> CQuadTree::getNodesWithEntitiesWhichIntersect(const CRect& rect) const
	{
		std::vector<CQuadTreeNode*> vResult;
		_mpRootNode->getNodesWithEntitiesWhichIntersect(vResult, rect);
		return vResult;
	}

	std::vector<CQuadTreeEntity*> CQuadTree::getEntitiesWithinRange(int iPosX, int iPosY, int iRange) const
	{
		// Create a rect which covers the maximum range from the given position
		CRect rectRange;
		rectRange.miMinX = iPosX - iRange;
		rectRange.miMaxX = iPosX + iRange;
		rectRange.miMinY = iPosY - iRange;
		rectRange.miMaxY = iPosY + iRange;

		// Go through the nodes and get a vector of nodes which intersect the rect and have entities in them
		std::vector<CQuadTreeNode*> vNodes = getNodesWithEntitiesWhichIntersect(rectRange);
		std::vector<CQuadTreeEntity*> vResult;
		for (unsigned int ui = 0; ui < vNodes.size(); ui++)
		{
			// Go through each entity within each node and add them to the resulting vector
			std::map<std::string, CQuadTreeEntity*>::iterator it = vNodes[ui]->_mmapEntities.begin();
			while (it != vNodes[ui]->_mmapEntities.end())
			{
				vResult.push_back(it->second);
				it++;
			}
		}
		return vResult;
	}

	std::vector<CQuadTreeEntity*> CQuadTree::getEntitiesWithinRect(const CRect& rect) const
	{
		// Go through the nodes and get a vector of the nodes which intersect the rect and have entities in them
		std::vector<CQuadTreeNode*> vNodes = getNodesWithEntitiesWhichIntersect(rect);
		std::vector<CQuadTreeEntity*> vResult;
		for (unsigned int ui = 0; ui < vNodes.size(); ui++)
		{
			// Go through each entity within each node and add them to the resulting vector
			std::map<std::string, CQuadTreeEntity*>::iterator it = vNodes[ui]->_mmapEntities.begin();
			while (it != vNodes[ui]->_mmapEntities.end())
			{
				vResult.push_back(it->second);
				it++;
			}
		}
		return vResult;
	}

	unsigned int CQuadTree::getNodeDepthCurrent(void)
	{
		// We have to recompute this, so go through all nodes, get their depth and compare
		_muiCurrentMaxNodeDepth = 0;
		_mpRootNode->getMaxNodeDepth(_muiCurrentMaxNodeDepth);
		return _muiCurrentMaxNodeDepth;
	}

	unsigned int CQuadTree::getNodeDepthMax(void) const
	{
		return _muiMaxNodeDepth;
	}

}