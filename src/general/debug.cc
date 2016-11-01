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
  if (condition) {
    if (tag.compare("") == 0) {
      std::cout << "DEBUG " << counter++ << ": " << message << std::endl;
    }
    else {
      std::cout << "DEBUG " << tag << ": " << message << std::endl;
    }
  }
}

}


