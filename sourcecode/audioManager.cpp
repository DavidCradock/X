#include "PCH.h"
#include "audioManager.h"
#include "log.h"

namespace X
{
	AudioSample::AudioSample(void)
	{
		buffer.pAudioData = nullptr;
	}

	AudioSample::~AudioSample(void)
	{
		unload();
	}

	void AudioSample::load(void)
	{
		// Make sure filename has non-zero size
		ThrowIfFalse(bool(_mstrAudioFilename.size() > 0), "AudioResource::load() failed as filename of the audio sample data has zero size.");
		// Open the file
		HANDLE hFile = CreateFile(
			StringToWString(_mstrAudioFilename).c_str(),
			GENERIC_READ,
			FILE_SHARE_READ,
			NULL,
			OPEN_EXISTING,
			0,
			NULL);

		ThrowIfTrue(INVALID_HANDLE_VALUE == hFile, "AudioSample::load() failed. Invalid file handle.");

		ThrowIfTrue(INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN), "AudioSample::load() failed. Invalid set file pointer.");

		// Locate the 'RIFF' chunk in the audio file, and check the file type.
		DWORD dwChunkSize;
		DWORD dwChunkPosition;
		//check the file type, should be fourccWAVE or 'XWMA'
		_findChunk(hFile, fourccRIFF, dwChunkSize, dwChunkPosition);
		DWORD filetype;
		_readChunkData(hFile, &filetype, sizeof(DWORD), dwChunkPosition);
		ThrowIfTrue(filetype != fourccWAVE, "AudioSample::load() failed. File type is not wave.");

		// Locate the 'fmt ' chunk, and copy its contents into a WAVEFORMATEXTENSIBLE structure.
		_findChunk(hFile, fourccFMT, dwChunkSize, dwChunkPosition);
		_readChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);

		// Locate the 'data' chunk, and read its contents into a buffer.
		//fill out the audio data buffer with the contents of the fourccDATA chunk
		_findChunk(hFile, fourccDATA, dwChunkSize, dwChunkPosition);
		BYTE* pDataBuffer = new BYTE[dwChunkSize];
		_readChunkData(hFile, pDataBuffer, dwChunkSize, dwChunkPosition);

		// Populate an XAUDIO2_BUFFER structure.
		buffer.AudioBytes = dwChunkSize;        // size of the audio buffer in bytes
		buffer.pAudioData = pDataBuffer;        // buffer containing audio data
		buffer.Flags = XAUDIO2_END_OF_STREAM;   // tell the source voice not to expect any data after this buffer
	}

	void AudioSample::loadFormat(void)
	{
		// Make sure filename has non-zero size
		ThrowIfFalse(bool(_mstrAudioFilename.size() > 0), "AudioResource::loadFormat() failed as filename of the audio sample data has zero size.");
		// Open the file
		HANDLE hFile = CreateFile(
			StringToWString(_mstrAudioFilename).c_str(),
			GENERIC_READ,
			FILE_SHARE_READ,
			NULL,
			OPEN_EXISTING,
			0,
			NULL);

		ThrowIfTrue(INVALID_HANDLE_VALUE == hFile, "AudioSample::loadFormat() failed. Invalid file handle.");
		ThrowIfTrue(INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN), "AudioSample::loadFormat() failed. Invalid set file pointer.");

		// Locate the 'RIFF' chunk in the audio file, and check the file type.
		DWORD dwChunkSize;
		DWORD dwChunkPosition;
		//check the file type, should be fourccWAVE or 'XWMA'
		_findChunk(hFile, fourccRIFF, dwChunkSize, dwChunkPosition);
		DWORD filetype;
		_readChunkData(hFile, &filetype, sizeof(DWORD), dwChunkPosition);
		ThrowIfTrue(filetype != fourccWAVE, "AudioSample::loadFormat() failed. File type is not wave.");

		// Locate the 'fmt ' chunk, and copy its contents into a WAVEFORMATEXTENSIBLE structure.
		_findChunk(hFile, fourccFMT, dwChunkSize, dwChunkPosition);
		_readChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
	}

	void AudioSample::unload(void)
	{
		if (buffer.pAudioData != nullptr)
		{
			delete buffer.pAudioData;
			buffer.pAudioData = nullptr;
		}
	}

	HRESULT AudioSample::_findChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition)
	{
		Log* pLog = Log::getPointer();
		HRESULT hr = S_OK;
		ThrowIfTrue(INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN), "AudioSample::_findChunk() failed. Invalid set file pointer."); //return HRESULT_FROM_WIN32(GetLastError());
		DWORD dwChunkType;
		DWORD dwChunkDataSize;
		DWORD dwRIFFDataSize = 0;
		DWORD dwFileType;
		DWORD bytesRead = 0;
		DWORD dwOffset = 0;
		while (hr == S_OK)
		{
			DWORD dwRead;
			ThrowIfTrue(0 == ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL), "AudioSample::_findChunk() failed. Unable to read file."); //hr = HRESULT_FROM_WIN32(GetLastError());

			ThrowIfTrue(0 == ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL), "AudioSample::_findChunk() failed. Unable to read file."); //hr = HRESULT_FROM_WIN32(GetLastError());

			switch (dwChunkType)
			{
			case fourccRIFF:
				dwRIFFDataSize = dwChunkDataSize;
				dwChunkDataSize = 4;
				ThrowIfTrue(0 == ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL), "AudioSample::_findChunk() failed. Unable to read file."); //hr = HRESULT_FROM_WIN32(GetLastError());
				break;
			default:
				ThrowIfTrue(INVALID_SET_FILE_POINTER == SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT), "AudioSample::_findChunk() failed. Invalid set file pointer."); //return HRESULT_FROM_WIN32(GetLastError());
			}

			dwOffset += sizeof(DWORD) * 2;

			if (dwChunkType == fourcc)
			{
				dwChunkSize = dwChunkDataSize;
				dwChunkDataPosition = dwOffset;
				return S_OK;
			}

			dwOffset += dwChunkDataSize;
			ThrowIfTrue(bytesRead >= dwRIFFDataSize, "AudioSample::_findChunk() failed. bytesRead >= dwRIFFDataSize."); //return S_FALSE;
		}
		return S_OK;
	}

	HRESULT AudioSample::_readChunkData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset)
	{
		HRESULT hr = S_OK;
		ThrowIfTrue(INVALID_SET_FILE_POINTER == SetFilePointer(hFile, bufferoffset, NULL, FILE_BEGIN), "AudioSample::_readChunkData() failed. Invalid set file pointer.");//return HRESULT_FROM_WIN32(GetLastError());
		DWORD dwRead;
		ThrowIfTrue(0 == ReadFile(hFile, buffer, buffersize, &dwRead, NULL), "AudioSample::_readChunkData() failed. Unable to read file."); //hr = HRESULT_FROM_WIN32(GetLastError());
		return hr;
	}

	AudioEmitter::AudioEmitter(WAVEFORMATEXTENSIBLE& wfx, unsigned int iMaxSimultaneousInstances)
	{
		ThrowIfTrue(0 == iMaxSimultaneousInstances, "AudioEmitter::AudioEmitter() given 0 iMaxSimultaneousInstances,");
		_muiMaxSimultaneousInstances = iMaxSimultaneousInstances;
		_muiVecVoicesIndex = 0;
		// Create voices for the audio emitter
		HRESULT hr;
		for (int iVoice = 0; iVoice < (int)iMaxSimultaneousInstances; iVoice++)
		{
			IXAudio2SourceVoice* pVoice;
			// Create a source voice by calling the IXAudio2::CreateSourceVoice method on an instance of the XAudio2 engine.
			// The format of the voice is specified by the values set in a WAVEFORMATEX structure.
			ThrowIfTrue(FAILED(hr = AudioManager::getPointer()->mpXAudio2->CreateSourceVoice(&pVoice, (WAVEFORMATEX*)&wfx)), "AudioEmitter::AudioEmitter() failed to create source voice.");
			_mvecVoices.push_back(pVoice);
		}

	}

	AudioEmitter::~AudioEmitter()
	{
		stopAll();	// Stop playback of all voices for this emitter
		for (int iVoice = 0; iVoice < _mvecVoices.size(); iVoice++)
		{
			_mvecVoices[iVoice]->DestroyVoice();
		}
	}

	void AudioEmitter::play(float fVolume, float fPlaybackSpeed, bool bLoop)
	{
		// Find the sample which this emitter uses
		AudioManager* pAudioManager = AudioManager::getPointer();
		if (!pAudioManager->getExistsSample(_mstrSampleName, _mstrSampleGroupname))
		{
			std::string strErr("AudioEmitter::play() failed as it's set sample of ");
			strErr += _mstrSampleName + " in group " + _mstrSampleGroupname + " doesn't exist in the audio manager.";
			ThrowIfTrue(1, strErr);
		}
		// Make sure the audio sample is loaded
		if (false == pAudioManager->getSampleLoaded(_mstrSampleName, _mstrSampleGroupname))
			return;

		// Before playing from the current voice index, stop it from playing incase it is so
		IXAudio2SourceVoice* pSourceVoice = _mvecVoices[_muiVecVoicesIndex];
		pSourceVoice->Stop();
		pSourceVoice->FlushSourceBuffers();

		// Get pointer to the audio sample
		AudioSample* pAudioSample = AudioManager::getPointer()->getSample(_mstrSampleName, _mstrSampleGroupname);

		// Set the sample's loop variables
		if (bLoop)	// Loop the thing?
		{
			pAudioSample->buffer.LoopBegin = 0;
			pAudioSample->buffer.LoopLength = 0;
			pAudioSample->buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
		}
		else
		{
			pAudioSample->buffer.LoopBegin = 0;
			pAudioSample->buffer.LoopLength = 0;
			pAudioSample->buffer.LoopCount = 0;
		}
		HRESULT hr;
		hr = pSourceVoice->SubmitSourceBuffer(&pAudioSample->buffer);
		ThrowIfTrue(FAILED(hr), "AudioEmitter::play() failed. Error submitting source buffer.");

		// Set voice volume, pan and frequency
		pSourceVoice->SetVolume(fVolume);
		pSourceVoice->SetFrequencyRatio(fPlaybackSpeed);

		hr = pSourceVoice->Start(0);
		ThrowIfTrue(FAILED(hr), "AudioEmitter::play() failed during call to pSourceVoice->Start()");

		// Deal with voice index
		_muiVecVoicesIndex++;
		if (_muiVecVoicesIndex >= _mvecVoices.size())
			_muiVecVoicesIndex = 0;
	}

	void AudioEmitter::stopAll(void)
	{
		HRESULT hr;
		for (int iVoice = 0; iVoice < _mvecVoices.size(); iVoice++)
		{
			hr = _mvecVoices[iVoice]->Stop();	// Stop playback of this voice
			ThrowIfTrue(FAILED(hr), "AudioEmitter::stopAll() failed. Error calling IXAudio2SourceVoice->Stop().");
			hr = _mvecVoices[iVoice]->FlushSourceBuffers();
			ThrowIfTrue(FAILED(hr), "AudioEmitter::stopAll() failed. Error calling IXAudio2SourceVoice->FlushSourceBuffers().");
		}
	}

	void AudioEmitter::stop(unsigned int uiIndex)
	{
		// Make sure valid index is given
		if (uiIndex >= _muiMaxSimultaneousInstances)
		{
			std::string strErr("AudioEmitter::stop(");
			strErr += std::to_string(uiIndex);
			strErr += ") failed as given index has to be less than ";
			strErr += std::to_string(_muiMaxSimultaneousInstances);
			ThrowIfTrue(1, strErr);
		}

		HRESULT hr;
		hr = _mvecVoices[uiIndex]->Stop();	// Stop playback of this voice
		ThrowIfTrue(FAILED(hr), "AudioEmitter::stop() failed. Error calling IXAudio2SourceVoice->Stop().");
		hr = _mvecVoices[uiIndex]->FlushSourceBuffers();
		ThrowIfTrue(FAILED(hr), "AudioEmitter::stop() failed. Error calling IXAudio2SourceVoice->FlushSourceBuffers().");
	}

	unsigned int AudioEmitter::getNumVoicesPlaying(void)
	{
		unsigned int iNumberVoicesPlaying = 0;
		XAUDIO2_VOICE_STATE state;
		for (int i = 0; i < _mvecVoices.size(); i++)
		{
			_mvecVoices[i]->GetState(&state);
			if (state.BuffersQueued > 0)
				iNumberVoicesPlaying++;
		}
		return iNumberVoicesPlaying;
	}

	void AudioEmitter::setVolume(unsigned int uiIndex, float fVolume)
	{
		// Make sure valid index is given
		if (uiIndex >= _muiMaxSimultaneousInstances)
		{
			std::string strErr("AudioEmitter::setVolume(");
			strErr += std::to_string(uiIndex);
			strErr += ") failed as given index has to be less than ";
			strErr += std::to_string(_muiMaxSimultaneousInstances);
			ThrowIfTrue(1, strErr);
		}
		_mvecVoices[uiIndex]->SetVolume(fVolume);
	}

	void AudioEmitter::setFrequency(unsigned int uiIndex, float fFrequency)
	{
		// Make sure valid index is given
		if (uiIndex >= _muiMaxSimultaneousInstances)
		{
			std::string strErr("AudioEmitter::setFrequency(");
			strErr += std::to_string(uiIndex);
			strErr += ") failed as given index has to be less than ";
			strErr += std::to_string(_muiMaxSimultaneousInstances);
			ThrowIfTrue(1, strErr);
		}
		_mvecVoices[uiIndex]->SetFrequencyRatio(fFrequency);
	}

	AudioManager::AudioManager()
	{
		Log* pLog = Log::getPointer();
		pLog->add("AudioManager constructor called.");
		pLog->add("AudioManager initialising COM.");
		HRESULT hr;
		hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
		ThrowIfTrue(FAILED(hr), "AudioManager() failed to initialise COM.");

		pLog->add("AudioManager creating XAudio2 engine instance.");
		mpXAudio2 = nullptr;
		ThrowIfTrue(FAILED(hr = XAudio2Create(&mpXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR)), "AudioManager() failed to create instance of the XAudio2 engine.");

		pLog->add("AudioManager creating mastering voice.");
		mpMasterVoice = nullptr;
		ThrowIfTrue(FAILED(hr = mpXAudio2->CreateMasteringVoice(&mpMasterVoice)), "AudioManager() failed to create mastering voice.");

		addNewSampleGroup("default");
		pLog->add("AudioManager initialisation complete.");
	}

	unsigned int AudioManager::getNumSampleGroups(void)
	{
		return (unsigned int)mmapGroup.size();
	}

	unsigned int AudioManager::getNumSamplesInGroup(const std::string& strGroupName)
	{
		if (!sampleGroupExists(strGroupName))
		{
			std::string err("AudioManager::getNumSamplesInGroup(\"");
			err.append(strGroupName);
			err.append("\") failed. The group doesn't exist.");
			ThrowIfTrue(1, err);
		}
		std::map<std::string, Group*>::iterator itg = mmapGroup.find(strGroupName);
		return (unsigned int)itg->second->mmapResource.size();;
	}

	unsigned int AudioManager::getNumSamplesInGroupLoaded(const std::string& strGroupName)
	{
		if (!sampleGroupExists(strGroupName))
		{
			std::string err("AudioManager::getNumSamplesInGroupLoaded(\"");
			err.append(strGroupName);
			err.append("\") failed. The group doesn't exist.");
			ThrowIfTrue(1, err);
		}
		std::map<std::string, Group*>::iterator itg = mmapGroup.find(strGroupName);

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

	const std::string& AudioManager::getSampleGroupName(unsigned int iGroupIndex)
	{
		if (iGroupIndex >= mmapGroup.size())
		{
			std::string err("AudioManager::getSampleGroupName(");

			err.append(std::to_string(iGroupIndex));
			err.append(") failed. Invalid index given. Number of groups is ");
			err.append(std::to_string(getNumSampleGroups()));
			ThrowIfTrue(1, err);
		}
		std::map<std::string, Group*>::iterator itg = mmapGroup.begin();
		unsigned int i = 0;
		while (i < iGroupIndex)
		{
			itg++;
			++i;
		}
		return itg->first;
	}

	void AudioManager::addNewSampleGroup(const std::string& strNewGroupName)
	{
		if (sampleGroupExists(strNewGroupName))
		{
			return;
		}
		Group* pNewGroup = new Group;
		mmapGroup[strNewGroupName] = pNewGroup;
	}

	bool AudioManager::sampleGroupExists(const std::string& strGroupName)
	{
		std::map<std::string, Group*>::iterator it = mmapGroup.find(strGroupName);
		if (it == mmapGroup.end())
			return false;
		return true;
	}

	void AudioManager::loadSampleGroup(const std::string& strGroupName)
	{
		// Group doesn't exist?
		if (!sampleGroupExists(strGroupName))
		{
			std::string err("AudioManager::loadSampleGroup(\"");
			err.append(strGroupName);
			err.append("\") failed. As the given named group doesn't exist.");
			ThrowIfTrue(1, err);
		}

		// Load any unloaded resources within the group
		std::map<std::string, Group*>::iterator itg = mmapGroup.find(strGroupName);

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

	void AudioManager::loadSampleGroupSingle(const std::string& strGroupName)
	{
		// Group doesn't exist?
		if (!sampleGroupExists(strGroupName))
		{
			std::string err("AudioManager::loadSampleGroupSingle(\"");
			err.append(strGroupName);
			err.append("\") failed. As the given named group doesn't exist.");
			ThrowIfTrue(1, err);
		}

		// Load any unloaded resources within the group
		std::map<std::string, Group*>::iterator itg = mmapGroup.find(strGroupName);

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

	void AudioManager::unloadSampleGroup(const std::string& strGroupName)
	{
		// Group doesn't exist?
		if (!sampleGroupExists(strGroupName))
		{
			std::string err("AudioManager::unloadSampleGroup(\"");
			err.append(strGroupName);
			err.append("\") failed. As the given named group doesn't exist.");
			ThrowIfTrue(1, err);
		}

		// Unload any loaded resources within the group
		std::map<std::string, Group*>::iterator itg = mmapGroup.find(strGroupName);

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

	AudioSample* AudioManager::addSample(const std::string& strNewResourceName, const std::string& strGroupName)
	{
		// Group doesn't exist?
		if (!sampleGroupExists(strGroupName))
		{
			std::string err("AudioManager::addSample(\"");
			err.append(strNewResourceName);
			err.append("\", \"");
			err.append(strGroupName);
			err.append("\") failed. As the given named group of \"");
			err.append(strGroupName);
			err.append("\" which the new resource was to be placed into, doesn't exist.");
			ThrowIfTrue(1, err);
		}

		// Resource already exists in the group?
		std::map<std::string, Group*>::iterator itg = mmapGroup.find(strGroupName);								// Get iterator to the group (we know it exists)
		std::map<std::string, ResourceSample*>::iterator it = itg->second->mmapResource.find(strNewResourceName);		// Try to find the named resource in the group
		if (itg->second->mmapResource.end() != it)
		{
			it->second->uiReferenceCount++;
			return it->second->pResource;
		}

		// If we get here, we have got to create, then add the resource to the existing named group
		ResourceSample* pNewRes = new ResourceSample;
		pNewRes->pResource = new AudioSample();
		pNewRes->pResource->_mstrAudioFilename = strNewResourceName;
		pNewRes->uiReferenceCount = 1;
		pNewRes->bLoaded = false;
		itg->second->mmapResource[strNewResourceName] = pNewRes;

		// Load in the wave format structure, not the whole sample data.
		// We do this so we can add emitters without having to load the entire sample data in first.
		pNewRes->pResource->loadFormat();

		return pNewRes->pResource;
	}

	AudioSample* AudioManager::getSample(const std::string& strResourceName, const std::string& strGroupName)
	{
		// Group doesn't exist?
		if (!sampleGroupExists(strGroupName))
		{
			std::string err("AudioManager::getSample(\"");
			err.append(strResourceName);
			err.append("\", \"");
			err.append(strGroupName);
			err.append("\") failed. As the given named group of \"");
			err.append(strGroupName);
			err.append("\" doesn't exist.");
			ThrowIfTrue(1, err);
		}

		// Resource doesn't exist in the group?
		std::map<std::string, Group*>::iterator itg = mmapGroup.find(strGroupName);								// Get iterator to the group (we know it exists)
		std::map<std::string, ResourceSample*>::iterator it = itg->second->mmapResource.find(strResourceName);	// Try to find the named resource in the group
		if (itg->second->mmapResource.end() == it)
		{
			std::string err("AudioManager::getSample(\"");
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

	bool AudioManager::getExistsSample(const std::string& strResourceName, const std::string& strGroupName)
	{
		std::map<std::string, Group*>::iterator itg = mmapGroup.find(strGroupName);
		if (itg == mmapGroup.end())
			return false;
		std::map<std::string, ResourceSample*>::iterator it = itg->second->mmapResource.find(strResourceName);
		if (it == itg->second->mmapResource.end())
			return false;
		return true;
	}

	bool AudioManager::getSampleLoaded(const std::string& strResourceName, const std::string& strGroupName)
	{
		std::map<std::string, Group*>::iterator itg = mmapGroup.find(strGroupName);
		if (itg == mmapGroup.end())
			return false;
		std::map<std::string, ResourceSample*>::iterator it = itg->second->mmapResource.find(strResourceName);
		if (it == itg->second->mmapResource.end())
			return false;
		return it->second->bLoaded;
	}

	void AudioManager::removeSample(const std::string& strResourceName, const std::string& strGroupName)
	{
		// Group doesn't exist?
		if (!sampleGroupExists(strGroupName))
		{
			std::string err("AudioManager::removeSample(\"");
			err.append(strResourceName);
			err.append("\", \"");
			err.append(strGroupName);
			err.append("\") failed. As the given named group of \"");
			err.append(strGroupName);
			err.append("\" which the resource is supposed to be in, doesn't exist.");
			ThrowIfTrue(1, err);
		}

		// Resource doesn't exist in the group?
		std::map<std::string, Group*>::iterator itg = mmapGroup.find(strGroupName);								// Get iterator to the group (we know it exists)
		std::map<std::string, ResourceSample*>::iterator it = itg->second->mmapResource.find(strResourceName);	// Try to find the named resource in the group
		if (itg->second->mmapResource.end() == it)
		{
			std::string err("AudioManager::removeSample(\"");
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

	unsigned int AudioManager::getMemoryUsage(void)
	{
		XAUDIO2_PERFORMANCE_DATA pPerformanceData;// = 0;
		mpXAudio2->GetPerformanceData(&pPerformanceData);
		return pPerformanceData.MemoryUsageInBytes;
	}

	AudioEmitter* AudioManager::addEmitter(const std::string& strEmitterName, const std::string& strSampleName, unsigned int iMaxSimultaneousInstances, const std::string& strSampleGroupname)
	{
		// Make sure the named sample exists
		std::string strErr("AudioManager::addEmitter(");
		strErr += strEmitterName + ", " + strSampleName + ", " + std::to_string(iMaxSimultaneousInstances) + ", " + strSampleGroupname + ") ";
		// Make sure the named group exists
		std::map<std::string, Group*>::iterator itg = mmapGroup.find(strSampleGroupname);
		if (itg == mmapGroup.end())
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
		// Make sure the emitter doesn't currently exist
		if (getExistsEmitter(strEmitterName))
		{
			std::string strErr("AudioManager::addEmitter() failed as the named of emitter of ");
			strErr += strEmitterName;
			strErr += " already exists.";
			ThrowIfTrue(1, strErr);
		}
		// Create the new emitter object
		AudioEmitter* pNewEmitter = new AudioEmitter(it->second->pResource->wfx, iMaxSimultaneousInstances);
		ThrowIfFalse(pNewEmitter, "AudioManager::addEmitter() failed to allocate memory for the new emitter");

		// Store the sample's name and group within the emitter as emitters only work with their set sample data
		pNewEmitter->_mstrSampleName = strSampleName;
		pNewEmitter->_mstrSampleGroupname = strSampleGroupname;

		// Store it in the hash map
		_mmapEmitters[strEmitterName] = pNewEmitter;
		return pNewEmitter;
	}

	void AudioManager::removeEmitter(const std::string& strEmitterName)
	{
		std::map<std::string, AudioEmitter*>::iterator it = _mmapEmitters.find(strEmitterName);
		if (it == _mmapEmitters.end())
		{
			std::string strErr("AudioManager::removeEmitter(");
			strErr += strEmitterName;
			strErr += ") failed as the named emitter doesn't exist.";
			ThrowIfTrue(1, strErr);
		}
		delete it->second;
		_mmapEmitters.erase(it);
	}

	bool AudioManager::getExistsEmitter(const std::string& strEmitterName)
	{
		std::map<std::string, AudioEmitter*>::iterator it = _mmapEmitters.find(strEmitterName);
		if (it == _mmapEmitters.end())
			return false;
		return true;
	}

	AudioEmitter* AudioManager::getEmitter(const std::string& strEmitterName)
	{
		std::map<std::string, AudioEmitter*>::iterator it = _mmapEmitters.find(strEmitterName);
		if (it == _mmapEmitters.end())
		{
			std::string strErr("AudioManager::getEmitter(");
			strErr += strEmitterName;
			strErr += ") failed as the named emitter doesn't exist.";
			ThrowIfTrue(1, strErr);
		}
		return it->second;
	}
}