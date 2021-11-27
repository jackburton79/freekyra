// Standard Includes -----------------------------------------------------------
#include <string.h>
// System Includes -------------------------------------------------------------

// Project Includes ------------------------------------------------------------
#include "Decode.h"
#include "Palette.h"

// Local Includes --------------------------------------------------------------

// Local Defines ---------------------------------------------------------------

// Globals ---------------------------------------------------------------------

//------------------------------------------------------------------------------
GPalette::GPalette(RGBQUAD *colors, int32 count)
	:	fCount(count)
{
	memcpy(fColors, colors, count * sizeof(RGBQUAD));
	memset(fColors + count * sizeof(RGBQUAD), 0,
		(256 - count) * sizeof(RGBQUAD));
}
//------------------------------------------------------------------------------
GPalette::GPalette(uint8 *data)
	:	fCount(256)
{
	uint16 size = *((uint16*)data); data += 2;

	if (size)
	{
		uint16 format = *((uint16*)data); data += 2;
		uint16 decSize = *((uint16*)data); data += 2;
		uint16 reserved = *((uint16*)data); data += 2;
		uint16 paletteSize = *((uint16*)data); data += 2;

		uint8 palette[3 * 256];

		decode80(data, palette);

		for (int i = 0; i < 256; i++)
		{
			fColors[i].rgbRed = palette[i * 3] << 2;
			fColors[i].rgbGreen = palette[i * 3 + 1] << 2;
			fColors[i].rgbBlue = palette[i * 3 + 2] << 2;
			fColors[i].rgbReserved = 0;
		}
	}
	else
	{
		data -= 2;

		for (int i = 0; i < 256; i++)
		{
			fColors[i].rgbRed = *data++ << 2;
			fColors[i].rgbGreen = *data++ << 2;
			fColors[i].rgbBlue = *data++ << 2;
			fColors[i].rgbReserved = 0;
		}
	}
}
//------------------------------------------------------------------------------
GPalette::~GPalette()
{
}
//------------------------------------------------------------------------------
RGBQUAD *GPalette::Colors()
{
	return fColors;
}
//------------------------------------------------------------------------------
int32 GPalette::CountColors()
{
	return fCount;
}
//------------------------------------------------------------------------------
