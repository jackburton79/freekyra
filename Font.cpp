// Standard Includes -----------------------------------------------------------
#include <stdio.h>

// System Includes -------------------------------------------------------------

// Project Includes ------------------------------------------------------------
#include "Font.h"

// Local Includes --------------------------------------------------------------

// Local Defines ---------------------------------------------------------------

// Globals ---------------------------------------------------------------------

//------------------------------------------------------------------------------
Font::Font(uint8 *data)
	:	fData(data),
		fCount(0),
		fHeight(0),
		fWidth(0),
		fCharOffset(NULL),
		fCharWidth(NULL),
		fCharHeight(NULL)
{
	printf("Font size=%d ", *((uint16*)data)); data += 2;
	
	if (*((uint16*)data) != 0x0500)
	{
		printf("Error: 0x0500 expected\n");
		return;
	}

	data += 2;

	if (*((uint16*)data) != 0x000e)
	{
		printf("Error: 0x000e expected\n");
		return;
	}

	data += 2;

	if (*((uint16*)data) != 0x0014)
	{
		printf("Error: 0x0014 expected\n");
		return;
	}

	data += 2;

	fCharWidth = fData + *((uint16*)data); data += 2;
	fCharBits = fData + *((uint16*)data); data += 2;
	fCharHeight = (uint16*)(fData + *((uint16*)data)); data += 2;

	if (*((uint16*)data) != 0x1012)
	{
		printf("Error: 0x1012 expected\n");
		return;
	}

	data += 2;

	fCount = *((uint16*)data) - 1; data += 2;
	fHeight = *data++;
	fWidth = *data++;

	fCharOffset = (uint16*)data;

	printf("count=%d, height=%d, width=%d\n", fCount, fHeight, fWidth);
}
//------------------------------------------------------------------------------
Font::~Font()
{
}
//------------------------------------------------------------------------------
uint8 *Font::GetChar(uint8 c, int8 *width, int8 *height, int8 *y)
{
	*width = fCharWidth[c];
	*height = fCharHeight[c] >> 8;
	*y = fCharHeight[c] & 0xFF;

	//printf("Character %c(%02x) width=%d, height=%d, y=%d, offset=%d\n", c, c,
	//	*width, *height, *y, fCharOffset[c]);

	return fData + fCharOffset[c];
}
//------------------------------------------------------------------------------
