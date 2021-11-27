// Standard Includes -----------------------------------------------------------
#include <stdio.h>

// System Includes -------------------------------------------------------------

// Project Includes ------------------------------------------------------------
#include "Decode.h"
#include "SHPMovie.h"

// Local Includes --------------------------------------------------------------

// Local Defines ---------------------------------------------------------------

// Globals ---------------------------------------------------------------------

//------------------------------------------------------------------------------
SHPMovie::SHPMovie(uint8 *data)
	:	Movie(data),
		fData(NULL),
		fFrames(0),
		fOffsets(NULL),
		fCachedData(NULL),
		fCachedSize(0)
{
	fData = data;

	// Read header
	fFrames = *((uint16*)data); data += 2;
	printf("SHPMovie: %d frames\n", fFrames);

	// Read data
	fOffsets = (int32*)data;
}
//------------------------------------------------------------------------------
SHPMovie::~SHPMovie()
{
	if (fCachedData)
		free(fCachedData);
}
//------------------------------------------------------------------------------
uint8 *SHPMovie::GetFrame(int32 frame, int32 *_width, int32 *_height)
{
	uint8 *ptr = fData + fOffsets[frame] + 2;

	// Read frame header
	uint16 compression = *((uint16*)ptr); ptr += 2;
	uint8 height = *((uint8*)ptr); ptr += 1;
	uint16 width = *((uint16*)ptr); ptr += 2;
	uint8 height2 = *((uint8*)ptr); ptr += 1;
	uint16 size_in = *((uint16*)ptr); ptr += 2;
	uint16 size_out = *((uint16*)ptr); ptr += 2;

	printf("compression=%04x, height=%d, width=%d, %d, size_in=%d, size_out=%d\n",
		compression, height, width, height2, size_in, size_out);

	// Read palette offsets
	uint8 *offsets = ptr;
	{
		printf("  Palette redirection:");
		for (int32 j = 0; j < 16; j++)
		printf(" %02x", *(ptr++));
		printf("\n");
	}

	if (fCachedData == NULL || fCachedSize < width * height)
		fCachedData = (uint8*)realloc(fCachedData, width * height);

	// Read image
	if (compression < 2)
	{
		uint8 *buffer80 = new uint8[size_out];
		printf("%d bytes decoded\n", decode80(ptr, buffer80));
		Decode00(buffer80, fCachedData, size_out);
	}
	else if (compression == 3)
		Decode00(ptr, fCachedData, size_out);
	else if (compression == 5)
	{
		// When compression 5 is used, an additional byte comes in front
		// of the palette offsets. This byte is the amount of palette
		// offsets following it.
		uint8 offset_count = *offsets;

		offsets++;
		ptr = offsets + offset_count;

		uint8 *buffer80 = new uint8[size_out];
		printf("%d bytes decoded\n", decode80(ptr, buffer80));
		Decode00(buffer80, fCachedData, size_out);
	}

	for (uint32 j = 0; j < width * height; j ++)
		fCachedData[j] = offsets[fCachedData[j]];

	// Return data
	if (_width)
		*_width = width;
	if (_height)
		*_height = height;
	return fCachedData;
}
//------------------------------------------------------------------------------
