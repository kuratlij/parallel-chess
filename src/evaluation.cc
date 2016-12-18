/*
 * evaluation.cc
 *
 *  Created on: Nov 6, 2016
 *      Author: jonathan
 */

#include "evaluation.h"

namespace {

int kSign[2] = {1,-1};
const Score kBaseCentipawnValues[6] = {0, 960, 480, 310, 300, 100};
const Score kActivityBonus[6] = {0, 1, 4, 5, 10, 0};
const Score kPawnPushBonus = 3;
const Score kDoublePawnPenalty = -30;
}

namespace evaluation {

Score ScoreBoard(Board board) {
  PieceBitboardSet set = board.get_piece_bitboards();
  Score score = 0;
  BitBoard pieces[] = {0, 0};
  for (PieceType piece = kKing; piece <= kPawn; piece++) {
    pieces[kWhite] |= set.piece_bitboards[kWhite][piece];
    pieces[kBlack] |= set.piece_bitboards[kBlack][piece];
  }
  BitBoard all_pieces = pieces[kWhite] | pieces[kBlack];
  for (Color color = kWhite; color <= kBlack; color++) {
    for (PieceType piece_type = kQueen; piece_type <= kPawn; piece_type++) {
      score += kSign[color] * kBaseCentipawnValues[piece_type]
               * bitops::PopCount(set.piece_bitboards[color][piece_type]);
    }

    for (PieceType piece_type = kQueen; piece_type <= kKnight; piece_type++) {
      BitBoard pieces = set.piece_bitboards[color][piece_type];
      while (pieces) {
        BitBoard piece = bitops::GetLSB(pieces);
        Square piece_square = bitops::NumberOfTrailingZeros(piece);
        BitBoard attack_map = magic::GetAttackMap(piece_type, piece_square, all_pieces);
        score += kSign[color] * bitops::PopCount(attack_map) * kActivityBonus[piece_type];
        bitops::PopLSB(pieces);
      }
    }
  }
  BitBoard s_filled = bitops::FillSouth(set.piece_bitboards[kWhite][kPawn], ~0);
  score += kSign[kWhite] * bitops::PopCount(s_filled) * kPawnPushBonus;
  score += kSign[kWhite] * bitops::PopCount(s_filled & set.piece_bitboards[kWhite][kPawn])
      * kDoublePawnPenalty;
  BitBoard n_filled = bitops::FillNorth(set.piece_bitboards[kBlack][kPawn], ~0);
  score += kSign[kBlack] * bitops::PopCount(n_filled) * kPawnPushBonus;
  score += kSign[kBlack] * bitops::PopCount(n_filled & set.piece_bitboards[kBlack][kPawn])
      * kDoublePawnPenalty;


  if (board.get_turn() == kWhite) {
    return score;
  }
  return -score;
}

}
