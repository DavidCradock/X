#include "PCH.h"
#include "demo2Dstate2.h"

namespace X
{
	void CDemo2DState2::onEnter(void)
	{
		// Create state window
		CGUIContainer* pCont = x->pGUI->addContainer("state2");
		pCont->setDimensions(640, 340);
		pCont->setPosition(0, 0);
		pCont->mstrTitleText = "State two.";
		// Add text scroll
		std::string strTxt;
		strTxt += "Welcome to state 2.\n";
		strTxt += "This state shows particle systems in action.\n";
		strTxt += "Controls...\n";
		strTxt += "The particle system's emitter follows the mouse cursor.\n";
		strTxt += "Cursor keys move the camera.\n";
		strTxt += " \nDetails...\n";
		pCont->addTextScroll("TextScroll2", 0, 0, 640, 240, strTxt);

		// Add text which has information about the particle system
		CGUITheme* pTheme = x->pGUI->getTheme(pCont->getThemeName());
		CResourceFont* pFont = x->pResource->getFont(pTheme->mFonts.text);
		
		float fYpos = 260;
		pCont->addText("PSinfo0", 0, fYpos, "");	fYpos += pFont->getTextHeight();
		pCont->addText("PSinfo1", 0, fYpos, "");

		// Add a particle system to the SC2DRenderer
		C2DWorld* pWorld = x->p2dRenderer->addWorld("world");
		C2DCamera* pCamera = pWorld->addCamera("camera");
		CVector2f vWndDims = x->pWindow->getDimensions();
		CVector2f vCameraPosition(vWndDims.x * -0.5f, vWndDims.y * -0.5f);
		pCamera->setPosition(vCameraPosition);
		C2DLayer* pLayer = pWorld->addLayer("layer0");
		C2DParticleSystem* pPS = pLayer->addParticleSystem("particleSystem0");
		C2DParticleEmitter* pPSEmitter = pPS->addEmitter("emitter0");
		pPSEmitter->setBirthRate(1000);
		pPSEmitter->setMinimumDistance(0.0f);
		pPSEmitter->setMaximumDistance(0.0f);
		C2DParticleType* pType = pPS->getParticleType("default");
		pType->stageDeath.strTextureAtlasImageName = "data/x/textures/particles/smoke0129.png";

	}

	void CDemo2DState2::onExit(void)
	{
		x->pGUI->removeContainer("state2");

		// Remove world
		x->p2dRenderer->removeWorld("world");
	}

	void CDemo2DState2::onActive(CFiniteStateMachine* pFSM)
	{
		// Timer delta
		timer.update();

		// Get World, layer and camera
		C2DWorld* pWorld = x->p2dRenderer->getWorld("world");
		C2DLayer* pLayer0 = pWorld->getLayer("layer0");
		C2DCamera* pCamera = pWorld->getCamera(0);

		// Reposition the particle system's emitter to the position of the mouse cursor
		C2DParticleSystem* pPS = pLayer0->getParticleSystem("particleSystem0");
		C2DParticleEmitter* pEmitter = pPS->getEmitter("emitter0");
		CVector2f vNewEmitterPos = x->pInput->mouse.getCursorPos();
		// Transform position by camera
		vNewEmitterPos += pCamera->getPosition();
		pEmitter->setPosition(vNewEmitterPos);

		// Update GUI with info from particle system
		CGUIContainer* pCont = x->pGUI->getContainer("state2");
		std::string strTxt;
		strTxt = "Particles Total: ";	StringUtils::appendInt(strTxt, pPS->getNumParticles());			pCont->getText("PSinfo0")->setText(strTxt);
		strTxt = "Particles Dead: ";	StringUtils::appendInt(strTxt, pPS->getNumDeadParticles());		pCont->getText("PSinfo1")->setText(strTxt);

		// Now move camera
		CVector2f vCamPos = pCamera->getPosition();
		if (x->pInput->key.pressed(KC_UP))
			vCamPos.y -= timer.getSecondsPast() * 250.0f;
		if (x->pInput->key.pressed(KC_DOWN))
			vCamPos.y += timer.getSecondsPast() * 250.0f;
		if (x->pInput->key.pressed(KC_LEFT))
			vCamPos.x -= timer.getSecondsPast() * 250.0f;
		if (x->pInput->key.pressed(KC_RIGHT))
			vCamPos.x += timer.getSecondsPast() * 250.0f;
		pCamera->setPosition(vCamPos);

	
	}
}