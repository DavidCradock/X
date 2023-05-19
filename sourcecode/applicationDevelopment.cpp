#include "PCH.h"
#include "applicationDevelopment.h"
#include "audioManager.h"
#include "log.h"
#include "utilities.h"

namespace X
{
	void ApplicationDevelopment::initOnce(void)
	{
		AudioManager* pAudioManager = AudioManager::getPointer();
		pAudioManager->addSample("audio/drone_loops/Falcon_Volkm_Beauty_120BPM.wav");
		pAudioManager->addSample("audio/drone_loops/Falcon_Volkm_Density_120BPM.wav");
		pAudioManager->removeAllSamples();

		Log* pLog = Log::getPointer();
		std::string strCurrentDir = "X:\\media\\Audio";// getCurrentDirectory();
		pLog->add("CurrentDirectory: " + strCurrentDir);
		pLog->add("Files in current directory...");
		std::string fileExtension = "wav";
		std::vector<std::string> files = getFilesInDir(strCurrentDir, fileExtension, true);
		for (int i = 0; i < files.size(); ++i)
		{
			pLog->add(files[i]);
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