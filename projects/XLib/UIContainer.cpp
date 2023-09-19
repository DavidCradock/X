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

	void CUIContainer::setDimensions(int iX, int iY)
	{
		setDimensions(float(iX), float(iY));
	}

	void CUIContainer::setDimensions(const CVector2f& vDimensions)
	{
		setDimensions(vDimensions.x, vDimensions.y);
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

	void CUIContainer::setPosition(int iX, int iY)
	{
		setPosition(float(iX), float(iY));
	}

	void CUIContainer::setPosition(const CVector2f& vPosition)
	{
		setPosition(vPosition.x, vPosition.y);
	}

	CVector2f CUIContainer::getPosition(void) const
	{
		return _mvPosition;
	}

	void CUIContainer::setVisible(bool bVisible)
	{
		_mbVisible = bVisible;
		if (!_mbVisible)
			reset();
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

	CUITheme::SSettings* CUIContainer::themeGetSettings(void) const
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
			CResourceTexture2DAtlas* pAtlas = pTheme->getTextureAtlas();
			vPosition += pAtlas->getImageDetailsPointer(pTheme->getSettings()->images.windowBG.colour.cornerTL)->vDims;
		}
		return vPosition;
	}

	CVector2f CUIContainer::getWidgetAreaDimensions(void) const
	{
		return _mvContainersWidgetAreaDimensions;
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
	*  the map object in the header and then all CUIContainer methods below those need to be modified to include the new widget.
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

	CUIButton* CUIContainer::buttonAdd(const std::string& strName, int iPosX, int iPosY, int iWidth, int iHeight)
	{
		return buttonAdd(strName, float(iPosX), float(iPosY), float(iWidth), float(iHeight));
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
	/* CUIButtonImage */
	/************************************************************************************************************************************************************/

	CUIButtonImage* CUIContainer::buttonImageAdd(const std::string& strName, float fPosX, float fPosY, float fWidth, float fHeight, const std::string& strTextureFromFileResourceNameUp, const std::string& strTextureFromFileResourceNameOver, const std::string& strTextureFromFileResourceNameDown)
	{
		// Throw exception if named object already exists
		ThrowIfTrue(_mmapButtonImages.find(strName) != _mmapButtonImages.end(), "CUIContainer::buttonImageAdd(\"" + strName + "\") failed. Named object already exists.");

		// Make sure the texture resources exist
		ThrowIfFalse(x->pResource->getTexture2DFromFileExists(strTextureFromFileResourceNameUp), "CUIContainer::buttonImageAdd(\"" + strTextureFromFileResourceNameUp + "\") failed. The named resource does not exist.");
		ThrowIfFalse(x->pResource->getTexture2DFromFileExists(strTextureFromFileResourceNameOver), "CUIContainer::buttonImageAdd(\"" + strTextureFromFileResourceNameOver + "\") failed. The named resource does not exist.");
		ThrowIfFalse(x->pResource->getTexture2DFromFileExists(strTextureFromFileResourceNameDown), "CUIContainer::buttonImageAdd(\"" + strTextureFromFileResourceNameDown + "\") failed. The named resource does not exist.");

		// Create new object
		CUIButtonImage* pNewObject = new CUIButtonImage(this, strTextureFromFileResourceNameUp, strTextureFromFileResourceNameOver, strTextureFromFileResourceNameDown);
		ThrowIfFalse(pNewObject, "CUIContainer::buttonImageAdd() failed to allocate memory for new object.");

		_mmapButtonImages[strName] = pNewObject;

		// Set settings for new object
		pNewObject->setDimensions(CVector2f(fWidth, fHeight));
		pNewObject->setPosition(CVector2f(fPosX, fPosY));
		pNewObject->setText(strName);

		// Update the scrollbars of the container as the new widget may not fit within the widget area
		computeScrollbars();
		return pNewObject;
	}

	CUIButtonImage* CUIContainer::buttonImageAdd(const std::string& strName, int iPosX, int iPosY, int iWidth, int iHeight, const std::string& strTextureFromFileResourceNameUp, const std::string& strTextureFromFileResourceNameOver, const std::string& strTextureFromFileResourceNameDown)
	{
		return buttonImageAdd(strName, float(iPosX), float(iPosY), float(iWidth), float(iHeight), strTextureFromFileResourceNameUp, strTextureFromFileResourceNameOver, strTextureFromFileResourceNameDown);
	}

	CUIButtonImage* CUIContainer::buttonImageGet(const std::string& strName)
	{
		auto it = _mmapButtonImages.find(strName);
		ThrowIfTrue(_mmapButtonImages.end() == it, "CUIContainer::buttonImageGet(\"" + strName + "\") failed. Named object doesn't exist.");
		return it->second;
	}

	void CUIContainer::buttonImageRemove(const std::string& strName)
	{
		auto it = _mmapButtonImages.find(strName);
		if (_mmapButtonImages.end() == it)
			return;
		delete it->second;
		_mmapButtonImages.erase(it);
		computeScrollbars();
	}

	void CUIContainer::buttonImageRemoveAll(void)
	{
		auto it = _mmapButtonImages.begin();
		while (it != _mmapButtonImages.end())
		{
			delete it->second;
			it++;
		}
		_mmapButtonImages.clear();
		computeScrollbars();
	}

	/************************************************************************************************************************************************************/
	/* CUICheckbox */
	/************************************************************************************************************************************************************/

	CUICheckbox* CUIContainer::checkboxAdd(const std::string& strName, float fPosX, float fPosY, float fWidth, float fHeight)
	{
		// Throw exception if named object already exists
		ThrowIfTrue(_mmapCheckboxes.find(strName) != _mmapCheckboxes.end(), "CUIContainer::checkboxAdd(\"" + strName + "\") failed. Named object already exists.");

		// Create new object
		CUICheckbox* pNewObject = new CUICheckbox(this);
		ThrowIfFalse(pNewObject, "CUIContainer::checkboxAdd() failed to allocate memory for new object.");

		_mmapCheckboxes[strName] = pNewObject;

		// Set settings for new object
		pNewObject->setDimensions(CVector2f(fWidth, fHeight));
		pNewObject->setPosition(CVector2f(fPosX, fPosY));

		// Update the scrollbars of the container as the new widget may not fit within the widget area
		computeScrollbars();
		return pNewObject;
	}

	CUICheckbox* CUIContainer::checkboxAdd(const std::string& strName, int iPosX, int iPosY, int iWidth, int iHeight)
	{
		return checkboxAdd(strName, float(iPosX), float(iPosY), float(iWidth), float(iHeight));
	}

	CUICheckbox* CUIContainer::checkboxGet(const std::string& strName)
	{
		auto it = _mmapCheckboxes.find(strName);
		ThrowIfTrue(_mmapCheckboxes.end() == it, "CUIContainer::checkboxGet(\"" + strName + "\") failed. Named object doesn't exist.");
		return it->second;
	}

	void CUIContainer::checkboxRemove(const std::string& strName)
	{
		auto it = _mmapCheckboxes.find(strName);
		if (_mmapCheckboxes.end() == it)
			return;
		delete it->second;
		_mmapCheckboxes.erase(it);
		computeScrollbars();
	}

	void CUIContainer::checkboxRemoveAll(void)
	{
		auto it = _mmapCheckboxes.begin();
		while (it != _mmapCheckboxes.end())
		{
			delete it->second;
			it++;
		}
		_mmapCheckboxes.clear();
		computeScrollbars();
	}

	/************************************************************************************************************************************************************/
	/* CUIColourSelector */
	/************************************************************************************************************************************************************/

	CUIColourSelector* CUIContainer::colourSelectorAdd(const std::string& strName, float fPosX, float fPosY)
	{
		// Throw exception if named object already exists
		ThrowIfTrue(_mmapColourSelectors.find(strName) != _mmapColourSelectors.end(), "CUIContainer::colourSelectorAdd(\"" + strName + "\") failed. Named object already exists.");

		// Create new object
		CUIColourSelector* pNewObject = new CUIColourSelector(this, strName, fPosX, fPosY);
		ThrowIfFalse(pNewObject, "CUIContainer::colourSelectorAdd() failed to allocate memory for new object.");

		_mmapColourSelectors[strName] = pNewObject;

		// Update the scrollbars of the container as the new widget may not fit within the widget area
		computeScrollbars();
		return pNewObject;
	}

	CUIColourSelector* CUIContainer::colourSelectorAdd(const std::string& strName, int iPosX, int iPosY)
	{
		return colourSelectorAdd(strName, float(iPosX), float(iPosY));
	}

	CUIColourSelector* CUIContainer::colourSelectorGet(const std::string& strName)
	{
		auto it = _mmapColourSelectors.find(strName);
		ThrowIfTrue(_mmapColourSelectors.end() == it, "CUIContainer::colourSelectorGet(\"" + strName + "\") failed. Named object doesn't exist.");
		return it->second;
	}

	void CUIContainer::colourSelectorRemove(const std::string& strName)
	{
		auto it = _mmapColourSelectors.find(strName);
		if (_mmapColourSelectors.end() == it)
			return;
		delete it->second;
		_mmapColourSelectors.erase(it);
		computeScrollbars();
	}

	void CUIContainer::colourSelectorRemoveAll(void)
	{
		auto it = _mmapColourSelectors.begin();
		while (it != _mmapColourSelectors.end())
		{
			delete it->second;
			it++;
		}
		_mmapColourSelectors.clear();
		computeScrollbars();
	}

	/************************************************************************************************************************************************************/
	/* CUIImage */
	/************************************************************************************************************************************************************/

	CUIImage* CUIContainer::imageAdd(const std::string& strName, float fPosX, float fPosY, float fWidth, float fHeight)
	{
		// Throw exception if named object already exists
		ThrowIfTrue(_mmapImages.find(strName) != _mmapImages.end(), "CUIContainer::imageAdd(\"" + strName + "\") failed. Named object already exists.");

		// Create new object
		CUIImage* pNewObject = new CUIImage(this);
		ThrowIfFalse(pNewObject, "CUIContainer::imageAdd() failed to allocate memory for new object.");

		_mmapImages[strName] = pNewObject;

		// Set settings for new object
		pNewObject->setDimensions(CVector2f(fWidth, fHeight));
		pNewObject->setPosition(CVector2f(fPosX, fPosY));

		// Update the scrollbars of the container as the new widget may not fit within the widget area
		computeScrollbars();
		return pNewObject;
	}

	CUIImage* CUIContainer::imageAdd(const std::string& strName, int iPosX, int iPosY, int iWidth, int iHeight)
	{
		return imageAdd(strName, float(iPosX), float(iPosY), float(iWidth), float(iHeight));
	}

	CUIImage* CUIContainer::imageGet(const std::string& strName)
	{
		auto it = _mmapImages.find(strName);
		ThrowIfTrue(_mmapImages.end() == it, "CUIContainer::imageGet(\"" + strName + "\") failed. Named object doesn't exist.");
		return it->second;
	}

	void CUIContainer::imageRemove(const std::string& strName)
	{
		auto it = _mmapImages.find(strName);
		if (_mmapImages.end() == it)
			return;
		delete it->second;
		_mmapImages.erase(it);
		computeScrollbars();
	}

	void CUIContainer::imageRemoveAll(void)
	{
		auto it = _mmapImages.begin();
		while (it != _mmapImages.end())
		{
			delete it->second;
			it++;
		}
		_mmapImages.clear();
		computeScrollbars();
	}

	/************************************************************************************************************************************************************/
	/* CUILineGraph */
	/************************************************************************************************************************************************************/

	CUILineGraph* CUIContainer::lineGraphAdd(const std::string& strName, float fPosX, float fPosY, float fWidth, float fHeight)
	{
		// Throw exception if named object already exists
		ThrowIfTrue(_mmapLineGraphs.find(strName) != _mmapLineGraphs.end(), "CUIContainer::lineGraphAdd(\"" + strName + "\") failed. Named object already exists.");

		// Create new object
		CUILineGraph* pNewObject = new CUILineGraph(this);
		ThrowIfFalse(pNewObject, "CUIContainer::lineGraphAdd() failed to allocate memory for new object.");

		_mmapLineGraphs[strName] = pNewObject;

		// Set settings for new object
		pNewObject->setDimensions(CVector2f(fWidth, fHeight));
		pNewObject->setPosition(CVector2f(fPosX, fPosY));

		// Update the scrollbars of the container as the new widget may not fit within the widget area
		computeScrollbars();
		return pNewObject;
	}

	CUILineGraph* CUIContainer::lineGraphAdd(const std::string& strName, int iPosX, int iPosY, int iWidth, int iHeight)
	{
		return lineGraphAdd(strName, float(iPosX), float(iPosY), float(iWidth), float(iHeight));
	}

	CUILineGraph* CUIContainer::lineGraphGet(const std::string& strName)
	{
		auto it = _mmapLineGraphs.find(strName);
		ThrowIfTrue(_mmapLineGraphs.end() == it, "CUIContainer::lineGraphGet(\"" + strName + "\") failed. Named object doesn't exist.");
		return it->second;
	}

	void CUIContainer::lineGraphRemove(const std::string& strName)
	{
		auto it = _mmapLineGraphs.find(strName);
		if (_mmapLineGraphs.end() == it)
			return;
		delete it->second;
		_mmapLineGraphs.erase(it);
		computeScrollbars();
	}

	void CUIContainer::lineGraphRemoveAll(void)
	{
		auto it = _mmapLineGraphs.begin();
		while (it != _mmapLineGraphs.end())
		{
			delete it->second;
			it++;
		}
		_mmapLineGraphs.clear();
		computeScrollbars();
	}

	/************************************************************************************************************************************************************/
	/* CUIProgressbar */
	/************************************************************************************************************************************************************/

	CUIProgressbar* CUIContainer::progressbarAdd(const std::string& strName, float fPosX, float fPosY, float fWidth, float fHeight)
	{
		// Throw exception if named object already exists
		ThrowIfTrue(_mmapProgressbars.find(strName) != _mmapProgressbars.end(), "CUIContainer::progressbarAdd(\"" + strName + "\") failed. Named object already exists.");

		// Create new object
		CUIProgressbar* pNewObject = new CUIProgressbar(this);
		ThrowIfFalse(pNewObject, "CUIContainer::progressbarAdd() failed to allocate memory for new object.");

		_mmapProgressbars[strName] = pNewObject;

		// Set settings for new object
		pNewObject->setDimensions(CVector2f(fWidth, fHeight));
		pNewObject->setPosition(CVector2f(fPosX, fPosY));

		// Update the scrollbars of the container as the new widget may not fit within the widget area
		computeScrollbars();
		return pNewObject;
	}

	CUIProgressbar* CUIContainer::progressbarAdd(const std::string& strName, int iPosX, int iPosY, int iWidth, int iHeight)
	{
		return progressbarAdd(strName, float(iPosX), float(iPosY), float(iWidth), float(iHeight));
	}

	CUIProgressbar* CUIContainer::progressbarGet(const std::string& strName)
	{
		auto it = _mmapProgressbars.find(strName);
		ThrowIfTrue(_mmapProgressbars.end() == it, "CUIContainer::progressbarGet(\"" + strName + "\") failed. Named object doesn't exist.");
		return it->second;
	}

	void CUIContainer::progressbarRemove(const std::string& strName)
	{
		auto it = _mmapProgressbars.find(strName);
		if (_mmapProgressbars.end() == it)
			return;
		delete it->second;
		_mmapProgressbars.erase(it);
		computeScrollbars();
	}

	void CUIContainer::progressbarRemoveAll(void)
	{
		auto it = _mmapProgressbars.begin();
		while (it != _mmapProgressbars.end())
		{
			delete it->second;
			it++;
		}
		_mmapProgressbars.clear();
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

	CUIScrollbar* CUIContainer::scrollbarAdd(const std::string& strName, int iPosX, int iPosY, int iWidth, int iHeight, float fTabRatio)
	{
		return scrollbarAdd(strName, float(iPosX), float(iPosY), float(iWidth), float(iHeight), fTabRatio);
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

	CUIText* CUIContainer::textAdd(const std::string& strName, int iPosX, int iPosY, int iWidth, int iHeight, const std::string& strText)
	{
		return textAdd(strName, float(iPosX), float(iPosY), float(iWidth), float(iHeight), strText);
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
	/* CUITextEdit */
	/************************************************************************************************************************************************************/

	CUITextEdit* CUIContainer::textEditAdd(const std::string& strName, float fPosX, float fPosY, float fWidth, float fHeight, const std::string& strText)
	{
		// Throw exception if named object already exists
		ThrowIfTrue(_mmapTextEdits.find(strName) != _mmapTextEdits.end(), "CUIContainer::textEditAdd(\"" + strName + "\") failed. Named object already exists.");

		// Create new object
		CUITextEdit* pNewObject = new CUITextEdit(this, strName);
		ThrowIfFalse(pNewObject, "CUIContainer::textEditAdd() failed to allocate memory for new object.");

		_mmapTextEdits[strName] = pNewObject;

		// Set settings for new object
		pNewObject->setDimensions(CVector2f(fWidth, fHeight));
		pNewObject->setPosition(CVector2f(fPosX, fPosY));
		pNewObject->setText(strText);

		// Update the scrollbars of the container as the new widget may not fit within the widget area
		computeScrollbars();
		return pNewObject;
	}

	CUITextEdit* CUIContainer::textEditAdd(const std::string& strName, int iPosX, int iPosY, int iWidth, int iHeight, const std::string& strText)
	{
		return textEditAdd(strName, float(iPosX), float(iPosY), float(iWidth), float(iHeight), strText);
	}

	CUITextEdit* CUIContainer::textEditGet(const std::string& strName)
	{
		auto it = _mmapTextEdits.find(strName);
		ThrowIfTrue(_mmapTextEdits.end() == it, "CUIContainer::textEditGet(\"" + strName + "\") failed. Named object doesn't exist.");
		return it->second;
	}

	void CUIContainer::textEditRemove(const std::string& strName)
	{
		auto it = _mmapTextEdits.find(strName);
		if (_mmapTextEdits.end() == it)
			return;
		delete it->second;
		_mmapTextEdits.erase(it);
		computeScrollbars();
	}

	void CUIContainer::textEditRemoveAll(void)
	{
		auto it = _mmapTextEdits.begin();
		while (it != _mmapTextEdits.end())
		{
			delete it->second;
			it++;
		}
		_mmapTextEdits.clear();
		computeScrollbars();
	}

	/************************************************************************************************************************************************************/
	/* All code below here needs to be modified to accommodate a new widget */
	/************************************************************************************************************************************************************/

	void CUIContainer::widgetRemoveAll(void)
	{
		buttonRemoveAll();
		buttonImageRemoveAll();
		checkboxRemoveAll();
		colourSelectorRemoveAll();
		imageRemoveAll();
		lineGraphRemoveAll();
		progressbarRemoveAll();
		scrollbarRemoveAll();
		textRemoveAll();
		textEditRemoveAll();

		// Recompute this container's scrollbars
		computeScrollbars();
	}

	void CUIContainer::reset(void)
	{
		{
			auto it = _mmapButtons.begin();
			while (it != _mmapButtons.end())
			{
				it->second->reset();
				it++;
			}
		}

		{
			auto it = _mmapButtonImages.begin();
			while (it != _mmapButtonImages.end())
			{
				it->second->reset();
				it++;
			}
		}

		{
			auto it = _mmapCheckboxes.begin();
			while (it != _mmapCheckboxes.end())
			{
				it->second->reset();
				it++;
			}
		}

		// For each CUIColourSelector (No need)

		// For each CUIImage widget (No need)

		// For each CUILineGraph widget (No need)

		// For eacg CUIProgressbar widger (No need)

		{
			auto it = _mmapScrollbars.begin();
			while (it != _mmapScrollbars.end())
			{
				it->second->reset();
				it++;
			}
		}

		// For each CUIText widget (No need)

		{
			auto it = _mmapTextEdits.begin();
			while (it != _mmapTextEdits.end())
			{
				it->second->reset();
				it++;
			}
		}

	}

	void CUIContainer::_helperComputeMaxWidgetCornerPos(void)
	{
		_mvMaxWidgetCornerPos.setZero();

		{
			auto it = _mmapButtons.begin();
			while (it != _mmapButtons.end())
			{
				if (it->second->_mbVisible)
				{
					CVector2f vBRPos = it->second->getPosition() + it->second->getDimensions();
					if (_mvMaxWidgetCornerPos.x < vBRPos.x)
						_mvMaxWidgetCornerPos.x = vBRPos.x;
					if (_mvMaxWidgetCornerPos.y < vBRPos.y)
						_mvMaxWidgetCornerPos.y = vBRPos.y;
				}
				it++;
			}
		}

		{
			auto it = _mmapButtonImages.begin();
			while (it != _mmapButtonImages.end())
			{
				if (it->second->_mbVisible)
				{
					CVector2f vBRPos = it->second->getPosition() + it->second->getDimensions();
					if (_mvMaxWidgetCornerPos.x < vBRPos.x)
						_mvMaxWidgetCornerPos.x = vBRPos.x;
					if (_mvMaxWidgetCornerPos.y < vBRPos.y)
						_mvMaxWidgetCornerPos.y = vBRPos.y;
				}
				it++;
			}
		}

		{
			auto it = _mmapCheckboxes.begin();
			while (it != _mmapCheckboxes.end())
			{
				if (it->second->_mbVisible)
				{
					CVector2f vBRPos = it->second->getPosition() + it->second->getDimensions();
					if (_mvMaxWidgetCornerPos.x < vBRPos.x)
						_mvMaxWidgetCornerPos.x = vBRPos.x;
					if (_mvMaxWidgetCornerPos.y < vBRPos.y)
						_mvMaxWidgetCornerPos.y = vBRPos.y;
				}
				it++;
			}
		}

		{
			auto it = _mmapColourSelectors.begin();
			while (it != _mmapColourSelectors.end())
			{
				if (it->second->_mbVisible)
				{
					CVector2f vBRPos = it->second->getPosition() + it->second->getDimensions();
					if (_mvMaxWidgetCornerPos.x < vBRPos.x)
						_mvMaxWidgetCornerPos.x = vBRPos.x;
					if (_mvMaxWidgetCornerPos.y < vBRPos.y)
						_mvMaxWidgetCornerPos.y = vBRPos.y;
				}
				it++;
			}
		}

		{
			auto it = _mmapImages.begin();
			while (it != _mmapImages.end())
			{
				if (it->second->_mbVisible)
				{
					CVector2f vBRPos = it->second->getPosition() + it->second->getDimensions();
					if (_mvMaxWidgetCornerPos.x < vBRPos.x)
						_mvMaxWidgetCornerPos.x = vBRPos.x;
					if (_mvMaxWidgetCornerPos.y < vBRPos.y)
						_mvMaxWidgetCornerPos.y = vBRPos.y;
				}
				it++;
			}
		}

		{
			auto it = _mmapLineGraphs.begin();
			while (it != _mmapLineGraphs.end())
			{
				if (it->second->_mbVisible)
				{
					CVector2f vBRPos = it->second->getPosition() + it->second->getDimensions();
					if (_mvMaxWidgetCornerPos.x < vBRPos.x)
						_mvMaxWidgetCornerPos.x = vBRPos.x;
					if (_mvMaxWidgetCornerPos.y < vBRPos.y)
						_mvMaxWidgetCornerPos.y = vBRPos.y;
				}
				it++;
			}
		}

		{
			auto it = _mmapProgressbars.begin();
			while (it != _mmapProgressbars.end())
			{
				if (it->second->_mbVisible)
				{
					CVector2f vBRPos = it->second->getPosition() + it->second->getDimensions();
					if (_mvMaxWidgetCornerPos.x < vBRPos.x)
						_mvMaxWidgetCornerPos.x = vBRPos.x;
					if (_mvMaxWidgetCornerPos.y < vBRPos.y)
						_mvMaxWidgetCornerPos.y = vBRPos.y;
				}
				it++;
			}
		}

		{
			auto it = _mmapScrollbars.begin();
			while (it != _mmapScrollbars.end())
			{
				if (it->second->_mbVisible)
				{
					CVector2f vBRPos = it->second->getPosition() + it->second->getDimensions();
					if (_mvMaxWidgetCornerPos.x < vBRPos.x)
						_mvMaxWidgetCornerPos.x = vBRPos.x;
					if (_mvMaxWidgetCornerPos.y < vBRPos.y)
						_mvMaxWidgetCornerPos.y = vBRPos.y;
				}
				it++;
			}
		}

		{
			auto it = _mmapTexts.begin();
			while (it != _mmapTexts.end())
			{
				if (it->second->getVisible())
				{
					CVector2f vBRPos = it->second->getPosition() + it->second->getDimensions();
					if (_mvMaxWidgetCornerPos.x < vBRPos.x)
						_mvMaxWidgetCornerPos.x = vBRPos.x;
					if (_mvMaxWidgetCornerPos.y < vBRPos.y)
						_mvMaxWidgetCornerPos.y = vBRPos.y;
				}
				it++;
			}
		}

		{
			auto it = _mmapTextEdits.begin();
			while (it != _mmapTextEdits.end())
			{
				if (it->second->getVisible())
				{
					CVector2f vBRPos = it->second->getPosition() + it->second->getDimensions();
					if (_mvMaxWidgetCornerPos.x < vBRPos.x)
						_mvMaxWidgetCornerPos.x = vBRPos.x;
					if (_mvMaxWidgetCornerPos.y < vBRPos.y)
						_mvMaxWidgetCornerPos.y = vBRPos.y;
				}
				it++;
			}
		}
	}

	void CUIContainer::update(float fTimeDeltaSec)
	{
//		if (!_mbVisible)
//			return;

		{
			auto it = _mmapButtons.begin();
			while (it != _mmapButtons.end())
			{
				it->second->update(fTimeDeltaSec);
				it++;
			}
		}

		{
			auto it = _mmapButtonImages.begin();
			while (it != _mmapButtonImages.end())
			{
				it->second->update(fTimeDeltaSec);
				it++;
			}
		}

		{
			auto it = _mmapCheckboxes.begin();
			while (it != _mmapCheckboxes.end())
			{
				it->second->update(fTimeDeltaSec);
				it++;
			}
		}

		{
			auto it = _mmapColourSelectors.begin();
			while (it != _mmapColourSelectors.end())
			{
				it->second->update(fTimeDeltaSec);
				it++;
			}
		}
		
		// For each CUIImage widget (No need)

		{
			auto it = _mmapLineGraphs.begin();
			while (it != _mmapLineGraphs.end())
			{
				it->second->update(fTimeDeltaSec);
				it++;
			}
		}

		// For each CUIProgressbar widget (No need)

		{
			auto it = _mmapScrollbars.begin();
			while (it != _mmapScrollbars.end())
			{
				it->second->update(fTimeDeltaSec);
				it++;
			}
		}

		// For each CUIText widget (No need)

		{
			auto it = _mmapTextEdits.begin();
			while (it != _mmapTextEdits.end())
			{
				it->second->update(fTimeDeltaSec);
				it++;
			}
		}

		// The container's two scrollbars
		_mpScrollbarH->update(fTimeDeltaSec, false);
		_mpScrollbarV->update(fTimeDeltaSec, false);

		_computeWidgetOffset();
	}

	void CUIContainer::render(void)
	{
		if (!_mbVisible)
			return;

		_renderWidgetBackgrounds();


		x->pRenderer->scissorTestEnable();

		{
			auto it = _mmapButtons.begin();
			while (it != _mmapButtons.end())
			{
				it->second->renderNonBG();
				it++;
			}
		}

		{
			auto it = _mmapButtonImages.begin();
			while (it != _mmapButtonImages.end())
			{
				it->second->renderNonBG();
				it++;
			}
		}

		// For each CUICheckbox widget (No need)

		{
			auto it = _mmapColourSelectors.begin();
			while (it != _mmapColourSelectors.end())
			{
				it->second->renderNonBG();
				it++;
			}
		}

		{
			auto it = _mmapImages.begin();
			while (it != _mmapImages.end())
			{
				it->second->renderNonBG();
				it++;
			}
		}

		{
			auto it = _mmapLineGraphs.begin();
			while (it != _mmapLineGraphs.end())
			{
				it->second->renderNonBG();
				it++;
			}
		}

		// For each CUIProgressbar widget (No need)

		// For each CUIScrollbar widget (No need)

		{
			auto it = _mmapTexts.begin();
			while (it != _mmapTexts.end())
			{
				it->second->renderNonBG();
				it++;
			}
		}

		x->pRenderer->scissorTestDisable();
		{
			auto it = _mmapTextEdits.begin();
			while (it != _mmapTextEdits.end())
			{
				it->second->renderNonBG();
				it++;
			}
		}

	}

	void CUIContainer::_renderWidgetBackgrounds(void)
	{
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

		{
			auto it = _mmapButtons.begin();
			while (it != _mmapButtons.end())
			{
				it->second->renderBG(pVB);
				it++;
			}
		}

		// For each CUIButtonImage widget (No need)

		{
			auto it = _mmapCheckboxes.begin();
			while (it != _mmapCheckboxes.end())
			{
				it->second->renderBG(pVB);
				it++;
			}
		}

		// For each CUIColourSelector widget (No need)

		// For each CUIImage widget (No need)

		{
			auto it = _mmapLineGraphs.begin();
			while (it != _mmapLineGraphs.end())
			{
				it->second->renderBG(pVB);
				it++;
			}
		}

		{
			auto it = _mmapProgressbars.begin();
			while (it != _mmapProgressbars.end())
			{
				it->second->renderBG(pVB);
				it++;
			}
		}

		{
			auto it = _mmapScrollbars.begin();
			while (it != _mmapScrollbars.end())
			{
				it->second->renderBG(pVB);
				it++;
			}
		}

		{
			auto it = _mmapTextEdits.begin();
			while (it != _mmapTextEdits.end())
			{
				it->second->renderBG(pVB);
				it++;
			}
		}

		// Render all the widget backgrounds added above
		x->pRenderer->scissorTestEnable();
		x->pRenderer->scissorTest(
			(int)getWidgetAreaTLCornerPosition().x,
			int(x->pWindow->getHeight() - getWidgetAreaTLCornerPosition().y - _mvContainersWidgetAreaDimensions.y),
			(int)_mvContainersWidgetAreaDimensions.x,
			(int)_mvContainersWidgetAreaDimensions.y);
		pVB->update();
		pVB->render();
		pVB->removeGeom();
		x->pRenderer->scissorTestDisable();

		// Render the container's scrollbars
		_mpScrollbarH->renderBG(pVB, false);
		_mpScrollbarV->renderBG(pVB, false);
		pVB->update();
		pVB->render();
		pVB->removeGeom();
		pAtlas->unbind();
		pShader->unbind();	// Unbind the GUI shader
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
	}

	void CUIContainer::renderTooltips(void)
	{

	}

	void CUIContainer::_onToggleFullscreen(void)
	{
		{
			auto it = _mmapTexts.begin();
			while (it != _mmapTexts.end())
			{
				it->second->setFramebufferNeedsUpdating();
				it++;
			}
		}

	}

	/************************************************************************************************************************************************************/
	/* The end!!!!!!! :) */
	/************************************************************************************************************************************************************/
}