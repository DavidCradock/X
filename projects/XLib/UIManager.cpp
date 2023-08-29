#include "PCH.h"
#include "UIManager.h"
#include "singletons.h"

namespace X
{
	SCUIManager::SCUIManager()
	{
		_mTimer.update();
	}

	void SCUIManager::render(void)
	{
		// Update everything first
		_update();

		// Make sure the UI framebuffer is of the same size as the window
		CResourceFramebuffer* pUIFB = x->pResource->getFramebuffer(x->pResource->defaultRes.framebuffer_ui);
		CVector2f fWindowDims = x->pWindow->getDimensions();
		if (pUIFB->getDimensions() != fWindowDims)
		{
			pUIFB->resize((unsigned int)fWindowDims.x, (unsigned int)fWindowDims.y);
		}
		pUIFB->bindAsRenderTarget(true, false);

		// For each container
		for (size_t i = 0; i < _mmanContainers.getNumber(); i++)
		{
			_mmanContainers.get(i)->render(false);
		}

		// For each window, render with the ones at the back, first.
		auto itWindow = _mlistWindowZOrder.begin();
		while (itWindow != _mlistWindowZOrder.end())
		{
			_mmanWindows.get(*itWindow)->render();
			itWindow++;
		}

		pUIFB->unbindAsRenderTarget();

	}

	std::string SCUIManager::getMouseIsOver(void)
	{
		return _mstrMouseIsOver;
	}

	void SCUIManager::_update(void)
	{
		// Set _mstrMouseIsOver to hold name of container or window the mouse is currently over
		// Set _bMouseIsOverAWindow to differentiate between a container or window.
		// _mstrMouseIsOver may have zero size if mouse isn't over anything.
		_updateMouseIsOver();

		// Update timing and limit delta to a small value
		_mTimer.update();
		float fTimeDeltaSeconds = _mTimer.getSecondsPast();
		if (fTimeDeltaSeconds > 0.1f)
			fTimeDeltaSeconds = 0.1f;

		// For each container, update all of it's widgets
		for (size_t i = 0; i < _mmanContainers.getNumber(); i++)
		{
			_mmanContainers.get(i)->update(fTimeDeltaSeconds, false);
		}

		// For each window

		// Deal with moving windows
		// Stop moving any windows if the mouse button if up
		if (!x->pInput->mouse.leftButDown())
			_mstrWindowBeingMoved.clear();
		// If the mouse button has been pressed once, see if we can start moving a window
		// and set _mstrMouseIsOver to the window's name
		if (x->pInput->mouse.leftButtonOnce())
		{
			// Is the mouse over a window or container?
			if (_mstrMouseIsOver.size())
			{
				// Is the mouse over a window?
				if (_bMouseIsOverAWindow)
				{
					// Get the window the mouse is over
					CUIWindow* pWindow = _mmanWindows.get(_mstrMouseIsOver);
					// Check to see if the mouse cursor is over the window's titlebar
					CRect rctTitlebar = pWindow->getTitlebarArea();
					if (rctTitlebar.doesPositionFitWithin(x->pInput->mouse.getCursorPos()))
					{
						// Start moving the thing...
						_mstrWindowBeingMoved = _mstrMouseIsOver;
					}
				}
			}
		}
		// Deal with updating the position of a window that is set as being moved
		if (_mstrWindowBeingMoved.size())
		{
			// Compute new position of the window
			CUIWindow* pWindow = _mmanWindows.get(_mstrWindowBeingMoved);
			CVector2f vNewPos = pWindow->getPosition();
			vNewPos.x += x->pInput->mouse.getMouseDeltaGUI().x;
			vNewPos.y += x->pInput->mouse.getMouseDeltaGUI().y;
			
			// Limit the window to the screen.
			CVector2f vWindowActualDims = pWindow->getDimsIncludingTheme();
			bool bLimitMouseMovementX = false;
			bool bLimitMouseMovementY = false;
			// Off left edge of screen
			if (vNewPos.x < 0)
			{
				vNewPos.x = 0;
				bLimitMouseMovementX = true;
			}
			// Off right edge of screen
			else if (vNewPos.x + vWindowActualDims.x > x->pWindow->getDimensions().x)
			{
				vNewPos.x = x->pWindow->getDimensions().x - vWindowActualDims.x;
				bLimitMouseMovementX = true;
			}
			// Off top edge of screen
			if (vNewPos.y < 0)
			{
				vNewPos.y = 0;
				bLimitMouseMovementY = true;
			}
			// Off bottom edge of screen
			else if (vNewPos.y + vWindowActualDims.y > x->pWindow->getDimensions().y)
			{
				vNewPos.y = x->pWindow->getDimensions().y - vWindowActualDims.y;
				bLimitMouseMovementY = true;
			}
			// Limit movement of mouse cursor
			if (bLimitMouseMovementX || bLimitMouseMovementY)
			{
				CVector2f vMouseOldPos = x->pInput->mouse.getCursorPos();
				if (bLimitMouseMovementX)
					vMouseOldPos.x -= x->pInput->mouse.getMouseDeltaGUI().x;
				if (bLimitMouseMovementY)
					vMouseOldPos.y -= x->pInput->mouse.getMouseDeltaGUI().y;
				x->pInput->mouse.setMousePos(vMouseOldPos);
			}

			// Set position of the window
			pWindow->setPosition(vNewPos);
		}

		// Deal with Z order of windows
		// If a window is being moved, it MUST be the window in front
		if (_mstrWindowBeingMoved.size())
		{
			windowMoveToFront(_mstrWindowBeingMoved);
		}
		// If the cursor is over a window and the mouse button is down, bring that window to the front
		else if (x->pInput->mouse.leftButtonOnce())
		{
			if (_bMouseIsOverAWindow)
			{
				if (_mstrMouseIsOver.size())
				{
					windowMoveToFront(_mstrMouseIsOver);
				}
			}
		}
		
		// Deal with setting focus for all windows
		if (x->pInput->mouse.leftButtonOnce())
		{
			if (_bMouseIsOverAWindow)
			{
				if (_mstrMouseIsOver.size())
				{
					windowSetFocused(_mstrMouseIsOver);
				}
			}
		}

		// For each window, update all of it's widgets
		// Also set focus state of all windows
		for (size_t i = 0; i < _mmanWindows.getNumber(); i++)
		{
			// Update the window's widgets
			CUIWindow* pWindow = _mmanWindows.get(i);
			pWindow->update(fTimeDeltaSeconds);
		}

	}

	/************************************************************************************************************************************************************/
	/* Container related */
	/************************************************************************************************************************************************************/

	CUIContainer* SCUIManager::containerAdd(const std::string& strName, bool bLocked)
	{
		CUIContainer* pNew = _mmanContainers.add(strName, bLocked);
		pNew->_mstrName = strName;
		return pNew;
	}

	bool SCUIManager::containerExists(const std::string& strName) const
	{
		return _mmanContainers.exists(strName);
	}

	CUIContainer* SCUIManager::containerGet(const std::string& strName) const
	{
		return _mmanContainers.get(strName);
	}

	CUIContainer* SCUIManager::containerGet(size_t index) const
	{
		return _mmanContainers.get(index);
	}

	std::string SCUIManager::containerGetName(size_t index) const
	{
		return _mmanContainers.getName(index);
	}

	size_t SCUIManager::containerGetNumber(void) const
	{
		return _mmanContainers.getNumber();
	}

	void SCUIManager::containerRemove(const std::string& strName, bool bForceRemoveLocked)
	{
		if (_mmanContainers.exists(strName))
			_mmanContainers.remove(strName, bForceRemoveLocked);
	}

	void SCUIManager::containerRemoveAll(bool bForceRemoveLocked)
	{
		_mmanContainers.removeAll(bForceRemoveLocked);
	}

	/************************************************************************************************************************************************************/
	/* Theme related */
	/************************************************************************************************************************************************************/

	CUITheme* SCUIManager::themeAdd(const std::string& strName, bool bLocked)
	{
		return _mmanThemes.add(strName, bLocked);
	}

	bool SCUIManager::themeExists(const std::string& strName) const
	{
		return _mmanThemes.exists(strName);
	}

	CUITheme* SCUIManager::themeGet(const std::string& strName) const
	{
		return _mmanThemes.get(strName);
	}

	CUITheme* SCUIManager::themeGet(size_t index) const
	{
		return _mmanThemes.get(index);
	}

	std::string SCUIManager::themeGetName(size_t index) const
	{
		return _mmanThemes.getName(index);
	}

	size_t SCUIManager::themeGetNumber(void) const
	{
		return _mmanThemes.getNumber();
	}

	void SCUIManager::themeRemove(const std::string& strName, bool bForceRemoveLocked)
	{
		if (_mmanThemes.exists(strName))
			_mmanThemes.remove(strName, bForceRemoveLocked);
	}

	void SCUIManager::themeRemoveAll(bool bForceRemoveLocked)
	{
		_mmanThemes.removeAll(bForceRemoveLocked);
	}

	void SCUIManager::themeSetForAll(const std::string& strName)
	{
		ThrowIfFalse(_mmanThemes.exists(strName), "SCUIManager::setThemeForAll(\"" + strName + "\") failed. Invalid name given.");

		// Set theme name for all containers
		for (size_t i = 0; i < _mmanContainers.getNumber(); i++)
		{
			_mmanContainers.get(i)->themeNameSet(strName);
		}

		// Set theme name for all windows
		for (size_t i = 0; i < _mmanWindows.getNumber(); i++)
		{
			_mmanWindows.get(i)->themeNameSet(strName);
		}
	}

	/************************************************************************************************************************************************************/
	/* Window related */
	/************************************************************************************************************************************************************/

	CUIWindow* SCUIManager::windowAdd(const std::string& strName, bool bLocked)
	{
		// Add window name to the z order list
		_mlistWindowZOrder.push_back(strName);

		CUIWindow* pNew = _mmanWindows.add(strName, bLocked);
		pNew->_mstrName = strName;
		return pNew;
	}

	bool SCUIManager::windowExists(const std::string& strName) const
	{
		return _mmanWindows.exists(strName);
	}

	CUIWindow* SCUIManager::windowGet(const std::string& strName) const
	{
		return _mmanWindows.get(strName);
	}

	CUIWindow* SCUIManager::windowGet(size_t index) const
	{
		return _mmanWindows.get(index);
	}

	std::string SCUIManager::windowGetName(size_t index) const
	{
		return _mmanWindows.getName(index);
	}

	size_t SCUIManager::windowGetNumber(void) const
	{
		return _mmanWindows.getNumber();
	}

	void SCUIManager::windowRemove(const std::string& strName, bool bForceRemoveLocked)
	{
		if (_mmanWindows.exists(strName))
			_mmanWindows.remove(strName, bForceRemoveLocked);

		// If the window no longer exists, remove window name from the Z order list
		if (!_mmanWindows.exists(strName))
		{
			auto itlist = std::find(_mlistWindowZOrder.begin(), _mlistWindowZOrder.end(), strName);
			if (itlist != _mlistWindowZOrder.end())
				_mlistWindowZOrder.erase(itlist);
		}
	}

	void SCUIManager::windowRemoveAll(bool bForceRemoveLocked)
	{
		// Get names of each window for use in a bit.
		std::vector<std::string> vecstrWindowNames;
		for (size_t i = 0; i < _mmanWindows.getNumber(); i++)
			vecstrWindowNames.push_back(_mmanWindows.getName(i));

		// Attempt to remove all containers
		_mmanWindows.removeAll(bForceRemoveLocked);

		// Remove all windows from the z order list if they no longer exist
		auto itWindowNames = vecstrWindowNames.begin();
		while (itWindowNames != vecstrWindowNames.end())
		{
			if (!_mmanWindows.exists(*itWindowNames))
			{
				auto itlist = std::find(_mlistWindowZOrder.begin(), _mlistWindowZOrder.end(), *itWindowNames);
				if (itlist != _mlistWindowZOrder.end())
					_mlistWindowZOrder.erase(itlist);
			}
			itWindowNames++;
		}
	}

	void SCUIManager::windowMoveToFront(const std::string& strName)
	{
		std::list<std::string>::iterator itlist = std::find(_mlistWindowZOrder.begin(), _mlistWindowZOrder.end(), strName);
		if (itlist != _mlistWindowZOrder.end())
		{
			_mlistWindowZOrder.erase(itlist);
			_mlistWindowZOrder.push_back(strName);
		}
	}

	std::string SCUIManager::windowBeingMoved(void)
	{
		return _mstrWindowBeingMoved;
	}

	void SCUIManager::windowBeingMoved(const std::string& strWindowBeingMoved)
	{
		_mstrWindowBeingMoved = strWindowBeingMoved;
	}

	void SCUIManager::_updateMouseIsOver(void)
	{
		// Compute which window name the mouse cursor is currently over, if any
		_mstrMouseIsOver.clear();
		CVector2f vMouseCursorPos = x->pInput->mouse.getCursorPos();
		// Go through each window by Z order to see which window the cursor is over, if any
		auto itWindowR = _mlistWindowZOrder.rbegin();
		CUIWindow* pWindow;
		while (itWindowR != _mlistWindowZOrder.rend())
		{
			// Get window dims and position
			pWindow = windowGet((*itWindowR));
			CVector2f vPos = pWindow->getPosition();
			CVector2f vDims = pWindow->getDimsIncludingTheme();
			if (vMouseCursorPos.x >= vPos.x)
			{
				if (vMouseCursorPos.x <= vPos.x + vDims.x)
				{
					if (vMouseCursorPos.y >= vPos.y)
					{
						if (vMouseCursorPos.y <= vPos.y + vDims.y)
						{
							// Mouse is over this window
							_mstrMouseIsOver = *itWindowR;
							_bMouseIsOverAWindow = true;
							break;
						}
					}
				}
			}
			itWindowR++;
		}
		// If we haven't found a window that the mouse cursor if over, see if it is over any containers
		if (!_mstrMouseIsOver.size())
		{
			for (size_t i = 0; i < _mmanContainers.getNumber(); i++)
			{
				CUIContainer* pCont = _mmanContainers.get(i);
				CVector2f vPos = pCont->getPosition();
				CVector2f vDims = pCont->getDimensions();
				if (vMouseCursorPos.x >= vPos.x)
				{
					if (vMouseCursorPos.x <= vPos.x + vDims.x)
					{
						if (vMouseCursorPos.y >= vPos.y)
						{
							if (vMouseCursorPos.y <= vPos.y + vDims.y)
							{
								// Mouse is over this window
								_mstrMouseIsOver = _mmanContainers.getName(i);
								_bMouseIsOverAWindow = false;
								break;
							}
						}
					}
				}
			}
		}
	}

	void SCUIManager::windowSetFocused(const std::string strWindowName)
	{
		ThrowIfFalse(windowExists(strWindowName), "SCUIManager::windowSetFocused(\"" + strWindowName + "\") failed. Given window name does not exist.");
		windowSetUnfocusedAll();
		_mmanWindows.get(strWindowName)->_mbInFocus = true;
	}

	void SCUIManager::windowSetUnfocusedAll(void)
	{
		for (size_t i = 0; i < _mmanWindows.getNumber(); i++)
		{
			_mmanWindows.get(i)->_mbInFocus = false;
		}
	}
}