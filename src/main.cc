/*
 * main.cc
 *
 *  Created on: Nov 1, 2016
 *      Author: Jonas Kuratli, Jonathan Maurer and Jonathan Rosenthal
 */

#include <iostream>
#include <vector>
#include "general_headers.h"
#include "board.h"

int main() {
  Board board;
  board.Make(GetMove(parse::StringToSquare("e2"), parse::StringToSquare("e4")));
  board.Make(GetMove(parse::StringToSquare("b7"), parse::StringToSquare("b5")));
  board.Print();
  std::vector<Move> moves = board.GetMoves();
  std::cout << "number of legal moves: " << moves.size() << std::endl;
  debug::Print("Exiting Program");
  return 0;
}
