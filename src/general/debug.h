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
#include <cstdlib>

namespace debug {

const bool kDebug = false;
const bool kDebugMain = true;
const bool kDebugBoard = true;
const bool kDebugSearch = false;
const bool kDebugUCI = true;
const bool kDebugEvaluation = true;

/**
 * Prints a command line string if condition is met. By default there
 * is an incrementing counter used to tag messages.
 */
void Print(const std::string message, const bool condition = true,
    const std::string tag = "");


void Error(const std::string message, bool exit=true);

void SearchDebug(std::string message, int depth);


}


#endif
