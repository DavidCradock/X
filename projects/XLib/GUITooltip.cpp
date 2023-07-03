#include "PCH.h"
#include "GUITooltip.h"
#include "log.h"
#include "resourceManager.h"

namespace X
{
	GUITooltip::GUITooltip()
	{
		_mbEnabled = false;
	}

	GUITooltip::~GUITooltip()
	{

	}

	void GUITooltip::render(void* pParentContainer, const std::string& strFramebufferToSampleFrom)
	{

	}

	void GUITooltip::update(void* pParentContainer, GUIBaseObject* pGUIOwner, bool bMouseOverOwner)
	{

	}

	void GUITooltip::setEnabled(bool bEnabled)
	{
		_mbEnabled = bEnabled;
	}

	bool GUITooltip::getEnabled(void)
	{
		return _mbEnabled;
	}

	GUIText* GUITooltip::addText(const std::string& strName, float fPosX, float fPosY, const std::string& strText)
	{
		// If resource already exists
		std::map<std::string, GUIText*>::iterator it = _mmapTexts.find(strName);
		ThrowIfTrue(it != _mmapTexts.end(), "GUITooltip::addText(" + strName + ") failed. The named object already exists.");
		GUIText* pNewRes = new GUIText;
		ThrowIfFalse(pNewRes, "GUITooltip::addText(" + strName + ") failed. Could not allocate memory for the new object.");
		pNewRes->mfPositionX = fPosX;
		pNewRes->mfPositionY = fPosY;
		pNewRes->mstrText = strText;
		_mmapTexts[strName] = pNewRes;
		return pNewRes;
	}

	GUIText* GUITooltip::getText(const std::string& strName)
	{
		std::map<std::string, GUIText*>::iterator it = _mmapTexts.find(strName);
		ThrowIfTrue(it == _mmapTexts.end(), "GUITooltip::getText(" + strName + ") failed. The named object doesn't exist.");
		return it->second;
	}

	void GUITooltip::removeText(const std::string& strName)
	{
		std::map<std::string, GUIText*>::iterator it = _mmapTexts.find(strName);
		if (it == _mmapTexts.end())
			return;
		delete it->second;
		_mmapTexts.erase(it);
	}

	GUILineGraph* GUITooltip::addLineGraph(const std::string& strName, float fPosX, float fPosY, float fWidth, float fHeight)
	{
		// If resource already exists
		std::map<std::string, GUILineGraph*>::iterator it = _mmapLineGraphs.find(strName);
		ThrowIfTrue(it != _mmapLineGraphs.end(), "GUITooltip::addLineGraph(" + strName + ") failed. The named object already exists.");
		GUILineGraph* pNewRes = new GUILineGraph;
		ThrowIfFalse(pNewRes, "GUITooltip::addLineGraph(" + strName + ") failed. Could not allocate memory for the new object.");
		pNewRes->mfPositionX = fPosX;
		pNewRes->mfPositionY = fPosY;
		pNewRes->mfWidth = fWidth;
		pNewRes->mfHeight = fHeight;
		_mmapLineGraphs[strName] = pNewRes;
		return pNewRes;
	}

	GUILineGraph* GUITooltip::getLineGraph(const std::string& strName)
	{
		std::map<std::string, GUILineGraph*>::iterator it = _mmapLineGraphs.find(strName);
		ThrowIfTrue(it == _mmapLineGraphs.end(), "GUITooltip::getLineGraph(" + strName + ") failed. The named object doesn't exist.");
		return it->second;
	}

	void GUITooltip::removeLineGraph(const std::string& strName)
	{
		std::map<std::string, GUILineGraph*>::iterator it = _mmapLineGraphs.find(strName);
		if (it == _mmapLineGraphs.end())
			return;
		delete it->second;
		_mmapLineGraphs.erase(it);
	}

	GUIProgressBar* GUITooltip::addProgressBar(const std::string& strName, float fPosX, float fPosY, float fWidth, float fHeight)
	{
		// If resource already exists
		std::map<std::string, GUIProgressBar*>::iterator it = _mmapProgressBars.find(strName);
		ThrowIfTrue(it != _mmapProgressBars.end(), "GUITooltip::addProgressBar(" + strName + ") failed. The named object already exists.");
		GUIProgressBar* pNewRes = new GUIProgressBar;
		ThrowIfFalse(pNewRes, "GUITooltip::addProgressBar(" + strName + ") failed. Could not allocate memory for the new object.");
		pNewRes->mfPositionX = fPosX;
		pNewRes->mfPositionY = fPosY;
		pNewRes->mfWidth = fWidth;
		pNewRes->mfHeight = fHeight;
		_mmapProgressBars[strName] = pNewRes;
		return pNewRes;
	}

	GUIProgressBar* GUITooltip::getProgressBar(const std::string& strName)
	{
		std::map<std::string, GUIProgressBar*>::iterator it = _mmapProgressBars.find(strName);
		ThrowIfTrue(it == _mmapProgressBars.end(), "GUITooltip::getProgressBar(" + strName + ") failed. The named object doesn't exist.");
		return it->second;
	}

	void GUITooltip::removeProgressBar(const std::string& strName)
	{
		std::map<std::string, GUIProgressBar*>::iterator it = _mmapProgressBars.find(strName);
		if (it == _mmapProgressBars.end())
			return;
		delete it->second;
		_mmapProgressBars.erase(it);
	}

	GUIImage* GUITooltip::addImage(const std::string& strName, float fPosX, float fPosY, const std::string& strImageFilename, float fWidth, float fHeight)
	{
		// If resource already exists
		std::map<std::string, GUIImage*>::iterator it = _mmapImages.find(strName);
		ThrowIfTrue(it != _mmapImages.end(), "GUITooltip::addImage(" + strName + ") failed. The named object already exists.");
		GUIImage* pNewRes = new GUIImage;
		ThrowIfFalse(pNewRes, "GUITooltip::addImage(" + strName + ") failed. Could not allocate memory for the new object.");
		pNewRes->mfPositionX = fPosX;
		pNewRes->mfPositionY = fPosY;
		pNewRes->mfWidth = fWidth;
		pNewRes->mfHeight = fHeight;
		pNewRes->_mstrTexturename = strImageFilename;
		_mmapImages[strName] = pNewRes;

		// Add strImageFilename to the resource manager
		ResourceManager* pResMan = ResourceManager::getPointer();
		ResourceTexture2D* pTex = pResMan->addTexture2D(strImageFilename, strImageFilename);

		// If a value less than 0 is passed to width/height, set widget to dims of the image
		if (fWidth < 0)
			pNewRes->mfWidth = pTex->mvDimensions.x;
		if (fHeight < 0)
			pNewRes->mfHeight = pTex->mvDimensions.y;

		return pNewRes;
	}

	GUIImage* GUITooltip::getImage(const std::string& strName)
	{
		std::map<std::string, GUIImage*>::iterator it = _mmapImages.find(strName);
		ThrowIfTrue(it == _mmapImages.end(), "GUITooltip::getImage(" + strName + ") failed. The named object doesn't exist.");
		return it->second;
	}

	void GUITooltip::removeImage(const std::string& strName)
	{
		std::map<std::string, GUIImage*>::iterator it = _mmapImages.find(strName);
		if (it == _mmapImages.end())
			return;

		// Remove _mstrImageFilename from the resource manager
		ResourceManager* pResMan = ResourceManager::getPointer();
		pResMan->removeTexture2D(it->second->_mstrTexturename);

		delete it->second;
		_mmapImages.erase(it);
	}

	GUIImageAnimated* GUITooltip::addImageAnimated(const std::string& strName, float fPosX, float fPosY, const std::vector<std::string>& vecStrImageFilenames, float fWidth, float fHeight)
	{
		// If resource already exists
		std::map<std::string, GUIImageAnimated*>::iterator it = _mmapImageAnimateds.find(strName);
		ThrowIfTrue(it != _mmapImageAnimateds.end(), "GUITooltip::addImageAnimated(" + strName + ") failed. The named object already exists.");
		GUIImageAnimated* pNewRes = new GUIImageAnimated;
		ThrowIfFalse(pNewRes, "GUITooltip::addImageAnimated(" + strName + ") failed. Could not allocate memory for the new object.");
		pNewRes->mfPositionX = fPosX;
		pNewRes->mfPositionY = fPosY;
		pNewRes->mfWidth = fWidth;
		pNewRes->mfHeight = fHeight;
		pNewRes->_mstrResourceTexture2DAnimationName = strName;
		_mmapImageAnimateds[strName] = pNewRes;

		// Add ResourceTexture2DAnimation to the resource manager
		ResourceManager* pResMan = ResourceManager::getPointer();
		ResourceTexture2DAnimation* pTex = pResMan->addTexture2DAnimation(strName, vecStrImageFilenames);

		// If a value less than 0 is passed to width/height, set widget to dims of the image
		if (fWidth < 0)
			pNewRes->mfWidth = pTex->mvDimensions.x;
		if (fHeight < 0)
			pNewRes->mfHeight = pTex->mvDimensions.y;

		return pNewRes;
	}

	GUIImageAnimated* GUITooltip::getImageAnimated(const std::string& strName)
	{
		std::map<std::string, GUIImageAnimated*>::iterator it = _mmapImageAnimateds.find(strName);
		ThrowIfTrue(it == _mmapImageAnimateds.end(), "GUITooltip::getImageAnimated(" + strName + ") failed. The named object doesn't exist.");
		return it->second;
	}

	void GUITooltip::removeImageAnimated(const std::string& strName)
	{
		std::map<std::string, GUIImageAnimated*>::iterator it = _mmapImageAnimateds.find(strName);
		if (it == _mmapImageAnimateds.end())
			return;

		// Remove ResourceTexture2DAnimation from the resource manager
		ResourceManager* pResMan = ResourceManager::getPointer();
		pResMan->removeTexture2DAnimation(it->second->_mstrResourceTexture2DAnimationName);

		delete it->second;
		_mmapImageAnimateds.erase(it);
	}

	GUIImageFramebuffer* GUITooltip::addImageFramebuffer(const std::string& strName, float fPosX, float fPosY, const std::string& strFBname, float fWidth, float fHeight)
	{
		// If resource already exists
		std::map<std::string, GUIImageFramebuffer*>::iterator it = _mmapImageFramebuffers.find(strName);
		ThrowIfTrue(it != _mmapImageFramebuffers.end(), "GUITooltip::addImageFramebuffer(" + strName + ") failed. The named object already exists.");
		GUIImageFramebuffer* pNewRes = new GUIImageFramebuffer;
		ThrowIfFalse(pNewRes, "GUITooltip::addImageFramebuffer(" + strName + ") failed. Could not allocate memory for the new object.");
		pNewRes->mfPositionX = fPosX;
		pNewRes->mfPositionY = fPosY;
		pNewRes->mfWidth = fWidth;
		pNewRes->mfHeight = fHeight;
		pNewRes->_mstrFBname = strFBname;
		_mmapImageFramebuffers[strName] = pNewRes;

		// Get ResourceFramebuffer from the resource manager to set size of this object
		ResourceManager* pResMan = ResourceManager::getPointer();
		ResourceFramebuffer* pFB = pResMan->getFramebuffer(strFBname);

		// If a value less than 0 is passed to width/height, set widget to dims of the image
		if (fWidth < 0)
			pNewRes->mfWidth = (float)pFB->getWidth();
		if (fHeight < 0)
			pNewRes->mfHeight = (float)pFB->getHeight();

		return pNewRes;
	}

	GUIImageFramebuffer* GUITooltip::getImageFramebuffer(const std::string& strName)
	{
		std::map<std::string, GUIImageFramebuffer*>::iterator it = _mmapImageFramebuffers.find(strName);
		ThrowIfTrue(it == _mmapImageFramebuffers.end(), "GUITooltip::getImageFramebuffer(" + strName + ") failed. The named object doesn't exist.");
		return it->second;
	}

	void GUITooltip::removeImageFramebuffer(const std::string& strName)
	{
		std::map<std::string, GUIImageFramebuffer*>::iterator it = _mmapImageFramebuffers.find(strName);
		if (it == _mmapImageFramebuffers.end())
			return;

		delete it->second;
		_mmapImageFramebuffers.erase(it);
	}

	GUIImageDepthbuffer* GUITooltip::addImageDepthbuffer(const std::string& strName, float fPosX, float fPosY, const std::string& strDBname, float fWidth, float fHeight)
	{
		// If resource already exists
		std::map<std::string, GUIImageDepthbuffer*>::iterator it = _mmapImageDepthbuffers.find(strName);
		ThrowIfTrue(it != _mmapImageDepthbuffers.end(), "GUITooltip::addImageDepthbuffer(" + strName + ") failed. The named object already exists.");
		GUIImageDepthbuffer* pNewRes = new GUIImageDepthbuffer;
		ThrowIfFalse(pNewRes, "GUITooltip::addImageDepthbuffer(" + strName + ") failed. Could not allocate memory for the new object.");
		pNewRes->mfPositionX = fPosX;
		pNewRes->mfPositionY = fPosY;
		pNewRes->mfWidth = fWidth;
		pNewRes->mfHeight = fHeight;
		pNewRes->_mstrDBname = strDBname;
		_mmapImageDepthbuffers[strName] = pNewRes;

		// Get ResourceFramebuffer from the resource manager to set size of this object
		ResourceManager* pResMan = ResourceManager::getPointer();
		ResourceDepthbuffer* pDB = pResMan->getDepthbuffer(strDBname);

		// If a value less than 0 is passed to width/height, set widget to dims of the image
		if (fWidth < 0)
			pNewRes->mfWidth = (float)pDB->getWidth();
		if (fHeight < 0)
			pNewRes->mfHeight = (float)pDB->getHeight();

		return pNewRes;
	}

	GUIImageDepthbuffer* GUITooltip::getImageDepthbuffer(const std::string& strName)
	{
		std::map<std::string, GUIImageDepthbuffer*>::iterator it = _mmapImageDepthbuffers.find(strName);
		ThrowIfTrue(it == _mmapImageDepthbuffers.end(), "GUITooltip::getImageDepthbuffer(" + strName + ") failed. The named object doesn't exist.");
		return it->second;
	}

	void GUITooltip::removeImageDepthbuffer(const std::string& strName)
	{
		std::map<std::string, GUIImageDepthbuffer*>::iterator it = _mmapImageDepthbuffers.find(strName);
		if (it == _mmapImageDepthbuffers.end())
			return;

		delete it->second;
		_mmapImageDepthbuffers.erase(it);
	}
}