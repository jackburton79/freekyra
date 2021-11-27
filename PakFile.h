#ifndef _PAK_FILE_H_
#define _PAK_FILE_H_

// Standard Includes -----------------------------------------------------------

// System Includes -------------------------------------------------------------

// Project Includes ------------------------------------------------------------
#include "Bitmap.h"
#include "Font.h"
#include "Movie.h"
#include "Palette.h"
#include "Sound.h"
#include "Types.h"

// Local Includes --------------------------------------------------------------

// Local Defines ---------------------------------------------------------------

// Globals ---------------------------------------------------------------------
struct PakChunk {
	const char	*fName;
	const uint8	*fBuffer;
	uint32		fSize;

	PakChunk	*fNext;
	PakChunk	*fPrevious;
};

class PakFile;

typedef bool (*EnumResourcesCallback)(PakFile *file, const char *name,
								   void *userdata);

// Class PakFile ---------------------------------------------------------------
class PakFile {
public:
					PakFile(const char *file);
					PakFile(uint8 *buffer, uint32 size);

virtual				~PakFile();

		uint8		*LoadResource(const char *name);
		GBitmap		*LoadBitmap(const char *name);
		Font		*LoadFont(const char *name);
		Movie		*LoadMovie(const char *name);
		GPalette	*LoadPalette(const char *name);
		Sound		*LoadSound(const char *name);

		void		EnumResources(EnumResourcesCallback callback,
						void *userdata);
		const char	*NameAt(int32 index);

private:
friend class Storage;
		void		ReadDirectory();

		uint8		*fBuffer;
		uint32		fSize;
		PakChunk	*fFirstChunk;
		PakChunk	*fLastChunk;
		PakFile		*fNext;
};
//------------------------------------------------------------------------------

#endif // _PAK_FILE_H_
