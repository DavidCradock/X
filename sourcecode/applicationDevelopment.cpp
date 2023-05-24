#include "PCH.h"
#include "applicationDevelopment.h"
#include "audioManager.h"
#include "log.h"
#include "utilities.h"
#include "graphicsPipelines.h"

namespace X
{
	void ApplicationDevelopment::initOnce(void)
	{
		GraphicsPipelineManager* pGPM = GraphicsPipelineManager::getPointer();
		GraphicsPipeline *pGP = pGPM->add("dev");
		
		AudioManager* pAudioManager = AudioManager::getPointer();
		pAudioManager->addSample("audio/drone_loops/Falcon_Volkm_Beauty_120BPM.wav", "default");
		pAudioManager->loadSampleGroup("default");
		AudioEmitter *pEmitter = pAudioManager->addEmitter("emitter", "audio/drone_loops/Falcon_Volkm_Beauty_120BPM.wav", 8, "default");
		pEmitter->play(1, 1, true);
		float fFrequency = 1.0f;
		while (1)
		{
			Sleep(10);
			fFrequency -= 0.001f;
			pEmitter->setFrequency(0, fFrequency);
			if (fFrequency < 0.0f)
				break;
		}

	}

	void ApplicationDevelopment::onStart(void)
	{
	}

	void ApplicationDevelopment::onStop(void)
	{

	}

	bool ApplicationDevelopment::onUpdate(void)
	{
		return true;
	}
}