#include "PCH.h"
#include "applicationDevelopment.h"
#include "audioManager.h"
#include "log.h"
#include "utilities.h"
#include "graphicsPipelineManager.h"

namespace X
{
	void ApplicationDevelopment::initOnce(void)
	{
		GraphicsPipelineManager* pGPM = GraphicsPipelineManager::getPointer();
		pGPM->addNewGroup("group1");
		pGPM->add("res0", "default");
		pGPM->add("res1", "default");
		pGPM->add("res2", "group1");
//		GraphicsPipeline* pGP1 = pGPM->get("res0", "default");
//		GraphicsPipeline* pGP2 = pGPM->get("res2", "group1");
		pGPM->getExists("res0", "default");
		pGPM->getExists("res2", "group1");
		std::string strGroupName0 = pGPM->getGroupName(0);
		std::string strGroupName1 = pGPM->getGroupName(1);
		unsigned int iNumGroups = pGPM->getNumGroups();
		unsigned int iNumResInGroup0 = pGPM->getNumResInGroup("default");
		unsigned int iNumResInGroup1 = pGPM->getNumResInGroup("group1");
		unsigned int iNumResInGroupLoaded0 = pGPM->getNumResInGroupLoaded("default");
		unsigned int iNumResInGroupLoaded1 = pGPM->getNumResInGroupLoaded("group1");
		bool bGroupExists0 = pGPM->groupExists("default");
		bool bGroupExists1 = pGPM->groupExists("group1");
		pGPM->loadGroup("default");
		pGPM->loadGroupSingle("group1");
		pGPM->remove("res0", "default");
		pGPM->unloadGroup("default");
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