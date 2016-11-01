/*
 * board.h
 *
 *  Created on: Nov 1, 2016
 *      Author: jonathan
 */

#ifndef BOARD_H_
#define BOARD_H_

#include "general/types.h"

class Board{
public:
  Board();
private:
  Piece piece_bitboards[kNumPlayers][kNumPieceTypes];
};



#endif /* BOARD_H_ */
