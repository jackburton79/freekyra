#ifndef _MOVIE_H_
#define _MOVIE_H_

// Standard Includes -----------------------------------------------------------

// System Includes -------------------------------------------------------------

// Project Includes ------------------------------------------------------------
#include "Palette.h"
#include "Types.h"

// Local Includes --------------------------------------------------------------

// Local Defines ---------------------------------------------------------------

// Globals ---------------------------------------------------------------------

// Class Movie -----------------------------------------------------------------
class Movie {
public:
					Movie(uint8 *data) {}
virtual				~Movie() {}

virtual	uint8		*GetFrame(int32 frame, int32 *width = NULL,
						  int32 *height = NULL) = 0;
virtual	int32		CountFrames() = 0;
virtual	GPalette	*Palette() = 0;
};
//------------------------------------------------------------------------------

#endif // _MOVIE_H_
