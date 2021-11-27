#ifndef _BITMAP_H_
#define _BITMAP_H_

// Standard Includes -----------------------------------------------------------

// System Includes -------------------------------------------------------------

// Project Includes ------------------------------------------------------------
#include "Palette.h"
#include "Types.h"

// Local Includes --------------------------------------------------------------

// Local Defines ---------------------------------------------------------------

// Globals ---------------------------------------------------------------------

// Class GBitmap ---------------------------------------------------------------
class GBitmap {
public:
					GBitmap(uint8 *data);
					~GBitmap();

		GPalette	*Palette() { return fPalette; }

//private:
		GPalette	*fPalette;
		uint8		fBits[320 * 200];
};
//------------------------------------------------------------------------------

#endif // _BITMAP_H_
