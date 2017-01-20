/*
 * settings.cc
 *
 *  Created on: Dec 15, 2016
 *      Author: jonathan
 */

#include "settings.h"
#include <string>

namespace {

int num_threads=96;
bool run_parallel = true;
std::string eval_path = "./chess_eval_24h_parallel_spinlock/";
std::string fen_path = "./fens/";


}

namespace settings {

void set_run_parallel(bool parallel) {
  run_parallel = parallel;
}
bool get_run_parallel() {
  return run_parallel;
}

std::string get_fen_path(){
  return fen_path;
}

void set_eval_path(std::string path){
  eval_path = path;
}
std::string get_eval_path() {
  return eval_path;
}

void set_num_threads(int num){
  num_threads = num;
}
int get_num_threads(){
  return num_threads;
}
}
