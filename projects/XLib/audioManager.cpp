#include "PCH.h"
#include "audioManager.h"
#include "stringUtils.h"
#include "singletons.h"
#include "logging.h"
#include "resourceLoadingScreen.h"

namespace X
{
	SCAudioManager::SCAudioManager()
	{
		SCLog* pLog = SCLog::getPointer();
		pLog->add("SCAudioManager::SCAudioManager() called.");

		pLog->add("SCAudioManager initialising COM.");
		HRESULT hr;
		hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
		ThrowIfTrue(FAILED(hr), "SCAudioManager() failed to initialise COM.");

		pLog->add("SCAudioManager creating XAudio2 engine instance.");
		_mpXAudio2 = nullptr;
		ThrowIfTrue(FAILED(hr = XAudio2Create(&_mpXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR)), "SCAudioManager() failed to create instance of the XAudio2 engine.");

		pLog->add("SCAudioManager creating mastering voice.");
		_mpMasterVoice = nullptr;
		ThrowIfTrue(FAILED(hr = _mpXAudio2->CreateMasteringVoice(&_mpMasterVoice)), "SCAudioManager() failed to create mastering voice.");

		addNewSampleGroup("default");
		pLog->add("SCAudioManager initialisation complete.");
	}

	unsigned int SCAudioManager::getNumSampleGroups(void)
	{
		return (unsigned int)_mmapGroup.size();
	}

	unsigned int SCAudioManager::getNumSamplesInGroup(const std::string& strGroupName)
	{
		if (!sampleGroupExists(strGroupName))
		{
			std::string err("SCAudioManager::getNumSamplesInGroup(\"");
			err.append(strGroupName);
			err.append("\") failed. The group doesn't exist.");
			ThrowIfTrue(1, err);
		}
		std::map<std::string, Group*>::iterator itg = _mmapGroup.find(strGroupName);
		return (unsigned int)itg->second->mmapResource.size();;
	}

	unsigned int SCAudioManager::getNumSamplesInGroupLoaded(const std::string& strGroupName)
	{
		if (!sampleGroupExists(strGroupName))
		{
			std::string err("SCAudioManager::getNumSamplesInGroupLoaded(\"");
			err.append(strGroupName);
			err.append("\") failed. The group doesn't exist.");
			ThrowIfTrue(1, err);
		}
		std::map<std::string, Group*>::iterator itg = _mmapGroup.find(strGroupName);

		unsigned int iResLoadedTotal = 0;
		std::map<std::string, ResourceSample*>::iterator it = itg->second->mmapResource.begin();
		while (it != itg->second->mmapResource.end())
		{
			if (it->second->bLoaded)
				++iResLoadedTotal;
			it++;
		}
		return iResLoadedTotal;
	}

	const std::string& SCAudioManager::getSampleGroupName(unsigned int iGroupIndex)
	{
		if (iGroupIndex >= _mmapGroup.size())
		{
			std::string err("SCAudioManager::getSampleGroupName(");

			err.append(std::to_string(iGroupIndex));
			err.append(") failed. Invalid index given. Number of groups is ");
			err.append(std::to_string(getNumSampleGroups()));
			ThrowIfTrue(1, err);
		}
		std::map<std::string, Group*>::iterator itg = _mmapGroup.begin();
		unsigned int i = 0;
		while (i < iGroupIndex)
		{
			itg++;
			++i;
		}
		return itg->first;
	}

	void SCAudioManager::addNewSampleGroup(const std::string& strNewGroupName)
	{
		if (sampleGroupExists(strNewGroupName))
		{
			return;
		}
		Group* pNewGroup = new Group;
		_mmapGroup[strNewGroupName] = pNewGroup;
	}

	bool SCAudioManager::sampleGroupExists(const std::string& strGroupName)
	{
		std::map<std::string, Group*>::iterator it = _mmapGroup.find(strGroupName);
		if (it == _mmapGroup.end())
			return false;
		return true;
	}

	void SCAudioManager::loadSampleGroup(const std::string& strGroupName)
	{
		// Group doesn't exist?
		if (!sampleGroupExists(strGroupName))
		{
			std::string err("SCAudioManager::loadSampleGroup(\"");
			err.append(strGroupName);
			err.append("\") failed. As the given named group doesn't exist.");
			ThrowIfTrue(1, err);
		}

		// Load any unloaded resources within the group
		std::map<std::string, Group*>::iterator itg = _mmapGroup.find(strGroupName);

		// For each resource in this group...
		std::map<std::string, ResourceSample*>::iterator it = itg->second->mmapResource.begin();
		while (it != itg->second->mmapResource.end())
		{
			if (!it->second->bLoaded)
			{
				it->second->pResource->load();
				it->second->bLoaded = true;
			}
			it++;
		}
	}

	void SCAudioManager::loadSampleGroupSingle(const std::string& strGroupName)
	{
		// Group doesn't exist?
		if (!sampleGroupExists(strGroupName))
		{
			std::string err("SCAudioManager::loadSampleGroupSingle(\"");
			err.append(strGroupName);
			err.append("\") failed. As the given named group doesn't exist.");
			ThrowIfTrue(1, err);
		}

		// Load any unloaded resources within the group
		std::map<std::string, Group*>::iterator itg = _mmapGroup.find(strGroupName);

		// For each resource in this group...
		std::map<std::string, ResourceSample*>::iterator it = itg->second->mmapResource.begin();
		while (it != itg->second->mmapResource.end())
		{
			if (!it->second->bLoaded)
			{
				it->second->pResource->load();
				it->second->bLoaded = true;
				return;	// We've changed a resource from unloaded to loaded state, aka, we've loaded a resource, simply return.
			}
			it++;
		}
	}

	void SCAudioManager::unloadSampleGroup(const std::string& strGroupName)
	{
		// Group doesn't exist?
		if (!sampleGroupExists(strGroupName))
		{
			std::string err("SCAudioManager::unloadSampleGroup(\"");
			err.append(strGroupName);
			err.append("\") failed. As the given named group doesn't exist.");
			ThrowIfTrue(1, err);
		}

		// Unload any loaded resources within the group
		std::map<std::string, Group*>::iterator itg = _mmapGroup.find(strGroupName);

		// For each resource in this group...
		std::map<std::string, ResourceSample*>::iterator it = itg->second->mmapResource.begin();
		while (it != itg->second->mmapResource.end())
		{
			if (it->second->bLoaded)
			{
				it->second->pResource->unload();
				it->second->bLoaded = false;
			}
			it++;
		}
	}

	CAudioSample* SCAudioManager::addSample(const std::string& strNewResourceName, const std::string& strGroupName)
	{
		// Call loading screen
		SCResourceLoadingScreen::getPointer()->loadingResource("Audio sample", strNewResourceName);

		// Group doesn't exist?
		if (!sampleGroupExists(strGroupName))
		{
			std::string err("SCAudioManager::addSample(\"");
			err.append(strNewResourceName);
			err.append("\", \"");
			err.append(strGroupName);
			err.append("\") failed. As the given named group of \"");
			err.append(strGroupName);
			err.append("\" which the new resource was to be placed into, doesn't exist.");
			ThrowIfTrue(1, err);
		}

		// Resource already exists in the group?
		std::map<std::string, Group*>::iterator itg = _mmapGroup.find(strGroupName);									// Get iterator to the group (we know it exists)
		std::map<std::string, ResourceSample*>::iterator it = itg->second->mmapResource.find(strNewResourceName);		// Try to find the named resource in the group
		if (itg->second->mmapResource.end() != it)
		{
			it->second->uiReferenceCount++;
			return it->second->pResource;
		}

		// If we get here, we have got to create, then add the resource to the existing named group
		ResourceSample* pNewRes = new ResourceSample;
		pNewRes->pResource = new CAudioSample();
		pNewRes->pResource->_mstrAudioFilename = strNewResourceName;
		pNewRes->uiReferenceCount = 1;
		pNewRes->bLoaded = false;
		itg->second->mmapResource[strNewResourceName] = pNewRes;

		// Load in the wave format structure, not the whole sample data.
		// We do this so we can add emitters without having to load the entire sample data in first.
		pNewRes->pResource->loadFormat();

		return pNewRes->pResource;
	}

	CAudioSample* SCAudioManager::getSample(const std::string& strResourceName, const std::string& strGroupName)
	{
		// Group doesn't exist?
		if (!sampleGroupExists(strGroupName))
		{
			std::string err("SCAudioManager::getSample(\"");
			err.append(strResourceName);
			err.append("\", \"");
			err.append(strGroupName);
			err.append("\") failed. As the given named group of \"");
			err.append(strGroupName);
			err.append("\" doesn't exist.");
			ThrowIfTrue(1, err);
		}

		// Resource doesn't exist in the group?
		std::map<std::string, Group*>::iterator itg = _mmapGroup.find(strGroupName);							// Get iterator to the group (we know it exists)
		std::map<std::string, ResourceSample*>::iterator it = itg->second->mmapResource.find(strResourceName);	// Try to find the named resource in the group
		if (itg->second->mmapResource.end() == it)
		{
			std::string err("SCAudioManager::getSample(\"");
			err.append(strResourceName);
			err.append("\", \"");
			err.append(strGroupName);
			err.append("\") failed. Although the given named group of \"");
			err.append(strGroupName);
			err.append("\" exists, the named resource couldn't be found.");
			ThrowIfTrue(1, err);
		}

		// Is the resource in an unloaded state?
		if (!it->second->bLoaded)
		{
			// Load it
			it->second->pResource->load();
			it->second->bLoaded = true;
		}
		// Return the resource pointer...
		return it->second->pResource;
	}

	bool SCAudioManager::getExistsSample(const std::string& strResourceName, const std::string& strGroupName)
	{
		std::map<std::string, Group*>::iterator itg = _mmapGroup.find(strGroupName);
		if (itg == _mmapGroup.end())
			return false;
		std::map<std::string, ResourceSample*>::iterator it = itg->second->mmapResource.find(strResourceName);
		if (it == itg->second->mmapResource.end())
			return false;
		return true;
	}

	bool SCAudioManager::getSampleLoaded(const std::string& strResourceName, const std::string& strGroupName)
	{
		std::map<std::string, Group*>::iterator itg = _mmapGroup.find(strGroupName);
		if (itg == _mmapGroup.end())
			return false;
		std::map<std::string, ResourceSample*>::iterator it = itg->second->mmapResource.find(strResourceName);
		if (it == itg->second->mmapResource.end())
			return false;
		return it->second->bLoaded;
	}

	void SCAudioManager::removeSample(const std::string& strResourceName, const std::string& strGroupName)
	{
		// Group doesn't exist?
		if (!sampleGroupExists(strGroupName))
		{
			std::string err("SCAudioManager::removeSample(\"");
			err.append(strResourceName);
			err.append("\", \"");
			err.append(strGroupName);
			err.append("\") failed. As the given named group of \"");
			err.append(strGroupName);
			err.append("\" which the resource is supposed to be in, doesn't exist.");
			ThrowIfTrue(1, err);
		}

		// Resource doesn't exist in the group?
		std::map<std::string, Group*>::iterator itg = _mmapGroup.find(strGroupName);							// Get iterator to the group (we know it exists)
		std::map<std::string, ResourceSample*>::iterator it = itg->second->mmapResource.find(strResourceName);	// Try to find the named resource in the group
		if (itg->second->mmapResource.end() == it)
		{
			std::string err("SCAudioManager::removeSample(\"");
			err.append(strResourceName);
			err.append("\", \"");
			err.append(strGroupName);
			err.append("\") failed. Although the given named group of \"");
			err.append(strGroupName);
			err.append("\" which the resource is supposed to be in, exists, the named resource couldn't be found.");
			ThrowIfTrue(1, err);
		}

		// If we get here, we've found the resource in the group
		// Decrement it's reference count 
		it->second->uiReferenceCount--;
		// If the reference count is now at zero
		if (it->second->uiReferenceCount <= 0)
		{
			// If it's in a loaded state, unload it
			if (it->second->bLoaded)
			{
				it->second->pResource->unload();
				it->second->bLoaded = false;
			}

			// Destroy the resource
			delete it->second->pResource;
			delete it->second;
			itg->second->mmapResource.erase(it);
		}
	}

	unsigned int SCAudioManager::getMemoryUsage(void)
	{
		XAUDIO2_PERFORMANCE_DATA pPerformanceData;// = 0;
		_mpXAudio2->GetPerformanceData(&pPerformanceData);
		return pPerformanceData.MemoryUsageInBytes;
	}

	CAudioEmitter* SCAudioManager::addEmitter(const std::string& strEmitterName, const std::string& strSampleName, unsigned int iMaxSimultaneousInstances, const std::string& strSampleGroupname)
	{
		std::string strErr("SCAudioManager::addEmitter(");
		strErr += strEmitterName + ", " + strSampleName + ", " + std::to_string(iMaxSimultaneousInstances) + ", " + strSampleGroupname + ") ";
		// Make sure the named group exists
		std::map<std::string, Group*>::iterator itg = _mmapGroup.find(strSampleGroupname);
		if (itg == _mmapGroup.end())
		{
			strErr += "failed as the named group doesn't exist.";
			ThrowIfTrue(1, strErr);
		}
		// Make sure the named sample exists
		std::map<std::string, ResourceSample*>::iterator it = itg->second->mmapResource.find(strSampleName);
		if (it == itg->second->mmapResource.end())
		{
			strErr += "failed as although the named group exists, the named sample doesn't.";
			ThrowIfTrue(1, strErr);
		}
		// If we get here, the sample was found in the named group.

		auto itEmitter = _mmapResourceEmitters.find(strEmitterName);
		CAudioEmitter* pEmitter = 0;
		if (itEmitter != _mmapResourceEmitters.end())
		{
			itEmitter->second->uiReferenceCount++;
			pEmitter = itEmitter->second->pResource;
		}
		else
		{
			// Create new resource
			ResourceEmitter* pNewResEmitter = new ResourceEmitter;
			ThrowIfMemoryNotAllocated(pNewResEmitter);

			// Create the new emitter object
			pNewResEmitter->uiReferenceCount = 1;
			pNewResEmitter->pResource = new CAudioEmitter(it->second->pResource->wfx, iMaxSimultaneousInstances);
			ThrowIfMemoryNotAllocated(pNewResEmitter->pResource);
			pEmitter = pNewResEmitter->pResource;	// Store pointer so we can return it below.

			// Store the sample's name and group within the emitter as emitters only work with their set sample data
			pNewResEmitter->pResource->_mstrSampleName = strSampleName;
			pNewResEmitter->pResource->_mstrSampleGroupname = strSampleGroupname;

			// Store it in the hash map
			_mmapResourceEmitters[strEmitterName] = pNewResEmitter;
		}
		return pEmitter;
	}

	void SCAudioManager::removeEmitter(const std::string& strEmitterName)
	{
		auto it = _mmapResourceEmitters.find(strEmitterName);
		if (it == _mmapResourceEmitters.end())
		{
			std::string strErr("SCAudioManager::removeEmitter(");
			strErr += strEmitterName;
			strErr += ") failed as the named emitter doesn't exist.";
			ThrowIfTrue(1, strErr);
		}
		it->second->uiReferenceCount--;
		if (it->second->uiReferenceCount == 0)
		{
			delete it->second->pResource;
			delete it->second;
			_mmapResourceEmitters.erase(it);
		}
	}

	bool SCAudioManager::getExistsEmitter(const std::string& strEmitterName)
	{
		auto it = _mmapResourceEmitters.find(strEmitterName);
		if (it == _mmapResourceEmitters.end())
			return false;
		return true;
	}

	CAudioEmitter* SCAudioManager::getEmitter(const std::string& strEmitterName)
	{
		auto it = _mmapResourceEmitters.find(strEmitterName);
		if (it == _mmapResourceEmitters.end())
		{
			std::string strErr("SCAudioManager::getEmitter(");
			strErr += strEmitterName;
			strErr += ") failed as the named emitter doesn't exist.";
			ThrowIfTrue(1, strErr);
		}
		return it->second->pResource;
	}
}