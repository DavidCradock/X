#include "PCH.h"
#include "2DMapImageType.h"
#include "singletons.h"

namespace X
{
	C2DMapImageType::C2DMapImageType(const std::string& strResourceTexture2DAtlasName)
	{
		_mstrResourceTexture2DAtlasName = strResourceTexture2DAtlasName;
		_mpAtlas = x->pResource->getTexture2DAtlas(strResourceTexture2DAtlasName);
		_mfAnimRateFPS = 30;
	}

	C2DMapImageType::~C2DMapImageType()
	{
		_mvecImageFrames.clear();
	}

	void C2DMapImageType::update(float fTimeDeltaSeconds)
	{
		if (_mfAnimRateFPS > 0.0f)
		{
			_mfCurrentFrame += fTimeDeltaSeconds * _mfAnimRateFPS;
			if ((size_t)_mfCurrentFrame >= _mvecImageFrames.size())
				_mfCurrentFrame = 0;
		}
	}

	void C2DMapImageType::setFramerate(float fFramesPerSecond)
	{
		if (fFramesPerSecond < 0)
			fFramesPerSecond = 0;
		_mfAnimRateFPS = fFramesPerSecond;
	}

	float C2DMapImageType::getFramerate(void) const
	{
		return _mfAnimRateFPS;
	}

	void C2DMapImageType::setCurrentFrameNumber(int iFrameNumber)
	{
		ThrowIfTrue(iFrameNumber >= _mvecImageFrames.size(), "C2DMapImageType::setCurrentFrameNumber(" + std::to_string(iFrameNumber) + ") failed. Invalid frame number given.");
		_mfCurrentFrame = (float)iFrameNumber;
	}

	int C2DMapImageType::getCurrentFrameNumber(void) const
	{
		return (int)_mfCurrentFrame;
	}

	int C2DMapImageType::getNumberOfFrames(void) const
	{
		return (int)_mvecImageFrames.size();
	}

	void C2DMapImageType::addFrameImage(const std::string& strImageFilenameInAtlas)
	{
		_mvecImageFrames.push_back(_mpAtlas->getImageDetails(strImageFilenameInAtlas));
	}

	std::string C2DMapImageType::getCurrentFrameImageName(void) const
	{
		return _mvecImageFrames[int(_mfCurrentFrame)].strImageFilename;
	}

	CVector2f C2DMapImageType::getCurrentFrameImageDimensions(void) const
	{
		return _mvecImageFrames[int(_mfCurrentFrame)].v2fDimensions;
	}

	CImageAtlasDetails::STexCoords C2DMapImageType::getCurrentFrameImageTextureCoords(void) const
	{
		return _mvecImageFrames[int(_mfCurrentFrame)].sTexCoords;
	}
}