#ifndef _GAME_H_
#define _GAME_H_

// Standard Includes -----------------------------------------------------------

// System Includes -------------------------------------------------------------

// Project Includes ------------------------------------------------------------
#include "Types.h"

// Local Includes --------------------------------------------------------------

// Local Defines ---------------------------------------------------------------

// Globals ---------------------------------------------------------------------

// Class Game ------------------------------------------------------------------
class Game {
public:
				Game();
virtual			~Game();

		void	SetState(int32 index);
		void	ClearState(int32 index);
		int32	GetState(int32 index);

		void	Store(int32 index, uint8 item);
		uint8	Retrieve(int32 index);

		void	PickUp(int32 index);
		void	Drop();
		int32	Holding();

private:

		int8	fState[512];
		uint8	fStorage[64];
		int32	fHolding;
};
//------------------------------------------------------------------------------

extern Game *game;

#endif // _GAME_H_
