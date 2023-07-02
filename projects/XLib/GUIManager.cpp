#include "PCH.h"
#include "GUIManager.h"
#include "log.h"
#include "input.h"
#include "window.h"
#include "resourceManager.h"
#include "audioManager.h"
#include "utilities.h"

namespace X
{
	GUIManager::GUIManager()
	{
		_mfScale = 1.0f;
		// Add default theme
		GUITheme *pTheme = addTheme("default");
		pTheme->loadTextures();
		pTheme->addFontsToManager();

		AudioManager::getPointer()->addNewSampleGroup("gui");
		pTheme->addAudioToManager();
		setAudioVol(1.0f);
		_mbWindowBeingMoved = false;
	}

	void GUIManager::render(const std::string& strFramebufferToSampleFrom)
	{
		InputManager* pInput = InputManager::getPointer();

		// Update the timer object
		_mTimer.update();

		// Update each container
		// Go through each container, starting with the one at the front first
		GUIContainer* pContainer;
		std::list<std::string>::iterator it = _mlistContainerZOrder.end();
		bool bMouseIsOverContainerWhichIsAboveThisOne = false;
		
		// Holds the name of a container which wishes to be moved on top
		// We store this and then after we've gone through all containers, move it to the top, to prevent messing with the list whilst updating
		std::string strWindowWantsToBePlacedInFront;
		while (it != _mlistContainerZOrder.begin())
		{
			it--;
			pContainer = getContainer(*it);

			if (pContainer->update(bMouseIsOverContainerWhichIsAboveThisOne))
			{
				// If clicked upon and mouse over this container, mark as wanting to be placed on top
				if (pInput->mouse.leftButDown())
				{
					strWindowWantsToBePlacedInFront = *it;
				}
				bMouseIsOverContainerWhichIsAboveThisOne = true;
			}
		}
		
		// Move container that has been clicked upon to the front
		if (strWindowWantsToBePlacedInFront.length())
		{
			moveContainerToFront(strWindowWantsToBePlacedInFront);
		}

		// Render each container

		// Go through each container, starting with the one at the back first
		it = _mlistContainerZOrder.begin();
		
		while (it != _mlistContainerZOrder.end())
		{
			pContainer = getContainer(*it);
			
			// Render the container
			pContainer->render(strFramebufferToSampleFrom);
			
			it++;
		}
		
		// Update the default containers if they are shown
		_updateDefaultContainers();
	}

	void GUIManager::setScale(float fScalingValue)
	{
		_mfScale = fScalingValue;
	}

	float GUIManager::getScale(void)
	{
		return _mfScale;
	}

	GUITheme* GUIManager::addTheme(const std::string& strName)
	{
		// Attempt to find by name
		std::map<std::string, GUITheme*>::iterator it = _mmapThemes.find(strName);
		// If it already exists, return a pointer to that.
		if (it != _mmapThemes.end())
			return it->second;

		// Allocate the new theme
		GUITheme* pNew = new GUITheme;
		ThrowIfFalse(pNew, "GUIManager::addTheme(" + strName + ") failed. Unable to allocate memory for the new theme.");

		// Place in the hashmap
		_mmapThemes[strName] = pNew;
		return pNew;
	}

	GUITheme* GUIManager::getTheme(const std::string& strName)
	{
		std::map<std::string, GUITheme*>::iterator it = _mmapThemes.find(strName);
		ThrowIfTrue(it == _mmapThemes.end(), "GUIManager::getTheme(" + strName + ") failed. Theme doesn't exist.");
		return it->second;
	}

	bool GUIManager::getThemeExists(const std::string& strName)
	{
		std::map<std::string, GUITheme*>::iterator it = _mmapThemes.find(strName);
		return it != _mmapThemes.end();
	}

	void GUIManager::removeTheme(const std::string& strName)
	{
		std::map<std::string, GUITheme*>::iterator it = _mmapThemes.find(strName);
		if (it == _mmapThemes.end())
			return;	// Doesn't exist.
		delete it->second;
		_mmapThemes.erase(it);
	}

	void GUIManager::removeAllThemes(void)
	{
		std::map<std::string, GUITheme*>::iterator it = _mmapThemes.begin();
		while (it != _mmapThemes.end())
		{
			delete it->second;
			it++;
		}
		_mmapThemes.clear();
	}

	int GUIManager::getNumberOfThemes(void)
	{
		return (int)_mmapThemes.size();
	}

	std::string GUIManager::getThemeName(int iIndex)
	{
		ThrowIfTrue(iIndex < 0 || iIndex >= (int)_mmapThemes.size(), "GUIManager::getThemeName() failed. Invalid index of " + std::to_string(iIndex) + " was given.");
		std::map<std::string, GUITheme*>::iterator it = _mmapThemes.begin();
		int iCount = 0;
		while (iCount < iIndex)
			it++;
		return it->first;
	}

	void GUIManager::setThemeForAll(const std::string& strName)
	{
		ThrowIfTrue(1, "GUIManager::setThemeForAll() failed. This method isn't implemented yet!");
	}


	GUIContainer* GUIManager::addContainer(const std::string& strName)
	{
		// Add container name to the Z order list
		_mlistContainerZOrder.push_back(strName);

		// Attempt to find by name
		std::map<std::string, GUIContainer*>::iterator it = _mmapContainers.find(strName);
		// If it already exists, return a pointer to that.
		if (it != _mmapContainers.end())
			return it->second;

		// Allocate the new object
		GUIContainer* pNew = new GUIContainer;
		ThrowIfFalse(pNew, "GUIManager::addContainer(" + strName + ") failed. Unable to allocate memory for the new container.");

		pNew->mstrTitleText = strName;

		// Store name of container inside the container, as it's used to create unique names for things such as the framebuffers used by the GUITextScroll object.
		pNew->_mstrName;	

		// Place in the hashmap
		_mmapContainers[strName] = pNew;
		return pNew;
	}

	GUIContainer* GUIManager::getContainer(const std::string& strName)
	{
		std::map<std::string, GUIContainer*>::iterator it = _mmapContainers.find(strName);
		ThrowIfTrue(it == _mmapContainers.end(), "GUIManager::getContainer(" + strName + ") failed. Container doesn't exist.");
		return it->second;
	}

	bool GUIManager::getContainerExists(const std::string& strName)
	{
		std::map<std::string, GUIContainer*>::iterator it = _mmapContainers.find(strName);
		return it != _mmapContainers.end();
	}

	void GUIManager::removeContainer(const std::string& strName)
	{
		// Remove container name from the Z order list
		std::list<std::string>::iterator itlist = std::find(_mlistContainerZOrder.begin(), _mlistContainerZOrder.end(), strName);
		if (itlist != _mlistContainerZOrder.end())
		{
			_mlistContainerZOrder.erase(itlist);
		}

		std::map<std::string, GUIContainer*>::iterator it = _mmapContainers.find(strName);
		if (it == _mmapContainers.end())
			return;	// Doesn't exist.
		delete it->second;
		_mmapContainers.erase(it);
	}

	void GUIManager::removeAllContainers(void)
	{
		// Clear the list holding Zordering
		_mlistContainerZOrder.clear();

		std::map<std::string, GUIContainer*>::iterator it = _mmapContainers.begin();
		while (it != _mmapContainers.end())
		{
			delete it->second;
			it++;
		}
		_mmapContainers.clear();
	}

	int GUIManager::getNumberOfContainers(void)
	{
		return (int)_mmapContainers.size();
	}

	std::string GUIManager::getContainerName(int iIndex)
	{
		ThrowIfTrue(iIndex < 0 || iIndex >= (int)_mmapContainers.size(), "GUIManager::getContainerName() failed. Invalid index of " + std::to_string(iIndex) + " was given.");
		std::map<std::string, GUIContainer*>::iterator it = _mmapContainers.begin();
		int iCount = 0;
		while (iCount < iIndex)
			it++;
		return it->first;
	}

	void GUIManager::moveContainerToFront(const std::string& strContainerName)
	{
		std::list<std::string>::iterator itlist = std::find(_mlistContainerZOrder.begin(), _mlistContainerZOrder.end(), strContainerName);
		if (itlist != _mlistContainerZOrder.end())
		{
			_mlistContainerZOrder.erase(itlist);
			_mlistContainerZOrder.push_back(strContainerName);
		}
	}

	bool GUIManager::getWindowBeingMoved(void)
	{
		return _mbWindowBeingMoved;
	}

	void GUIManager::setWindowBeingMoved(bool bWindowBeingMoved)
	{
		_mbWindowBeingMoved = bWindowBeingMoved;
	}

	void GUIManager::setAudioVol(float fVol)
	{
		clamp(fVol, 0.0f, 1.0f);
		_mfAudioVol = fVol;
	}

	float GUIManager::getAudioVol(void)
	{
		return _mfAudioVol;
	}

	void GUIManager::_createDefaultContainers(void)
	{
		GUIContainer* pCont = addContainer("X:Default:Statistics");
		pCont->setDimensions(450.0f, 480.0f);
		pCont->setPosition(0.0f, 0.0f);
		pCont->setVisible(false);
		pCont->mstrTitleText = "Statistics";

		GUILineGraph* pLineGraph = pCont->addLineGraph("Stats_FPS_Linegraph", 0, 0, 320, 120);
		GUIColour col;
		col.set(1.0f, 1.0f, 1.0f, 0.5f);	pLineGraph->addDataset("FPS", col);
		col.set(1.0f, 1.0f, 1.0f, 1.0f);	pLineGraph->addDataset("FPSAVR", col);
		pCont->addText("Text_FPSMax", 330, 0, "FPSMax: ")->setColour(false, 1.0f, 1.0f, 1.0f, 1.0f);
		pCont->addText("Text_FPSMin", 330, 120, "FPSMin: ")->setColour(false, 1.0f, 1.0f, 1.0f, 1.0f);
		pCont->addText("Text_FPS", 330, 100, "FPS: ")->setColour(false, 1.0f, 1.0f, 1.0f, 0.5f);
		pCont->addText("Text_FPSAVR", 330, 140, "FPSAVR: ")->setColour(false, 1.0f, 1.0f, 1.0f, 1.0f);
		_mDefContStatistics.fAddValueToLinegraphDataset = 0.0f;

		pCont->addText("Text_Mem1", 0, 160, "");
		pCont->addText("Text_Mem2", 0, 180, "");
	}

	void GUIManager::_updateDefaultContainers(void)
	{
		GUIManager* pGUIMan = GUIManager::getPointer();
		GUIContainer* pCont = getContainer("X:Default:Statistics");
		GUITheme* pTheme = pGUIMan->getTheme(pCont->mstrThemename);
		ResourceManager* pRM = ResourceManager::getPointer();

		if (pCont->getVisible())
		{
			GUILineGraph* pLineGraph = pCont->getLineGraph("Stats_FPS_Linegraph");
			
			// Add new value to linegraph data set
			_mDefContStatistics.fAddValueToLinegraphDataset += _mTimer.getSecondsPast();
			if (_mDefContStatistics.fAddValueToLinegraphDataset >= 0.1f)
			{
				_mDefContStatistics.fAddValueToLinegraphDataset = 0.0f;
				GUILineGraphDataSet* pDataSetFPS = pLineGraph->getDataset("FPS");
				GUILineGraphDataSet* pDataSetFPSAVR = pLineGraph->getDataset("FPSAVR");

				pDataSetFPS->addValue(_mTimer.getFPS());
				pDataSetFPSAVR->addValue(_mTimer.getFPSAveraged());

				while (pDataSetFPS->getNumValues() > 160)
					pDataSetFPS->removeValue();
				while (pDataSetFPSAVR->getNumValues() > 160)
					pDataSetFPSAVR->removeValue();

				// Set min and max text
				float fMaxFPS = pDataSetFPS->getHighestValue();
				float fMaxFPSAVR = pDataSetFPSAVR->getHighestValue();
				float fMax = fMaxFPS;
				if (fMax < fMaxFPSAVR)
					fMax = fMaxFPSAVR;

				std::string str = "FPSMax: " + std::to_string((int)fMax);
				pCont->getText("Text_FPSMax")->mstrText = str;

				float fMinFPS = pDataSetFPS->getLowestValue();
				float fMinFPSAVR = pDataSetFPSAVR->getLowestValue();
				float fMin = fMinFPS;
				if (fMin < fMinFPSAVR)
					fMin = fMinFPSAVR;

				str = "FPSMin: " + std::to_string((int)fMin);
				GUIText* pText = pCont->getText("Text_FPSMin");
				pText->mstrText = str;

				// Set Text_FPSMin position
				float fTextHeight = pRM->getFont(pTheme->mFonts.text)->getTextHeight();
				pText->mfPositionY = 120.0f - fTextHeight;

				// Set text values
				pText = pCont->getText("Text_FPS");
				pText->mstrText = "FPS: " + std::to_string((int)_mTimer.getFPS());
				pText->mfPositionY = 60.0f - (fTextHeight * 1.0f);
				pText = pCont->getText("Text_FPSAVR");
				pText->mstrText = "FPSAVR: " + std::to_string((int)_mTimer.getFPSAveraged());
				pText->mfPositionY = 60.0f - (fTextHeight * 0.0f);

				// Memory text
				SMemInfo memInfo;
				getMemInfo(memInfo);
				pCont->getText("Text_Mem1")->mstrText = "MemProcessWorkingSetSize: " + std::to_string(((memInfo.proc.iWorkingSetSize / 1024) / 1024)) + "MB";
				pCont->getText("Text_Mem2")->mstrText = "MemProcessiPrivateWorkingSet: " + std::to_string((((memInfo.proc.iPrivateWorkingSet) / 1024) / 1024)) + "MB";
			}
		}
	}
}