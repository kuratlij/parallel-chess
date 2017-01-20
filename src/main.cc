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
#include "uci.h"
#include "general/test.h"

int main() {
  hash::initialize();
  magic::Initialize();
  /*Test test;
  int game_nr_or_fens = 5;
  int desired_depth = 8;
  test.big_test(1, game_nr_or_fens, desired_depth);*/
  uci::Loop();
//  debug::Print("Exiting Program", debug::kDebugMain);
  return 0;
}
