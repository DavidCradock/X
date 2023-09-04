#include "PCH.h"
#include "UIContainer.h"
#include "UIManager.h"
#include "singletons.h"

namespace X
{
	CUIContainer::CUIContainer()
	{
		_mstrThemename = "default";
		_mvPosition.set(0, 0);
		_mvContainersWidgetAreaDimensions.set(100, 100);
		_mbVisible = true;
		_mbContainerIsWindow = false;

		_mpScrollbarH = new CUIScrollbar(this);
		ThrowIfFalse(_mpScrollbarH, "CUIContainer() failed to allocate memory.");
		_mpScrollbarH->setTabPos(0);

		_mpScrollbarV = new CUIScrollbar(this);
		ThrowIfFalse(_mpScrollbarV, "CUIContainer() failed to allocate memory.");
		_mpScrollbarV->setTabPos(0);

		computeScrollbars();
	}

	CUIContainer::~CUIContainer()
	{
		widgetRemoveAll();
		delete _mpScrollbarH;
		delete _mpScrollbarV;
	}

	void CUIContainer::setDimensions(float fX, float fY)
	{
		_mvContainersWidgetAreaDimensions.x = fX;
		_mvContainersWidgetAreaDimensions.y = fY;
		computeScrollbars();
	}

	void CUIContainer::setDimensions(const CVector2f& vDimensions)
	{
		_mvContainersWidgetAreaDimensions = vDimensions;
		computeScrollbars();
	}

	CVector2f CUIContainer::getDimensions(void) const
	{
		return _mvContainersWidgetAreaDimensions;
	}

	void CUIContainer::setPosition(float fX, float fY)
	{
		_mvPosition.x = fX;
		_mvPosition.y = fY;
	}

	void CUIContainer::setPosition(const CVector2f& vPosition)
	{
		_mvPosition = vPosition;
	}

	CVector2f CUIContainer::getPosition(void) const
	{
		return _mvPosition;
	}

	void CUIContainer::setVisible(bool bVisible)
	{
		_mbVisible = bVisible;
	}

	bool CUIContainer::getVisible(void) const
	{
		return _mbVisible;
	}

	std::string CUIContainer::themeNameGet(void) const
	{
		return _mstrThemename;
	}

	CUITheme* CUIContainer::themeGet(void) const
	{
		return x->pUI->themeGet(_mstrThemename);
	}

	const CUITheme::SSettings* CUIContainer::themeGetSettings(void) const
	{
		return x->pUI->themeGet(_mstrThemename)->getSettings();
	}

	void CUIContainer::themeNameSet(const std::string& strThemeToUse)
	{
		if (_mstrThemename != strThemeToUse)
		{
			_mstrThemename = strThemeToUse;
			computeScrollbars();
		}
	}

	std::string CUIContainer::getName(void)
	{
		return _mstrName;
	}

	CVector2f CUIContainer::getWidgetAreaTLCornerPosition(void) const
	{
		CVector2f vPosition = _mvPosition;
		if (_mbContainerIsWindow)
		{
			CUITheme* pTheme = x->pUI->themeGet(_mstrThemename);
			vPosition += pTheme->getTextureAtlas()->getImageDetails(pTheme->getSettings()->images.windowBG.colour.cornerTL).vDims;
		}
		return vPosition;
	}

	CVector2f CUIContainer::getWidgetOffset(void) const
	{
		return _mvWidgetOffset;
	}

	void CUIContainer::computeScrollbars(void)
	{
		// Get required theme settings
		CUITheme* pTheme = themeGet();
		const CUITheme::SSettings* pSettings = pTheme->getSettings();

		// Computes the position of this container's bottom right most widget's corner position.
		// Stores in _mvMaxWidgetCornerPos
		_helperComputeMaxWidgetCornerPos();

		// Compute offset of widgets outside of widget area.
		// This is the number of pixels outside the widget area to the bottom right most widget's BR corner
		CVector2f vOffsetOutsideWidgetArea;
		if (_mvMaxWidgetCornerPos.x > _mvContainersWidgetAreaDimensions.x)
			vOffsetOutsideWidgetArea.x = _mvMaxWidgetCornerPos.x - _mvContainersWidgetAreaDimensions.x;
		if (_mvMaxWidgetCornerPos.y > _mvContainersWidgetAreaDimensions.y)
			vOffsetOutsideWidgetArea.y = _mvMaxWidgetCornerPos.y - _mvContainersWidgetAreaDimensions.y;

		// Here we compute and set tab ratios
		CVector2f vTabRatios(1, 1);
		// If all widgets fit
		if (_mvMaxWidgetCornerPos.x > 0)
		{
			vTabRatios.x = (vOffsetOutsideWidgetArea.x + _mvContainersWidgetAreaDimensions.x) / _mvContainersWidgetAreaDimensions.x;
			vTabRatios.x = 1.0f / vTabRatios.x;
			if (vTabRatios.x < 0.1f)
				vTabRatios.x = 0.1f;
		}
		if (_mvMaxWidgetCornerPos.y > 0)
		{
			vTabRatios.y = (vOffsetOutsideWidgetArea.y + _mvContainersWidgetAreaDimensions.y) / _mvContainersWidgetAreaDimensions.y;
			vTabRatios.y = 1.0f / vTabRatios.y;
			if (vTabRatios.y < 0.1f)
				vTabRatios.y = 0.1f;
		}

		// Set tab ratios for both scrollbars
		_mpScrollbarH->setTabRatio(vTabRatios.x);
		_mpScrollbarV->setTabRatio(vTabRatios.y);

		// Also, hide them if everything fits
		_mpScrollbarH->_mbVisible = !(vTabRatios.x == 1.0f);
		_mpScrollbarV->_mbVisible = !(vTabRatios.y == 1.0f);

		// Horizontal scrollbar
		// Position
		CVector2f vPos;
		vPos.x = 0;
		vPos.y = _mvContainersWidgetAreaDimensions.y;
		_mpScrollbarH->_mvPosition = vPos;
		// Dimensions
		CVector2f vDims;
		vDims.x = _mvContainersWidgetAreaDimensions.x;
		vDims.y = pSettings->floats.windowScrollbarHorizontalHeight;
		_mpScrollbarH->_mvDimensions = vDims;

		// Vertical scrollbar
		// Position
		vPos.x = _mvContainersWidgetAreaDimensions.x;	// Offset by left edge of window border + dims of widget area
		vPos.y = 0;
		_mpScrollbarV->_mvPosition = vPos;
		// Dimensions
		vDims.x = pSettings->floats.windowScrollbarVerticalWidth;
		vDims.y = _mvContainersWidgetAreaDimensions.y;
		_mpScrollbarV->_mvDimensions = vDims;
	}

	void CUIContainer::_computeWidgetOffset(void)
	{
		// Set the amount returned by getWidgetOffset()
		// The amount to offset each widget position due to this container's scrollbars

		// We already have _mvMaxWidgetCornerPos which holds the position of this container's bottom right most widget's corner position.
		// The above was computed by the _helperComputeMaxWidgetCornerPos() method which gets called from computeScrollbars() which gets called whenever 
		// A widget is added, removed, modified position or dims or if the container changes or if the theme changes.

		// Compute offset of widgets outside of widget area.
		// This is the number of pixels outside the widget area to the bottom right most widget's BR corner
		CVector2f vOffsetOutsideWidgetArea;
		if (_mvMaxWidgetCornerPos.x > _mvContainersWidgetAreaDimensions.x)
			vOffsetOutsideWidgetArea.x = _mvMaxWidgetCornerPos.x - _mvContainersWidgetAreaDimensions.x;
		if (_mvMaxWidgetCornerPos.y > _mvContainersWidgetAreaDimensions.y)
			vOffsetOutsideWidgetArea.y = _mvMaxWidgetCornerPos.y - _mvContainersWidgetAreaDimensions.y;

		if (_mpScrollbarH->getVisible())
		{
			_mvWidgetOffset.x = _mpScrollbarH->getTabPos() * -vOffsetOutsideWidgetArea.x;
		}
		if (_mpScrollbarV->getVisible())
		{
			_mvWidgetOffset.y = (1.0f - _mpScrollbarV->getTabPos()) * -vOffsetOutsideWidgetArea.y;
		}
	}

	/************************************************************************************************************************************************************/
	/* All widget code below
	*  When adding a new widget type, we add the usual widgetnameAdd(), widgetnameGet(), widgetnameRemove() and widgetnameRemoveall() methods,
	*  then all CUIContainer methods below those need to be modified to include the new widget.
	*  All the code above does not need to be modified to accomodate a new widget.
	*/
	/************************************************************************************************************************************************************/

	/************************************************************************************************************************************************************/
	/* CUIButton */
	/************************************************************************************************************************************************************/

	CUIButton* CUIContainer::buttonAdd(const std::string& strName, float fPosX, float fPosY, float fWidth, float fHeight)
	{
		// Throw exception if named object already exists
		ThrowIfTrue(_mmapButtons.find(strName) != _mmapButtons.end(), "CUIContainer::buttonAdd(\"" + strName + "\") failed. Named object already exists.");

		// Create new object
		CUIButton* pNewObject = new CUIButton(this);
		ThrowIfFalse(pNewObject, "CUIContainer::buttonAdd() failed to allocate memory for new object.");

		_mmapButtons[strName] = pNewObject;

		// Set settings for new object
		pNewObject->setDimensions(CVector2f(fWidth, fHeight));
		pNewObject->setPosition(CVector2f(fPosX, fPosY));
		pNewObject->setText(strName);

		// Update the scrollbars of the container as the new widget may not fit within the widget area
		computeScrollbars();
		return pNewObject;
	}

	CUIButton* CUIContainer::buttonGet(const std::string& strName)
	{
		auto it = _mmapButtons.find(strName);
		ThrowIfTrue(_mmapButtons.end() == it, "CUIContainer::buttonGet(\"" + strName + "\") failed. Named object doesn't exist.");
		return it->second;
	}

	void CUIContainer::buttonRemove(const std::string& strName)
	{
		auto it = _mmapButtons.find(strName);
		if (_mmapButtons.end() == it)
			return;
		delete it->second;
		_mmapButtons.erase(it);
		computeScrollbars();
	}

	void CUIContainer::buttonRemoveAll(void)
	{
		auto it = _mmapButtons.begin();
		while (it != _mmapButtons.end())
		{
			delete it->second;
			it++;
		}
		_mmapButtons.clear();
		computeScrollbars();
	}

	/************************************************************************************************************************************************************/
	/* CUIScrollbar */
	/************************************************************************************************************************************************************/

	CUIScrollbar* CUIContainer::scrollbarAdd(const std::string& strName, float fPosX, float fPosY, float fWidth, float fHeight, float fTabRatio)
	{
		// Throw exception if named object already exists
		ThrowIfTrue(_mmapScrollbars.find(strName) != _mmapScrollbars.end(), "CUIContainer::scrollbarAdd(\"" + strName + "\") failed. Named object already exists.");

		// Create new object
		CUIScrollbar* pNewObject = new CUIScrollbar(this);
		ThrowIfFalse(pNewObject, "CUIContainer::scrollbarAdd() failed to allocate memory for new object.");

		_mmapScrollbars[strName] = pNewObject;

		// Set settings for new object
		pNewObject->setDimensions(CVector2f(fWidth, fHeight));
		pNewObject->setPosition(CVector2f(fPosX, fPosY));

		// Update the scrollbars of the container as the new widget may not fit within the widget area
		computeScrollbars();
		return pNewObject;
	}

	CUIScrollbar* CUIContainer::scrollbarGet(const std::string& strName)
	{
		auto it = _mmapScrollbars.find(strName);
		ThrowIfTrue(_mmapScrollbars.end() == it, "CUIContainer::scrollbarGet(\"" + strName + "\") failed. Named object doesn't exist.");
		return it->second;
	}

	void CUIContainer::scrollbarRemove(const std::string& strName)
	{
		auto it = _mmapScrollbars.find(strName);
		if (_mmapScrollbars.end() == it)
			return;
		delete it->second;
		_mmapScrollbars.erase(it);
		computeScrollbars();
	}

	void CUIContainer::scrollbarRemoveAll(void)
	{
		auto it = _mmapScrollbars.begin();
		while (it != _mmapScrollbars.end())
		{
			delete it->second;
			it++;
		}
		_mmapScrollbars.clear();
		computeScrollbars();
	}

	/************************************************************************************************************************************************************/
	/* CUIText */
	/************************************************************************************************************************************************************/

	CUIText* CUIContainer::textAdd(const std::string& strName, float fPosX, float fPosY, float fWidth, float fHeight, const std::string& strText)
	{
		// Throw exception if named object already exists
		ThrowIfTrue(_mmapTexts.find(strName) != _mmapTexts.end(), "CUIContainer::textAdd(\"" + strName + "\") failed. Named object already exists.");

		// Create new object
		CUIText* pNewObject = new CUIText(this, strName);
		ThrowIfFalse(pNewObject, "CUIContainer::textAdd() failed to allocate memory for new object.");

		_mmapTexts[strName] = pNewObject;

		// Set settings for new object
		pNewObject->setDimensions(CVector2f(fWidth, fHeight));
		pNewObject->setPosition(CVector2f(fPosX, fPosY));
		pNewObject->setText(strText);

		// Update the scrollbars of the container as the new widget may not fit within the widget area
		computeScrollbars();
		return pNewObject;
	}

	CUIText* CUIContainer::textGet(const std::string& strName)
	{
		auto it = _mmapTexts.find(strName);
		ThrowIfTrue(_mmapTexts.end() == it, "CUIContainer::textGet(\"" + strName + "\") failed. Named object doesn't exist.");
		return it->second;
	}

	void CUIContainer::textRemove(const std::string& strName)
	{
		auto it = _mmapTexts.find(strName);
		if (_mmapTexts.end() == it)
			return;
		delete it->second;
		_mmapTexts.erase(it);
		computeScrollbars();
	}

	void CUIContainer::textRemoveAll(void)
	{
		auto it = _mmapTexts.begin();
		while (it != _mmapTexts.end())
		{
			delete it->second;
			it++;
		}
		_mmapTexts.clear();
		computeScrollbars();
	}

	/************************************************************************************************************************************************************/
	/* All code below here needs to be modified to accomodate a new widget */
	/************************************************************************************************************************************************************/

	void CUIContainer::widgetRemoveAll(void)
	{
		buttonRemoveAll();
		scrollbarRemoveAll();
		textRemoveAll();

		// Recompute this container's scrollbars
		computeScrollbars();
	}

	void CUIContainer::reset(void)
	{
		// For each CUIButton widget
		auto itButton = _mmapButtons.begin();
		while (itButton != _mmapButtons.end())
		{
			itButton->second->reset();
			itButton++;
		}

		// For each CUIScrollbar widget
		auto itScrollbar = _mmapScrollbars.begin();
		while (itScrollbar != _mmapScrollbars.end())
		{
			itScrollbar->second->reset();
			itScrollbar++;
		}

		// For each CUIText object (No need)
	}

	void CUIContainer::_helperComputeMaxWidgetCornerPos(void)
	{
		_mvMaxWidgetCornerPos.setZero();

		// For each CUIButton widget
		auto itButton = _mmapButtons.begin();
		while (itButton != _mmapButtons.end())
		{
			if (itButton->second->_mbVisible)
			{
				CVector2f vBRPos = itButton->second->getPosition() + itButton->second->getDimensions();
				if (_mvMaxWidgetCornerPos.x < vBRPos.x)
					_mvMaxWidgetCornerPos.x = vBRPos.x;
				if (_mvMaxWidgetCornerPos.y < vBRPos.y)
					_mvMaxWidgetCornerPos.y = vBRPos.y;
			}
			itButton++;
		}

		// For each CUIScrollbar widget
		auto itScrollbar = _mmapScrollbars.begin();
		while (itScrollbar != _mmapScrollbars.end())
		{
			if (itScrollbar->second->_mbVisible)
			{
				CVector2f vBRPos = itScrollbar->second->getPosition() + itScrollbar->second->getDimensions();
				if (_mvMaxWidgetCornerPos.x < vBRPos.x)
					_mvMaxWidgetCornerPos.x = vBRPos.x;
				if (_mvMaxWidgetCornerPos.y < vBRPos.y)
					_mvMaxWidgetCornerPos.y = vBRPos.y;
			}
			itScrollbar++;
		}

		// For each CUIText object
		auto itText = _mmapTexts.begin();
		while (itText != _mmapTexts.end())
		{
			if (itText->second->_mbVisible)
			{
				CVector2f vBRPos = itText->second->getPosition() + itText->second->getDimensions();
				if (_mvMaxWidgetCornerPos.x < vBRPos.x)
					_mvMaxWidgetCornerPos.x = vBRPos.x;
				if (_mvMaxWidgetCornerPos.y < vBRPos.y)
					_mvMaxWidgetCornerPos.y = vBRPos.y;
			}
			itText++;
		}


	}

	void CUIContainer::update(float fTimeDeltaSec)
	{
		// For each CUIButton widget
		auto itButton = _mmapButtons.begin();
		while (itButton != _mmapButtons.end())
		{
			itButton->second->update(fTimeDeltaSec);
			itButton++;
		}

		// For each CUIScrollbar widget
		auto itScrollbar = _mmapScrollbars.begin();
		while (itScrollbar != _mmapScrollbars.end())
		{
			itScrollbar->second->update(fTimeDeltaSec);
			itScrollbar++;
		}

		// For each CUIText widget (No need)


		// The container's two scrollbars
		_mpScrollbarH->update(fTimeDeltaSec, false);
		_mpScrollbarV->update(fTimeDeltaSec, false);

		_computeWidgetOffset();
	}

	void CUIContainer::render(void)
	{
		if (!_mbVisible)
			return;

		// Get required resources needed to render things
		CResourceVertexBufferCPT2* pVB = x->pResource->getVertexBufferCPT2(x->pResource->defaultRes.vertexbufferCPT2_default);
		CResourceShader* pShader = x->pResource->getShader(x->pResource->defaultRes.shader_ui);
		CResourceFramebuffer* pUIFB = x->pResource->getFramebuffer(x->pResource->defaultRes.framebuffer_ui);
		CUITheme* pTheme = SCUIManager::getPointer()->themeGet(_mstrThemename);
		const CUITheme::SSettings* pSettings = pTheme->getSettings();
		//CResourceTexture2DAtlas* pAtlas = pTheme->getTextureAtlas();

		pShader->bind();

		// Setup the projection matrix as orthographic
		CMatrix matProjection;
		matProjection.setProjectionOrthographic(0.0f, (float)pUIFB->getWidth(), 0.0f, (float)pUIFB->getHeight(), -1.0f, 1.0f);
		pShader->setMat4("transform", matProjection);

		// Tell OpenGL, for each sampler, to which texture unit it belongs to
		pShader->setInt("texture0", 0);
		pShader->setFloat("fMouseCursorDistance", pSettings->floats.normalMouseCursorDistance);
		pShader->setVec4("v4AmbientLight", pSettings->colours.ambientLight);
		pShader->setVec4("v4MouseLight", pSettings->colours.mouseLight);

		// Set mouse position, inverting Y position
		CVector2f vMousePos = x->pInput->mouse.getCursorPos();
		vMousePos.y = float(x->pWindow->getHeight()) - vMousePos.y;
		pShader->setVec2("v2MousePos", vMousePos);

		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);

		// Get textures and bind them
		CResourceTexture2DAtlas* pAtlas = pTheme->getTextureAtlas();
		pAtlas->bindAtlas(0, 0);

		pVB->removeGeom();

		// For each CUIButton widget
		auto itButton = _mmapButtons.begin();
		while (itButton != _mmapButtons.end())
		{
			itButton->second->render(pVB);
			itButton++;
		}

		// For each CUIScrollbar widget
		auto itScrollbar = _mmapScrollbars.begin();
		while (itScrollbar != _mmapScrollbars.end())
		{
			itScrollbar->second->render(pVB);
			itScrollbar++;
		}

		// Render all the widget backgrounds added above
		glEnable(GL_SCISSOR_TEST);
		glScissor((int)getWidgetAreaTLCornerPosition().x, int(x->pWindow->getHeight() - getWidgetAreaTLCornerPosition().y - _mvContainersWidgetAreaDimensions.y), (int)_mvContainersWidgetAreaDimensions.x, (int)_mvContainersWidgetAreaDimensions.y);
		pVB->update();
		pVB->render();
		pVB->removeGeom();
		glDisable(GL_SCISSOR_TEST);

		// Render the two scrollbars used for scrolling through contents of the container
		pVB->removeGeom();
		_mpScrollbarH->render(pVB, false);
		_mpScrollbarV->render(pVB, false);
		pVB->update();
		pVB->render();
		pVB->removeGeom();
		pAtlas->unbind();
		pShader->unbind();	// Unbind the GUI shader
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);

		// Render font stuff
		// For each CUIButton widget, render non-font stuff
		glEnable(GL_SCISSOR_TEST);
		itButton = _mmapButtons.begin();
		while (itButton != _mmapButtons.end())
		{
			itButton->second->renderFonts();
			itButton++;
		}

		// For each CUIScrollbar widget (Not needed, doesn't have any font stuff)

		// For each CUIText widget
		auto itText = _mmapTexts.begin();
		while (itText != _mmapTexts.end())
		{
			itText->second->render();
			itText++;
		}



		glDisable(GL_SCISSOR_TEST);
	}

	void CUIContainer::renderTooltips(void)
	{

	}

	/************************************************************************************************************************************************************/
	/* The end!!!!!!! :) */
	/************************************************************************************************************************************************************/
}