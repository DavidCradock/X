#include "PCH.h"
#include "audioManager.h"
#include "stringUtils.h"
#include "singletons.h"

namespace X
{
	CAudioSample::CAudioSample(void)
	{
		buffer.pAudioData = nullptr;
	}

	CAudioSample::~CAudioSample(void)
	{
		unload();
	}

	void CAudioSample::load(void)
	{
		// Make sure filename has non-zero size
		ThrowIfFalse(bool(_mstrAudioFilename.size() > 0), "CAudioSample::load() failed as filename of the audio sample data has zero size.");
		// Open the file
		HANDLE hFile = CreateFile(
			StringUtils::stringToWide(_mstrAudioFilename).c_str(),
			GENERIC_READ,
			FILE_SHARE_READ,
			NULL,
			OPEN_EXISTING,
			0,
			NULL);

		ThrowIfTrue(INVALID_HANDLE_VALUE == hFile, "CAudioSample::load() failed. Invalid file handle.");

		ThrowIfTrue(INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN), "CAudioSample::load() failed. Invalid set file pointer.");

		// Locate the 'RIFF' chunk in the audio file, and check the file type.
		DWORD dwChunkSize;
		DWORD dwChunkPosition;
		//check the file type, should be fourccWAVE or 'XWMA'
		_findChunk(hFile, fourccRIFF, dwChunkSize, dwChunkPosition);
		DWORD filetype;
		_readChunkData(hFile, &filetype, sizeof(DWORD), dwChunkPosition);
		ThrowIfTrue(filetype != fourccWAVE, "CAudioSample::load() failed. File type is not wave.");

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

	void CAudioSample::loadFormat(void)
	{
		// Make sure filename has non-zero size
		ThrowIfFalse(bool(_mstrAudioFilename.size() > 0), "CAudioSample::loadFormat() failed as filename of the audio sample data has zero size.");
		// Open the file
		HANDLE hFile = CreateFile(
			StringUtils::stringToWide(_mstrAudioFilename).c_str(),
			GENERIC_READ,
			FILE_SHARE_READ,
			NULL,
			OPEN_EXISTING,
			0,
			NULL);

		ThrowIfTrue(INVALID_HANDLE_VALUE == hFile, "CAudioSample::loadFormat() failed. Invalid file handle.");
		ThrowIfTrue(INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN), "CAudioSample::loadFormat() failed. Invalid set file pointer.");

		// Locate the 'RIFF' chunk in the audio file, and check the file type.
		DWORD dwChunkSize;
		DWORD dwChunkPosition;
		//check the file type, should be fourccWAVE or 'XWMA'
		_findChunk(hFile, fourccRIFF, dwChunkSize, dwChunkPosition);
		DWORD filetype;
		_readChunkData(hFile, &filetype, sizeof(DWORD), dwChunkPosition);
		ThrowIfTrue(filetype != fourccWAVE, "CAudioSample::loadFormat() failed. File type is not wave.");

		// Locate the 'fmt ' chunk, and copy its contents into a WAVEFORMATEXTENSIBLE structure.
		_findChunk(hFile, fourccFMT, dwChunkSize, dwChunkPosition);
		_readChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
	}

	void CAudioSample::unload(void)
	{
		if (buffer.pAudioData != nullptr)
		{
			delete buffer.pAudioData;
			buffer.pAudioData = nullptr;
		}
	}

	HRESULT CAudioSample::_findChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition)
	{
		HRESULT hr = S_OK;
		ThrowIfTrue(INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN), "CAudioSample::_findChunk() failed. Invalid set file pointer."); //return HRESULT_FROM_WIN32(GetLastError());
		DWORD dwChunkType;
		DWORD dwChunkDataSize;
		DWORD dwRIFFDataSize = 0;
		DWORD dwFileType;
		DWORD bytesRead = 0;
		DWORD dwOffset = 0;
		while (hr == S_OK)
		{
			DWORD dwRead;
			ThrowIfTrue(0 == ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL), "CAudioSample::_findChunk() failed. Unable to read file."); //hr = HRESULT_FROM_WIN32(GetLastError());

			ThrowIfTrue(0 == ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL), "CAudioSample::_findChunk() failed. Unable to read file."); //hr = HRESULT_FROM_WIN32(GetLastError());

			switch (dwChunkType)
			{
			case fourccRIFF:
				dwRIFFDataSize = dwChunkDataSize;
				dwChunkDataSize = 4;
				ThrowIfTrue(0 == ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL), "CAudioSample::_findChunk() failed. Unable to read file."); //hr = HRESULT_FROM_WIN32(GetLastError());
				break;
			default:
				ThrowIfTrue(INVALID_SET_FILE_POINTER == SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT), "CAudioSample::_findChunk() failed. Invalid set file pointer."); //return HRESULT_FROM_WIN32(GetLastError());
			}

			dwOffset += sizeof(DWORD) * 2;

			if (dwChunkType == fourcc)
			{
				dwChunkSize = dwChunkDataSize;
				dwChunkDataPosition = dwOffset;
				return S_OK;
			}

			dwOffset += dwChunkDataSize;
			ThrowIfTrue(bytesRead >= dwRIFFDataSize, "CAudioSample::_findChunk() failed. bytesRead >= dwRIFFDataSize."); //return S_FALSE;
		}
		return S_OK;
	}

	HRESULT CAudioSample::_readChunkData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset)
	{
		HRESULT hr = S_OK;
		ThrowIfTrue(INVALID_SET_FILE_POINTER == SetFilePointer(hFile, bufferoffset, NULL, FILE_BEGIN), "CAudioSample::_readChunkData() failed. Invalid set file pointer.");//return HRESULT_FROM_WIN32(GetLastError());
		DWORD dwRead;
		ThrowIfTrue(0 == ReadFile(hFile, buffer, buffersize, &dwRead, NULL), "CAudioSample::_readChunkData() failed. Unable to read file."); //hr = HRESULT_FROM_WIN32(GetLastError());
		return hr;
	}

	CAudioEmitter::CAudioEmitter(WAVEFORMATEXTENSIBLE& wfx, unsigned int iMaxSimultaneousInstances)
	{
		ThrowIfTrue(0 == iMaxSimultaneousInstances, "CAudioEmitter::CAudioEmitter() given 0 iMaxSimultaneousInstances,");
		_muiMaxSimultaneousInstances = iMaxSimultaneousInstances;
		_muiVecVoicesIndex = 0;
		// Create voices for the audio emitter
		HRESULT hr;
		for (int iVoice = 0; iVoice < (int)iMaxSimultaneousInstances; iVoice++)
		{
			IXAudio2SourceVoice* pVoice;
			// Create a source voice by calling the IXAudio2::CreateSourceVoice method on an instance of the XAudio2 engine.
			// The format of the voice is specified by the values set in a WAVEFORMATEX structure.
			ThrowIfTrue(FAILED(hr = SCAudioManager::getPointer()->_mpXAudio2->CreateSourceVoice(&pVoice, (WAVEFORMATEX*)&wfx)), "CAudioEmitter::CAudioEmitter() failed to create source voice.");
			_mvecVoices.push_back(pVoice);
		}

	}

	CAudioEmitter::~CAudioEmitter()
	{
		stopAll();	// Stop playback of all voices for this emitter
		for (int iVoice = 0; iVoice < _mvecVoices.size(); iVoice++)
		{
			_mvecVoices[iVoice]->DestroyVoice();
		}
	}

	void CAudioEmitter::play(float fVolume, float fPlaybackSpeed, bool bLoop)
	{
		// Find the sample which this emitter uses
		if (!x->pAudio->getExistsSample(_mstrSampleName, _mstrSampleGroupname))
		{
			std::string strErr("CAudioEmitter::play() failed as it's set sample of ");
			strErr += _mstrSampleName + " in group " + _mstrSampleGroupname + " doesn't exist in the audio manager.";
			ThrowIfTrue(1, strErr);
		}
		// Make sure the audio sample is loaded
		if (false == x->pAudio->getSampleLoaded(_mstrSampleName, _mstrSampleGroupname))
			return;

		// Before playing from the current voice index, stop it from playing incase it is so
		IXAudio2SourceVoice* pSourceVoice = _mvecVoices[_muiVecVoicesIndex];
		pSourceVoice->Stop();
		pSourceVoice->FlushSourceBuffers();

		// Get pointer to the audio sample
		CAudioSample* pAudioSample = x->pAudio->getSample(_mstrSampleName, _mstrSampleGroupname);

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
		ThrowIfTrue(FAILED(hr), "CAudioEmitter::play() failed. Error submitting source buffer.");

		// Set voice volume, pan and frequency
		pSourceVoice->SetVolume(fVolume);
		pSourceVoice->SetFrequencyRatio(fPlaybackSpeed);

		hr = pSourceVoice->Start(0);
		ThrowIfTrue(FAILED(hr), "CAudioEmitter::play() failed during call to pSourceVoice->Start()");

		// Deal with voice index
		_muiVecVoicesIndex++;
		if (_muiVecVoicesIndex >= _mvecVoices.size())
			_muiVecVoicesIndex = 0;
	}

	void CAudioEmitter::stopAll(void)
	{
		HRESULT hr;
		for (int iVoice = 0; iVoice < _mvecVoices.size(); iVoice++)
		{
			hr = _mvecVoices[iVoice]->Stop();	// Stop playback of this voice
			ThrowIfTrue(FAILED(hr), "CAudioEmitter::stopAll() failed. Error calling IXAudio2SourceVoice->Stop().");
			hr = _mvecVoices[iVoice]->FlushSourceBuffers();
			ThrowIfTrue(FAILED(hr), "CAudioEmitter::stopAll() failed. Error calling IXAudio2SourceVoice->FlushSourceBuffers().");
		}
	}

	void CAudioEmitter::stop(unsigned int uiIndex)
	{
		// Make sure valid index is given
		if (uiIndex >= _muiMaxSimultaneousInstances)
		{
			std::string strErr("CAudioEmitter::stop(");
			strErr += std::to_string(uiIndex);
			strErr += ") failed as given index has to be less than ";
			strErr += std::to_string(_muiMaxSimultaneousInstances);
			ThrowIfTrue(1, strErr);
		}

		HRESULT hr;
		hr = _mvecVoices[uiIndex]->Stop();	// Stop playback of this voice
		ThrowIfTrue(FAILED(hr), "CAudioEmitter::stop() failed. Error calling IXAudio2SourceVoice->Stop().");
		hr = _mvecVoices[uiIndex]->FlushSourceBuffers();
		ThrowIfTrue(FAILED(hr), "CAudioEmitter::stop() failed. Error calling IXAudio2SourceVoice->FlushSourceBuffers().");
	}

	unsigned int CAudioEmitter::getNumVoicesPlaying(void) const
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

	void CAudioEmitter::setVolume(unsigned int uiIndex, float fVolume)
	{
		// Make sure valid index is given
		if (uiIndex >= _muiMaxSimultaneousInstances)
		{
			std::string strErr("CAudioEmitter::setVolume(");
			strErr += std::to_string(uiIndex);
			strErr += ") failed as given index has to be less than ";
			strErr += std::to_string(_muiMaxSimultaneousInstances);
			ThrowIfTrue(1, strErr);
		}
		_mvecVoices[uiIndex]->SetVolume(fVolume);
	}

	float CAudioEmitter::getVolume(unsigned int uiIndex) const
	{
		// Make sure valid index is given
		if (uiIndex >= _muiMaxSimultaneousInstances)
		{
			std::string strErr("CAudioEmitter::getVolume(");
			strErr += std::to_string(uiIndex);
			strErr += ") failed as given index has to be less than ";
			strErr += std::to_string(_muiMaxSimultaneousInstances);
			ThrowIfTrue(1, strErr);
		}
		float fVolume = 0.0f;
		_mvecVoices[uiIndex]->GetVolume(&fVolume);
		return fVolume;
	}

	void CAudioEmitter::setFrequency(unsigned int uiIndex, float fFrequency)
	{
		// Make sure valid index is given
		if (uiIndex >= _muiMaxSimultaneousInstances)
		{
			std::string strErr("CAudioEmitter::setFrequency(");
			strErr += std::to_string(uiIndex);
			strErr += ") failed as given index has to be less than ";
			strErr += std::to_string(_muiMaxSimultaneousInstances);
			ThrowIfTrue(1, strErr);
		}
		_mvecVoices[uiIndex]->SetFrequencyRatio(fFrequency);
	}

	float CAudioEmitter::getFrequency(unsigned int uiIndex) const
	{
		// Make sure valid index is given
		if (uiIndex >= _muiMaxSimultaneousInstances)
		{
			std::string strErr("CAudioEmitter::getFrequency(");
			strErr += std::to_string(uiIndex);
			strErr += ") failed as given index has to be less than ";
			strErr += std::to_string(_muiMaxSimultaneousInstances);
			ThrowIfTrue(1, strErr);
		}
		float fFrequency = 0.0f;
		_mvecVoices[uiIndex]->GetFrequencyRatio(&fFrequency);
		return fFrequency;
	}

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
		// Make sure the named sample exists
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
		// Make sure the emitter doesn't currently exist
		if (getExistsEmitter(strEmitterName))
		{
			std::string strErr("SCAudioManager::addEmitter() failed as the named of emitter of ");
			strErr += strEmitterName;
			strErr += " already exists.";
			ThrowIfTrue(1, strErr);
		}
		// Create the new emitter object
		CAudioEmitter* pNewEmitter = new CAudioEmitter(it->second->pResource->wfx, iMaxSimultaneousInstances);
		ThrowIfFalse(pNewEmitter, "SCAudioManager::addEmitter() failed to allocate memory for the new emitter");

		// Store the sample's name and group within the emitter as emitters only work with their set sample data
		pNewEmitter->_mstrSampleName = strSampleName;
		pNewEmitter->_mstrSampleGroupname = strSampleGroupname;

		// Store it in the hash map
		_mmapEmitters[strEmitterName] = pNewEmitter;
		return pNewEmitter;
	}

	void SCAudioManager::removeEmitter(const std::string& strEmitterName)
	{
		std::map<std::string, CAudioEmitter*>::iterator it = _mmapEmitters.find(strEmitterName);
		if (it == _mmapEmitters.end())
		{
			std::string strErr("SCAudioManager::removeEmitter(");
			strErr += strEmitterName;
			strErr += ") failed as the named emitter doesn't exist.";
			ThrowIfTrue(1, strErr);
		}
		delete it->second;
		_mmapEmitters.erase(it);
	}

	bool SCAudioManager::getExistsEmitter(const std::string& strEmitterName)
	{
		std::map<std::string, CAudioEmitter*>::iterator it = _mmapEmitters.find(strEmitterName);
		if (it == _mmapEmitters.end())
			return false;
		return true;
	}

	CAudioEmitter* SCAudioManager::getEmitter(const std::string& strEmitterName)
	{
		std::map<std::string, CAudioEmitter*>::iterator it = _mmapEmitters.find(strEmitterName);
		if (it == _mmapEmitters.end())
		{
			std::string strErr("SCAudioManager::getEmitter(");
			strErr += strEmitterName;
			strErr += ") failed as the named emitter doesn't exist.";
			ThrowIfTrue(1, strErr);
		}
		return it->second;
	}
}