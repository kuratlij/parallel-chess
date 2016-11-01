/*
 * types.h
 *
 *  Created on: Nov 1, 2016
 *      Author: jonathan
 */

#ifndef GENERAL_TYPES_H_
#define GENERAL_TYPES_H_

#include <cstdint>

//A square is defined by its index
typedef int32_t Square;
//A bitboard is a 64bit boolean feature representation of the board.
typedef int64_t BitBoard;

typedef int32_t Color;
const Color kWhite = 0;
const Color kBlack = 1;
const Color kEmpty = 2;

typedef int32_t PieceType;
const PieceType kKing = 0;
const PieceType kQueen = 1;
const PieceType kRook = 2;
const PieceType kBishop = 3;
const PieceType kKnight = 4;
const PieceType kPawn = 5;

//A piece is an object with a PieceType, Color and Square
typedef int32_t Piece;


#endif /* GENERAL_TYPES_H_ */
