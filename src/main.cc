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
  Test test;
  // Set parameters
  // Amount of random settings that will be evaluated for different number of threads
  int amount_of_settings = 16;

  // The depth that is searched
  int desired_depth = 8;

  // Amount of moves that are searched per setting
  int moves_per_setting = 1;

  // Set to false to run sequential version

  // Run the test
  test.big_test(moves_per_setting, amount_of_settings, desired_depth, false);
  //uci::Loop();
//  debug::Print("Exiting Program", debug::kDebugMain);
  return 0;
}
