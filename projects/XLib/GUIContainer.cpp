#include "PCH.h"
#include "GUIContainer.h"
#include "input.h"
#include "resourceManager.h"
#include "GUIManager.h"
#include "window.h"

namespace X
{
	GUIContainer::GUIContainer()
	{
		mstrThemename = "default";
		mbContainerIsWindow = true;
		_mbWindowBeingMoved = false;
	}

	// Containers are rendered in order of ZOrder, with the back most being rendered first
	void GUIContainer::render(const std::string& strFramebufferToSampleFrom)
	{
		// Get required resources needed to render
		GUIManager* pGUI = GUIManager::getPointer();
		ResourceManager* pRM = ResourceManager::getPointer();
		Window* pWindow = Window::getPointer();
		ResourceTriangle* pTri = pRM->getTriangle("X:gui");
		ResourceShader* pShader = pRM->getShader("X:gui");
		GUITheme* pTheme = pGUI->getTheme(mstrThemename);
		

		pShader->bind();
		
		// Setup the projection matrix as orthographic
		glm::mat4 matProjection = glm::ortho(0.0f, float(pWindow->getWidth()), float(pWindow->getHeight()), 0.0f, -1.0f, 1.0f);
		pShader->setMat4("transform", matProjection);

		// Tell OpenGL, for each sampler, to which texture unit it belongs to
		pShader->setInt("texture0_blur", 0);
		pShader->setInt("texture1_colour", 1);
		pShader->setInt("texture2_glow", 2);
		pShader->setInt("texture3_normal", 3);
		pShader->setInt("texture4_reflection", 4);
		pShader->setInt("texture5_background", 5);
		pShader->setFloat("fBlurAmount", pTheme->mfBlurAmount);
		pShader->setFloat("fGlowAmount", pTheme->mfGlowAmount);
		pShader->setFloat("fNormalAmount", pTheme->mfNormalAmount);
		pShader->setFloat("fReflectionAmount", pTheme->mfReflectionAmount);
		

		if (mbContainerIsWindow)
		{
			// Get textures and background sample framebuffer
			ResourceTexture2D* pTexBlur = pRM->getTexture2D(pTheme->mImages.containerBlur);
			ResourceTexture2D* pTexColour = pRM->getTexture2D(pTheme->mImages.containerColour);
			ResourceTexture2D* pTexGlow = pRM->getTexture2D(pTheme->mImages.containerGlow);
			ResourceTexture2D* pTexNormal = pRM->getTexture2D(pTheme->mImages.containerNormal);
			ResourceTexture2D* pTexReflection = pRM->getTexture2D(pTheme->mImages.reflection);
			ResourceFramebuffer* pFBSample = pRM->getFramebuffer(strFramebufferToSampleFrom);

			// Bind textures
			pTexBlur->bind(0);
			pTexColour->bind(1);
			pTexGlow->bind(2);
			pTexNormal->bind(3);
			pTexReflection->bind(4);
			pFBSample->bindAsTexture(5);

			// Render the container centre
			pTri->removeGeom();
			pTri->addQuad2D(
				glm::vec2(mfPositionX, mfPositionY),	// Position
				glm::vec2(mfWidth, mfHeight),			// Dimensions
				glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),		// Vertex colour
				mTC.centre.BL,
				mTC.centre.BR,
				mTC.centre.TR,
				mTC.centre.TL);

			// Render the left edge
			glm::vec2 vTexDimsDiv3 = pTexColour->mvDimensions * 0.3333333f;
			pTri->addQuad2D(
				glm::vec2(mfPositionX - vTexDimsDiv3.x, mfPositionY),	// Position
				glm::vec2(vTexDimsDiv3.x, mfHeight),					// Dimensions
				glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),						// Vertex colour
				mTC.left.BL,
				mTC.left.BR,
				mTC.left.TR,
				mTC.left.TL);

			// Render the right edge
			pTri->addQuad2D(
				glm::vec2(mfPositionX + mfWidth, mfPositionY),	// Position
				glm::vec2(vTexDimsDiv3.x, mfHeight),			// Dimensions
				glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),				// Vertex colour
				mTC.right.BL,
				mTC.right.BR,
				mTC.right.TR,
				mTC.right.TL);

			// Render the top edge
			pTri->addQuad2D(
				glm::vec2(mfPositionX, mfPositionY - vTexDimsDiv3.y),	// Position
				glm::vec2(mfWidth, vTexDimsDiv3.y),						// Dimensions
				glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),						// Vertex colour
				mTC.top.BL,
				mTC.top.BR,
				mTC.top.TR,
				mTC.top.TL);

			// Render the bottom edge
			pTri->addQuad2D(
				glm::vec2(mfPositionX, mfPositionY + mfHeight),		// Position
				glm::vec2(mfWidth, vTexDimsDiv3.y),					// Dimensions
				glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),					// Vertex colour
				mTC.bottom.BL,
				mTC.bottom.BR,
				mTC.bottom.TR,
				mTC.bottom.TL);

			// Render the top left corner
			pTri->addQuad2D(
				glm::vec2(mfPositionX - vTexDimsDiv3.x, mfPositionY - vTexDimsDiv3.y),	// Position
				glm::vec2(vTexDimsDiv3.x, vTexDimsDiv3.y),								// Dimensions
				glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),										// Vertex colour
				mTC.topLeft.BL,
				mTC.topLeft.BR,
				mTC.topLeft.TR,
				mTC.topLeft.TL);

			// Render the top right corner
			pTri->addQuad2D(
				glm::vec2(mfPositionX + mfWidth, mfPositionY - vTexDimsDiv3.y),	// Position
				glm::vec2(vTexDimsDiv3.x, vTexDimsDiv3.y),						// Dimensions
				glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),								// Vertex colour
				mTC.topRight.BL,
				mTC.topRight.BR,
				mTC.topRight.TR,
				mTC.topRight.TL);

			// Render the bottom left corner
			pTri->addQuad2D(
				glm::vec2(mfPositionX - vTexDimsDiv3.x, mfPositionY + mfHeight),	// Position
				glm::vec2(vTexDimsDiv3.x, vTexDimsDiv3.y),							// Dimensions
				glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),									// Vertex colour
				mTC.bottomLeft.BL,
				mTC.bottomLeft.BR,
				mTC.bottomLeft.TR,
				mTC.bottomLeft.TL);

			// Render the bottom right corner
			pTri->addQuad2D(
				glm::vec2(mfPositionX + mfWidth, mfPositionY + mfHeight),	// Position
				glm::vec2(vTexDimsDiv3.x, vTexDimsDiv3.y),					// Dimensions
				glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),							// Vertex colour
				mTC.bottomRight.BL,
				mTC.bottomRight.BR,
				mTC.bottomRight.TR,
				mTC.bottomRight.TL);

			pTri->update();
			pTri->draw();

			// Unbind textures
			pTexBlur->unbindAll();
		}

		pShader->unbind();

	}

	// Containers are updated in order of ZOrder, with the front most being updated first
	bool GUIContainer::update(bool bMouseIsOverContainerWhichIsAboveThisOne)
	{
		ResourceManager* pRM = ResourceManager::getPointer();
		GUIManager* pGUI = GUIManager::getPointer();
		GUITheme* pTheme = pGUI->getTheme(mstrThemename);
		Window* pWindow = Window::getPointer();
		glm::vec2 vWindowDims = pWindow->getDimensions();

		InputManager* pInput = InputManager::getPointer();
		glm::vec2 vMousePos = pInput->mouse.getCursorPos();

		
		glm::vec2 vTexDimsDiv3 = pRM->getTexture2D(pTheme->mImages.containerColour)->mvDimensions * 0.3333333f;

		// Determine whether mouse cursor is over this container
		bool bMouseOver = false;
		if (!bMouseIsOverContainerWhichIsAboveThisOne)	// Only perform check if mouse is NOT over a container which is above this one
		{
			if (mbContainerIsWindow)
			{
				if (vMousePos.x >= mfPositionX - vTexDimsDiv3.x)
					if (vMousePos.x <= mfPositionX + mfWidth + vTexDimsDiv3.x)
						if (vMousePos.y >= mfPositionY - vTexDimsDiv3.y)
							if (vMousePos.y <= mfPositionY + mfHeight + vTexDimsDiv3.y)
							{
								// Mouse cursor is over this container
								bMouseOver = true;
							}
			}
			else
			{
				if (vMousePos.x >= mfPositionX)
					if (vMousePos.x <= mfPositionX + mfWidth)
						if (vMousePos.y >= mfPositionY)
							if (vMousePos.y <= mfPositionY + mfHeight)
							{
								// Mouse cursor is over this container
								bMouseOver = true;
							}
			}
		}

		// The mouse is over this window, we may accept mouse clicks on this container's items
		if (bMouseOver)
		{
			// Dragging of window?
			if (mbContainerIsWindow)
			{
				if (pInput->mouse.leftButDown())
				{
					if (!pGUI->getWindowBeingMoved())
					{
						_mbWindowBeingMoved = true;
						pGUI->setWindowBeingMoved(true);
					}
				}
			}
		}
		else  // The mouse is not over this container, or it is, but there is another container above this one so only update time based stuff, not clickable stuff.
		{

		}

		// Stop dragging if mouse button is no longer down
		if (_mbWindowBeingMoved)
		{
			if (!pInput->mouse.leftButDown())
			{
				_mbWindowBeingMoved = false;
				pGUI->setWindowBeingMoved(false);
			}
		}

		if (_mbWindowBeingMoved)
		{
			mfPositionX += pInput->mouse.getMouseDeltaGUI().x;
			mfPositionY += pInput->mouse.getMouseDeltaGUI().y;
		}

		// Clamp the container's position so that it's borders are viewable IF the container is set as a window
		if (mbContainerIsWindow)
		{
			// Get a texture so we can determine dimensions
			ResourceTexture2D* pTexColour = pRM->getTexture2D(pTheme->mImages.containerColour);
			glm::vec2 vTexDimsDiv3 = pTexColour->mvDimensions * 0.3333333f;
			if (mfPositionX < vTexDimsDiv3.x)
				mfPositionX = vTexDimsDiv3.x;
			else if (mfPositionX + mfWidth + vTexDimsDiv3.x > vWindowDims.x)
				mfPositionX = vWindowDims.x - mfWidth - vTexDimsDiv3.x;
			if (mfPositionY < vTexDimsDiv3.x)
				mfPositionY = vTexDimsDiv3.x;
			else if (mfPositionY + mfWidth + vTexDimsDiv3.y > vWindowDims.y)
				mfPositionY = vWindowDims.y - mfHeight - vTexDimsDiv3.y;

		}

		return bMouseOver;
	}
}