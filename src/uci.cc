/*
 * uci.cc
 *
 *  Created on: Nov 4, 2016
 *      Author: jonathan
 */

#include "uci.h"

namespace {

std::vector<std::string> &splits(const std::string &s, char delimeter,
    std::vector<std::string> &elements) {
    std::stringstream string_stream(s);
    std::string item;
    while (std::getline(string_stream, item, delimeter)) {
        elements.push_back(item);
    }
    return elements;
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    splits(s, delim, elems);
    return elems;
}

const std::string kEngineIsReady = "readyok";
const std::string kEngineNamePrefix = "id name ";
const std::string kEngineAuthorPrefix = "id author ";
const std::string kOk = "uciok";


}

namespace uci {

bool Equals(std::string string_a, std::string string_b) {
  return string_a.compare(string_b) == 0;
}

void Reply(std::string message) {
  std::cout << message << std::endl;
}

void Loop() {
  Board board;
  while (true) {
    std::string in;
    std::getline(std::cin, in);
    std::vector<std::string> tokens = split(in, ' ');
    unsigned int index = 0;
    std::string command = tokens[index++];
    if (Equals(command, "quit")) {
      break;
    }
    else if (Equals(command, "isready")) {
      Reply(kEngineIsReady);
    }
    else if (Equals(command, "uci")) {
      Reply(kEngineNamePrefix + settings::engine_name + " "
          + settings::engine_version);
      Reply(kEngineAuthorPrefix + settings::engine_author);
      Reply(kOk);
    }
    else {
      Reply("Received unknown command: " + command);
    }
  }
}

}
