#include "PCH.h"
#include "2DMap.h"
#include "singletons.h"
#include "vector3f.h"
#include "stringUtils.h"
#include "logging.h"
#include "resourceManager.h"

namespace X
{
	C2DMap::C2DMap(const std::string& strResourceTexture2DAtlasName, int iNumTilesX, int iNumTilesY)
	{
		reset(strResourceTexture2DAtlasName, iNumTilesX, iNumTilesY);
	}

	C2DMap::~C2DMap()
	{
		if (_mmapImageTypes.size())
		{
			std::map<std::string, C2DMapImageType*>::iterator itIT = _mmapImageTypes.begin();
			while (itIT != _mmapImageTypes.end())
			{
				delete itIT->second;
				itIT++;
			}
			_mmapImageTypes.clear();
		}
		_mvecTiles.clear();
	}

	void C2DMap::reset(const std::string& strResourceTexture2DAtlasName, int iNumTilesX, int iNumTilesY)
	{
		_muiNumberOfTilesRendered = 0;
		_mstrResourceTexture2DAtlasName = strResourceTexture2DAtlasName;
		_mbVisible = true;

		if (_mmapImageTypes.size())
		{
			std::map<std::string, C2DMapImageType*>::iterator itIT = _mmapImageTypes.begin();
			while (itIT != _mmapImageTypes.end())
			{
				delete itIT->second;
				itIT++;
			}
			_mmapImageTypes.clear();
		}
		_mvecTiles.clear();

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
					imageTexCoords.vBL,
					imageTexCoords.vBR,
					imageTexCoords.vTR,
					imageTexCoords.vTL);
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

	void C2DMap::getTileIndicesAtScreenPosition(const CVector2f& vCameraPosition, const CVector2f& vScreenPosition, int& iTileIndexX, int& iTileIndexY) const
	{
		// Make sure some image types exist
		ThrowIfFalse(_mmapImageTypes.size(), "C2DMap::getTileIndicesAtScreenPosition() failed. No C2DMapImageType added. Use C2DMap::addImageType()");

		// Get dimensions of a tile from one of the image types
		CVector2f vTileDims = _mmapImageTypes.begin()->second->getCurrentFrameImageDimensions();

		iTileIndexX = int((vScreenPosition.x + vCameraPosition.x) / vTileDims.x);
		iTileIndexY = int((vScreenPosition.y + vCameraPosition.y) / vTileDims.y);

		// Make sure indicies are in range of the vectors
		if (iTileIndexX < 0 || iTileIndexX >= (int)_mvecTiles.size())
			iTileIndexX = iTileIndexY = -1;
		if (iTileIndexY < 0 || iTileIndexY >= (int)_mvecTiles[0].size())
			iTileIndexX = iTileIndexY = -1;
	}

	C2DMapTile* C2DMap::getTileAtScreenPosition(const CVector2f& vCameraPosition, const CVector2f& vScreenPosition)
	{
		int iTileIndexX, iTileIndexY;
		getTileIndicesAtScreenPosition(vCameraPosition, vScreenPosition, iTileIndexX, iTileIndexY);

		// Make sure indicies are valid
		if (iTileIndexX == -1 || iTileIndexY == -1)
			return 0;
		return &_mvecTiles[iTileIndexX][iTileIndexY];
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

	void C2DMap::save(const std::string& strFilename) const
	{
		std::ofstream file(strFilename, std::ios::out | std::ios::trunc | std::ios::binary);
		ThrowIfTrue(!file, "C2DMap::save(" + strFilename + ") failed. Unable to open file for writing.");

		// Write texture atlas name used
		StringUtils::stringWrite(_mstrResourceTexture2DAtlasName, file);
		
		// Write whether set to visible
		file.write((char*)&_mbVisible, sizeof(bool));

		// Write number of tiles along X and Y
		size_t sizeNumTilesX = _mvecTiles.size();
		size_t sizeNumTilesY = _mvecTiles[0].size();
		file.write((char*)&sizeNumTilesX, sizeof(size_t));
		file.write((char*)&sizeNumTilesY, sizeof(size_t));

		// Write number of image types
		size_t sizeImageTypes = _mmapImageTypes.size();
		file.write((char*)&sizeImageTypes, sizeof(size_t));

		// Write each image type
		std::map<std::string, C2DMapImageType*>::iterator itIT = _mmapImageTypes.begin();
		while (itIT != _mmapImageTypes.end())
		{
			// Write image type's name
			StringUtils::stringWrite(itIT->first, file);

			// Write image type's animation rate
			file.write((char*)&itIT->second->_mfAnimRateFPS, sizeof(float));

			// Write number of image frames in each image type
			size_t numImageFrames = itIT->second->_mvecImageFrames.size();
			file.write((char*)&numImageFrames, sizeof(size_t));

			// Write each image frame in each image type
			for (size_t i = 0; i < numImageFrames; i++)
			{
				// Write out each CImageAtlasDetail
				itIT->second->_mvecImageFrames[i].write(file);
			}
			itIT++;
		}

		// Write out each tile
		for (size_t ix = 0; ix < _mvecTiles.size(); ix++)
		{
			for (size_t iy = 0; iy < _mvecTiles[ix].size(); iy++)
			{
				_mvecTiles[ix][iy].write(file);
			}
		}

		ThrowIfFalse(file.good(), "C2DMap::save(" + strFilename + ") failed. Error occurred whilst writing.");
		file.close();
	}

	void C2DMap::load(const std::string& strFilename)
	{
		std::ifstream file(strFilename, std::ios::in | std::ios::binary);
		ThrowIfTrue(!file, "C2DMap::load(" + strFilename + ") failed. Unable to open file for writing.");

		// Read in atlas name used
		StringUtils::stringRead(_mstrResourceTexture2DAtlasName, file);

		// Read whether set to visible
		file.read((char*)&_mbVisible, sizeof(bool));

		// Read number of tiles along X and Y
		size_t sizeNumTilesX;
		size_t sizeNumTilesY;
		file.read((char*)&sizeNumTilesX, sizeof(size_t));
		file.read((char*)&sizeNumTilesY, sizeof(size_t));

		// Now we know the required values, reset this map
		reset(_mstrResourceTexture2DAtlasName, (int)sizeNumTilesX, (int)sizeNumTilesY);

		// Read number of image types
		size_t sizeImageTypes;
		file.read((char*)&sizeImageTypes, sizeof(size_t));

		CResourceTexture2DAtlas* pAtlasResource = x->pResource->getTexture2DAtlas(_mstrResourceTexture2DAtlasName);

		// Read each image type
		for (size_t i = 0; i < sizeImageTypes; i++)
		{
			// Read image type's name
			std::string strImageTypeName;
			StringUtils::stringRead(strImageTypeName, file);
			
			C2DMapImageType* pImageType = addImageType(strImageTypeName);
			
			// Read image type's animation rate
			file.read((char*)&pImageType->_mfAnimRateFPS, sizeof(float));

			// Read number of image frames in each image type
			size_t numImageFrames;
			file.read((char*)&numImageFrames, sizeof(size_t));

			// Read each image frame in each image type
			for (size_t i = 0; i < numImageFrames; i++)
			{
				// Write out each CImageAtlasDetail
				CImageAtlasDetails imageAtlasDetails;
				imageAtlasDetails.read(file);

				pImageType->addFrameImage(imageAtlasDetails.strImageFilename);
			}
		}

		// Read in each tile
		for (size_t ix = 0; ix < _mvecTiles.size(); ix++)
		{
			for (size_t iy = 0; iy < _mvecTiles[ix].size(); iy++)
			{
				_mvecTiles[ix][iy].read(file);
				_mvecTiles[ix][iy]._mpMap = this;
				_mvecTiles[ix][iy]._mpImageType = getImageType(_mvecTiles[ix][iy]._mstrImageTypeName);
			}
		}

		ThrowIfFalse(file.good(), "C2DMap::load(" + strFilename + ") failed. Error occurred whilst writing.");
		file.close();
	}

	std::string C2DMap::getAtlasResourceName(void) const
	{
		return _mstrResourceTexture2DAtlasName;
	}
}