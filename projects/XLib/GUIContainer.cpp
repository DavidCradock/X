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

	}

	// Containers are rendered in order of ZOrder, with the back most being rendered first
	void GUIContainer::render(void)
	{
		// Get required resources needed to render
		GUIManager* pGUI = GUIManager::getPointer();
		ResourceManager* pRM = ResourceManager::getPointer();
		ResourceTriangle* pTri = pRM->getTriangle("X:gui");
		ResourceShader* pShader = pRM->getShader("X:gui");
		GUITheme* pTheme = pGUI->getTheme(mstrThemename);
		ResourceTexture2D* pTexBlur = pRM->getTexture2D(pTheme->mImages.containerBlur);
		ResourceTexture2D* pTexColour = pRM->getTexture2D(pTheme->mImages.containerColour);
		ResourceTexture2D* pTexGlow = pRM->getTexture2D(pTheme->mImages.containerGlow);
		ResourceTexture2D* pTexNormal = pRM->getTexture2D(pTheme->mImages.containerNormal);
		ResourceTexture2D* pTexReflection = pRM->getTexture2D(pTheme->mImages.reflection);
		Window* pWindow = Window::getPointer();

		pShader->bind();
		// Setup the projection matrix as orthographic
		glm::mat4 matProjection = glm::ortho(0.0f, float(pWindow->getWidth()), float(pWindow->getHeight()), 0.0f, -1.0f, 1.0f);

		pShader->unbind();

	}

	// Containers are updated in order of ZOrder, with the front most being updated first
	bool GUIContainer::update(bool bMouseIsOverContainerWhichIsAboveThisOne)
	{
		InputManager* pInput = InputManager::getPointer();
		glm::vec2 vMousePos = pInput->mouse.getCursorPos();

		// Determine whether mouse cursor is over this container
		bool bMouseOver = false;
		if (!bMouseIsOverContainerWhichIsAboveThisOne)	// Only perform check if mouse is NOT over a container which is above this one
		{
			if ((int)vMousePos.x >= miPositionX)
				if ((int)vMousePos.x <= miPositionX + miWidth)
					if ((int)vMousePos.y >= miPositionY)
						if ((int)vMousePos.y <= miPositionY + miHeight)
						{
							// Mouse cursor is over this container
							bMouseOver = true;
						}
		}

		// The mouse is over this window, we may accept mouse clicks on this container's items
		if (bMouseOver)
		{

		}
		else  // The mouse is not over this container, or it is, but there is another container above this one so only update time based stuff, not clickable stuff.
		{

		}
		{

		}

		return bMouseOver;
	}
}