/*
 * debug.cc
 *
 *  Created on: Nov 1, 2016
 *      Author: jonathan
 */

#include "debug.h"

namespace debug {


int counter = 0;

void Print(const std::string message, const bool condition,
    const std::string tag) {
  if (condition && kDebug) {
    if (tag.compare("") == 0) {
      std::cout << "DEBUG " << counter++ << ": " << message << std::endl;
    }
    else {
      std::cout << "DEBUG " << tag << ": " << message << std::endl;
    }
  }
}

void Error(const std::string message, bool exit) {
  std::cout << "ERROR: " << message << std::endl;
  if (exit) {
    std::exit(1);
  }
}

}


