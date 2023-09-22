#pragma once
#include "PCH.h"

namespace X
{
	// An audio emitter which emits sounds from loaded audio samples
	class CAudioEmitter
	{
		friend class SCAudioManager;
	public:
		CAudioEmitter(WAVEFORMATEXTENSIBLE& wfx, unsigned int iMaxSimultaneousInstances = 8);
		~CAudioEmitter();

		// Plays an instance for this emitter using given values
		// If the sample isn't currently loaded, this silently fails
		void play(float fVolume = 1.0f, float fPlaybackSpeed = 1.0f, bool bLoop = false);

		// Stops playback of all voices for this emitter
		void stopAll(void);

		// Stops playback of a voice at the specified index.
		// The index can be in the range of 0 to maximum number of simultaneous instances, minus one.
		// If an invalid index is given, an exception occurs.
		void stop(unsigned int uiIndex);

		// Returns the number of voices which are currently playing for this emitter
		unsigned int getNumVoicesPlaying(void) const;

		// Sets a voice's volume whilst it's playing
		// If an invalid index is given, an exception occurs
		void setVolume(unsigned int uiIndex, float fVolume);

		// Retrieves a voice's currently set volume
		// If an invalid index is given, an exception occurs
		float getVolume(unsigned int uiIndex) const;

		// Sets a voice's frequency whilst it's playing
		// If an invalid index is given, an exception occurs
		void setFrequency(unsigned int uiIndex, float fVolume);

		// Retrieves a voice's currently set frequency
		// If an invalid index is given, an exception occurs
		float getFrequency(unsigned int uiIndex) const;

	private:
		unsigned int _muiMaxSimultaneousInstances;		// Maximum number of sounds which can be played back simultaneously
		std::vector<IXAudio2SourceVoice*> _mvecVoices;	// Multiple instances playback
		unsigned int _muiVecVoicesIndex;				// Which voice index to play next
		std::string _mstrSampleName;					// The named sample which this emitter uses for playback
		std::string _mstrSampleGroupname;				// The named sample's groupname which it is located within.
	};


}