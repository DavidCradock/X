#include "PCH.h"
#include "UIManager.h"
#include "singletons.h"

namespace X
{
	SCUIManager::SCUIManager()
	{
		_mTimer.update();
		_mstrMouseCursorThemename = "default";
	}

	void SCUIManager::render(void)
	{
		// Update everything first
		x->pProfiler->begin("x->pUI->_update()");
		_update();
		x->pProfiler->end("x->pUI->_update()");

		x->pProfiler->begin("SCUIManager::render()");

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
			_mmanContainers.get(i)->render();
		}

		// For each window, render with the ones at the back, first.
		auto itWindow = _mlistWindowZOrder.begin();
		while (itWindow != _mlistWindowZOrder.end())
		{
			_mmanWindows.get(*itWindow)->render();
			itWindow++;
		}

		// Render each container's tooltips
		for (size_t i = 0; i < _mmanContainers.getNumber(); i++)
		{
			_mmanContainers.get(i)->renderTooltips();
		}

		// Debug rendering of the window resizing areas
//		itWindow = _mlistWindowZOrder.begin();
//		while (itWindow != _mlistWindowZOrder.end())
//		{
//			_mmanWindows.get(*itWindow)->_debugRenderAreas();
//			itWindow++;
//		}
		
		pUIFB->unbindAsRenderTarget();

		x->pProfiler->end("SCUIManager::render()");
	}

	std::string SCUIManager::getMouseIsOverWhichContainer(void)
	{
		return _mstrMouseIsOver;
	}

	void SCUIManager::reset(void)
	{
		// For each container
		for (size_t i = 0; i < _mmanContainers.getNumber(); i++)
		{
			_mmanContainers.get(i)->reset();
		}

		// For each window
		for (size_t i = 0; i < _mmanWindows.getNumber(); i++)
		{
			_mmanWindows.get(i)->reset();
		}
	}

	void SCUIManager::onToggleFullscreen(void)
	{
		// For each container
		for (size_t i = 0; i < _mmanContainers.getNumber(); i++)
		{
			_mmanContainers.get(i)->_onToggleFullscreen();
		}

		// For each window
		for (size_t i = 0; i < _mmanWindows.getNumber(); i++)
		{
			_mmanWindows.get(i)->_onToggleFullscreen();
		}

		// When the window is toggled between windowed and fullscreen, it is destroyed.
		// Because of this, the mouse cursor needs to be reset to what it was prior to the window being destroyed.
		// We do this here...
		x->pResource->getMouseCursor(x->pUI->mouseCursorGetPreviouslySet())->set();
	}

	CUIDefaultContainers* SCUIManager::getDefaultContainers(void)
	{
		return &_mDefaultContainers;
	}

	void SCUIManager::_initialiseDefaultContainers(void)
	{
		_mDefaultContainers.initialise();
	}

	void SCUIManager::_update(void)
	{
		// Sets _mstrMouseIsOver to hold name of container or window the mouse is currently over
		// Sets _bMouseIsOverAWindow to differentiate between a container or window.
		// _mstrMouseIsOver may have zero size if mouse isn't over anything.
		_updateMouseIsOver();

		// Update timing and limit delta to a small value
		_mTimer.update();
		float fTimeDeltaSeconds = _mTimer.getSecondsPast();
		if (fTimeDeltaSeconds > 0.1f)
			fTimeDeltaSeconds = 0.1f;

		// Update default containers
		_mDefaultContainers.update(fTimeDeltaSeconds);

		// For each container, update all of it's widgets
		for (size_t i = 0; i < _mmanContainers.getNumber(); i++)
		{
			_mmanContainers.get(i)->update(fTimeDeltaSeconds);
		}

		// Deal with moving windows
		_updateWindowMoving();

		// Deal with resizing windows
		_updateWindowResizing();

		// Deal with Z order of windows
		// If a window is being moved or resized, surely it must be the window in front
		if (_mstrWindowBeingMoved.size())
			windowMoveToFront(_mstrWindowBeingMoved);
		else if (_mstrWindowBeingResized.size())
			windowMoveToFront(_mstrWindowBeingResized);
		
		// If the cursor is over a window and the mouse button is down, bring that window to the front and set that window as focused
		// Deal with setting focus for all windows
		if (x->pInput->mouse.leftButtonOnce())
		{
			if (_bMouseIsOverAWindow)
			{
				if (_mstrMouseIsOver.size())
				{
					windowMoveToFront(_mstrMouseIsOver);
					windowSetFocused(_mstrMouseIsOver);
				}
			}
		}

		// For each window, update all of it's widgets
		for (size_t i = 0; i < _mmanWindows.getNumber(); i++)
		{
			// Update the window's widgets
			CUIWindow* pWindow = _mmanWindows.get(i);
			pWindow->update(fTimeDeltaSeconds);
		}
	}

	void SCUIManager::_updateWindowMoving(void)
	{
		// If a window is currently being resized, don't start moving any windows
		if (_mstrWindowBeingResized.size())
			return;

		// Stop moving any windows if the mouse button is up
		if (!x->pInput->mouse.leftButDown())
			_mstrWindowBeingMoved.clear();

		// If the mouse button has been pressed once, see if we can start moving a window
		// and set _mstrWindowBeingMoved to _mstrMouseIsOver (the window's name)
		if (x->pInput->mouse.leftButtonOnce())
		{
			// Is the mouse over either a container or a window?
			if (_mstrMouseIsOver.size())
			{
				// Is the mouse over a window?
				if (_bMouseIsOverAWindow)
				{
					// Get a pointer to the window the mouse is over
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
	}

	void SCUIManager::_updateWindowResizing(void)
	{
		// If a window is currently being moved, don't start resizing any windows
		if (_mstrWindowBeingMoved.size())
			return;

		// _mstrMouseIsOver holds the name of a container or window which the mouse is currently over
		// _bMouseIsOverAWindow is set to differentiate between a container or window.
		// _mstrMouseIsOver may have zero size if mouse isn't over anything.
		// We've also checked to see if a window can and is being moved and if _mstrWindowBeingMoved has a length, then a window is being moved.
		// If the mouse button has been pressed once, see if we can start resizing a window
		// and set _mstrWindowBeingResized to _mstrMouseIsOver (the window's name)

		if (!_mstrMouseIsOver.size())
			mouseCursorSetToNormal();

		// Is the mouse over either a container or a window?
		if (_mstrMouseIsOver.size())
		{
			// Is the mouse over a window?
			if (_bMouseIsOverAWindow)
			{
				// Get a pointer to the window the mouse is over
				CUIWindow* pWindow = _mmanWindows.get(_mstrMouseIsOver);

				// If the window is set to be resizable
				if (pWindow->getResizable())
				{
					// Check to see if the mouse cursor is over one of the window's 8 edges/corners
					CUIWindow::EWindowArea mouseOverResizeArea = pWindow->_getMouseOverResizableArea();
					if (CUIWindow::EWindowArea::mouseOverNone != mouseOverResizeArea)
					{
						// Set correct mouse cursor
						if (CUIWindow::EWindowArea::mouseOverCornerBL == mouseOverResizeArea)
							x->pUI->mouseCursorSetToWindowResizeTRtoBL();
						else if (CUIWindow::EWindowArea::mouseOverCornerBR == mouseOverResizeArea)
							x->pUI->mouseCursorSetToWindowResizeTLtoBR();
//						else if (CUIWindow::EWindowArea::mouseOverCornerTL == mouseOverResizeArea)
//							x->pUI->mouseCursorSetToWindowResizeTLtoBR();
//						else if (CUIWindow::EWindowArea::mouseOverCornerTR == mouseOverResizeArea)
//							x->pUI->mouseCursorSetToWindowResizeTRtoBL();
						else if (CUIWindow::EWindowArea::mouseOverEdgeB == mouseOverResizeArea)
							x->pUI->mouseCursorSetToWindowResizeTtoB();
						else if (CUIWindow::EWindowArea::mouseOverEdgeL == mouseOverResizeArea)
							x->pUI->mouseCursorSetToWindowResizeLtoR();
						else if (CUIWindow::EWindowArea::mouseOverEdgeR == mouseOverResizeArea)
							x->pUI->mouseCursorSetToWindowResizeLtoR();
//						else if (CUIWindow::EWindowArea::mouseOverEdgeT == mouseOverResizeArea)
//							x->pUI->mouseCursorSetToWindowResizeTtoB();
					}
					else
					{
						x->pUI->mouseCursorSetToNormal();
					}

					if (x->pInput->mouse.leftButtonOnce())
					{
						// Start resizing the thing...
						_mstrWindowBeingResized = _mstrMouseIsOver;

						// Store which area the mouse is over
						_eWindowAreaTriggeredResizing = mouseOverResizeArea;
					}
				}
			}
		}

		// Deal with updating the position and dimensions of a window that is set as being resized
		if (_mstrWindowBeingResized.size())
		{
			CUIWindow* pWindow = _mmanWindows.get(_mstrWindowBeingResized);
			CVector2f vMouseDelta = x->pInput->mouse.getMouseDeltaGUI();

			// Depending upon which area triggered the resizing which is stored in _eWindowAreaTriggeredResizing,
			// Adjust dimensions and position of window accordingly.

			CVector2f vPos = pWindow->getPosition();
			CVector2f vDims = pWindow->getDimensions();
			CVector2f vMinDims;
			CVector2f vMaxDims;
			pWindow->getResizableDims(vMinDims, vMaxDims);

			if (CUIWindow::EWindowArea::mouseOverCornerBL == _eWindowAreaTriggeredResizing)
			{
				if (vMouseDelta.x > 0 && vDims.x > vMinDims.x)
				{
					vPos.x += vMouseDelta.x;
					vDims.x -= vMouseDelta.x;
				}
				else if (vMouseDelta.x < 0 && vDims.x < vMaxDims.x)
				{
					vPos.x += vMouseDelta.x;
					vDims.x -= vMouseDelta.x;
				}
				if (vMouseDelta.y > 0 && vDims.y < vMaxDims.y)
				{
					vDims.y += vMouseDelta.y;
				}
				else if (vMouseDelta.y < 0 && vDims.y > vMinDims.y)
				{
					vDims.y += vMouseDelta.y;
				}
			}
			else if (CUIWindow::EWindowArea::mouseOverCornerBR == _eWindowAreaTriggeredResizing)
			{
				if (vMouseDelta.x > 0 && vDims.x < vMaxDims.x)
				{
					vDims.x += vMouseDelta.x;
				}
				else if (vMouseDelta.x < 0 && vDims.x > vMinDims.x)
				{
					vDims.x += vMouseDelta.x;
				}
				if (vMouseDelta.y > 0 && vDims.y < vMaxDims.y)
				{
					vDims.y += vMouseDelta.y;
				}
				else if (vMouseDelta.y < 0 && vDims.y > vMinDims.y)
				{
					vDims.y += vMouseDelta.y;
				}
			}
			else if (CUIWindow::EWindowArea::mouseOverCornerTL == _eWindowAreaTriggeredResizing)
			{
//				vPos.x += vMouseDelta.x;
//				vPos.y += vMouseDelta.y;
//				vDims.x += vMouseDelta.x;
//				vDims.y += vMouseDelta.y;
			}
			else if (CUIWindow::EWindowArea::mouseOverCornerTR == _eWindowAreaTriggeredResizing)
			{
//				vPos.x += vMouseDelta.x;
//				vPos.y += vMouseDelta.y;
//				vDims.x += vMouseDelta.x;
//				vDims.y += vMouseDelta.y;
			}
			else if (CUIWindow::EWindowArea::mouseOverEdgeB == _eWindowAreaTriggeredResizing)
			{
				if (vMouseDelta.y > 0 && vDims.y < vMaxDims.y)
				{
					vDims.y += vMouseDelta.y;
				}
				else if (vMouseDelta.y < 0 && vDims.y > vMinDims.y)
				{
					vDims.y += vMouseDelta.y;
				}
			}
			else if (CUIWindow::EWindowArea::mouseOverEdgeL == _eWindowAreaTriggeredResizing)
			{
				if (vMouseDelta.x > 0 && vDims.x > vMinDims.x)
				{
					vPos.x += vMouseDelta.x;
					vDims.x -= vMouseDelta.x;
				}
				else if (vMouseDelta.x < 0 && vDims.x < vMaxDims.x)
				{
					vPos.x += vMouseDelta.x;
					vDims.x -= vMouseDelta.x;
				}
			}
			else if (CUIWindow::EWindowArea::mouseOverEdgeR == _eWindowAreaTriggeredResizing)
			{
				if (vMouseDelta.x > 0 && vDims.x < vMaxDims.x)
				{
					vDims.x += vMouseDelta.x;
				}
				else if (vMouseDelta.x < 0 && vDims.x > vMinDims.x)
				{
					vDims.x += vMouseDelta.x;
				}
			}
			else if (CUIWindow::EWindowArea::mouseOverEdgeT == _eWindowAreaTriggeredResizing)
			{
//				vPos.x += vMouseDelta.x;
//				vPos.y += vMouseDelta.y;
//				vDims.x += vMouseDelta.x;
//				vDims.y += vMouseDelta.y;
			}
			pWindow->setPosition(vPos);

			// Limit dims to min/max
			clamp(vDims.x, vMinDims.x, vMaxDims.x);
			clamp(vDims.y, vMinDims.y, vMaxDims.y);

			pWindow->setDimensions(vDims);
		}

		// Stop resizing any windows if the mouse button is up
		if (_mstrWindowBeingResized.size())
		{
			if (!x->pInput->mouse.leftButDown())
			{
				x->pUI->mouseCursorSetToNormal();
				_mstrWindowBeingResized.clear();
			}
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
		pNew->_mstrTitlebarText = strName;
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
			pWindow = windowGet((*itWindowR));
			// Window has to be visible
			if (pWindow->getVisible())
			{
				// Get window dims and position
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
			}
			itWindowR++;
		}
		// If we haven't found a window that the mouse cursor if over, see if it is over any containers
		if (!_mstrMouseIsOver.size())
		{
			for (size_t i = 0; i < _mmanContainers.getNumber(); i++)
			{
				CUIContainer* pCont = _mmanContainers.get(i);
				// Container has to be visible
				if (pCont->getVisible())
				{
					CVector2f vPos = pCont->getPosition();
					CVector2f vDims = pCont->getDimensions();

					// Increase dims to accomodate the scrollbar as it is placed at pCont->getPosition() + pCont->getDimensions().
					vDims.y += pCont->themeGetSettings()->floats.windowScrollbarHorizontalHeight;
					vDims.x += pCont->themeGetSettings()->floats.windowScrollbarVerticalWidth;

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

	
	void SCUIManager::_helperAddWidgetBGGeometry(
		const CVector2f& vPosition,
		const CVector2f& vDimensions,
		CUITheme::SImageType &imageType,
		const CColour& colour,
		CUIContainer* pContainer,
		CResourceVertexBufferCPT2* pVB)
	{
		CUITheme* pTheme = pContainer->themeGet();
		CResourceTexture2DAtlas* pAtlas = pTheme->getTextureAtlas();
		
		CImageAtlasDetails* idColC = pAtlas->getImageDetailsPointer(imageType.colour.centre);
		CImageAtlasDetails* idNormC = pAtlas->getImageDetailsPointer(imageType.normal.centre);
		CImageAtlasDetails* idColBL = pAtlas->getImageDetailsPointer(imageType.colour.cornerBL);
		CImageAtlasDetails* idNormBL = pAtlas->getImageDetailsPointer(imageType.normal.cornerBL);
		CImageAtlasDetails* idColBR = pAtlas->getImageDetailsPointer(imageType.colour.cornerBR);
		CImageAtlasDetails* idNormBR = pAtlas->getImageDetailsPointer(imageType.normal.cornerBR);
		CImageAtlasDetails* idColTL = pAtlas->getImageDetailsPointer(imageType.colour.cornerTL);
		CImageAtlasDetails* idNormTL = pAtlas->getImageDetailsPointer(imageType.normal.cornerTL);
		CImageAtlasDetails* idColTR = pAtlas->getImageDetailsPointer(imageType.colour.cornerTR);
		CImageAtlasDetails* idNormTR = pAtlas->getImageDetailsPointer(imageType.normal.cornerTR);
		CImageAtlasDetails* idColB = pAtlas->getImageDetailsPointer(imageType.colour.edgeB);
		CImageAtlasDetails* idNormB = pAtlas->getImageDetailsPointer(imageType.normal.edgeB);
		CImageAtlasDetails* idColL = pAtlas->getImageDetailsPointer(imageType.colour.edgeL);
		CImageAtlasDetails* idNormL = pAtlas->getImageDetailsPointer(imageType.normal.edgeL);
		CImageAtlasDetails* idColR = pAtlas->getImageDetailsPointer(imageType.colour.edgeR);
		CImageAtlasDetails* idNormR = pAtlas->getImageDetailsPointer(imageType.normal.edgeR);
		CImageAtlasDetails* idColT = pAtlas->getImageDetailsPointer(imageType.colour.edgeT);
		CImageAtlasDetails* idNormT = pAtlas->getImageDetailsPointer(imageType.normal.edgeT);

		const CUITheme::SSettings* pThemeSettings = pTheme->getSettings();

		// Set actual position of widget
		CVector2f vPos = pContainer->getWidgetAreaTLCornerPosition();
		vPos += vPosition;

		CVector2f vDims;

		// Top left corner
		vDims = idColTL->vDims;
		CVector2f vCellPos = vPos;
		pVB->addQuad2D(
			vCellPos,
			vDims,
			colour,
			idColTL->sTexCoords.vBL,
			idColTL->sTexCoords.vBR,
			idColTL->sTexCoords.vTR,
			idColTL->sTexCoords.vTL,
			idNormTL->sTexCoords.vBL,
			idNormTL->sTexCoords.vBR,
			idNormTL->sTexCoords.vTR,
			idNormTL->sTexCoords.vTL);

		// Top edge
		vCellPos = vPos;
		vCellPos.x += idColTL->vDims.x;
		vDims.x = vDimensions.x - idColTR->vDims.x;
		vDims.y = idColT->vDims.y;
		pVB->addQuad2D(
			vCellPos,
			vDims,
			colour,
			idColT->sTexCoords.vBL,
			idColT->sTexCoords.vBR,
			idColT->sTexCoords.vTR,
			idColT->sTexCoords.vTL,
			idNormT->sTexCoords.vBL,
			idNormT->sTexCoords.vBR,
			idNormT->sTexCoords.vTR,
			idNormT->sTexCoords.vTL);

		// Top right corner
		vCellPos = vPos;
		vCellPos.x += vDimensions.x - idColTR->vDims.x;
		vDims = idColTR->vDims;
		pVB->addQuad2D(
			vCellPos,
			vDims,
			colour,
			idColTR->sTexCoords.vBL,
			idColTR->sTexCoords.vBR,
			idColTR->sTexCoords.vTR,
			idColTR->sTexCoords.vTL,
			idNormTR->sTexCoords.vBL,
			idNormTR->sTexCoords.vBR,
			idNormTR->sTexCoords.vTR,
			idNormTR->sTexCoords.vTL);

		// Left edge
		vCellPos = vPos;
		vCellPos.y += idColTL->vDims.y;
		vDims.x = idColL->vDims.x;
		vDims.y = vDimensions.y - idColTL->vDims.y - idColBL->vDims.y;
		pVB->addQuad2D(
			vCellPos,
			vDims,
			colour,
			idColL->sTexCoords.vBL,
			idColL->sTexCoords.vBR,
			idColL->sTexCoords.vTR,
			idColL->sTexCoords.vTL,
			idNormL->sTexCoords.vBL,
			idNormL->sTexCoords.vBR,
			idNormL->sTexCoords.vTR,
			idNormL->sTexCoords.vTL);

		// Centre
		vCellPos = vPos;
		vCellPos.x += idColL->vDims.x;
		vCellPos.y += idColTL->vDims.y;
		vDims.x = vDimensions.x - idColL->vDims.x - idColR->vDims.x;
		vDims.y = vDimensions.y - idColTL->vDims.y - idColBL->vDims.y;
		pVB->addQuad2D(
			vCellPos,
			vDims,
			colour,
			idColC->sTexCoords.vBL,
			idColC->sTexCoords.vBR,
			idColC->sTexCoords.vTR,
			idColC->sTexCoords.vTL,
			idNormC->sTexCoords.vBL,
			idNormC->sTexCoords.vBR,
			idNormC->sTexCoords.vTR,
			idNormC->sTexCoords.vTL);

		// Right edge
		vCellPos = vPos;
		vCellPos.x += vDimensions.x - idColR->vDims.x;
		vCellPos.y += idColTL->vDims.y;
		vDims.x = idColR->vDims.x;
		vDims.y = vDimensions.y - idColL->vDims.y - idColR->vDims.y;
		pVB->addQuad2D(
			vCellPos,
			vDims,
			colour,
			idColR->sTexCoords.vBL,
			idColR->sTexCoords.vBR,
			idColR->sTexCoords.vTR,
			idColR->sTexCoords.vTL,
			idNormR->sTexCoords.vBL,
			idNormR->sTexCoords.vBR,
			idNormR->sTexCoords.vTR,
			idNormR->sTexCoords.vTL);

		// Bottom left corner
		vCellPos = vPos;
		vCellPos.y += vDimensions.y - idColBL->vDims.y;
		vDims = idColBL->vDims;
		pVB->addQuad2D(
			vCellPos,
			vDims,
			colour,
			idColBL->sTexCoords.vBL,
			idColBL->sTexCoords.vBR,
			idColBL->sTexCoords.vTR,
			idColBL->sTexCoords.vTL,
			idNormBL->sTexCoords.vBL,
			idNormBL->sTexCoords.vBR,
			idNormBL->sTexCoords.vTR,
			idNormBL->sTexCoords.vTL);

		// Bottom edge
		vCellPos = vPos;
		vCellPos.x += idColBL->vDims.x;
		vCellPos.y += vDimensions.y - idColB->vDims.y;
		vDims.x = vDimensions.x - idColBL->vDims.x - idColBR->vDims.x;
		vDims.y = idColB->vDims.y;
		pVB->addQuad2D(
			vCellPos,
			vDims,
			colour,
			idColB->sTexCoords.vBL,
			idColB->sTexCoords.vBR,
			idColB->sTexCoords.vTR,
			idColB->sTexCoords.vTL,
			idNormB->sTexCoords.vBL,
			idNormB->sTexCoords.vBR,
			idNormB->sTexCoords.vTR,
			idNormB->sTexCoords.vTL);

		// Bottom right corner
		vCellPos = vPos;
		vCellPos.x += vDimensions.x - idColBR->vDims.x;
		vCellPos.y += vDimensions.y - idColBR->vDims.y;
		vDims = idColBR->vDims;
		pVB->addQuad2D(
			vCellPos,
			vDims,
			colour,
			idColBR->sTexCoords.vBL,
			idColBR->sTexCoords.vBR,
			idColBR->sTexCoords.vTR,
			idColBR->sTexCoords.vTL,
			idNormBR->sTexCoords.vBL,
			idNormBR->sTexCoords.vBR,
			idNormBR->sTexCoords.vTR,
			idNormBR->sTexCoords.vTL);
	}

	void SCUIManager::_helperColourAdjust(CColour& colourToAdjust, const CColour& colourTarget, float fTimeDeltaSecs, float fFadeSpeed)
	{
		if (colourToAdjust.red < colourTarget.red)
		{
			colourToAdjust.red += fTimeDeltaSecs * fFadeSpeed;
			if (colourToAdjust.red > colourTarget.red)
				colourToAdjust.red = colourTarget.red;
		}
		else
		{
			colourToAdjust.red -= fTimeDeltaSecs * fFadeSpeed;
			if (colourToAdjust.red < colourTarget.red)
				colourToAdjust.red = colourTarget.red;
		}

		if (colourToAdjust.green < colourTarget.green)
		{
			colourToAdjust.green += fTimeDeltaSecs * fFadeSpeed;
			if (colourToAdjust.green > colourTarget.green)
				colourToAdjust.green = colourTarget.green;
		}
		else
		{
			colourToAdjust.green -= fTimeDeltaSecs * fFadeSpeed;
			if (colourToAdjust.green < colourTarget.green)
				colourToAdjust.green = colourTarget.green;
		}

		if (colourToAdjust.blue < colourTarget.blue)
		{
			colourToAdjust.blue += fTimeDeltaSecs * fFadeSpeed;
			if (colourToAdjust.blue > colourTarget.blue)
				colourToAdjust.blue = colourTarget.blue;
		}
		else
		{
			colourToAdjust.blue -= fTimeDeltaSecs * fFadeSpeed;
			if (colourToAdjust.blue < colourTarget.blue)
				colourToAdjust.blue = colourTarget.blue;
		}

		if (colourToAdjust.alpha < colourTarget.alpha)
		{
			colourToAdjust.alpha += fTimeDeltaSecs * fFadeSpeed;
			if (colourToAdjust.alpha > colourTarget.alpha)
				colourToAdjust.alpha = colourTarget.alpha;
		}
		else
		{
			colourToAdjust.alpha -= fTimeDeltaSecs * fFadeSpeed;
			if (colourToAdjust.alpha < colourTarget.alpha)
				colourToAdjust.alpha = colourTarget.alpha;
		}
	}

	void SCUIManager::mouseCursorSetThemename(const std::string& strThemeName)
	{
		ThrowIfFalse(themeExists(strThemeName), "SCUIManager::mouseCursorSetThemename(\"" + strThemeName + "\") failed. The named theme does not exist.");
		_mstrMouseCursorThemename = strThemeName;
	}

	std::string SCUIManager::mouseCursorGetThemename(void)
	{
		return _mstrMouseCursorThemename;
	}

	void SCUIManager::mouseCursorSetToBusy(void)
	{
		CUITheme* pTheme = themeGet(_mstrMouseCursorThemename);
		const CUITheme::SSettings* pSettings = pTheme->getSettings();
		CResourceMouseCursor* pMouseCursor = x->pResource->getMouseCursor(pSettings->cursors.busy);
		pMouseCursor->set();
	}

	std::string SCUIManager::mouseCursorGetResourceNameBusy(void)
	{
		CUITheme* pTheme = themeGet(_mstrMouseCursorThemename);
		const CUITheme::SSettings* pSettings = pTheme->getSettings();
		return pSettings->cursors.busy;
	}

	void SCUIManager::mouseCursorSetToNormal(void)
	{
		CUITheme* pTheme = themeGet(_mstrMouseCursorThemename);
		const CUITheme::SSettings* pSettings = pTheme->getSettings();
		CResourceMouseCursor* pMouseCursor = x->pResource->getMouseCursor(pSettings->cursors.normal);
		pMouseCursor->set();
	}

	std::string SCUIManager::mouseCursorGetResourceNameNormal(void)
	{
		CUITheme* pTheme = themeGet(_mstrMouseCursorThemename);
		const CUITheme::SSettings* pSettings = pTheme->getSettings();
		return pSettings->cursors.normal;
	}

	void SCUIManager::mouseCursorSetToWindowResizeLtoR(void)
	{
		CUITheme* pTheme = themeGet(_mstrMouseCursorThemename);
		const CUITheme::SSettings* pSettings = pTheme->getSettings();
		CResourceMouseCursor* pMouseCursor = x->pResource->getMouseCursor(pSettings->cursors.resize_LtoR);
		pMouseCursor->set();
	}

	std::string SCUIManager::mouseCursorGetResourceNameResizeLtoR(void)
	{
		CUITheme* pTheme = themeGet(_mstrMouseCursorThemename);
		const CUITheme::SSettings* pSettings = pTheme->getSettings();
		return pSettings->cursors.resize_LtoR;
	}

	void SCUIManager::mouseCursorSetToWindowResizeTLtoBR(void)
	{
		CUITheme* pTheme = themeGet(_mstrMouseCursorThemename);
		const CUITheme::SSettings* pSettings = pTheme->getSettings();
		CResourceMouseCursor* pMouseCursor = x->pResource->getMouseCursor(pSettings->cursors.resize_TLtoBR);
		pMouseCursor->set();
	}

	std::string SCUIManager::mouseCursorGetResourceNameResizeTLtoBR(void)
	{
		CUITheme* pTheme = themeGet(_mstrMouseCursorThemename);
		const CUITheme::SSettings* pSettings = pTheme->getSettings();
		return pSettings->cursors.resize_TLtoBR;
	}

	void SCUIManager::mouseCursorSetToWindowResizeTRtoBL(void)
	{
		CUITheme* pTheme = themeGet(_mstrMouseCursorThemename);
		const CUITheme::SSettings* pSettings = pTheme->getSettings();
		CResourceMouseCursor* pMouseCursor = x->pResource->getMouseCursor(pSettings->cursors.resize_TRtoBL);
		pMouseCursor->set();
	}

	std::string SCUIManager::mouseCursorGetResourceNameResizeTRtoBL(void)
	{
		CUITheme* pTheme = themeGet(_mstrMouseCursorThemename);
		const CUITheme::SSettings* pSettings = pTheme->getSettings();
		return pSettings->cursors.resize_TRtoBL;
	}

	void SCUIManager::mouseCursorSetToWindowResizeTtoB(void)
	{
		CUITheme* pTheme = themeGet(_mstrMouseCursorThemename);
		const CUITheme::SSettings* pSettings = pTheme->getSettings();
		CResourceMouseCursor* pMouseCursor = x->pResource->getMouseCursor(pSettings->cursors.resize_TtoB);
		pMouseCursor->set();
	}

	std::string SCUIManager::mouseCursorGetResourceNameResizeTtoB(void)
	{
		CUITheme* pTheme = themeGet(_mstrMouseCursorThemename);
		const CUITheme::SSettings* pSettings = pTheme->getSettings();
		return pSettings->cursors.resize_TtoB;
	}

	void SCUIManager::mouseCursorSetToResource(const std::string& strMouseCursorResourceName)
	{
		ThrowIfFalse(x->pResource->getMouseCursorExists(strMouseCursorResourceName), "SCUIManager::setMouseCursorToResource(\"" + strMouseCursorResourceName + "\") failed. The named mouse cursor resource does not exist.");
		CResourceMouseCursor* pMouseCursor = x->pResource->getMouseCursor(strMouseCursorResourceName);
		pMouseCursor->set();
	}

	std::string SCUIManager::mouseCursorGetPreviouslySet(void) const
	{
		return _mstrMouseCursorResNamePreviouslySet;
	}

	void SCUIManager::mouseCursorSetPreviouslySet(const std::string& strResourceName)
	{
		_mstrMouseCursorResNamePreviouslySet = strResourceName;
	}
}