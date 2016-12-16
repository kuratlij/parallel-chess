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
//spin_lock spin_lock;
//read_write_lock read_write_lock;


int HashFunction(HashType hash) {
  return hash % size;
}

Entry GetEntry(HashType hash) {
  //spin_lock.lock();
  //read_write_lock.read();
  Entry result = table.at(HashFunction(hash));
  //spin_lock.unlock();
  //read_write_lock.readEnd();
  return result;
}

void SaveEntry(HashType hash, Move best_move, Score score) {
  int index = HashFunction(hash);
  //spin_lock.lock();
  //read_write_lock.write();int64_t data = result.best_move;
  u_int64_t data = best_move;
  data <<=32;
  data ^= score;
  table.at(index).hash = hash^data;
  table.at(index).best_move = best_move;
  table.at(index).score = score;
  //spin_lock.unlock();
  //read_write_lock.writeEnd();
}

bool ValidateHash(Entry entry, HashType hash){
  int64_t data = entry.best_move;
  data <<=32;
  data ^= entry.score;
  data ^= hash;
  return entry.hash == data;
}

void ClearTable() {
  //std::fill(table.begin(), table.end(), 0);
}

}

