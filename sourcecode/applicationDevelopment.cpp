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
//		GraphicsPipeline *pGP = pGPM->add("dev");
//		pGP->setFilenames("GPUprograms/dev_v.spv", "GPUprograms/dev_f.spv");

		pGPM->loadGroup("default");

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