#include "PCH.h"
#include "2DMap.h"
#include "singletons.h"
#include "vector3f.h"

namespace X
{
	C2DMap::C2DMap(const std::string& strResourceTexture2DAtlasName, int iNumTilesX, int iNumTilesY)
	{
		_mstrResourceTexture2DAtlasName = strResourceTexture2DAtlasName;
		_mbVisible = true;

		// Create tiles
		C2DMapTile newTile(this);
		std::vector<C2DMapTile> newVec;
		for (int ix = 0; ix < iNumTilesX; ix++)
		{
			_mvecTiles.push_back(newVec);
			for (int iy = 0; iy < iNumTilesY; iy++)
			{
				_mvecTiles[ix].push_back(newTile);
			}
		}
		_muiNumberOfTilesRendered = 0;
	}

	void C2DMap::setVisible(bool bVisible)
	{
		_mbVisible = bVisible;
	}

	bool C2DMap::getVisible(void) const
	{
		return _mbVisible;
	}

	void C2DMap::render(C2DCamera& camera, CMatrix& matrixView, CMatrix& matrixProjection)
	{
		// Make sure some image types exist
		ThrowIfFalse(_mmapImageTypes.size(), "C2DMap::render() failed. No C2DMapImageType added. Use C2DMap::addImageType()");
		_mTimer.update();
		float fTimeDeltaSeconds = _mTimer.getSecondsPast();

		// Update frame numbers for each image type
		std::map<std::string, C2DMapImageType*>::iterator it = _mmapImageTypes.begin();
		while (it != _mmapImageTypes.end())
		{
			it->second->update(fTimeDeltaSeconds);
			it++;
		}

		// Get back buffer frame buffer
		CResourceFramebuffer* pBackbuffer = x->pResource->getFramebuffer(x->pResource->defaultRes.framebuffer_backbuffer_FB);

		// Get viewport corner positions from camera position
		CVector2f v2CameraPos = camera.getPosition();
		CVector2f vViewportMin(v2CameraPos.x, v2CameraPos.y);
		CVector2f vViewportMax = vViewportMin;
		vViewportMax += pBackbuffer->getDimensions();

		// Set identity world matrix
		CMatrix matrixWorld;

		// Get shader and parse matrices and uniforms
		CResourceShader* pShader = x->pResource->getShader(x->pResource->defaultRes.shader_VBCPT);
		pShader->bind();
		pShader->setInt("texture0", 0);
		pShader->setMat4("matrixWorld", matrixWorld);
		pShader->setMat4("matrixView", matrixView);
		pShader->setMat4("matrixProjection", matrixProjection);

		// Get texture atlas and bind it
		CResourceTexture2DAtlas* pAtlasTexture = x->pResource->getTexture2DAtlas(_mstrResourceTexture2DAtlasName);
		pAtlasTexture->bind();

		// Get the vertex buffer used to send geometry to the GPU
		CResourceVertexBufferCPT* pVertexBuffer = x->pResource->getVertexBufferCPT(x->pResource->defaultRes.vertexbufferCPT_default);
		pVertexBuffer->removeGeom();

		// Get dimensions of a tile from one of the image types
		CVector2f vTileDims = _mmapImageTypes.begin()->second->getCurrentFrameImageDimensions();

		// Add tiles to be rendered
		CVector2f vTileMinPos;	// Holds tile's top left corner position
		CVector2f vTileMaxPos;	// Holds tile's bottom right corner position
		_muiNumberOfTilesRendered = 0;
		for (size_t ix = 0; ix < _mvecTiles.size(); ix++)
		{
			// Get min and max positions of this tile
			vTileMinPos.x = (vTileDims.x * (float)ix);// +v2CameraPos.x;
			vTileMaxPos.x = vTileMinPos.x + vTileDims.x;

			// If tile is not onscreen
			if (vTileMaxPos.x < vViewportMin.x)
				continue;	// Skip this entire column
			if (vTileMinPos.x > vViewportMax.x)
				break;		// No more tiles to render

			for (size_t iy = 0; iy < _mvecTiles[ix].size(); iy++)
			{
				// Get min and max positions of this tile
				vTileMinPos.y = (vTileDims.y * (float)iy);// +v2CameraPos.y;
				vTileMaxPos.y = vTileMinPos.y + vTileDims.y;

				// If tile is not onscreen
				if (vTileMaxPos.y < vViewportMin.y)
					continue;	// Skip this row
				if (vTileMinPos.y > vViewportMax.y)
					break;	// Will this break out of just the above loop.

				// If we get here, the tile is on screen

				// Get the image type this tile uses
				C2DMapImageType* pTileImageType = _mvecTiles[ix][iy].getImageType();
				CImageAtlasDetails::STexCoords imageTexCoords = pTileImageType->getCurrentFrameImageTextureCoords();
				_muiNumberOfTilesRendered++;
				pVertexBuffer->addQuad2D(
					vTileMinPos,
					vTileDims,
					_mvecTiles[ix][iy]._mColour,
					imageTexCoords.bottom_left,
					imageTexCoords.bottom_right,
					imageTexCoords.top_right,
					imageTexCoords.top_left);
			}
		}
		pVertexBuffer->update();
		pVertexBuffer->render();
		pVertexBuffer->removeGeom();

		// Unbind stuff
		pShader->unbind();
		pAtlasTexture->unbind();
	}


	void C2DMap::setAllTilesImageType(const std::string& strImageTypeName)
	{
		for (size_t ix = 0; ix < _mvecTiles.size(); ix++)
		{
			for (size_t iy = 0; iy < _mvecTiles[ix].size(); iy++)
			{
				_mvecTiles[ix][iy].setImageType(strImageTypeName);
			}
		}
	}

	/**************************************************************************************************************************************************
	Image types
	**************************************************************************************************************************************************/

	C2DMapImageType* C2DMap::addImageType(const std::string& strName)
	{
		// If resource already exists
		std::map<std::string, C2DMapImageType*>::iterator it = _mmapImageTypes.find(strName);
		ThrowIfTrue(it != _mmapImageTypes.end(), "C2DMap::addImageType(" + strName + ") failed. The named object already exists.");
		C2DMapImageType* pNewRes = new C2DMapImageType(_mstrResourceTexture2DAtlasName);
		ThrowIfFalse(pNewRes, "C2DMap::addImageType(" + strName + ") failed. Could not allocate memory for the new object.");

		_mmapImageTypes[strName] = pNewRes;
		return pNewRes;
	}

	C2DMapImageType* C2DMap::getImageType(const std::string& strName) const
	{
		std::map<std::string, C2DMapImageType*>::iterator it = _mmapImageTypes.find(strName);
		ThrowIfTrue(it == _mmapImageTypes.end(), "C2DMap::getImageType(" + strName + ") failed. The named object doesn't exist.");
		return it->second;
	}

	void C2DMap::removeImageType(const std::string& strName)
	{
		std::map<std::string, C2DMapImageType*>::iterator it = _mmapImageTypes.find(strName);
		if (it == _mmapImageTypes.end())
			return;
		delete it->second;
		_mmapImageTypes.erase(it);
	}

	void C2DMap::removeAllImageTypes(void)
	{
		std::map<std::string, C2DMapImageType*>::iterator it = _mmapImageTypes.begin();
		while (it != _mmapImageTypes.end())
		{
			delete it->second;
			_mmapImageTypes.erase(it);
			it = _mmapImageTypes.begin();
		}
	}

	void C2DMap::getStatsTilesRendered(int& iNumberOfTilesRendered, int& iTotalNumberOfTiles)
	{
		iNumberOfTilesRendered = (int)_muiNumberOfTilesRendered;
		if (_mvecTiles.size())
		{
			iTotalNumberOfTiles = (int)_mvecTiles.size() * (int)_mvecTiles[0].size();
		}
		else
		{
			iTotalNumberOfTiles = 0;
		}
	}
}