#ifndef _FONT_H_
#define _FONT_H_

// Standard Includes -----------------------------------------------------------

// System Includes -------------------------------------------------------------

// Project Includes ------------------------------------------------------------
#include "Types.h"

// Local Includes --------------------------------------------------------------

// Local Defines ---------------------------------------------------------------

// Globals ---------------------------------------------------------------------

// Class Font ------------------------------------------------------------------
class Font {
public:
				Font(uint8 *data);
virtual			~Font();

		uint8	*GetChar(uint8 c, int8 *width, int8 *height, int8 *y);

//private:
		uint8	*fData;
		uint16	fCount;
		uint8	fHeight;
		uint8	fWidth;
		uint16	*fCharOffset;
		uint8	*fCharBits;
		uint8	*fCharWidth;
		uint16	*fCharHeight;
};
//------------------------------------------------------------------------------

#endif // _FONT_H_
