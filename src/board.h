/*
 * board.h
 *
 *  Created on: Nov 1, 2016
 *      Author: jonathan
 */

#ifndef BOARD_H_
#define BOARD_H_

#include "general_headers.h"
#include <vector>

/**
 * This type is used to store information which is necessary in order to
 * implement the unmake move function such as ep.
 */
typedef int64_t MoveHistoryInformation;

struct PieceBitboardSet {
  BitBoard piece_bitboards[kNumPlayers][kNumPieceTypes];
};

class Board{
public:
  //Board constructor initializes the board to the starting position.
  Board();
  //Sets the board to position defined by the argument FEN code.
  //Previous state information is reset.
  void SetBoard(std::string fen_code);
  void Make(Move move);
  void UnMake(Move move);
  PieceBitboardSet get_piece_bitboards();
  void Print();
  void AddPiece(Square square, Piece piece);
private:
  BitBoard piece_bitboards[kNumPlayers][kNumPieceTypes];
  Piece pieces[kBoardLength*kBoardLength];
  std::vector<Move> move_history;
  //Includes information necessary to restore a previous state.
  std::vector<MoveHistoryInformation> move_history_information;
  //4 bits are set representing white and black, queen- and kingside castling
  int32_t castling_rights;
};

#endif /* BOARD_H_ */
