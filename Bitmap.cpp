// Standard Includes -----------------------------------------------------------
#include <stdio.h>

// System Includes -------------------------------------------------------------

// Project Includes ------------------------------------------------------------
#include "Bitmap.h"
#include "Decode.h"
#include "Types.h"

// Local Includes --------------------------------------------------------------

// Local Defines ---------------------------------------------------------------

// Globals ---------------------------------------------------------------------

//------------------------------------------------------------------------------
GBitmap::GBitmap(uint8 *data)
	:	fPalette(NULL)
{
	uint16 size = *((uint16*)data); data += 2;
	uint16 format = *((uint16*)data); data += 2;
	uint16 decSize = *((uint16*)data); data += 2;
	uint16 reserved = *((uint16*)data); data += 2;
	uint16 paletteSize = *((uint16*)data); data += 2;

	if (paletteSize != 0)
	{
		printf("CPS has embedded palette\n");
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

	if (format == 4)
		decode80(data, fBits);
	else if (format == 3)
		decode80_3(data, fBits, decSize);
	else
	{
		printf("Unknown CPS format\n");
		return;
	}
}
//------------------------------------------------------------------------------
GBitmap::~GBitmap()
{
	if (fPalette)
		delete fPalette;
}
//------------------------------------------------------------------------------
