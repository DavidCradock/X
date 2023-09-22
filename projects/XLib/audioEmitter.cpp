#include "PCH.h"
#include "audioEmitter.h"
#include "singletons.h"
#include "audioManager.h"
#include "logging.h"

namespace X
{
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
}