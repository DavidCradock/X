#include "PCH.h"
#include "2DMapTile.h"
#include "2DMap.h"
#include "logging.h"
#include "stringUtils.h"

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

	void C2DMapTile::write(std::ofstream& file) const
	{
		ThrowIfFalse(file.is_open(), "C2DMapTile::write() failed. The given ofstream is not open.");

		file.write((char*)&mfUserData, sizeof(float));
		StringUtils::stringWrite(mstrUserData, file);
		file.write((char*)&mv2fUserData, sizeof(CVector2f));
		StringUtils::stringWrite(_mstrImageTypeName, file);
		file.write((char*)&_mColour, sizeof(CColour));
		ThrowIfFalse(file.good(), "C2DMapTile::write() failed. The ofstream is not good.");
	}

	void C2DMapTile::read(std::ifstream& file)
	{
		ThrowIfFalse(file.is_open(), "C2DMapTile::read() failed. The given ifstream is not open.");

		file.read((char*)&mfUserData, sizeof(float));
		StringUtils::stringRead(mstrUserData, file);
		file.read((char*)&mv2fUserData, sizeof(CVector2f));
		StringUtils::stringRead(_mstrImageTypeName, file);
		file.read((char*)&_mColour, sizeof(CColour));
		ThrowIfFalse(file.good(), "C2DMapTile::read() failed. The ifstream is not good.");
	}
}