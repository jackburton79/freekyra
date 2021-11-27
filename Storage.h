#ifndef _STORAGE_H_
#define _STORAGE_H_

// Standard Includes -----------------------------------------------------------

// System Includes -------------------------------------------------------------

// Project Includes ------------------------------------------------------------
#include "Bitmap.h"
#include "Font.h"
#include "Movie.h"
#include "PakFile.h"
#include "Palette.h"
#include "Sound.h"
#include "Types.h"

// Local Includes --------------------------------------------------------------

// Local Defines ---------------------------------------------------------------

// Globals ---------------------------------------------------------------------

// Class Storage ---------------------------------------------------------------
class Storage {
public:
					Storage();
virtual				~Storage();

		
		void		AddArchive(const char *file);
		PakFile		*ArchiveAt(int32 index);
		int32		CountArchives();

		uint8		*LoadResource(const char *name);
		GBitmap		*LoadBitmap(const char *name);
		Font		*LoadFont(const char *name);
		Movie		*LoadMovie(const char *name);
		GPalette	*LoadPalette(const char *name);
		Sound		*LoadSound(const char *name);

		void		EnumResources(EnumResourcesCallback callback,
						void *userdata);

private:
		PakFile		*fFirstArchive;
		PakFile		*fLastArchive;
		int32		fCount;
};
//------------------------------------------------------------------------------

extern Storage *storage;

#endif // _STORAGE_H_
