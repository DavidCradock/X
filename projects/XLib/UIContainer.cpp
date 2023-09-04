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
		_mpScrollbarV = new CUIScrollbar(this);
		ThrowIfFalse(_mpScrollbarV, "CUIContainer() failed to allocate memory.");
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
		_computeScrollbars();
	}

	void CUIContainer::setDimensions(const CVector2f& vDimensions)
	{
		_mvContainersWidgetAreaDimensions = vDimensions;
		_computeScrollbars();
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

	void CUIContainer::update(float fTimeDeltaSec)
	{
		CUITheme* pTheme = SCUIManager::getPointer()->themeGet(_mstrThemename);
//		CResourceTexture2DAtlas* pAtlas = pTheme->getTextureAtlas();

		// For each button object
		auto itButton = _mmapButtons.begin();
		while (itButton != _mmapButtons.end())
		{
			itButton->second->update(fTimeDeltaSec);
			itButton++;
		}

		// For each scrollbar object
		auto itScrollbar = _mmapScrollbars.begin();
		while (itScrollbar != _mmapScrollbars.end())
		{
			itScrollbar->second->update(fTimeDeltaSec);
			itScrollbar++;
		}

		// The container's two scrollbars
		_mpScrollbarH->update(fTimeDeltaSec);
		_mpScrollbarV->update(fTimeDeltaSec);

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

		// For each button object
		auto itButton = _mmapButtons.begin();
		while (itButton != _mmapButtons.end())
		{
			itButton->second->render(pVB);
			itButton++;
		}

		// For each scrollbar object
		auto itScrollbar = _mmapScrollbars.begin();
		while (itScrollbar != _mmapScrollbars.end())
		{
			itScrollbar->second->render(pVB);
			itScrollbar++;
		}

		// Render the two scrollbars used for scrolling through contents of the container
		_mpScrollbarH->render(pVB);
		_mpScrollbarV->render(pVB);

		pVB->update();
		pVB->render();

		pAtlas->unbind();
		pShader->unbind();	// Unbind the GUI shader
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);

		// Render font stuff
		// For each button object, render non-font stuff
		itButton = _mmapButtons.begin();
		while (itButton != _mmapButtons.end())
		{
			itButton->second->renderFonts();
			itButton++;
		}
	}

	void CUIContainer::renderTooltips(void)
	{

	}

	void CUIContainer::reset(void)
	{
		// For each button object
		auto itButton = _mmapButtons.begin();
		while (itButton != _mmapButtons.end())
		{
			itButton->second->reset();
			itButton++;
		}

		// For each scrollbar object
		auto itScrollbar = _mmapScrollbars.begin();
		while (itScrollbar != _mmapScrollbars.end())
		{
			itScrollbar->second->reset();
			itScrollbar++;
		}
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
			_computeScrollbars();
		}
	}

	void CUIContainer::widgetRemoveAll(void)
	{
		scrollbarRemoveAll();
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

	/************************************************************************************************************************************************************/
	/* Buttons */
	/************************************************************************************************************************************************************/

	// Add button to this container and return a pointer to it
	// If the name already exists, an exception occurs
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
		_computeScrollbars();

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
	}

	/************************************************************************************************************************************************************/
	/* Scrollbars */
	/************************************************************************************************************************************************************/

	// Add scrollbar to this container and return a pointer to it
	// If the name already exists, an exception occurs
	// If the width is greater than height, it is set as a horizontal scrollbar, else vertical.
	// fTabRatio is a value which is multiplied by the width/height(Depending on orientation) of the scrollbar's dims, to obtain tab dimensions
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
		_computeScrollbars();
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
	}

	void CUIContainer::_computeScrollbars(void)
	{
		// Get required theme settings
		CUITheme* pTheme = themeGet();
		const CUITheme::SSettings* pSettings = pTheme->getSettings();
		
		// Computes the positions of this container's topleft most and bottomright most widget's corner positions.
		// Stores in _mvMinWidgetCorner and _mvMaxWidgetCorner
		_helperGetMinMaxWidgetPos();

		// Compute offset of widgets outside of widget area
		CVector2f vOffsetOutsideWidgetAreaL;
		CVector2f vOffsetOutsideWidgetAreaR;
		if (_mvMinWidgetCorner.x < 0)
			vOffsetOutsideWidgetAreaL.x = -_mvMinWidgetCorner.x;
		if (_mvMinWidgetCorner.y < 0)
			vOffsetOutsideWidgetAreaL.y = -_mvMinWidgetCorner.y;
		if (_mvMaxWidgetCorner.x > _mvContainersWidgetAreaDimensions.x)
			vOffsetOutsideWidgetAreaR.x = _mvMaxWidgetCorner.x - _mvContainersWidgetAreaDimensions.x;
		if (_mvMaxWidgetCorner.y > _mvMaxWidgetCorner.y - _mvContainersWidgetAreaDimensions.y)
			vOffsetOutsideWidgetAreaR.y = _mvMaxWidgetCorner.y - _mvContainersWidgetAreaDimensions.y;

		// Compute total offset of widgets
		CVector2f vTotalOffset;
		vTotalOffset.x = vOffsetOutsideWidgetAreaL.x + vOffsetOutsideWidgetAreaR.x;
		vTotalOffset.y = vOffsetOutsideWidgetAreaL.y + vOffsetOutsideWidgetAreaR.y;

		// Here we compute and set tab ratios
		CVector2f vTabRatios(1, 1);
		// If all widgets fit
		if (vTotalOffset.x > 0)
		{
			vTabRatios.x = 1.0f - (vTotalOffset.x / _mvContainersWidgetAreaDimensions.x);
			if (vTabRatios.x < 0.1f)
				vTabRatios.x = 0.1f;
		}
		if (vTotalOffset.y > 0)
		{
			vTabRatios.y = 1.0f - (vTotalOffset.y / _mvContainersWidgetAreaDimensions.y);
			if (vTabRatios.y < 0.1f)
				vTabRatios.y = 0.1f;
		}

		// Set tab ratios for both scrollbars
		_mpScrollbarH->setTabRatio(vTabRatios.x);
		_mpScrollbarV->setTabRatio(vTabRatios.y);

		// Also, hide them if everything fits
		_mpScrollbarH->setVisible(!(vTabRatios.x == 1.0f));
		_mpScrollbarV->setVisible(!(vTabRatios.y == 1.0f));

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
		if (_mpScrollbarH->getVisible())
		{
			_mvWidgetOffset.x = 0;
			_mvWidgetOffset.x += 0;
		}
		if (_mpScrollbarV->getVisible())
		{

		}
	}

	void CUIContainer::_helperGetMinMaxWidgetPos(void)
	{
		_mvMinWidgetCorner.set(9999999.0f, 9999999.0f);
		_mvMaxWidgetCorner.set(-9999999.0f, -9999999.0f);

		bool bWidgetExists = false;

		// For each button object
		auto itButton = _mmapButtons.begin();
		while (itButton != _mmapButtons.end())
		{
			CVector2f vTLPos = itButton->second->getPosition();
			CVector2f vBRPos = vTLPos + itButton->second->getDimensions();
			if (_mvMinWidgetCorner.x > vTLPos.x)
				_mvMinWidgetCorner.x = vTLPos.x;
			if (_mvMinWidgetCorner.y > vTLPos.y)
				_mvMinWidgetCorner.y = vTLPos.y;
			if (_mvMaxWidgetCorner.x < vBRPos.x)
				_mvMaxWidgetCorner.x = vBRPos.x;
			if (_mvMaxWidgetCorner.y < vBRPos.y)
				_mvMaxWidgetCorner.y = vBRPos.y;
			bWidgetExists = true;
			itButton++;
		}

		// For each scrollbar object
		auto itScrollbar = _mmapScrollbars.begin();
		while (itScrollbar != _mmapScrollbars.end())
		{
			CVector2f vTLPos = itScrollbar->second->getPosition();
			CVector2f vBRPos = vTLPos + itScrollbar->second->getDimensions();
			if (_mvMinWidgetCorner.x > vTLPos.x)
				_mvMinWidgetCorner.x = vTLPos.x;
			if (_mvMinWidgetCorner.y > vTLPos.y)
				_mvMinWidgetCorner.y = vTLPos.y;
			if (_mvMaxWidgetCorner.x < vBRPos.x)
				_mvMaxWidgetCorner.x = vBRPos.x;
			if (_mvMaxWidgetCorner.y < vBRPos.y)
				_mvMaxWidgetCorner.y = vBRPos.y;
			bWidgetExists = true;
			itScrollbar++;
		}

		// If no widgets exist, set the given min and max to zero
		if (!bWidgetExists)
		{
			_mvMinWidgetCorner.setZero();
			_mvMaxWidgetCorner.setZero();
		}
	}
}