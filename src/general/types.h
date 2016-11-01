/*
 * types.h
 *
 *  Created on: Nov 1, 2016
 *      Author: jonathan
 */

#ifndef GENERAL_TYPES_H_
#define GENERAL_TYPES_H_

#include <cstdint>
#include "debug.h"

//A bitboard is a 64bit boolean feature representation of the board.
typedef int64_t BitBoard;
//A square is defined by its index
typedef int32_t Square;
//A piecetype is a reference to one of King,Queen,Rook,Bishop,Knight or Pawn
typedef int32_t PieceType;
//A color is a reference to one of the players or to neither.
typedef int32_t Color;
//A piece is an object with a PieceType, Color and Square
typedef int32_t Piece;

const Color kWhite = 0;
const Color kBlack = 1;
const Color kEmpty = 2;

const PieceType kKing = 0;
const PieceType kQueen = 1;
const PieceType kRook = 2;
const PieceType kBishop = 3;
const PieceType kKnight = 4;
const PieceType kPawn = 5;
const PieceType kNoPiece = 6;
const int kNumPieceTypes = 7;

const int kNumPlayers = 2;


/**
 * In the following we define data extraction and translation functions,
 * however we will not be defining bit data manipulation functions.
 * For bit twiddling functions see bitoperations.h
 */

inline int32_t GetSquareX(Square square) { return square % 8; }
inline int32_t GetSquareY(Square square) { return square / 8; }

inline BitBoard GetSquareBitBoard(const Square square) { return 0x1L << square; }
inline Square GetLowestFilledSquare(BitBoard bb) {
  debug::Error("GetLowestFilledSquare(BitBoard bb) in types.h is not implemented");
  return 0;
}


#endif /* GENERAL_TYPES_H_ */