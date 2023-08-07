#pragma once
#include "PCH.h"
#include "singleton.h"

namespace X
{
	// An audio sample containing sample data used for playback via audio emitters
	class CAudioSample
	{
		friend class SCAudioManager;
		friend class CAudioEmitter;
	public:
		CAudioSample();
		~CAudioSample();

		// Loads the resource so it's ready for use.
		// Used by the manager
		void load(void);

		// Loads the sample's wave format into wfx
		// Used by the manager.
		// This doesn't load in all the sample data, just the sample's format.
		// This is so the sample doesn't have to be loaded fully before setting up emitters
		void loadFormat(void);

		// Unloads the resource, freeing memory.
		// Used by the manager 
		void unload(void);

	private:
		std::string _mstrAudioFilename;		// Holds the name of the file which holds the sample data

		HRESULT _findChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition);
		HRESULT _readChunkData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset);
		XAUDIO2_BUFFER buffer = { 0 };		// Holds the sample data once the object is loaded
		WAVEFORMATEXTENSIBLE wfx = { 0 };	// Holds the information about the sample data once loadFormat() has been called
	};

	// An audio emitter which emits sounds from loaded audio samples
	class CAudioEmitter
	{
		friend class SCAudioManager;
	public:
		CAudioEmitter(WAVEFORMATEXTENSIBLE &wfx, unsigned int iMaxSimultaneousInstances = 8);
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

	// Use this to manage all audio samples and their playback via audio emitters
	// Note: Only supports playback of samples in the ".wav" format from disk.
	// To be honest, I doubt I'll add playback of .ogg or .mp3 as .wav is uncompressed with better audio quality, takes less time to decompress (as there isn't any) and 
	// storage space isn't too much of an issue, say for example an hour's worth of music only takes up around 600MB anyway.
	// I also found when playing around with other libraries such as FMOD or BASS that the loop points with compressed file formats were never precise enough, causing an
	// ever so slightly audible click or weird artifact with looping samples. .wav doesn't have this issue!
	// Who knows though, never say never!
	// Usage:
	// Audio sample data is stored inside groups
	// By default, a group named "default" is created for storage of audio samples upon construction.
	// Get a pointer to the audio manager...
	// SCAudioManager *pAudioManager = SCAudioManager::getPointer();
	// Add a new sample to the "default" group
	// pAudioManager->addSample("audio/fart.wav", "default");
	// // Add more samples if needed and then call..
	// pAudioManager->loadSampleGroup("default");	// Load all audio data, so it's ready for use
	// Create an emitter which will be used to playback a sample.
	// Emitters are "linked" to a single sample, they can only play that one sample
	// CAudioEmitter *pAudioEmitter = pAudioManager->addEmitter("MyBottomEmitter", "audio/fart.wav", 8, "default");
	// Now we can playback the sample with...
	// pAudioEmitter->play(1.0f, 1.0f, false);	// Where parameters are volume, playback speed and whether to loop the sample or not
	class SCAudioManager : public CSingleton<SCAudioManager>
	{
	public:
		friend class CAudioEmitter;
		SCAudioManager();

		// Return the number of sample resource groups which currently exist in the manager
		unsigned int getNumSampleGroups(void);

		// Returns the number of samples which currently exist in the named group
		// If the named group doesn't exist, an exception occurs
		unsigned int getNumSamplesInGroup(const std::string& strGroupName);

		// Returns the number of samples which are currently loaded in the named group
		// If the named group doesn't exist, an exception occurs
		unsigned int getNumSamplesInGroupLoaded(const std::string& strGroupName);

		// Returns the sample group name, at specified index
		// If invalid index given, an exception occurs
		const std::string& getSampleGroupName(unsigned int iGroupIndex);

		// Adds a new sample resource group with the given name which resources can be put into
		// If the named group already exists, this simply returns
		void addNewSampleGroup(const std::string& strNewGroupName);

		// Returns true if the named sample group exists, else false
		bool sampleGroupExists(const std::string& strGroupName);

		// Will load all sample resources in the named group so that they're ready for use.
		// If the named group doesn't exist, an exception occurs
		// If the group was previously loaded, then some more resources were added, only the newly added resources will be loaded
		// as the previous resources will have already been loaded.
		void loadSampleGroup(const std::string& strGroupName);

		// Will load a single unloaded sample resource in the named group so that it's ready for use.
		// This will load only a single unloaded resource within the named group, then return.
		// This method is usefull for creating loading screens, where you call this, then call getNumResInGroup and getNumResInGroupLoaded, when both return values from
		// these two methods are the same, all resources are loaded.
		// If the named group doesn't exist, an exception occurs
		// If the group was previously loaded, then some more resources were added, only the newly added resources will be loaded
		// as the previous resources will have already been loaded.
		void loadSampleGroupSingle(const std::string& strGroupName);

		// Will unload all sample resources in the named group to free up memory.
		// If the named group doesn't exist, an exception occurs
		void unloadSampleGroup(const std::string& strGroupName);

		// Adds a new sample resource to the named group
		// If the group name doesn't exist, an exception occurs.
		// If the resource name already exists, the resource's reference count is increased
		// If the resource doesn't previously exist and it's newly created, it'll be in it's unloaded state
		// strNewResourceName is used to refer to the sample data by name and is also the filename holding the sample's sample data
		CAudioSample* addSample(const std::string& strNewResourceName, const std::string& strGroupName = "default");

		// Returns a pointer to the named sample resource in it's named group
		// If either the group given doesn't exist, or the named resource doesn't exist, an exception occurs
		// Also, if the resource is in the unloaded state, it is loaded here
		CAudioSample* getSample(const std::string& strResourceName, const std::string& strGroupName = "default");

		// Returns true if the named sample resource exists in the named group, else false
		bool getExistsSample(const std::string& strResourceName, const std::string& strGroupName = "default");

		// Returns true if the named sample is currently loaded
		// Returns false if the sample couldn't be found
		bool getSampleLoaded(const std::string& strResourceName, const std::string& strGroupName = "default");

		// Removes the named sample resource from the named group
		// If either the resource or the group that it's in doesn't exist, an exception occurs
		// Remove all emitters that are playing this sample before calling this.
		void removeSample(const std::string& strResourceName, const std::string& strGroupName);

		// Returns memory usage in bytes
		unsigned int getMemoryUsage(void);

		// Adds an emitter which we use to playback the named sample.
		// If the named sample doesn't exist, an exception occurs
		// If the named emitter already exists, an exception occurs
		CAudioEmitter* addEmitter(const std::string& strEmitterName, const std::string& strSampleName, unsigned int iMaxSimultaneousInstances = 8, const std::string& strSampleGroupname = "default");

		// Removes the named emitter and stops playback
		// If either the resource or the group that it's in doesn't exist, an exception occurs
		void removeEmitter(const std::string& strEmitterName);

		// Returns whether the named emitter already exists
		bool getExistsEmitter(const std::string& strEmitterName);

		// Returns a pointer to the named emitter
		// If the named emitter doesn't exist, an exception occurs
		CAudioEmitter* getEmitter(const std::string& strEmitterName);
	private:

		// A resource and various variables needed by the manager for each resource
		struct ResourceSample
		{
			CAudioSample* pResource;			// Pointer to the resource
			unsigned int uiReferenceCount;	// How many times the resource has been added/removed
			bool bLoaded;					// Whether the resource is loaded or not
		};
		// An audio sample resource group holding each resource
		struct Group
		{
			std::map<std::string, ResourceSample*> mmapResource;	// Hash map holding named resource
		};
		std::map<std::string, Group*> _mmapGroup;					// Hash map holding named resource groups

		IXAudio2* _mpXAudio2;						// Main XAudio2 interface
		IXAudio2MasteringVoice* _mpMasterVoice;		// Mastering voice

		std::map<std::string, CAudioEmitter*> _mmapEmitters;		// Holds each named emitter
	};

}