#include "PCH.h"
#include "GUILineGraph.h"
#include "GUIManager.h"
#include "resourceManager.h"
#include "window.h"
#include "input.h"

namespace X
{
	void GUILineGraph::render(void* pParentContainer, const std::string& strFramebufferToSampleFrom)
	{
		GUIContainer* pContainer = (GUIContainer*)pParentContainer;
		GUIManager* pGUIManager = GUIManager::getPointer();
		GUITheme* pTheme = pGUIManager->getTheme(pContainer->mstrThemename);
		renderBackground(pParentContainer, strFramebufferToSampleFrom, pTheme->mImages.lineGraphBGColour, pTheme->mImages.lineGraphBGNormal);

		/*
		// Get required resources needed to render
		GUIManager* pGUI = GUIManager::getPointer();
		ResourceManager* pRM = ResourceManager::getPointer();
		Window* pWindow = Window::getPointer();
		ResourceTriangle* pTri = pRM->getTriangle("X:gui");
		ResourceShader* pShader = pRM->getShader("X:gui");
		GUITheme* pTheme = pGUI->getTheme(pContainer->mstrThemename);
		InputManager* pInput = InputManager::getPointer();

		pShader->bind();

		// Setup the projection matrix as orthographic
		glm::mat4 matProjection = glm::ortho(0.0f, float(pWindow->getWidth()), float(pWindow->getHeight()), 0.0f, -1.0f, 1.0f);
		pShader->setMat4("transform", matProjection);

		// Tell OpenGL, for each sampler, to which texture unit it belongs to
		pShader->setInt("texture0_colour", 0);
		pShader->setInt("texture1_normal", 1);
		pShader->setInt("texture2_reflection", 2);
		pShader->setInt("texture3_background", 3);
		pShader->setFloat("fBlurAmount", pTheme->mfBlurAmount);
		pShader->setFloat("fNormalAmount", pTheme->mfNormalAmount);
		pShader->setFloat("fReflectionAmount", pTheme->mfReflectionAmount);
		pShader->setFloat("fMouseCursorDistance", pTheme->mfMouseCursorDistance);

		// Set mouse position, inverting Y position
		glm::vec2 vMousePos = pInput->mouse.getCursorPos();
		vMousePos.y = float(pWindow->getHeight()) - vMousePos.y;
		pShader->setVec2("v2MousePos", vMousePos);

		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);

		// Get textures
		ResourceTexture2D* pTexColour = pRM->getTexture2D(pTheme->mImages.sliderBackColour);
		ResourceTexture2D* pTexNormal = pRM->getTexture2D(pTheme->mImages.sliderBackNormal);
		ResourceTexture2D* pTexReflection = pRM->getTexture2D(pTheme->mImages.reflection);
		ResourceFramebuffer* pFBSample = pRM->getFramebuffer(strFramebufferToSampleFrom);

		// Bind textures
		pTexColour->bind(0);
		pTexNormal->bind(1);
		pTexReflection->bind(2);
		pFBSample->bindAsTexture(3);

		// Render the linegraph centre
		glm::vec2 vTexDimsDiv3 = pTexColour->mvDimensions * 0.3333333f;
		float fPosX = pContainer->mfPositionX + mfPositionX;
		float fPosY = pContainer->mfPositionY + mfPositionY;
		pTri->removeGeom();
		pTri->addQuad2D(
			glm::vec2(
				fPosX,								// Position X
				fPosY),								// Position Y
			glm::vec2(mfWidth, mfHeight),			// Dimensions
			glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),		// Vertex colour
			mTC.centre.BL,
			mTC.centre.BR,
			mTC.centre.TR,
			mTC.centre.TL);

		// Render the left edge
		pTri->addQuad2D(
			glm::vec2(
				fPosX - vTexDimsDiv3.x,				// Position X
				fPosY),								// Position Y
			glm::vec2(vTexDimsDiv3.x, mfHeight),	// Dimensions
			glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),		// Vertex colour
			mTC.left.BL,
			mTC.left.BR,
			mTC.left.TR,
			mTC.left.TL);

		// Render the right edge
		pTri->addQuad2D(
			glm::vec2(
				fPosX + mfWidth,					// Position X
				fPosY),								// Position Y
			glm::vec2(vTexDimsDiv3.x, mfHeight),	// Dimensions
			glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),		// Vertex colour
			mTC.right.BL,
			mTC.right.BR,
			mTC.right.TR,
			mTC.right.TL);

		// Render the top edge
		pTri->addQuad2D(
			glm::vec2(
				fPosX,								// Position X
				fPosY - vTexDimsDiv3.y),			// Position Y
			glm::vec2(mfWidth, vTexDimsDiv3.y),		// Dimensions
			glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),		// Vertex colour
			mTC.top.BL,
			mTC.top.BR,
			mTC.top.TR,
			mTC.top.TL);

		// Render the bottom edge
		pTri->addQuad2D(
			glm::vec2(
				fPosX,								// Position X
				fPosY + mfHeight),					// Position Y
			glm::vec2(mfWidth, vTexDimsDiv3.y),		// Dimensions
			glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),		// Vertex colour
			mTC.bottom.BL,
			mTC.bottom.BR,
			mTC.bottom.TR,
			mTC.bottom.TL);

		// Render the top left corner
		pTri->addQuad2D(
			glm::vec2(
				fPosX - vTexDimsDiv3.x,					// Position X
				fPosY - vTexDimsDiv3.y),				// Position Y
			glm::vec2(vTexDimsDiv3.x, vTexDimsDiv3.y),	// Dimensions
			glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),			// Vertex colour
			mTC.topLeft.BL,
			mTC.topLeft.BR,
			mTC.topLeft.TR,
			mTC.topLeft.TL);

		// Render the top right corner
		pTri->addQuad2D(
			glm::vec2(
				fPosX + mfWidth,						// Position X
				fPosY - vTexDimsDiv3.y),				// Position Y
			glm::vec2(vTexDimsDiv3.x, vTexDimsDiv3.y),	// Dimensions
			glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),			// Vertex colour
			mTC.topRight.BL,
			mTC.topRight.BR,
			mTC.topRight.TR,
			mTC.topRight.TL);

		// Render the bottom left corner
		pTri->addQuad2D(
			glm::vec2(
				fPosX - vTexDimsDiv3.x,					// Position X
				fPosY + mfHeight),						// Position Y
			glm::vec2(vTexDimsDiv3.x, vTexDimsDiv3.y),	// Dimensions
			glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),			// Vertex colour
			mTC.bottomLeft.BL,
			mTC.bottomLeft.BR,
			mTC.bottomLeft.TR,
			mTC.bottomLeft.TL);

		// Render the bottom right corner
		pTri->addQuad2D(
			glm::vec2(
				fPosX + mfWidth,						// Position X
				fPosY + mfHeight),						// Position Y
			glm::vec2(vTexDimsDiv3.x, vTexDimsDiv3.y),	// Dimensions
			glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),			// Vertex colour
			mTC.bottomRight.BL,
			mTC.bottomRight.BR,
			mTC.bottomRight.TR,
			mTC.bottomRight.TL);

		pTri->update();
		pTri->draw();

		pTexColour->unbindAll();	// Unbind textures
		pShader->unbind();			// Unbind the GUI shader
		glDisable(GL_BLEND);
		*/
	}

	void GUILineGraph::update(void* pParentContainer, bool bParentContainerAcceptingMouseClicks)
	{
		GUIContainer* pContainer = (GUIContainer*)pParentContainer;

		_mTimer.update();
		float fSecondsPast = _mTimer.getSecondsPast();
		if (fSecondsPast > 0.1f)
			fSecondsPast = 0.1f;

	}
}