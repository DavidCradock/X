#pragma once
#include "PCH.h"

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
}