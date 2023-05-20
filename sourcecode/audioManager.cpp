#include "PCH.h"
#include "audioManager.h"
#include "log.h"
#include "utilities.h"

namespace X
{
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

		pLog->add("AudioManager initialisation complete.");
	}

	void AudioManager::addSample(const std::string& name, unsigned int iMaxNumberVoices)
	{
		Log* pLog = Log::getPointer();
		pLog->add("AudioManager::addSample(" + name + ", with iMaxNumberVoices set to " + std::to_string(iMaxNumberVoices));

		// Make sure valid params are passed
		ThrowIfTrue(iMaxNumberVoices < 1, "AudioManager::addSample() given iMaxNumberVoices < 1.");

		// Resource already exists?
		std::map<std::string, AudioSample*>::iterator itr = mmapSamples.find(name);
		if (mmapSamples.end() != itr)
		{
			return;
		}

		// If we get here, we have got to create, then add the resource
		AudioSample* pNewRes = new AudioSample();
		mmapSamples[name] = pNewRes;

		// Load in the sample data
		pNewRes->load(name);

		// Create voices for the sample
		HRESULT hr;
		for (int iSample = 0; iSample < (int)iMaxNumberVoices; iSample++)
		{
			IXAudio2SourceVoice* pVoice;
			// Create a source voice by calling the IXAudio2::CreateSourceVoice method on an instance of the XAudio2 engine.
			// The format of the voice is specified by the values set in a WAVEFORMATEX structure.
			ThrowIfTrue(FAILED(hr = mpXAudio2->CreateSourceVoice(&pVoice, (WAVEFORMATEX*)&pNewRes->wfx)), "AudioManager::addSample() failed to create source voice.");
			pNewRes->vecVoices.push_back(pVoice);
		}
	}

	void AudioManager::addSamples(const std::string& strDirectory, unsigned int iMaxNumberVoices, bool bRecursiveDirs)
	{
		Log* pLog = Log::getPointer();
		pLog->add("AudioManager::addSamples(" + strDirectory + ") called.");

		std::string strExtension(".wav");
		std::vector<std::string> strFilenames = getFilesInDir(strDirectory, strExtension, bRecursiveDirs);
		std::string strFilename;	// Holds file name including the given directory
		if (0 == strFilenames.size())
			return;
		pLog->add("Samples found: " + std::to_string(strFilenames.size()));
		for (int i = 0; i < (int)strFilenames.size(); ++i)
		{
			addSample(strFilenames[i], iMaxNumberVoices);
		}
	}

	bool AudioManager::getSampleExists(const std::string& name)
	{
		std::map<std::string, AudioSample*>::iterator itr = mmapSamples.find(name);
		if (itr == mmapSamples.end())
			return false;
		return true;
	}

	void AudioManager::removeSample(const std::string& name)
	{
		Log* pLog = Log::getPointer();
		pLog->add("AudioManager::removeSample(" + name + ") called.");

		// Resource doesn't exist in the group?
		std::map<std::string, AudioSample*>::iterator itr = mmapSamples.find(name);
		if (mmapSamples.end() == itr)
		{
			std::string err("AudioManager::removeSample(\"");
			err.append(name);
			err.append("\") failed because the named object couldn't be found.");
			ThrowIfTrue(1, err);
		}

		// Stop playback of all voices
		stopSample(name);

		// Destroy the resource
		delete itr->second;
		mmapSamples.erase(itr);
	}

	void AudioManager::removeAllSamples(void)
	{
		Log* pLog = Log::getPointer();
		pLog->add("AudioManager::removeAllSamples() called.");
		std::map<std::string, AudioSample*>::iterator itr = mmapSamples.begin();
		while (itr != mmapSamples.end())
		{
			removeSample(itr->first);
			itr = mmapSamples.begin();
		}
	}

	void AudioManager::playSample(const std::string& name, float fVolume, float fPlaybackSpeed, bool bLoop)
	{
		std::map<std::string, AudioSample*>::iterator itr = mmapSamples.find(name);
		ThrowIfTrue(itr == mmapSamples.end(), "AudioManager::playSample() failed to play sample as it doesn't exist.");

		HRESULT hr;
		AudioSample* audioSample = itr->second;
		IXAudio2SourceVoice* pSourceVoice = audioSample->vecVoices[audioSample->iVecVoicesIndex];
		pSourceVoice->Stop();
		pSourceVoice->FlushSourceBuffers();
		// Loop the thing?
		if (bLoop)
		{
			audioSample->buffer.LoopBegin = 0;
			audioSample->buffer.LoopLength = 0;
			audioSample->buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
		}
		else
		{
			audioSample->buffer.LoopBegin = 0;
			audioSample->buffer.LoopLength = 0;
			audioSample->buffer.LoopCount = 0;
		}
		hr = pSourceVoice->SubmitSourceBuffer(&audioSample->buffer);
		ThrowIfTrue(FAILED(hr), "AudioManager::playSample() failed. Error submitting source buffer.");

		// Set voice volume, pan and frequency
		pSourceVoice->SetVolume(fVolume);
		pSourceVoice->SetFrequencyRatio(fPlaybackSpeed);
		
		hr = pSourceVoice->Start(0);
		ThrowIfTrue(FAILED(hr), "AudioManager::playSample() failed during call to pSourceVoice->Start()");
		
		// Deal with voice index
		audioSample->iVecVoicesIndex++;
		if (audioSample->iVecVoicesIndex >= audioSample->vecVoices.size())
			audioSample->iVecVoicesIndex = 0;

		/*
		// Create a source voice by calling the IXAudio2::CreateSourceVoice method on an instance of the XAudio2 engine.
		// The format of the voice is specified by the values set in a WAVEFORMATEX structure.
		HRESULT hr;
		if (FAILED(hr = pXAudio2->CreateSourceVoice(&pSourceVoice, (WAVEFORMATEX*)&audioSample->wfx))) return;

		// Submit an XAUDIO2_BUFFER to the source voice using the function SubmitSourceBuffer.
		if (FAILED(hr = pSourceVoice->SubmitSourceBuffer(&audioSample->buffer)))
			return;

		// Use the Start function to start the source voice.
		// Since all XAudio2 voices send their output to the mastering voice by default, audio
		// from the source voice automatically makes its way to the audio device selected at initialization.
		// In a more complicated audio graph, the source voice would have to specify the voice to which its output should be sent.
		hr = pSourceVoice->Start(0);
		if (FAILED(hr))
			Log::getPointer()->exception("ManagerAudio::playSample() failed during call to pSourceVoice->Start()");
		*/
	}

	void AudioManager::stopSample(const std::string& name)
	{
		std::map<std::string, AudioSample*>::iterator itr = mmapSamples.find(name);
		ThrowIfTrue(itr == mmapSamples.end(), "AudioManager::stopSample() failed to play sample as it doesn't exist.");

		HRESULT hr;
		AudioSample* audioSample = itr->second;
		for (int i = 0; i < audioSample->vecVoices.size(); i++)
		{
			IXAudio2SourceVoice* pSourceVoice = audioSample->vecVoices[i];
			hr = pSourceVoice->Stop();
			ThrowIfTrue(FAILED(hr), "AudioManager::stopSample() failed. Error calling pSourceVoice->Stop().");
			hr = pSourceVoice->FlushSourceBuffers();
			ThrowIfTrue(FAILED(hr), "AudioManager::stopSample() failed. Error calling pSourceVoice->FlushSourceBuffers().");
		}
		audioSample->iVecVoicesIndex = 0;

	}

	int AudioManager::getNumberVoicesPlaying(const std::string& name)
	{
		std::map<std::string, AudioSample*>::iterator itr = mmapSamples.find(name);
		ThrowIfTrue(itr == mmapSamples.end(), "AudioManager::getNumberVoicesPlaying() failed to play sample as it doesn't exist.");

		int iNumberVoicesPlaying = 0;
		XAUDIO2_VOICE_STATE state;
		AudioSample* audioSample = itr->second;
		for (int i = 0; i < audioSample->vecVoices.size(); i++)
		{
			audioSample->vecVoices[i]->GetState(&state);
			if (state.BuffersQueued > 0)
				iNumberVoicesPlaying++;
		}
		return iNumberVoicesPlaying;
	}

	unsigned int AudioManager::getMemoryUsage(void)
	{
		XAUDIO2_PERFORMANCE_DATA pPerformanceData;// = 0;
		mpXAudio2->GetPerformanceData(&pPerformanceData);
		return pPerformanceData.MemoryUsageInBytes;
	}
}

