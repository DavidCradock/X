#include "PCH.h"
#include "UIContainer.h"
#include "UIManager.h"
#include "singletons.h"

namespace X
{
	CUIContainer::CUIContainer()
	{
		_mstrThemename = "default";
	}

	CUIContainer::~CUIContainer()
	{
		widgetRemoveAll();
	}

	void CUIContainer::update(float fTimeDeltaSec)
	{
		CUITheme* pTheme = SCUIManager::getPointer()->themeGet(_mstrThemename);
		CResourceTexture2DAtlas* pAtlas = pTheme->getTextureAtlas();

		// For each scrollbar object, render non-font stuff
		for (size_t i = 0; i < _mmanWidgetScrollbars.getNumber(); i++)
		{
			_mmanWidgetScrollbars.get(i)->update(fTimeDeltaSec, pAtlas, this, false, pTheme);
		}
	}

	void CUIContainer::render(void)
	{
		// Get required resources needed to render things
		CResourceVertexBufferCPT2* pVB = x->pResource->getVertexBufferCPT2(x->pResource->defaultRes.vertexbufferCPT2_default);
		CResourceShader* pShader = x->pResource->getShader(x->pResource->defaultRes.shader_ui);
		CResourceFramebuffer* pUIFB = x->pResource->getFramebuffer(x->pResource->defaultRes.framebuffer_ui);
		CUITheme* pTheme = SCUIManager::getPointer()->themeGet(_mstrThemename);

		pShader->bind();

		// Setup the projection matrix as orthographic
		CMatrix matProjection;
		matProjection.setProjectionOrthographic(0.0f, (float)pUIFB->getWidth(), 0.0f, (float)pUIFB->getHeight(), -1.0f, 1.0f);
		pShader->setMat4("transform", matProjection);

		// Tell OpenGL, for each sampler, to which texture unit it belongs to
		pShader->setInt("texture0_colourAndNormal", 0);
		pShader->setFloat("fNormalAmount", pTheme->getSettings()->floats.normalAmount);
		pShader->setFloat("fMouseCursorDistance", pTheme->getSettings()->floats.normalMouseCursorDistance);

		// Set mouse position, inverting Y position
		CVector2f vMousePos = x->pInput->mouse.getCursorPos();
		vMousePos.y = float(x->pWindow->getHeight()) - vMousePos.y;
		pShader->setVec2("v2MousePos", vMousePos);

		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);

		// Get textures and bind them
		CResourceTexture2DAtlas* pAtlas = pTheme->getTextureAtlas();
		pAtlas->bindAtlas();

		pVB->removeGeom();

		// For each scrollbar object, render non-font stuff
		for (size_t i = 0; i < _mmanWidgetScrollbars.getNumber(); i++)
		{
			_mmanWidgetScrollbars.get(i)->render(pAtlas, this, false, pTheme, pVB);
		}

		pVB->update();
		pVB->render();

		pAtlas->unbind();
		pShader->unbind();	// Unbind the GUI shader

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
	/* Scrollbars */
	/************************************************************************************************************************************************************/

	// Add scrollbar to this container and return a pointer to it
	// If the name already exists, an exception occurs
	// If the width is greater than height, it is set as a horizontal scrollbar, else vertical.
	// fTabRatio is a value which is multiplied by the width/height(Depending on orientation) of the scrollbar's dims, to obtain tab dimensions
	CUIScrollbar* CUIContainer::scrollbarAdd(const std::string& strName, float fPosX, float fPosY, float fWidth, float fHeight, float fTabRatio)
	{
		// Throw exception if named object already exists
		ThrowIfTrue(_mmanWidgetScrollbars.exists(strName), "CUIContainer::scrollbarAdd(\"" + strName + "\") failed. Named object already exists.");

		// Create new object
		CUIScrollbar* pNewObject = _mmanWidgetScrollbars.add(strName);

		// Set settings for new object
		pNewObject->setDimensions(CVector2f(fWidth, fHeight));
		pNewObject->setPosition(CVector2f(fPosX, fPosY));

		return pNewObject;
	}

	CUIScrollbar* CUIContainer::scrollbarGet(const std::string& strName)
	{
		return _mmanWidgetScrollbars.get(strName);
	}

	void CUIContainer::scrollbarRemove(const std::string& strName)
	{
		if (_mmanWidgetScrollbars.exists(strName))
			_mmanWidgetScrollbars.remove(strName);
	}

	void CUIContainer::scrollbarRemoveAll(void)
	{
		_mmanWidgetScrollbars.removeAll();
	}
}