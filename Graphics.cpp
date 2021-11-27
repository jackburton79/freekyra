// Standard Includes -----------------------------------------------------------
#include <stdio.h>
#include <string.h>
// System Includes -------------------------------------------------------------
#include <Bitmap.h>
#include <View.h>
#include <Window.h>

// Project Includes ------------------------------------------------------------
#include "Graphics.h"
#include "Storage.h"

// Local Includes --------------------------------------------------------------

// Local Defines ---------------------------------------------------------------

// Globals ---------------------------------------------------------------------
Graphics *graphics = NULL;

extern BWindow *window;
static BView *GetDC(HWND win)
{
	BView *view = win->FindView("view");
	return view;
}
//------------------------------------------------------------------------------
Graphics::Graphics(HWND hWnd)
	:	fBitmap(NULL),
		fFont(NULL),
		fhWnd(hWnd),
		fhDC(NULL),
		fhBitmap(NULL),
		fOldhBitmap(NULL)
{
	memset(fSprites, 0, kMaxSprite * sizeof(Sprite));
	memset(fAnimatedSprites, 0, 16 * sizeof(AnimatedSprite));

	// Setup DC
	HDC hDC = GetDC(fhWnd);
	
	fhBitmap = new BBitmap(BRect(0, 0, 319, 199), B_CMAP8);
	fBits = (uint8*)fhBitmap->Bits();
	
}
//------------------------------------------------------------------------------
Graphics::~Graphics()
{
	if (fBitmap)
		delete fBitmap;

	/*SelectObject(fhDC, fOldhBitmap);
	DeleteObject(fhBitmap);
    DeleteDC(fhDC);
	free(fInfo);
*/
}
//------------------------------------------------------------------------------
void Graphics::DrawBitmap(GBitmap *bitmap)
{
	memcpy(fBits, bitmap->fBits, 320 * 200);
}
//------------------------------------------------------------------------------
void Graphics::DrawBits(uint8 *bits, int32 src_w, int32 src_h,
						int32 dst_x, int32 dst_y)
{
	uint8 *src = bits;
	uint8 *dst = fBits + dst_y * 320 + dst_x;

	for (int32 j = 0; j < src_h; j++)
	{
		for (int32 i = 0; i < src_w; i++)
		{
			if (*src != 0)
				*dst = *src;
			src++;
			dst++;
		}
		dst += 320 - src_w;
	}
}
//------------------------------------------------------------------------------
void Graphics::SetBitmap(GBitmap *bitmap)
{
	if (fBitmap)
		delete fBitmap;

	fBitmap = bitmap;
}//------------------------------------------------------------------------------
void Graphics::SetFont(Font *font)
{
	fFont = font;
}
//------------------------------------------------------------------------------
void Graphics::SetPalette(GPalette *palette)
{
	//SetDIBColorTable(fhDC, 0, 256, palette->Colors());
	
}
//------------------------------------------------------------------------------
void Graphics::DrawBackground()
{
	if (fBitmap)
		memcpy(fBits, fBitmap->fBits, 320 * 200);
}
//------------------------------------------------------------------------------
void Graphics::Draw(HDC hDC)
{
	hDC->DrawBitmap(fhBitmap);
}	
//------------------------------------------------------------------------------
void Graphics::DefineSprite(int32 index, int32 x, int32 y, int32 w, int32 h)
{
	printf("DefineSprite(%d, %d, %d, %d, %d)\n", index, x, y, w, h);

	if (index >= kMaxSprite)
		printf("DefineSprite: index %d too big\n", index);

	fSprites[index].x = x;
	fSprites[index].y = y;
	fSprites[index].w = w;
	fSprites[index].h = h;
}
//------------------------------------------------------------------------------
void Graphics::DefineAnimatedSprite(int32 index, int32 flags, int32 u1,
									int32 u2, int32 x, int32 y, int32 w,
									int32 h, int32 u5, int32 u6, int32 u7,
									int32 u8, const char *name)
{
	printf("DefineAnimatedSprite(%d, %x, %d, %d, %d, %d, %d, %d, %d, %d, %d, "
		"%d, %s)\n", index, flags, u1, u2, x, y, w, h, u5, u6, u7, u8, name);

	if (index >= 16)
		printf("DefineAnimatedSprite: index %d too big\n", index);

	fAnimatedSprites[index].movie = (WSAMovie*)storage->LoadMovie(name);

	fAnimatedSprites[index].flags = flags;
	fAnimatedSprites[index].x = x;
	fAnimatedSprites[index].y = y;
	fAnimatedSprites[index].w = fAnimatedSprites[index].movie->Width();
	fAnimatedSprites[index].h = fAnimatedSprites[index].movie->Height();
}
//------------------------------------------------------------------------------
void Graphics::DrawSprite(int32 index, int32 x, int32 y, int32 u1)
{
	printf("DrawSprite(%d, %d, %d)\n", index, x, y, u1);

	if (index >= kMaxSprite)
	{
		printf("DrawSprite: index too big\n");
		return;
	}

	if (fSprites[index].w == 0 || fSprites[index].h == 0)
	{
		printf("DrawSprite: sprite invalid at index %d\n", index);
		return;
	}

	Sprite *sprite = &fSprites[index];

	// Draw sprite
	uint8 *src = fBitmap->fBits + sprite->y * 320 + sprite->x;
	uint8 *dst = fBits + y * 320 + x;

	for (int32 j = 0; j < sprite->h; j++)
	{
		for (int32 i = 0; i < sprite->w; i++)
		{
			if (*src != 0)
				*dst = *src;
			src++;
			dst++;
		}
		src += 320 - sprite->w;
		dst += 320 - sprite->w;
	}
}
//------------------------------------------------------------------------------
void Graphics::DrawSprite(int32 index, int32 x, int32 y, int32 u1, int32 u2)
{
	printf("DrawSprite(%d, %d, %d)\n", index, x, y, u1, u2);

	if (index >= 16)
	{
		printf("DrawSprite: index too big\n");
		return;
	}

	if (fSprites[index].w == 0 || fSprites[index].h == 0)
	{
		printf("DrawSprite: sprite invalid at index %d\n", index);
		return;
	}

	Sprite *sprite = &fSprites[index];

	// Draw sprite
	uint8 *src = fBitmap->fBits + sprite->y * 320 + sprite->x;
	uint8 *dst = fBits + y * 320 + x;

	for (int32 j = 0; j < sprite->h; j++)
	{
		for (int32 i = 0; i < sprite->w; i++)
		{
			if (*src != 0)
				*dst = *src;
			src++;
			dst++;
		}
		src += 320 - sprite->w;
		dst += 320 - sprite->w;
	}
}
//------------------------------------------------------------------------------
void Graphics::DrawAnimatedSprite(int32 index, int32 frame)
{
	printf("DrawAnimatedSprite(%d, %d)\n", index, frame);

	if (index >= 16)
	{
		printf("DrawAnimatedSprite: index too big\n");
		return;
	}

	AnimatedSprite *sprite = &fAnimatedSprites[index];

	if (sprite->w == 0 || sprite->h == 0 || sprite->movie == NULL)
	{
		printf("DrawAnimatedSprite: sprite invalid at index %d\n", index);
		return;
	}

	if (frame == 255)
	{
		uint8 *src = fBitmap->fBits + sprite->y * 320 + sprite->x;
		uint8 *dst = fBits + sprite->y * 320 + sprite->x;

		for (int32 y = 0; y < sprite->h; y++)
		{
			for (int32 x = 0; x < sprite->w; x++)
			{
				*dst = *src;
				src++;
				dst++;
			}
			src += 320 - sprite->w;
			dst += 320 - sprite->w;
		}

		return;
	}
	else if (frame >= fAnimatedSprites[index].movie->CountFrames())
	{
		printf("DrawAnimatedSprite: frame too big\n");
		return;
	}

	// Draw sprite
	uint8 *src1 = sprite->movie->GetFrame(frame);
	uint8 *src2 = fBitmap->fBits + sprite->y * 320 + sprite->x;
	uint8 *dst = fBits + sprite->y * 320 + sprite->x;

	for (int32 y = 0; y < sprite->h; y++)
	{
		for (int32 x = 0; x < sprite->w; x++)
		{
			*dst = (*src1 == 0) ? *src2 : *src1;
			src1++;
			src2++;
			dst++;
		}
		src2 += 320 - sprite->w;
		dst += 320 - sprite->w;
	}
}
//------------------------------------------------------------------------------
void Graphics::DrawString(const char *string, int32 x, int32 y, uint8 color)
{
	if (fFont)
	{
		int32 xPos = x;
		int8 yPos;
		int8 width;
		int8 height;

		for (const char *s = string; *s; s++)
		{
			uint8 *src = fFont->GetChar(*((uint8*)s), &width, &height, &yPos);
			uint8 *dst = fBits + (y + yPos) * 320 + xPos;
			uint8 index;

			for (int32 j = 0; j < height; j++)
			{
				for (int32 i = 0; i < width / 2; i++)
				{
					index = *src & 0x0F;

					if (index == 0)
						dst++;
					else if (index == 1)
						*dst++ = color;
					else if (index == 3)
						*dst++ = 0;

					index = (*src & 0xF0) >> 4;
					
					if (index == 0)
						dst++;
					else if (index == 1)
						*dst++ = color;
					else if (index == 3)
						*dst++ = 0;

					src++;
				}

				if (width & 0x1)
				{
					index = (*src++ & 0x0F);
					
					if (index == 0)
						dst++;
					else if (index == 1)
						*dst++ = color;
					else if (index == 3)
						*dst++ = 0;
				}

				dst += 320 - width;
			}

			xPos += width;
		}
	}
	else
	{
		printf("%s\n", string);
	}
}
//------------------------------------------------------------------------------
void Graphics::RedrawAndWait(int32 delay, int32 u1)
{
	printf("RedrawAndWait(%d, %d)\n", delay, u1);

	
	HDC hDC = GetDC(fhWnd);
	Draw(hDC);
	//ReleaseDC(fhWnd, hDC);
	
	//Sleep(delay * 10);
}
//------------------------------------------------------------------------------
