/*
 * board.h
 *
 *  Created on: Nov 1, 2016
 *      Author: jonathan
 */

#ifndef BOARD_H_
#define BOARD_H_

#include "general/types.h"
#include <vector>

/**
 * This type is used to store information which is necessary in order to
 * implement the unmake move function such as ep.
 */
typedef int64_t MoveHistoryInformation;

class Board{
public:
  Board();

  void Make(Move move);
  //This is public for now, optimally we should make this private and add
  //without setters.
  BitBoard piece_bitboards[kNumPlayers][kNumPieceTypes];
private:
  std::vector<Move> move_history;
  std::vector<MoveHistoryInformation> move_history_information;
};

#endif /* BOARD_H_ */
