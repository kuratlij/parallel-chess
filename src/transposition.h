/*
 * transposition.h
 *
 *  Created on: Dec 10, 2016
 *      Author: jonathan
 */

#ifndef TRANSPOSITION_H_
#define TRANSPOSITION_H_

#include "general_headers.h"

namespace table {

struct Entry {
  HashType hash;
  Move best_move;
  Score score;
};

void Initialize();
Entry GetEntry(HashType hash);
void SaveEntry(HashType hash, Move best_move, Score score);
bool ValidateHash(Entry entry,HashType hash);
}

#endif /* TRANSPOSITION_H_ */
