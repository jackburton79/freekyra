#ifndef _SOUND_H_
#define _SOUND_H_

// Standard Includes -----------------------------------------------------------

// System Includes -------------------------------------------------------------

// Project Includes ------------------------------------------------------------
#include "Types.h"

// Local Includes --------------------------------------------------------------

// Local Defines ---------------------------------------------------------------

// Globals ---------------------------------------------------------------------

// Class Sound -----------------------------------------------------------------
class Sound {
public:
				Sound(uint8 *data);
virtual			~Sound();

private:

//friend void CALLBACK waveOutCallback(HWAVEOUT waveOut, UINT uMsg,
//									 DWORD dwInstance, DWORD dwParam1,
//									 DWORD dwParam2);
//		WAVEHDR	fHeader;
};
//------------------------------------------------------------------------------

#endif // _SOUND_H_
