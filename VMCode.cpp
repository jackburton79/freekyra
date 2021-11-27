// Standard Includes -----------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// System Includes -------------------------------------------------------------

// Project Includes ------------------------------------------------------------
#include "Game.h"
#include "Graphics.h"
#include "Swap.h"
#include "VMCode.h"

// Local Includes --------------------------------------------------------------

// Local Defines ---------------------------------------------------------------

// Globals ---------------------------------------------------------------------

// Stack manipulation
// WARNING: Our stack grows in the opposite direction then the 80x86 one,
// so all + and - signs are switched.
// NOTE: The define of TOP can be used as l-value.
// TODO: Write debugging defines
#define PUSH(x)		(fStack[++fSP] = x)
#define POP()		(fStack[fSP--])
#define TOP()		(fStack[fSP])
#define ISEMPTY()	(fSP < 0)

//------------------------------------------------------------------------------
VMContext::VMContext(uint8 *code)
	:	fTextTable(NULL),
		fTextTableCount(NULL),
		fText(NULL),
		fDataTable(NULL),
		fDataTableCount(NULL),
		fData(NULL),
		fDataSize(NULL),
		fIP(0),
		fBP(-1),
		fSP(-1)
{
	srand(time(NULL));

	memset(fRegs, 0, 32 * sizeof(int32));
	memset(fStack, 0, 16 * sizeof(int32));

	if (code)
		Load(code);
}
//------------------------------------------------------------------------------
VMContext::~VMContext()
{
}
//------------------------------------------------------------------------------
void VMContext::Load(uint8 *code)
{
	char name[32];

	// Read FORM
	int32 i;
	for (i = 0; i < 4; i++)
		name[i] = *code++;
	name[4] = 0;

	int32 form_size = swap_l(*(uint32*)code); code += 4;

	// Read EMC2ORDR
	for (i = 0; i < 8; i++)
		name[i] = *code++;
	name[8] = 0;

	fDataTableCount = swap_l(*(uint32*)code) >> 1; code += 4;
	fDataTable = (uint16*)code; code += fDataTableCount * 2;

	// Read TEXT
	for (i = 0; i < 4; i++)
		name[i] = *code++;
	name[4] = 0;

	int32 text_size = swap_l(*(uint32*)code); code += 4;
	bool read_extra = text_size % 2 != 0;

	fTextTable = (uint16*)code;
	fTextTableCount = swap_s(*(uint16*)fTextTable) >> 1;
	fText = (char*)(code + (fTextTableCount << 1));
	code += text_size;

	if (read_extra)
		code++;

	// Read DATA
	for (i = 0; i < 4; i++)
		name[i] = *code++;
	name[4] = 0;

	fDataSize = swap_l(*(uint32*)code); code += 4;
	fData = code;
}
//------------------------------------------------------------------------------
bool VMContext::Run(int32 index)
{
	if (index >= fDataTableCount)
		return false;

	fIP = (swap_s(fDataTable[index]) << 1) + 2;
	fSP = fBP = -1;

	while (1)
	{
		unsigned char command = *(fData + fIP++);
		unsigned short arg;

		if (command & 0x80)
		{
			arg = ((command & 0x0F) << 8) | *(fData + fIP++);
			command &= 0xF0;
		}
		else if (command & 0x40)
			arg = *(fData + fIP++);
		else if (command & 0x20)
		{
			fIP++;
			arg = swap_s(*((unsigned short*)(fData + fIP)) & 0xFF7F); fIP += 2;
		}
		else
			return false;

		command &= 0x1F;

		switch (command)
		{
			case 0x0:
				// Go to line, arg is counted in words
				fIP = arg << 1;
				break;
			case 0x1:
				// Set return code
				fRET = arg;
				break;
			case 0x2:
				if (arg == 0) // Push return code
					PUSH(fRET);
				else if (arg == 1) // Push record (line and frame pointer)
				{
					int record = (((short)fBP) << 16) | ((fIP >> 1) + 1);
					PUSH(record);
					fBP = fSP;
				}
				else
					return false;
				break;
			case 0x3:
				// Push
				PUSH(arg);
				break;
			case 0x4:
				// Push
				PUSH(arg);
				break;
			case 0x5:
				// Push var
				PUSH(fRegs[arg]);
				printf("%5d Push Reg[%d] = %d\n", fIP, arg, fRegs[arg]);
				break;
			case 0x6:
				// Push frame (-)
				PUSH(fStack[fBP + arg]);
				break;
			case 0x7:
				// Push frame (+)
				PUSH(fStack[fBP - arg]);
				break;
			case 0x8:
				if (arg == 0) // Pop return code
					fRET = POP();
				else if (arg == 1) // Pop record
				{
					if (ISEMPTY())
						return true;

					int record = POP();
					
					fBP = (short)((record & 0xFFFF0000) >> 16);
					fIP = (record & 0x0000FFFF) * 2;
				}
				else
					return false;
				break;
			case 0x9:
				// Pop var
				fRegs[arg] = POP();
				printf("%5d Pop Reg[%d] = %d\n", fIP, arg, fRegs[arg]);
				break;
			case 0xA:
				// Pop frame (-)
				fStack[fBP + arg] = POP();
				break;
			case 0xB:
				// Pop frame (+)
				fStack[fBP - arg] = POP();
				break;
			case 0xC:
				// Add to SP
				fSP -= arg;
				break;
			case 0xD:
				// Sub from SP
				fSP += arg;
				break;
			case 0xE:
				Execute(arg);
				break;
			case 0xF:
				// If not go to, arg is counted in words
				if (!POP())
					fIP = arg << 1;
				break;
			case 0x10:
				// Negate
				if (arg == 0)
					TOP() = !TOP();
				else if (arg == 1)
					TOP() = -TOP();
				else if (arg == 2)
					TOP() = ~TOP();
				else
					return false;
				break;
			case 0x11:
			{
				// Evaluate
				int x, y, res;
				
				y = POP();
				x = POP();

				switch (arg)
				{
					case 0:
						res = x && y;
						break;
					case 1:
						res = x || y;
						break;
					case 2:
						res = x == y;
						printf("Evaluate(%d == %d) returned %d\n", x, y, res);
						break;
					case 3:
						res = x != y;
						break;
					case 4:
						res = x < y;
						break;
					case 5:
						res = x <= y;
						printf("Evaluate(%d <= %d) returned %d\n", x, y, res);
						break;
					case 6:
						res = x > y;
						break;
					case 7:
						res = x >= y;
						printf("Evaluate(%d >= %d) returned %d\n", x, y, res);
						break;
					case 8:
						res = x + y;
						break;
					case 9:
						res = x - y;
						break;
					case 10:
						res = x * y;
						break;
					case 11:
						res = x / y;
						break;
					case 12:
						res = x >> y;
						break;
					case 13:
						res = x << y;
						break;
					case 14:
						res = x & y;
						break;
					case 15:
						res = x | y;
						break;
					case 16:
						res = x % y;
						break;
					case 17:
						//res = x xor y;
						res = 0;
						break;
				}
				
				PUSH(res);
				break;
			}
			default:
				return false;
		}
	}
}
//------------------------------------------------------------------------------
void VMContext::Push(int32 value)
{
	PUSH(value);
}
//------------------------------------------------------------------------------
void VMContext::SetReg(int32 index, int32 value)
{
	if (index >= 16)
		return;

	fRegs[index] = value;
}
//------------------------------------------------------------------------------
const char *VMContext::GetString(int32 index)
{
	if (index >= fTextTableCount)
		return NULL;

	return ((char*)fTextTable) + swap_s(fTextTable[index]);
}
//------------------------------------------------------------------------------
void VMContext::Execute(int index)
{
	switch (index)
	{
		case 0:
			// Used in actor emc files in misc_emc.pak
			// SetShape(int32 index, int32 count, int32 ?, int32 ?,
			//		int32 ?, int32 ?, int32 ?);
			printf("0: %d, %d, %d\n", fStack[fSP], fStack[fSP - 1],
				fStack[fSP - 2]);
			break;
		case 1:
			// Used in actor emc files in misc_emc.pak
			// DrawShapeFrame(int32 frame, int32 delay);
			printf("1: %d, %d\n", fStack[fSP], fStack[fSP - 1]);
			break;
		case 2:
			// Used in start emc files in misc_emc.pak
			// DefineActor(int32 index, int32 string, int32 , int32 , int32 , int32);
			printf("DefineActor(%d, %s, %d, %d, %d, %d)\n", fStack[fSP],
				GetString(fStack[fSP - 1]), fStack[fSP - 2], fStack[fSP - 3],
				fStack[fSP - 4], fStack[fSP - 5]);
			break;
		case 3:
			printf("%5d 3: %d, %d, %d, %d\n", fIP, fStack[fSP], fStack[fSP - 1],
				fStack[fSP - 2], fStack[fSP - 3]);
			break;
		case 4:
			printf("%5d 4: %d\n", fIP, fStack[fSP]);
			break;
		case 5:
			printf("%5d 5: %d\n", fIP, fStack[fSP]);
			break;
		case 6:
			printf("%5d 6: %d\n", fIP, fStack[fSP]);
			break;
		case 8:
			printf("%5d 8: %d\n", fIP, fStack[fSP]);
			break;
		case 11:
			printf("%5d 11: %d, %d, %d\n", fIP, fStack[fSP], fStack[fSP - 1],
				fStack[fSP - 2]);
			break;
		case 12:
			printf("%5d 12: %d\n", fIP, fStack[fSP]);
			break;
		case 13:
			printf("WalkTo(%d, %d, %d, %d)\n", fStack[fSP], fStack[fSP - 1],
				fStack[fSP - 2], fStack[fSP - 3]);
			break;
		case 15:
			printf("%5d 15: %d, %d, %d, %d\n", fIP, fStack[fSP], fStack[fSP - 1],
				fStack[fSP - 2], fStack[fSP - 3]);
		case 17:
			printf("%5d 17: %d, %d\n", fIP, fStack[fSP], fStack[fSP - 1]);
			break;
		case 18:
			printf("%5d 18:\n", fIP);
			break;
		case 20:
			printf("%5d 20: %d\n", fIP, fStack[fSP]);
			break;
		case 23:
			printf("%5d 23: %d, %d, %d, %d, %d, %d, %d, %d\n", fIP,
				fStack[fSP], fStack[fSP - 1], fStack[fSP - 2], fStack[fSP - 3],
				fStack[fSP - 4], fStack[fSP - 5], fStack[fSP - 6], fStack[fSP - 7],
				fStack[fSP - 8]);
			break;
		case 24:
			printf("%5d 24: %d, %d, %d, %d, %d, %d, %d, %d\n", fIP,
				fStack[fSP], fStack[fSP - 1], fStack[fSP - 2], fStack[fSP - 3],
				fStack[fSP - 4], fStack[fSP - 5], fStack[fSP - 6], fStack[fSP - 7]);
			break;
		case 25:
			printf("%5d 25: %d, %d, %d, %d\n", fIP,
				fStack[fSP], fStack[fSP - 1], fStack[fSP - 2], fStack[fSP - 3]);
			break;
		case 31:
			printf("SetMouseBounds(%d, %d, %d, %d)\n", fStack[fSP], fStack[fSP - 1],
				fStack[fSP - 2], fStack[fSP - 3]);
			break;
		case 32:
			printf("%5d 32: %d, %d\n", fIP, fStack[fSP], fStack[fSP - 1]);
			break;
		case 35:
			printf("%5d 35: %d, %d, %d, %d\n", fIP, fStack[fSP], fStack[fSP - 1],
				fStack[fSP - 2], fStack[fSP - 3]);
			break;
		case 37:
			printf("%5d 37: %d, %d\n", fIP, fStack[fSP], fStack[fSP - 1]);
			break;
		case 39:
			fRET = game->GetState(fStack[fSP]);
			break;
		case 40:
			game->ClearState(fStack[fSP]);
			break;
		case 41:
			game->SetState(fStack[fSP]);
			break;
		case 42:
			printf("PickUp(%d)\n", fStack[fSP]);
			game->PickUp(fStack[fSP]);
			break;
		case 43:
			printf("Drop()\n");
			game->Drop();
			break;
		case 44:
			printf("%5d 44: returned 255\n", fIP);
			fRET = 255;
			break;
		case 45:
			printf("%5d 45:\n", fIP);
			break;
		case 46:
			printf("%5d 46: %d, %d\n", fIP, fStack[fSP], fStack[fSP - 1]);
			break;
		case 47:
			printf("SetMousePos(%d, %d)\n", fStack[fSP], fStack[fSP - 1]);
			break;
		case 48:
			printf("%5d 48:\n", fIP);
			break;
		case 49:
			printf("%5d 49: %d, %d, %d, %d, %d\n", fIP, fStack[fSP], fStack[fSP - 1],
				fStack[fSP - 2], fStack[fSP - 3], fStack[fSP - 4]);
			break;
		case 54:
			printf("%5d 54 %d\n", fIP, fStack[fSP]);
			break;
		case 55:
			graphics->RedrawAndWait(fStack[fSP], fStack[fSP - 1]);
			break;
		case 58:
			printf("%5d 58: %d, %d\n", fIP, fStack[fSP], fStack[fSP - 1]);
			break;
		case 59:
			printf("%5d 59: %d, %d\n", fIP, fStack[fSP], fStack[fSP - 1]);
			break;
		case 60:
			printf("%5d 60: %d, %d\n", fIP, fStack[fSP], fStack[fSP - 1]);
			break;
		case 62:
			graphics->DrawSprite(fStack[fSP], fStack[fSP - 1],
				fStack[fSP - 2], fStack[fSP - 3]);
			break;
		case 63:
			graphics->DrawSprite(fStack[fSP], fStack[fSP - 1],
				fStack[fSP - 2], fStack[fSP - 3], fStack[fSP - 4]);
			break;
		case 69:
			printf("%5d 69: %d\n", fIP, fStack[fSP]);
			break;
		case 70:
			printf("%5d 70:\n", fIP);
			break;
		case 75:
			printf("%5d 75: %d\n", fIP, fStack[fSP]);
			break;
		case 80:
			printf("%5d 80: %d, %d, %d, %d, %d\n", fIP, fStack[fSP], fStack[fSP - 1],
				fStack[fSP - 2], fStack[fSP - 3], fStack[fSP - 4]);
			break;
		case 81:
			printf("GoToRoom(%d)\n", fStack[fSP]);
			break;
		case 86:
			printf("%5d 86: %d, %d\n", fIP, fStack[fSP], fStack[fSP - 1]);
			break;
		case 88:
			printf("%5d 88: %d, %d\n", fIP, fStack[fSP], fStack[fSP - 1]);
			break;
		case 89:
			printf("%5d 89: %d\n", fIP, fStack[fSP]);
			break;
		case 92:
			printf("%5d 92: %d, %d, %d, %d\n", fIP, fStack[fSP], fStack[fSP - 1],
				fStack[fSP - 2], fStack[fSP - 3]);
			break;
		case 96:
			fRET = fStack[fSP] + rand() % (fStack[fSP - 1] - fStack[fSP] + 1);
			printf("%5d Random(%d, %d) returned %d\n", fIP, fStack[fSP],
				fStack[fSP - 1], fRET);
			break;
		case 97:
			printf("%5d 97: returned %d\n", fIP, 0);
			fRET = 0;
			break;
		case 98:
			printf("%5d 98: %d\n", fIP, fStack[fSP]);
			break;
		case 103:
			printf("%5d 103: %d, %d, %d, %d, %d, %d\n", fIP, fStack[fSP], fStack[fSP - 1],
				fStack[fSP - 2], fStack[fSP - 3], fStack[fSP - 4], fStack[fSP - 5]);
			break;
		case 108:
			graphics->DefineSprite(fStack[fSP], fStack[fSP - 1],
				fStack[fSP - 2], fStack[fSP - 3], fStack[fSP - 4]);
			break;
		case 109:
			printf("%5d 109: %d, %d, %d\n", fIP, fStack[fSP], fStack[fSP - 1],
				fStack[fSP - 2]);
			break;
		case 112:
			graphics->DefineAnimatedSprite(fStack[fSP], fStack[fSP - 1],
				fStack[fSP - 2], fStack[fSP - 3], fStack[fSP - 4],
				fStack[fSP - 5], fStack[fSP - 6], fStack[fSP - 7],
				fStack[fSP - 8], fStack[fSP - 9], fStack[fSP - 10],
				fStack[fSP - 11], GetString(fStack[fSP - 12]));
			break;
		case 110:
			printf("PlayEMC(%s, %d, %d, %d)\n", GetString(fStack[fSP]),
				fStack[fSP - 1], fStack[fSP - 2], fStack[fSP - 3]);
			break;
		case 111:
			// Index of animation, delay
			printf("%5d 111: %d, %d\n", fIP, fStack[fSP], fStack[fSP - 1]);
			graphics->RedrawAndWait(fStack[fSP - 1], 0);
			break;
		case 114:
			graphics->DrawAnimatedSprite(fStack[fSP], fStack[fSP - 1]);
			break;
		case 117:
			printf("%5d 117: %d\n", fIP, fStack[fSP]);
			break;
		case 118:
			printf("%5d 118:\n", fIP);
			break;
		case 119:
			printf("%5d 119: %d\n", fIP, fStack[fSP]);
			break;
		case 121:
			// DefineRoom(int32 this_index, int32 file_index, int32 up_index,
			//		int32 right_index, int32 down_index, int32 left_index,
			//		int32 flags /*usually 0x1 | 0x2*/, int32 unknown);
			printf("DefineRoom(%d, %s, %d, %d, %d, %d, %d, %d)\n",
				fStack[fSP], GetString(fStack[fSP - 1]), fStack[fSP - 2], fStack[fSP - 3],
				fStack[fSP - 4], fStack[fSP - 5], fStack[fSP - 6], fStack[fSP - 7]);
			break;
		case 122:
			printf("%5d 122: %d, %d\n", fIP, fStack[fSP], fStack[fSP - 1]);
			break;
		case 128:
			printf("DrawString(%s, %d)\n", GetString(fStack[fSP]), fStack[fSP - 1]);
			graphics->DrawString(GetString(fStack[fSP]), 10, 10, 124);
			break;
		case 127:
			printf("%5d 136: %d\n", fIP, fStack[fSP]);
			break;
		case 129:
			printf("%5d 129: %d, %d\n", fIP, fStack[fSP], fStack[fSP - 1]);
			break;
		case 134:
			printf("%5d 134: %d\n", fIP, fStack[fSP]);
			break;
		case 135:
			printf("%5d 135: %d, %d\n", fIP, fStack[fSP], fStack[fSP - 1]);
			break;
		case 136:
			printf("%5d 136: %d\n", fIP, fStack[fSP]);
			break;
		case 138:
			printf("%5d 138: actor=%d\n", fIP, fStack[fSP]);
			break;
		case 139:
			printf("DrawString2(%s, actor=%d)\n", GetString(fStack[fSP]), fStack[fSP - 1]);
			graphics->DrawString(GetString(fStack[fSP]), 10, 10, 96);
			break;
		case 140:
			printf("%5d 140: actor=%d\n", fIP, fStack[fSP]);
			break;
		case 142:
			// 1 is book, 2 is cauldron
			printf("ShowBookOrCauldron(%d)\n", fStack[fSP]);
			break;
		case 143:
			// Stops an animation script, index 0 is Ordr[8]
			printf("%5d StopAnimation(%d)\n", fIP, fStack[fSP]);
			break;
		case 144:
			// Runs an animation script, index 0 is Ordr[8]
			printf("%5d StartAnimation(%d)\n", fIP, fStack[fSP]);
			break;
		case 145:
			// Checks if an animation script has finished, index 0 is Ordr[8]
			printf("%5d IsAnimationFinished(%d) returned 1\n", fIP, fStack[fSP]);
			fRET = 1;
			break;
		case 146:
			printf("%5d 146:\n", fIP);
			break;
		case 147:
			// Most likely a function to put an object into a storage
			// space in the game, like holes in trees.
			game->Store(fStack[fSP], fStack[fSP - 1]);
			printf("Store(%d, %d)\n", fStack[fSP], fStack[fSP - 1]);
			break;
		case 148:
			// Most likely a function to check a storage space in the game,
			// like holes in trees.
			fRET = game->Retrieve(fStack[fSP]);
			printf("Retrieve(%d) returned %d\n", fStack[fSP], fRET);
			break;
		case 151:
			printf("%5d 151: %d, %d\n", fIP, fStack[fSP], fStack[fSP - 1]);
			break;
		case 152:
			printf("%5d 152: %d, %d\n", fIP, fStack[fSP], fStack[fSP - 1]);
			break;
		case 153:
			printf("%5d 153:\n", fIP);
			break;
		case 163:
			printf("%5d 163: %d\n", fIP, fStack[fSP]);
			break;
		default:
			printf("%5d Execute: %d not implemented\n", fIP, index);
			break;
	}
}
//------------------------------------------------------------------------------
