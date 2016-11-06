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
typedef u_int64_t BitBoard;
//A square is defined by its index
typedef int32_t Square;
//A move references a source, a destination square and a move type, but no piece.
typedef int32_t Move;
//A movetype references whether a move is a normal move or a special move
//such as a pawn promotion or castling.
typedef Move MoveType;
//A piecetype is a reference to one of King,Queen,Rook,Bishop,Knight or Pawn
typedef int32_t PieceType;
//A color is a reference to one of the players.
typedef int32_t Color;
//A piece is an object with a PieceType, Color and Square
typedef int32_t Piece;
//A score is a value assigned to a position which represents how good the position
//for the player to move.
typedef int32_t Score;

const Color kWhite = 0;
const Color kBlack = 1;

const PieceType kKing = 0;
const PieceType kQueen = 1;
const PieceType kRook = 2;
const PieceType kBishop = 3;
const PieceType kKnight = 4;
const PieceType kPawn = 5;
const PieceType kNoPiece = 6;
const int kNumPieceTypes = 7;

const int kNumPlayers = 2;

const int kBoardLength = 8;

const MoveType kNormalMove = 0;
const MoveType kCastle = 1;
const MoveType kDoublePawnMove = 2;
const MoveType kEnPassant = 3;
const MoveType kCapture = 4;
//To get promotion piece we can subtract (kQeenPromotion - 1)
const MoveType kQueenPromotion = 5;
const MoveType kRookPromotion = 6;
const MoveType kBishopPromotion = 7;
const MoveType kKnightPromotion = 8;


/**
 * In the following we define data extraction and translation functions,
 * however we will not be defining bit data manipulation functions.
 * For bit twiddling functions see bitoperations.h
 */
inline int32_t GetSquareX(const Square square) { return square % 8; }
inline int32_t GetSquareY(const Square square) { return square / 8; }

inline Square GetMoveSource(const Move move) { return (move >> 6) & 0x3F; }
inline Square GetMoveDestination(const Move move) { return move & 0x3F; }
inline MoveType GetMoveType(const Move move) { return move >> 12; }
inline Move GetMove(Square source, Square destination, MoveType move_type = 0) {
  return move_type << 12 | (source << 6) | destination;
}

inline Piece GetPiece(Color color, PieceType piece_type) {
  return (color << 3) | piece_type;
}
inline Color GetPieceColor(Piece piece) { return piece >> 3; }
inline PieceType GetPieceType(Piece piece) { return piece & 0x7; }

inline BitBoard GetSquareBitBoard(const Square square) { return 0x1L << square; }


#endif /* GENERAL_TYPES_H_ */
