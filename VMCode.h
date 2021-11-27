#ifndef _VMCODE_H_
#define _VMCODE_H_

// Standard Includes -----------------------------------------------------------

// System Includes -------------------------------------------------------------

// Project Includes ------------------------------------------------------------
#include "Types.h"

// Local Includes --------------------------------------------------------------

// Local Defines ---------------------------------------------------------------

// Globals ---------------------------------------------------------------------
enum ScriptIndex {
    SCENE_SETUP		= 7,
	ONDRAW_EVENT	= 0,
    ONCLICK_EVENT	= 1, // regs[1] and regs[2] are mouse x, y regs[4] is action
	ONENTER_EVENT	= 4
};

// Class VMContext -------------------------------------------------------------
class VMContext {
public:
					VMContext(uint8 *code = NULL);
virtual				~VMContext();

		void		Load(uint8 *code);
		bool		Run(int32 index);

		void		Push(int32 value);

		void		SetReg(int32 index, int32 value);

		const char *GetString(int32 index);

private:
		void		Execute(int index);

		uint16		*fTextTable;
		int32		fTextTableCount;
		char		*fText;

		uint16		*fDataTable;
		int32		fDataTableCount;
		uint8		*fData;
		int32		fDataSize;

		int32		fIP;
		int32		fBP;
		int32		fSP;
		int32		fRET;

		int32		fRegs[32];
		int32		fStack[16];
};
//------------------------------------------------------------------------------

#endif // _VMCODE_H_
