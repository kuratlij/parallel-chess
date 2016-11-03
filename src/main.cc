/*
 * main.cc
 *
 *  Created on: Nov 1, 2016
 *      Author: Jonas Kuratli, Jonathan Maurer and Jonathan Rosenthal
 */

#include <iostream>
#include "general_headers.h"
#include "board.h"

int main() {
  Board board;
  board.Make(GetMove(parse::StringToSquare("e2"), parse::StringToSquare("e4")));
  board.Print();
  debug::Print("Exiting Program");
  return 0;
}
