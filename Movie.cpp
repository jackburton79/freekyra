// Standard Includes -----------------------------------------------------------
#include <stdio.h>

// System Includes -------------------------------------------------------------

// Project Includes ------------------------------------------------------------
#include "Decode.h"
#include "Movie.h"

// Local Includes --------------------------------------------------------------

// Local Defines ---------------------------------------------------------------

// Globals ---------------------------------------------------------------------

//------------------------------------------------------------------------------
Movie::Movie(uint8 *data)
	:	fData(NULL),
		fPalette(NULL),
		fCachedFrame(-2),
		fCachedData(NULL)
{
	fData = data;

	// Read header
	fFrames = *(uint16*)data; data += 2;
	fX = *(uint16*)data; data += 2;
	fY = *(uint16*)data; data += 2;
	fWidth = *(uint16*)data; data += 2;
	fHeight = *(uint16*)data; data += 2;
	fFlags = *(uint16*)data; data += 2;

	uint16 has_palette = *(uint16*)data; data += 2;

	if (has_palette != 0)
		printf("WSA has embedded palette\n");

	if (fFlags != 0)
		fFrames++;

	printf("Movie: %d frames, [%d, %d, %d, %d], 0x%x(%d)\n", fFrames, fX, fY,
		fWidth, fHeight, fFlags, fFlags);

	// Read data
	fOffsets = (int32*)data;

	if (has_palette)
	{
		data += (fFrames + 1) * sizeof(int32);

		RGBQUAD palette[256];

		for (uint32 i = 0; i < 256; i++)
		{
			palette[i].rgbRed = *data++ << 2;
			palette[i].rgbGreen = *data++ << 2;
			palette[i].rgbBlue = *data++ << 2;
			palette[i].rgbReserved = 0;
		}
		
		fPalette = new GPalette(palette, 256);
	}
}
//------------------------------------------------------------------------------
Movie::~Movie()
{
	if (fCachedData)
		delete fCachedData;
}
//------------------------------------------------------------------------------
uint8 *Movie::GetFrame(int32 frame)
{
	if (fCachedData)
	{
		if (fCachedFrame == frame)
			return fCachedData;
	}
	else
		fCachedData = new uint8[fWidth * fHeight];

	uint8 *image80;
	uint8 image40[64000];

	if (frame == fCachedFrame + 1)
	{
		image80 = fData + fOffsets[frame] + (fPalette ? 256 * 3 : 0);
		decode80(image80, image40);
		decode40(image40, fCachedData);
	}
	else
	{
		memset(fCachedData, 0, fWidth * fHeight);

		for (int32 i = 0; i <= frame; i++)
		{
			image80 = fData + fOffsets[i] + (fPalette ? 256 * 3 : 0);
			decode80(image80, image40);
			decode40(image40, fCachedData);
		}
	}

	fCachedFrame = frame;
	return fCachedData;
}
//------------------------------------------------------------------------------
