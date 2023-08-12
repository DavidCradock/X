#pragma once
#include "PCH.h"
#include "2DMapImageType.h"
#include "2DMapTile.h"
#include "2DCamera.h"
#include "timer.h"
#include "matrix.h"

namespace X
{
	// A 2D map consisting of tiles
	class C2DMap
	{
	public:
		// strResourceTexture2DAtlasName is the name of the CResourceTexture2DAtlas added to SCResourceManager which contains this map's image data.
		// Each image in the texture atlas should be the same size, otherwise expect weird overlapping rendering.
		// A map consists of a 2D array of tiles accessed with X and Y coordinates.
		// A map has to be created with a given size, there is no resizing as this is too slow.
		// Each tile has one or more images stored within the atlas which it uses to render itself.
		// If more than one image is given to a tile, it is animated.
		// By default, each tile has no image information and therefore is not rendered.
		// We need to add a named image type for each type of image we wish to use in the map.
		// Once added, we then add at least one image (more if animated) by giving the filename of the image stored in the atlas to the image type.
		// Then we can assign the image type to a tile which it'll then use when being rendered.
		// 
		// Example usage:
		// First create the texture atlas which holds all the map's tile's images inside a directory
		// std::vector<std::string> strMapTileImageFilenames = StringUtils::getFilesInDir("data/maps/MyFirstMapsTiles/");
		// NOTE: When creating the texture atlas for a map to use, create it with nearest filtering, otherwise teeny imprecision messes with the edges.
		// x->pResource->addTexture2DAtlas("MyMapsTextureAtlas", strMapTileImageFilenames, true, 1, false, true);
		//
		// Alternatively, instead of getting the filenames within the directory ourselves, we can instead use the easier method which does this for us...
		// x->pResource->addTexture2DAtlas("MyMapsTextureAtlas", "data/maps/MyFirstMapsTiles/");
		// 
		// Create the map, either locally, or within the SC2DRenderer...
		// C2DWorld *pWorld = x->p2dRenderer->addWorld("world");
		// C2DLayer *pLayer = pWorld->addLayer("layer");
		// C2DMap *pMap = pLayer->addMap("MyMap", "MyMapsTextureAtlas", 512, 512);	// Name of map, texture atlas to use and number of tiles along x and y.
		//
		// Now add image types which hold image frames for a tile to use.
		// Setting which image or images stored in the atlas it'll use
		// C2DMapImageType* pImageTypeSand = pMap->addImageType("sand");
		// pImageTypeSand->addFrameImage("data/maps/MyFirstMapsTiles/sand.png");
		//
		// Now get some tiles and set their image type...
		// 
		// TODO
		C2DMap(const std::string& strResourceTexture2DAtlasName, int iNumTilesX, int iNumTilesY);

		// Sets whether this map is rendered or not.
		// By default, they are visible
		void setVisible(bool bVisible);

		// Returns whether this map is currently set to be visible and rendered or not
		bool getVisible(void) const;

		// Renders (and updates) this map.
		// Called from SC2DRenderer::render()
		void render(C2DCamera& camera, CMatrix& matrixView, CMatrix& matrixProjection);

		// Sets all tiles to use the named image type.
		void setAllTilesImageType(const std::string& strImageTypeName);

		/**************************************************************************************************************************************************
		Image types
		**************************************************************************************************************************************************/

		// Add an image type to this map and return a pointer to it
		// If the name already exists, an exception occurs
		C2DMapImageType* addImageType(const std::string& strName);

		// Returns a pointer to the named object
		// If the object doesn't exist, an exception occurs
		C2DMapImageType* getImageType(const std::string& strName) const;

		// Removes the named object from the map
		// If the named object doesn't exist, this silently fails
		void removeImageType(const std::string& strName);

		// Removes all image types from this map
		void removeAllImageTypes(void);

		// For statistics only, sets the given variables to how many tiles are being rendered and the total number of tiles.
		void getStatsTilesRendered(int& iNumberOfTilesRendered, int& iTotalNumberOfTiles);
	private:
		CTimer _mTimer;
		std::string _mstrResourceTexture2DAtlasName;	// Name of the CResourceTexture2DAtlas added to SCResourceManager which holds this entity's image data
		bool _mbVisible;								// Whether this map is visible and rendered or not

		mutable std::map<std::string, C2DMapImageType*> _mmapImageTypes;	// Hashmap holding each image type

		std::vector< std::vector<C2DMapTile> > _mvecTiles;	// A vector of vectors of C2DMapTile objects holding all the tiles.

		unsigned int _muiNumberOfTilesRendered;			// What is says on the tin.
	};
}