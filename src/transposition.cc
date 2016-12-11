/*
 * transposition.cc
 *
 *  Created on: Dec 10, 2016
 *      Author: jonathan
 */

#include "transposition.h"

namespace table {

int size = 1000000;
std::vector<Entry> table(size);

int HashFunction(HashType hash) {
  return hash % size;
}

Entry GetEntry(HashType hash) {
  return table.at(HashFunction(hash));
}

void SaveEntry(HashType hash, Move best_move, Score score) {
  int index = HashFunction(hash);
  table.at(index).hash = hash;
  table.at(index).best_move = best_move;
  table.at(index).score = score;
}

}

