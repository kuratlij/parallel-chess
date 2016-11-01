/*
 * board.cc
 *
 *  Created on: Nov 1, 2016
 *      Author: jonathan
 */

#include "board.h"

Board::Board() {
  for (int player = kWhite; player <= kBlack; player++) {
    for (int piece_type = 0; piece_type < kNumPieceTypes; piece_type++) {
      piece_bitboards[player][piece_type] = 0;
    }
  }
  debug::Print("constructed board", debug::kDebugBoard);
}


