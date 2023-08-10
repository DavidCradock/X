#include "PCH.h"
#include "GUICheckbox.h"
#include "GUIManager.h"
#include "GUITooltip.h"
#include "singletons.h"

namespace X
{
	CGUICheckbox::CGUICheckbox()
	{
		_mbClicked = false;
		_mbStateON = false;
		_mfFade = 0.0f;
		mpTooltip = new CGUITooltip;
	}

	CGUICheckbox::~CGUICheckbox()
	{
		delete mpTooltip;
	}

	void CGUICheckbox::render(void* pParentContainer, const std::string& strFramebufferToSampleFrom)
	{
		CGUIContainer* pContainer = (CGUIContainer*)pParentContainer;
		CGUITheme* pTheme = pContainer->getTheme();

		// Get required resources needed to render
		CResourceVertexBufferCPT* pVB = x->pResource->getVertexBufferCPT("X:default");
		CResourceShader* pShader = x->pResource->getShader("X:gui");

		pShader->bind();

		// Setup the projection matrix as orthographic
		CMatrix matProjection;
		matProjection.setProjectionOrthographic(0.0f, float(x->pWindow->getWidth()), 0.0f, float(x->pWindow->getHeight()), -1.0f, 1.0f);
		pShader->setMat4("transform", matProjection);

		// Tell OpenGL, for each sampler, to which texture unit it belongs to
		pShader->setInt("texture0_colour", 0);
		pShader->setInt("texture1_normal", 1);
		pShader->setInt("texture2_reflection", 2);
		pShader->setInt("texture3_background", 3);
		pShader->setFloat("fNormalAmount", pTheme->mfNormalAmount);
		pShader->setFloat("fReflectionAmount", pTheme->mfReflectionAmount);
		pShader->setFloat("fMouseCursorDistance", pTheme->mfMouseCursorDistance);

		// Set mouse position, inverting Y position
		CVector2f vMousePos = x->pInput->mouse.getCursorPos();
		vMousePos.y = float(x->pWindow->getHeight()) - vMousePos.y;
		pShader->setVec2("v2MousePos", vMousePos);

		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);

		// Get textures
		CResourceTexture2DFromFile* pTexColourOFF = x->pResource->getTexture2DFromFile(pTheme->mImages.checkboxBGColourOFF);
		CResourceTexture2DFromFile* pTexColourON = x->pResource->getTexture2DFromFile(pTheme->mImages.checkboxBGColourON);
		CResourceTexture2DFromFile* pTexNormal = x->pResource->getTexture2DFromFile(pTheme->mImages.checkboxBGNormal);
		CResourceTexture2DFromFile* pTexReflection = x->pResource->getTexture2DFromFile(pTheme->mImages.reflection);
		CResourceFramebuffer* pFBSample = x->pResource->getFramebuffer(strFramebufferToSampleFrom);

		// Bind textures
		pTexNormal->bind(1);
		pTexReflection->bind(2);
		pFBSample->bindAsTexture(3);

		// Only render a single quad if not fading between
		bool bRenderJustOnce = false;
		if (_mfFade == 0.0f) // Just render OFF
		{
			pTexColourOFF->bind(0);
			bRenderJustOnce = true;
		}
		else if (_mfFade == 1.0f)	// Just render ON
		{
			pTexColourON->bind(0);
			bRenderJustOnce = true;
		}

		CVector2f vPos = CVector2f(pContainer->mfPositionX + mfPositionX, pContainer->mfPositionY + mfPositionY);
		CColour col(1.0f, 1.0f, 1.0f, 1.0f);
		pVB->removeGeom();
		if (bRenderJustOnce)
		{
			pVB->addQuad2D(vPos, CVector2f(mfWidth, mfHeight), col);
			pVB->update();
			pVB->render();
		}
		else // Render both
		{
			// Off
			col.alpha = 1.0f - _mfFade;
			pTexColourOFF->bind(0);
			pVB->addQuad2D(vPos, CVector2f(mfWidth, mfHeight), col);
			pVB->update();
			pVB->render();
			pVB->removeGeom();

			// On
			col.alpha = _mfFade;
			pTexColourON->bind(0);
			pVB->addQuad2D(vPos, CVector2f(mfWidth, mfHeight), col);
			pVB->update();
			pVB->render();
			pVB->removeGeom();
		}

		pTexColourON->unbindAll();	// Unbind textures
		pShader->unbind();	// Unbind the GUI shader
		glDisable(GL_BLEND);
	}

	void CGUICheckbox::update(void* pParentContainer, bool bParentContainerAcceptingMouseClicks)
	{
		_mTimer.update();
		float fSecondsPast = _mTimer.getSecondsPast();
		if (fSecondsPast > 0.1f)
			fSecondsPast = 0.1f;

		CGUIContainer* pContainer = (CGUIContainer*)pParentContainer;
		CVector2f vMousePos = x->pInput->mouse.getCursorPos();
		CGUITheme* pTheme = pContainer->getTheme();

		// Set dimensions of object based on container's theme
		CResourceTexture2DFromFile* pTextureOFF = x->pResource->getTexture2DFromFile(pTheme->mImages.checkboxBGColourOFF);
		CVector2f vDims = pTextureOFF->getDimensions();
		mfWidth = vDims.x;
		mfHeight = vDims.y;

		// Determine if mouse cursor is over AND whether it's been clicked upon
		bool bMouseOver = false;
		_mbClicked = false;
		if (bParentContainerAcceptingMouseClicks)
		{
			if (vMousePos.x > pContainer->mfPositionX + mfPositionX)
				if (vMousePos.x < pContainer->mfPositionX + mfPositionX + mfWidth)
					if (vMousePos.y > pContainer->mfPositionY + mfPositionY)
						if (vMousePos.y < pContainer->mfPositionY + mfPositionY + mfHeight)
							bMouseOver = true;
			if (bMouseOver)
			{
				if (x->pInput->mouse.leftButtonOnce())
				{
					_mbClicked = true;
					_mbStateON = !_mbStateON;
				}
			}
		}

		// Now set _mfFade
		if (!bMouseOver)
		{
			if (_mbStateON)
				_mfFade += fSecondsPast / pTheme->mfCheckboxFadeSpeed;
			else
				_mfFade -= fSecondsPast / pTheme->mfCheckboxFadeSpeed;
			clamp(_mfFade, 0.0f, 1.0f);
		}
		else // Mouse is over
		{
			if (_mbStateON)
			{
				_mfFade -= fSecondsPast / pTheme->mfCheckboxFadeSpeed;
				if (_mfFade < 0.5f)
					_mfFade = 0.5f;
			}
			else
			{
				_mfFade += fSecondsPast / pTheme->mfCheckboxFadeSpeed;
				if (_mfFade > 0.5f)
					_mfFade = 0.5f;
			}
		}

		// Update this object's tooltip
		mpTooltip->update(pParentContainer, (CGUIBaseObject*)this, bMouseOver);
	}

	bool CGUICheckbox::getClicked(void) const
	{
		return _mbClicked;
	}

	bool CGUICheckbox::getState(void) const
	{
		return _mbStateON;
	}

	void CGUICheckbox::setState(bool bOn)
	{
		_mbStateON = bOn;

	}
}