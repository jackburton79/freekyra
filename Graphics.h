#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

// Standard Includes -----------------------------------------------------------

// System Includes -------------------------------------------------------------

// Project Includes ------------------------------------------------------------
#include "Bitmap.h"
#include "Font.h"
#include "WSAMovie.h"
#include "Palette.h"
#include "Types.h"

// Local Includes --------------------------------------------------------------

// Local Defines ---------------------------------------------------------------
const int32 kMaxSprite = 32;

// Globals ---------------------------------------------------------------------
struct Sprite {
	int32 x;
	int32 y;
	int32 w;
	int32 h;
};

struct AnimatedSprite {
	int32 flags;
	int32 x;
	int32 y;
	int32 w;
	int32 h;
	WSAMovie *movie;
};

typedef BWindow* HWND;
typedef BView* HDC;
typedef BBitmap* HBITMAP;
// Class Graphics --------------------------------------------------------------
class Graphics {
public:
					Graphics(HWND hWnd);
virtual				~Graphics();

		void		DrawBitmap(GBitmap *bitmap);
		void		DrawBits(uint8 *bits, int32 src_w, int32 src_h,
						int32 dst_x, int32 dst_y);

		void		SetBitmap(GBitmap *bitmap);
		void		SetFont(Font *font);
		void		SetPalette(GPalette *palette);

		void		DrawBackground();
		void		Draw(HDC hDC);

		void		DefineSprite(int32 index, int32 x, int32 y, int32 w,
						int32 h);
		void		DefineAnimatedSprite(int32 index, int32 flags, int32, int32,
						int32 x, int32 y, int32, int32, int32, int32, int32,
						int32, const char *name);

		void		DrawSprite(int32 index, int32 x, int32 y, int32);
		void		DrawSprite(int32 index, int32 x, int32 y, int32, int32);
		void		DrawAnimatedSprite(int32 index, int32 frame);

		void		DrawString(const char *string, int32 x, int32 y, uint8 color);

		void		RedrawAndWait(int32 delay, int32 u1);

private:
		GBitmap			*fBitmap;
		Font			*fFont;
		Sprite			fSprites[kMaxSprite];
		AnimatedSprite	fAnimatedSprites[16];

		HWND		fhWnd;
		HDC			fhDC;
		//BITMAPINFO	*fInfo;
		uint8		*fBits;
		HBITMAP		fhBitmap;
		HBITMAP		fOldhBitmap;
};
//------------------------------------------------------------------------------

extern Graphics *graphics;

#endif // _GRAPHICS_H_
