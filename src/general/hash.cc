/*
 * zobrist.cc
 *
 *  Created on: Dec 10, 2016
 *      Author: jonathan
 */

#include "hash.h"

namespace hash {

std::mt19937_64 rng;
HashType pieces[2][7][64];
HashType color_hash;

void initialize() {
  for (Color color = kWhite; color <= kBlack; color++) {
    for (PieceType piece_type = kKing; piece_type < kNumPieceTypes; piece_type++) {
      for (Square square = 0; square < 64; square++) {
        pieces[color][piece_type][square] = 0;
      }
    }
  }

  for (Color color = kWhite; color <= kBlack; color++) {
    for (PieceType piece_type = kKing; piece_type <= kPawn; piece_type++) {
      for (Square square = 0; square < 64; square++) {
        pieces[color][piece_type][square] = rng();
      }
    }
  }
  color_hash = rng();
}

HashType get_piece(Piece piece, Square square) {
  return pieces[GetPieceColor(piece)][GetPieceType(piece)][square];
}
HashType get_color_hash() {
  return color_hash;
}

}
