/*
 * debug.h
 *
 *  Created on: Nov 1, 2016
 *      Author: jonathan
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#include <string>
#include <iostream>

namespace debug {

const bool kDebug = true;
const bool kDebugMain = true;

void Print(const std::string message, const bool condition = kDebug,
    const std::string tag = "");

}



#endif
