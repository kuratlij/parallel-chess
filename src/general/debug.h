/*
 * debug.h
 *
 *  Created on: Nov 1, 2016
 *      Author: jonathan
 */

#ifndef GENERAL_DEBUG_H_
#define GENERAL_DEBUG_H_

#include <string>
#include <iostream>

namespace debug {

const bool kDebug = true;
const bool kDebugMain = true;

/**
 * Prints a command line string if condition is met. By default there
 * is an incrementing counter used to tag messages.
 */
void Print(const std::string message, const bool condition = kDebug,
    const std::string tag = "");

}



#endif
