#ifndef _PALETTE_H_
#define _PALETTE_H_

// Standard Includes -----------------------------------------------------------

// System Includes -------------------------------------------------------------

// Project Includes ------------------------------------------------------------
#include "Types.h"

struct RGBQUAD {
	uint8 rgbRed;
	uint8 rgbGreen;
	uint8 rgbBlue;
	uint8 rgbReserved;
};

// Class GPalette --------------------------------------------------------------
class GPalette {
public:
				GPalette(RGBQUAD *colors, int32 count);
				GPalette(uint8 *data);
virtual			~GPalette();

		RGBQUAD	*Colors();
		int32	CountColors();

private:
		RGBQUAD	fColors[256];
		int32	fCount;
};
//------------------------------------------------------------------------------

#endif // _PALETTE_H_
