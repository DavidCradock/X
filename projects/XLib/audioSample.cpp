#include "PCH.h"
#include "audioSample.h"
#include "stringUtils.h"
#include "singletons.h"
#include "logging.h"
#include "resourceLoadingScreen.h"

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
}