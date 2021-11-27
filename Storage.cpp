// Standard Includes -----------------------------------------------------------

// System Includes -------------------------------------------------------------

// Project Includes ------------------------------------------------------------
#include "Storage.h"

// Local Includes --------------------------------------------------------------

// Local Defines ---------------------------------------------------------------

// Globals ---------------------------------------------------------------------
Storage *storage = new Storage;

//------------------------------------------------------------------------------
Storage::Storage()
	:	fFirstArchive(NULL),
		fLastArchive(NULL),
		fCount(0)
{
}
//------------------------------------------------------------------------------
Storage::~Storage()
{
	PakFile *archive = fFirstArchive;
	
	while (archive)
	{
		PakFile *deleteArchive = archive;
		archive = archive->fNext;
		delete deleteArchive;
	}
}
//------------------------------------------------------------------------------
void Storage::AddArchive(const char *file)
{
	// TODO: check if not already loaded
	PakFile *archive = new PakFile(file);

	if (fLastArchive)
		fLastArchive->fNext = archive;
	else
		fFirstArchive = fLastArchive = archive;

	fCount++;
}
//------------------------------------------------------------------------------
PakFile *Storage::ArchiveAt(int32 index)
{
	for (PakFile *ptr = fFirstArchive; ptr; ptr = ptr->fNext)
	{
		if (index == 0)
			return ptr;
		index--;
	}

	return NULL;
}
//------------------------------------------------------------------------------
int32 Storage::CountArchives()
{
	return fCount;
}
//------------------------------------------------------------------------------
uint8 *Storage::LoadResource(const char *name)
{
	uint8 *resource = NULL;

	for (PakFile *ptr = fFirstArchive; ptr; ptr = ptr->fNext)
	{
		if (resource = ptr->LoadResource(name))
			return resource;
	}

	return NULL;
}
//------------------------------------------------------------------------------
GBitmap *Storage::LoadBitmap(const char *name)
{
	GBitmap *bitmap = NULL;

	for (PakFile *ptr = fFirstArchive; ptr; ptr = ptr->fNext)
	{
		if (bitmap = ptr->LoadBitmap(name))
			return bitmap;
	}

	return NULL;
}
//------------------------------------------------------------------------------
Font *Storage::LoadFont(const char *name)
{
	Font *font = NULL;

	for (PakFile *ptr = fFirstArchive; ptr; ptr = ptr->fNext)
	{
		if (font = ptr->LoadFont(name))
			return font;
	}

	return NULL;
}
//------------------------------------------------------------------------------
Movie *Storage::LoadMovie(const char *name)
{
	Movie *movie = NULL;

	for (PakFile *ptr = fFirstArchive; ptr; ptr = ptr->fNext)
	{
		if (movie = ptr->LoadMovie(name))
			return movie;
	}

	return NULL;
}
//------------------------------------------------------------------------------
GPalette *Storage::LoadPalette(const char *name)
{
	GPalette *palette = NULL;

	for (PakFile *ptr = fFirstArchive; ptr; ptr = ptr->fNext)
	{
		if (palette = ptr->LoadPalette(name))
			return palette;
	}

	return NULL;
}
//------------------------------------------------------------------------------
Sound *Storage::LoadSound(const char *name)
{
	Sound *sound = NULL;

	for (PakFile *ptr = fFirstArchive; ptr; ptr = ptr->fNext)
	{
		if (sound = ptr->LoadSound(name))
			return sound;
	}

	return NULL;
}
//------------------------------------------------------------------------------
