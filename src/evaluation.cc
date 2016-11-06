/*
 * evaluation.cc
 *
 *  Created on: Nov 6, 2016
 *      Author: jonathan
 */

#include "evaluation.h"

namespace {

int kSign[2] = {1,-1};
Score kBaseCentipawnValues[6] = {0, 950, 500, 320, 310, 100};

}

namespace evaluation {

Score ScoreBoard(Board board) {
  PieceBitboardSet set = board.get_piece_bitboards();
  Score score = 0;
  for (Color color = kWhite; color < kBlack; color++) {
    for (PieceType piece_type = kQueen; piece_type <= kPawn; piece_type++) {
      score += kSign[color] * kBaseCentipawnValues[piece_type]
               * bitops::PopCount(set.piece_bitboards[color][piece_type]);
    }
  }
  if (board.get_turn() == kWhite) {
    return score;
  }
  return -score;
}

}
