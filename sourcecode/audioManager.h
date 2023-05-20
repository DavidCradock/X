#pragma once
#include "PCH.h"
#include "singleton.h"
#include "audioSample.h"

namespace X
{

	// Is responsible for playback of audio.
	// Note: Only supports playback of samples in the ".wav" format from disk.
	// To be honest, I doubt I'll add playback of .ogg or .mp3 as .wav is uncompressed with better audio quality, takes less time to decompress (as there isn't any) and 
	// storage space isn't too much of an issue, say for example an hour's worth of music only takes up around 600MB anyway.
	// I also found when playing around with other libraries such as FMOD or BASS that the loop points with compressed file formats were never precise enough, causing an
	// ever so slightly audible click or weird artifact with looping samples. .wav doesn't have this issue!
	// Who knows though, never say never!
	// Usage:
	// Get a pointer to this manager with AudioManager *pAudioManager = AudioManager::getPointer();
	// Then add a sample that you wish to play back with pAudioManager->addSample("data/audio/fart.wav");
	// Playback an instance of the sample with pAudioManager->playSample("data/audio/fart.wav", 1.0f /* Volume where 1.0f = normal vol */, 1.0f /*Playback speed where 1.0f = normal speed */);
	// Multiple instances of the sample can be played back upto iMaxNumberVoices which were passed to addSample();
	// Stop playback of all instances of the named sample currently being played with pAudioManager->stopSample();
	class AudioManager : public Singleton<AudioManager>
	{
	public:
		friend class AudioSample;
		AudioManager();

		// Adds a new audio sample and loads it's data in so it's ready to be played back
		// iMaxNumberVoices is the total number of times this sample can be played back simultaneously
		void addSample(const std::string& name, unsigned int iMaxNumberVoices = 8);

		// Adds new audio samples from the given directory and loads it's data in so they're ready to be played back
		// iMaxNumberVoices is the total number of times this sample can be played back simultaneously.
		// If bRecursiveDirs is true, then files within sub directories are also added instead of just the ones in the given directory.
		// If no files were found, this simply returns.
		// The samples can then be accessed via the directory followed by the filename of each sample filename.
		void addSamples(const std::string& strDirectory, unsigned int iMaxNumberVoices = 8, bool bRecursiveDirs = false);

		// Returns true if the named object exists, else false.
		bool getSampleExists(const std::string& name);

		// Removes an object.
		// Throws an exception if the named object doesn't exist.
		void removeSample(const std::string& name);

		// Removes all samples
		void removeAllSamples(void);

		// Plays the named sample
		// Multiple instances of this sample can be played back to iMaxNumberVoices parsed to addSample()
		void playSample(const std::string& name, float fVolume = 1.0f, float fPlaybackSpeed = 1.0f, bool bLoop = false);

		// Stops playback of all instances of the named sample.
		void stopSample(const std::string& name);

		// Returns the number of voices that a sample has which is currently playing
		// Usefull if you're playing music and wish to know if it's finished playing or not, amongst other things.
		int getNumberVoicesPlaying(const std::string& name);

		// Returns memory usage in bytes
		unsigned int getMemoryUsage(void);

	private:
		IXAudio2* mpXAudio2;
		IXAudio2MasteringVoice* mpMasterVoice;
		std::map<std::string, AudioSample*> mmapSamples;	// Holds each named sample and it's data
	};

}