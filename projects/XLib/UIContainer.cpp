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
		_mvDimensions.set(128, 128);
		_mbVisible = true;

		// Scrollbars
		
	}

	CUIContainer::~CUIContainer()
	{
		widgetRemoveAll();
	}

	void CUIContainer::setPosition(const CVector2f& vPosition)
	{
		_mvPosition = vPosition;
	}

	CVector2f CUIContainer::getPosition(void) const
	{
		return _mvPosition;
	}

	void CUIContainer::setDimensions(const CVector2f& vDimensions)
	{
		_mvDimensions = vDimensions;
	}

	CVector2f CUIContainer::getDimensions(void) const
	{
		return _mvDimensions;
	}

	void CUIContainer::setVisible(bool bVisible)
	{
		_mbVisible = bVisible;
	}

	bool CUIContainer::getVisible(void) const
	{
		return _mbVisible;
	}

	void CUIContainer::update(float fTimeDeltaSec, bool bIsWindow)
	{
		CUITheme* pTheme = SCUIManager::getPointer()->themeGet(_mstrThemename);
//		CResourceTexture2DAtlas* pAtlas = pTheme->getTextureAtlas();

		// For each button object, render non-font stuff
		for (size_t i = 0; i < _mmanButtons.getNumber(); i++)
		{
			_mmanButtons.get(i)->update(fTimeDeltaSec, this, bIsWindow, pTheme);
		}

		// For each scrollbar object, render non-font stuff
		for (size_t i = 0; i < _mmanScrollbars.getNumber(); i++)
		{
			_mmanScrollbars.get(i)->update(fTimeDeltaSec, this, bIsWindow, pTheme);
		}
	}

	void CUIContainer::render(bool bIsWindow)
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

		// For each button object, render non-font stuff
		for (size_t i = 0; i < _mmanButtons.getNumber(); i++)
		{
			_mmanButtons.get(i)->render(this, bIsWindow, pTheme, pVB);
		}

		// For each scrollbar object, render non-font stuff
		for (size_t i = 0; i < _mmanScrollbars.getNumber(); i++)
		{
			_mmanScrollbars.get(i)->render(this, bIsWindow, pTheme, pVB);
		}

		pVB->update();
		pVB->render();

		pAtlas->unbind();
		pShader->unbind();	// Unbind the GUI shader
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);


		// Render font stuff
		// For each button object, render non-font stuff
		for (size_t i = 0; i < _mmanButtons.getNumber(); i++)
		{
			_mmanButtons.get(i)->renderFonts(this, bIsWindow, pTheme);
		}
	}

	void CUIContainer::renderTooltips(void)
	{

	}

	std::string CUIContainer::themeNameGet(void) const
	{
		return _mstrThemename;
	}

	CUITheme* CUIContainer::themeGet(void) const
	{
		return SCUIManager::getPointer()->themeGet(_mstrThemename);
	}

	void CUIContainer::themeNameSet(const std::string& strThemeToUse)
	{
		_mstrThemename = strThemeToUse;
	}

	void CUIContainer::widgetRemoveAll(void)
	{
		scrollbarRemoveAll();
	}

	std::string CUIContainer::getName(void)
	{
		return _mstrName;
	}

	/************************************************************************************************************************************************************/
	/* Buttons */
	/************************************************************************************************************************************************************/

	// Add button to this container and return a pointer to it
	// If the name already exists, an exception occurs
	CUIButton* CUIContainer::buttonAdd(const std::string& strName, float fPosX, float fPosY, float fWidth, float fHeight)
	{
		// Throw exception if named object already exists
		ThrowIfTrue(_mmanButtons.exists(strName), "CUIContainer::buttonAdd(\"" + strName + "\") failed. Named object already exists.");

		// Create new object
		CUIButton* pNewObject = _mmanButtons.add(strName);

		// Set settings for new object
		pNewObject->setDimensions(CVector2f(fWidth, fHeight));
		pNewObject->setPosition(CVector2f(fPosX, fPosY));
		pNewObject->setText(strName);
		return pNewObject;
	}

	CUIButton* CUIContainer::buttonbarGet(const std::string& strName)
	{
		return _mmanButtons.get(strName);
	}

	void CUIContainer::buttonRemove(const std::string& strName)
	{
		if (_mmanButtons.exists(strName))
			_mmanButtons.remove(strName);
	}

	void CUIContainer::buttonRemoveAll(void)
	{
		_mmanButtons.removeAll();
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
		ThrowIfTrue(_mmanScrollbars.exists(strName), "CUIContainer::scrollbarAdd(\"" + strName + "\") failed. Named object already exists.");

		// Create new object
		CUIScrollbar* pNewObject = _mmanScrollbars.add(strName);

		// Set settings for new object
		pNewObject->setDimensions(CVector2f(fWidth, fHeight));
		pNewObject->setPosition(CVector2f(fPosX, fPosY));

		return pNewObject;
	}

	CUIScrollbar* CUIContainer::scrollbarGet(const std::string& strName)
	{
		return _mmanScrollbars.get(strName);
	}

	void CUIContainer::scrollbarRemove(const std::string& strName)
	{
		if (_mmanScrollbars.exists(strName))
			_mmanScrollbars.remove(strName);
	}

	void CUIContainer::scrollbarRemoveAll(void)
	{
		_mmanScrollbars.removeAll();
	}

	void CUIContainer::_computeScrollbars(void)
	{
		// Get required stuff from theme and atlas
		CUITheme* pTheme = themeGet();
		const CUITheme::SSettings* pSettings = pTheme->getSettings();
		CResourceTexture2DAtlas* pAtlas = pTheme->getTextureAtlas();
		CImageAtlasDetails idScrollbarTL = pAtlas->getImageDetails(pSettings->images.scrollbarBG.colour.cornerTL);
		CImageAtlasDetails idScrollbarL = pAtlas->getImageDetails(pSettings->images.scrollbarBG.colour.edgeL);
		CImageAtlasDetails idScrollbarT = pAtlas->getImageDetails(pSettings->images.scrollbarBG.colour.edgeT);
		CImageAtlasDetails idScrollbarBR = pAtlas->getImageDetails(pSettings->images.scrollbarBG.colour.cornerBR);
		
		// Horizontal scrollbar
		CVector2f vDims;
		CVector2f vPos;
		vDims.x = _mvDimensions.x;
		vDims.y = idScrollbarTL.vDims.y + idScrollbarL.vDims.y + idScrollbarBR.vDims.y;
		_mScrollbarH.setDimensions(vDims);
		vPos.x = 0;
		vPos.y = _mvDimensions.y;
		_mScrollbarH.setPosition(vPos);

		// Vertical scrollbar
//		vDims.x = idScrollbarTL.vDims.x + idScrollbarT.vDims.x +

	}
}