// Standard Includes -----------------------------------------------------------
#include <string.h>
// System Includes -------------------------------------------------------------

// Project Includes ------------------------------------------------------------
#include "Decode.h"
#include "Swap.h"

// Local Includes --------------------------------------------------------------

// Local Defines ---------------------------------------------------------------

// Globals ---------------------------------------------------------------------

//------------------------------------------------------------------------------
int decode80(const uint8 image_in[], uint8 image_out[]) 
{
    /*
    0 copy 0cccpppp p
    1 copy 10cccccc
    2 copy 11cccccc p p
    3 fill 11111110 c c v
    4 copy 11111111 c c p p
    */

    const uint8 *copyp;
    const uint8 *readp = image_in;
    uint8 *writep = image_out;
    uint16 code;
    uint16 count;

    while (true)
	{
        code = *readp++;
        if (~code & 0x80)
		{
            //bit 7 = 0
            //command 0 (0cccpppp p): copy
            count = (code >> 4) + 3;
            copyp = writep - (((code & 0xf) << 8) + *readp++);
            while (count--)
                *writep++ = *copyp++;
        }
		else
		{
            //bit 7 = 1
            count = code & 0x3f;
            if (~code & 0x40)
			{
                //bit 6 = 0
                if (count == 0)
                    //end of image
                    break;
                //command 1 (10cccccc): copy
                while (count--)
                    *writep++ = *readp++;
            }
			else
			{
                //bit 6 = 1
                if (count < 0x3e)
				{
                    //command 2 (11cccccc p p): copy
                    count += 3;
                    copyp = &image_out[*(uint16*)readp];
                    readp += 2;
                    while (count--)
                        *writep++ = *copyp++;
                }
				else if (count == 0x3e)
				{
                    //command 3 (11111110 c c v): fill
                    count = *(uint16*)readp;
                    readp += 2;
                    code = *readp++;
                    while (count--)
                        *writep++ = (uint8)code;
                }
				else
				{
                    //command 4 (copy 11111111 c c p p): copy
                    count = *(uint16*)readp;
                    readp += 2;
                    copyp = &image_out[*(uint16*)readp];
                    readp += 2;
                    while (count--)
                        *writep++ = *copyp++;
                }
            }
        }
    }

    return writep - image_out;
}
//------------------------------------------------------------------------------
int decode80_3(const uint8 image_in[], uint8 image_out[], const int32 size)
{	
	/*
    0 copy
    1 fill 
    2 fill 
    */

    const uint8 *readp = image_in;
    uint8 *writep = image_out;
    int16 code;
    int16 count;
	
	do
	{
        code = *(int8*)readp++;
        if (code > 0)
		{  // Copy 
           	count = code;
        	while (count--)
        		*writep++ = *readp++;        
        }
		else if (code == 0)
		{ // Fill(1) 
           	count = swap_s(*(uint16 *)readp);   	
        	readp += 2;
        	code = *readp++;
        	while (count--)
        		*writep++ = (uint8)code;
        }
		else if (code < 0)
		{ // Fill (2)
           	count = -code;
        	code = *readp++;
        	while (count--)
        		*writep++ = (uint8)code;
        }
    } while ((writep - image_out) < size);	
    
    //and, to be uniform to other decomp. functions...				
    return writep - image_out; 
}
//------------------------------------------------------------------------------
int decode40(const uint8 image_in[], uint8 image_out[])
{
    /*
    0 fill 00000000 c v
    1 copy 0ccccccc
    2 skip 10000000 c 0ccccccc
    3 copy 10000000 c 10cccccc
    4 fill 10000000 c 11cccccc v
    5 skip 1ccccccc
    */

    const uint8 *readp = image_in;
    uint8 *writep = image_out;
    uint16 code;
    uint16 count;

    while (true)
	{
        code = *readp++;
        if (~code & 0x80)
		{
           //bit 7 = 0
            if (code == 0)
			{
                //command 0 (00000000 c v): fill
                count = *readp++;
                code = *readp++;
                while (count--)
                    *writep++ ^= code;
            }
			else
			{
                //command 1 (0ccccccc): copy
                count = code;
                while (count--)
                    *writep++ ^= *readp++;
            }
        }
		else
		{
            //bit 7 = 1
            if (!(count = code & 0x7f))
			{
                count = *(uint16*)readp;
                readp += 2;
                code = count >> 8;
                if (~code & 0x80)
				{
                    //bit 7 = 0
                    //command 2 (10000000 c 0ccccccc): skip
                    if (!count)
                        // end of image
                        break;
                    writep += count;
                } else
				{
                    //bit 7 = 1
                    count &= 0x3fff;
                    if (~code & 0x40) {
                        //bit 6 = 0
                        //command 3 (10000000 c 10cccccc): copy
                        while (count--)
                            *writep++ ^= *readp++;
                    }
					else
					{
                        //bit 6 = 1
                        //command 4 (10000000 c 11cccccc v): fill
                        code = *readp++;
                        while (count--)
                            *writep++ ^= code;
                    }
                }
            }
            else //command 5 (1ccccccc): skip
            	writep += count;       
        }
    }
    return writep - image_out;
}
//------------------------------------------------------------------------------
int Decode00(const uint8 image_in[], uint8 image_out[], int32 size)
{
	/*
	Every value is stored as it is, except for zeroes. A zero is coded as 00nn
	with nn the amount of actual zeroes.
	*/

	const uint8 *end = image_in + size;

	while (image_in < end)
	{
		uint8 val = *image_in++;

		if (val)
			*image_out++ = val;
		else
		{
			uint8 count;
			count = *image_in++;

			if (count == 0)
				return 0;

			memset(image_out, 0, count);
			image_out += count;
		}
	}

	return 0;
}
//------------------------------------------------------------------------------