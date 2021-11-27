#ifndef _DECODE_H_
#define _DECODE_H_

// Standard Includes -----------------------------------------------------------

// System Includes -------------------------------------------------------------

// Project Includes ------------------------------------------------------------
#include "Types.h"

// Local Includes --------------------------------------------------------------

// Local Defines ---------------------------------------------------------------

// Globals ---------------------------------------------------------------------

//------------------------------------------------------------------------------

int decode80(const uint8 image_in[], uint8 image_out[]);
int decode80_3(const uint8 image_in[], uint8 image_out[], const int32 size);
int decode40(const uint8 image_in[], uint8 image_out[]);
int Decode00(const uint8 image_in[], uint8 image_out[], int32 size);

//------------------------------------------------------------------------------

#endif // _DECODE_H_
