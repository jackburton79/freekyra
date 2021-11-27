// Standard Includes -----------------------------------------------------------

// System Includes -------------------------------------------------------------
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

// Project Includes ------------------------------------------------------------
#include "PakFile.h"
#include "SHPMovie.h"
#include "WSAMovie.h"

// Local Includes --------------------------------------------------------------

// Local Defines ---------------------------------------------------------------

// Globals ---------------------------------------------------------------------

//------------------------------------------------------------------------------
PakFile::PakFile(const char *file)
	:	fBuffer(NULL),
		fSize(0),
		fFirstChunk(NULL),
		fLastChunk(NULL),
		fNext(NULL)
{
	int fd = open(file, O_BINARY, S_IREAD);

	if (fd == -1)
	{
		printf("failed to find file \"%s\"\n", file);
		return;
	}

	struct stat st;

	if (fstat(fd, &st) != 0)
	{
		printf("failed to query file \"%s\"\n", file);
		close(fd);
		return;
	}

	fSize = st.st_size;
	fBuffer = new uint8[fSize];

	read(fd, fBuffer, fSize);

	close(fd);

	ReadDirectory();
}
//------------------------------------------------------------------------------
PakFile::PakFile(uint8 *buffer, uint32 size)
	:	fBuffer(buffer),
		fSize(size),
		fFirstChunk(NULL),
		fLastChunk(NULL),
		fNext(NULL)
{
	ReadDirectory();
}
//------------------------------------------------------------------------------
PakFile::~PakFile()
{
	PakChunk *chunk = fFirstChunk;
	
	while (chunk)
	{
		PakChunk *deleteChunk = chunk;
		chunk = chunk->fNext;
		delete deleteChunk;
	}

	if (fBuffer)
		delete fBuffer;
}
//------------------------------------------------------------------------------
uint8 *PakFile::LoadResource(const char *name)
{
	for (PakChunk *ptr = fFirstChunk; ptr; ptr = ptr->fNext)
	{
		if (strcasecmp(ptr->fName, name) == 0)
			return (uint8*)ptr->fBuffer;
	}

	return NULL;
}
//------------------------------------------------------------------------------
Font *PakFile::LoadFont(const char *name)
{
	uint8 *resource = LoadResource(name);
		
	if (resource)
		return new Font(resource);
	else
		return NULL;
}
//------------------------------------------------------------------------------
GBitmap *PakFile::LoadBitmap(const char *name)
{
	uint8 *resource = LoadResource(name);
		
	if (resource)
		return new GBitmap(resource);
	else
		return NULL;
}
//------------------------------------------------------------------------------
Movie *PakFile::LoadMovie(const char *name)
{
	uint8 *resource = LoadResource(name);
		
	if (resource)
	{
		int32 len = name ? strlen(name) : 0;
		if (len < 3)
			return NULL;

		if (name[len - 3] == 'W' && name[len - 2] == 'S' &&
			name[len - 1] == 'A')
			return new WSAMovie(resource);
		else if (name[len - 3] == 'S' && name[len - 2] == 'H' &&
			name[len - 1] == 'P')
			return new SHPMovie(resource);
		else
			return NULL;
	}
	else
		return NULL;
}
//------------------------------------------------------------------------------
GPalette *PakFile::LoadPalette(const char *name)
{
	uint8 *resource = LoadResource(name);
		
	if (resource)
		return new GPalette(resource);
	else
		return NULL;
}
//------------------------------------------------------------------------------
Sound *PakFile::LoadSound(const char *name)
{
	uint8 *resource = LoadResource(name);
		
	if (resource)
		return new Sound(resource);
	else
		return NULL;
}
//------------------------------------------------------------------------------
void PakFile::EnumResources(EnumResourcesCallback callback, void *userdata)
{
	for (PakChunk *ptr = fFirstChunk; ptr; ptr = ptr->fNext)
		if (!(*callback)(this, ptr->fName, userdata))
			break;
}
//------------------------------------------------------------------------------
const char *PakFile::NameAt(int32 index)
{
	for (PakChunk *ptr = fFirstChunk; ptr; ptr = ptr->fNext)
	{
		if (index == 0)
			return ptr->fName;
		index--;
	}

	return NULL;
}
//------------------------------------------------------------------------------
void PakFile::ReadDirectory()
{
	unsigned int pos = 0, start_offset, end_offset;
	char *chunk_name;

	start_offset = *(int*)(fBuffer + pos); pos += 4;

	while (pos < fSize)
	{
		chunk_name = (char*)(fBuffer + pos); pos += strlen(chunk_name) + 1;

		if (*chunk_name == NULL)
			break;

		end_offset = *(int*)(fBuffer + pos); pos += 4;

		if (fLastChunk)
		{
			fLastChunk->fNext = new PakChunk;
			fLastChunk->fNext->fName = chunk_name;
			fLastChunk->fNext->fBuffer = fBuffer + start_offset,
			fLastChunk->fNext->fSize = end_offset - start_offset;
			fLastChunk->fNext->fNext = NULL;
			fLastChunk->fNext->fPrevious = NULL;
			fLastChunk = fLastChunk->fNext;
		}
		else
		{
			fFirstChunk = fLastChunk = new PakChunk;
			fFirstChunk->fName = chunk_name;
			fFirstChunk->fBuffer = fBuffer + start_offset,
			fFirstChunk->fSize = end_offset - start_offset;
			fFirstChunk->fNext = NULL;
			fFirstChunk->fPrevious = NULL;
		}

		start_offset = end_offset;
	}
}
//------------------------------------------------------------------------------

