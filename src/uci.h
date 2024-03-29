/*
 * uci.h
 *
 * The UCI namespace is intended in order to handle communication to various
 * GUI's and tools utilizing the UCI protocol.
 *
 *  Created on: Nov 1, 2016
 *      Author: jonathan
 */

#ifndef UCI_H_
#define UCI_H_

#include <string>
#include <sstream>
#include <iostream>
#include "general_headers.h"
#include "board.h"
#include "search.h"

namespace uci{

//Runs the UCI protocol until the client exits
void Loop();
void Info(std::string message);

}

#endif /* UCI_H_ */
