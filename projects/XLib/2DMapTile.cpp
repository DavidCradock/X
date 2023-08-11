#include "PCH.h"
#include "2DMapTile.h"
#include "2DMap.h"

namespace X
{
	C2DMapTile::C2DMapTile(C2DMap* pMapOwner)
	{
		_mpMap = pMapOwner;
		_mpImageType = 0;
	}

	void C2DMapTile::setImageType(const std::string& strImageTypeName)
	{
		_mstrImageTypeName = strImageTypeName;
		_mpImageType = _mpMap->getImageType(strImageTypeName);
	}

	C2DMapImageType* C2DMapTile::getImageType(void) const
	{
		return _mpImageType;
	}

	std::string C2DMapTile::getImageTypeName(void) const
	{
		return _mstrImageTypeName;
	}

	void C2DMapTile::setColour(const CColour& colour)
	{
		_mColour = colour;
	}
}