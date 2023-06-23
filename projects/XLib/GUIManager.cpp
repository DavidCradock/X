#include "PCH.h"
#include "GUIManager.h"
#include "log.h"
#include "input.h"
#include "window.h"
#include "resourceManager.h"

namespace X
{
	GUIManager::GUIManager()
	{
		_mfScale = 1.0f;
		// Add default theme
		GUITheme *pTheme = addTheme("default");
		pTheme->loadTextures();

	}

	void GUIManager::render(void)
	{
		InputManager* pInput = InputManager::getPointer();

		// Update the timer object
		mTimer.update();

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
		
		// Make sure the GUI framebuffer is the correct size
		Window* pWindow = Window::getPointer();
		ResourceManager* pRM = ResourceManager::getPointer();
		ResourceFramebuffer* pFB = pRM->getFramebuffer("X:gui");
		if (pFB->getWidth() != (int)pWindow->getWidth() || pFB->getHeight() != (int)pWindow->getHeight())
		{
			pFB->resize((unsigned int)pWindow->getWidth(), (unsigned int)pWindow->getHeight());
		}

		// Bind framebuffer as target
		pFB->bindAsRenderTarget(true);

		// Go through each container, starting with the one at the back first
		it = _mlistContainerZOrder.begin();
		while (it != _mlistContainerZOrder.end())
		{
			pContainer = getContainer(*it);
			
			// Render the container
			pContainer->render();
			
			it++;
		}

		// Unbind framebuffer
		pFB->unbindAsRenderTarget();

		// Render the GUI framebuffer to the backbuffer
		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
//		pFB->renderTo2DQuad(0, 0, (unsigned int)pWindow->getWidth(), (unsigned int)pWindow->getHeight());
		glDisable(GL_BLEND);
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

		// Allocate the new theme
		GUIContainer* pNew = new GUIContainer;
		ThrowIfFalse(pNew, "GUIManager::addContainer(" + strName + ") failed. Unable to allocate memory for the new container.");

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
}