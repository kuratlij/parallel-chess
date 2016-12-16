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

int main() {
  hash::initialize();
  magic::Initialize();
  uci::Loop();
  debug::Print("Exiting Program", debug::kDebugMain);
  return 0;
}
