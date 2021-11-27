// Standard Includes -----------------------------------------------------------
#include <stdio.h>
#include <string.h>
// System Includes -------------------------------------------------------------

// Project Includes ------------------------------------------------------------
#include "Sound.h"

// Local Includes --------------------------------------------------------------

// Local Defines ---------------------------------------------------------------

// Globals ---------------------------------------------------------------------

/*void CALLBACK waveOutCallback(HWAVEOUT waveOut, UINT uMsg, DWORD dwInstance,
							  DWORD dwParam1, DWORD dwParam2)
{
	Sound *sound = (Sound*)dwInstance;

	if (uMsg == WOM_CLOSE)
	{
	}
	else if (uMsg == WOM_DONE)
	{
		waveOutUnprepareHeader(waveOut, &sound->fHeader, sizeof(WAVEHDR));
		delete sound;
		waveOutClose(waveOut);
	}
	else if (uMsg == WOM_OPEN)
	{
	}
}*/
//------------------------------------------------------------------------------
Sound::Sound(uint8 *data)
{
	/*if (strncmp((char*)data, "Creative Voice File", 19) != 0)
	{
		printf("Invalid Creative Voice File: header missing\n");
		return;
	}

	data += 19;

	if (*data != 0x1A)
	{
		printf("Invalid Creative Voice File: eof missing\n");
		return;
	}

	data++;

	int16 offset = *((uint16*)data); data += 2;
	int16 version = *((uint16*)data); data += 2;
	int16 checksum = *((uint16*)data); data += 2;
	
	if (checksum != ~version +0x1234)
	{
		printf("Invalid Creative Voice File: checksum failed\n");
		return;
	}

	uint8 type = *data++;

	int32 size = 0;
	int32 sample_rate;
	int32 compression;

	// Check if sound data
	if (type == 1)
	{
		size |= *data++;
		size |= *data++ << 8;
		size |= *data++ << 16;

		sample_rate = *data++;
		sample_rate = 1000000 / (256 - sample_rate);
		compression = *data++;
	}

	printf("Sound: %d Hz, %d bit\n", sample_rate, compression == 0 ? 8 : 0);

	HWAVEOUT waveOut;

	WAVEFORMATEX format;

	format.wFormatTag = WAVE_FORMAT_PCM;
    format.nChannels = 1;
    format.nSamplesPerSec = sample_rate;
    format.nAvgBytesPerSec = sample_rate;
    format.nBlockAlign = 1;
    format.wBitsPerSample = 8;
    format.cbSize = 0;

	MMRESULT err = waveOutOpen(&waveOut, NULL, &format,
		(DWORD_PTR)waveOutCallback, (DWORD_PTR)this,
		CALLBACK_FUNCTION | WAVE_ALLOWSYNC);

	fHeader.lpData = (char*)data;
    fHeader.dwBufferLength = size - 2;
    fHeader.dwBytesRecorded = 0;
    fHeader.dwUser = 0;
    fHeader.dwFlags = 0;
    fHeader.dwLoops = 0;
    fHeader.lpNext = NULL;
    fHeader.reserved = 0;

	waveOutPrepareHeader(waveOut, &fHeader, sizeof(WAVEHDR)); 
	waveOutWrite(waveOut, &fHeader, sizeof(WAVEHDR));
*/
}
//------------------------------------------------------------------------------
Sound::~Sound()
{
}
//------------------------------------------------------------------------------
