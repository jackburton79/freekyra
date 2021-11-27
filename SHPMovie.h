#ifndef _SHP_MOVIE_H_
#define _SHP_MOVIE_H_

// Standard Includes -----------------------------------------------------------
#include <stdlib.h>
// System Includes -------------------------------------------------------------

// Project Includes ------------------------------------------------------------
#include "Movie.h"
#include "Palette.h"

// Local Includes --------------------------------------------------------------

// Local Defines ---------------------------------------------------------------

// Globals ---------------------------------------------------------------------

// Class SHPMovie --------------------------------------------------------------
class SHPMovie : public Movie {
public:
					SHPMovie(uint8 *data);
virtual				~SHPMovie();

virtual	uint8		*GetFrame(int32 frame, int32 *width = NULL,
						int32 *height = NULL);
virtual	int32		CountFrames() { return fFrames; }

virtual	GPalette	*Palette() { return NULL; }

private:
		uint8		*fData;
		int16		fFrames;
		int32		*fOffsets;

		uint8		*fCachedData;
		int32		fCachedSize;
};
//------------------------------------------------------------------------------

#endif // _SHP_MOVIE_H_
