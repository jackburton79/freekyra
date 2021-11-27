#ifndef _WSA_MOVIE_H_
#define _WSA_MOVIE_H_

// Standard Includes -----------------------------------------------------------

// System Includes -------------------------------------------------------------

// Project Includes ------------------------------------------------------------
#include "Movie.h"
#include "Palette.h"

// Local Includes --------------------------------------------------------------

// Local Defines ---------------------------------------------------------------

// Globals ---------------------------------------------------------------------

// Class WSAMovie --------------------------------------------------------------
class WSAMovie : public Movie {
public:
					WSAMovie(uint8 *data);
virtual				~WSAMovie();

virtual	uint8		*GetFrame(int32 frame, int32 *width = NULL,
						int32 *height = NULL);
virtual	int32		CountFrames() { return fFrames; }

		int32		X() { return fX; };
		int32		Y() { return fY; };
		int32		Width() { return fWidth; };
		int32		Height() { return fHeight; };

virtual	GPalette	*Palette() { return fPalette; }

private:
		int16		fFrames;
		int16		fX;
		int16		fY;
		int16		fWidth;
		int16		fHeight;
		int16		fFlags;
		int32		*fOffsets;
		uint8		*fData;

		GPalette	*fPalette;

		int32		fCachedFrame;
		uint8		*fCachedData;
};
//------------------------------------------------------------------------------

#endif // _WSA_MOVIE_H_
