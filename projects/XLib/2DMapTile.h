#pragma once
#include "PCH.h"
#include "2DMapImageType.h"

namespace X
{
	class C2DMap;

	// A tile used by C2DMap
	// Holds information about a single tile of the map
	// Has several public members which do nothing in relation to the map itself,
	// but can be set to store data we might need for a tile.
	// They are named mTYPEuserData???
	class C2DMapTile
	{
		friend class C2DMap;
	public:
		// pMapOwner is given from C2DMap
		C2DMapTile(C2DMap *pMapOwner);

		// Sets the name of the C2DMapImageType this tile uses for it's image data
		// Initially, these are "null"
		// If the name of the type doesn't exist in this tile's C2DMap owner, then an exception occurs
		void setImageType(const std::string& strImageTypeName);

		// Returns a pointer to the currently set C2DMapImageType this tile uses for it's image data
		C2DMapImageType* getImageType(void) const;

		// Returns the name of the currently set C2DMapImageType this tile uses for it's image data
		std::string getImageTypeName(void) const;

		// Sets the colour used to render this tile
		// By default, this is white with full alpha
		void setColour(const CColour& colour);

		float mfUserData;			// Nothing to do with the map itself, but can be set to store whatever data we desire
		std::string mstrUserData;	// Nothing to do with the map itself, but can be set to store whatever data we desire
		CVector2f mv2fUserData;		// Nothing to do with the map itself, but can be set to store whatever data we desire
	private:
		C2DMap* _mpMap;							// The C2DMap this object belongs to
		std::string _mstrImageTypeName;			// Named of the C2DMapImageType stored in C2DMap this tile uses for it's image data
		C2DMapImageType* _mpImageType;			// Pointer to the C2DMapImageType stored in C2DMap this tile uses for it's image data
		CColour _mColour;						// Colour used to render this tile
	};
}