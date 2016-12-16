/*
 * settings.cc
 *
 *  Created on: Dec 15, 2016
 *      Author: jonathan
 */

#include "settings.h"
#include <string>

namespace {

bool run_parallel = true;
std::string eval_path = "/home/jonas/chess_eval/";


}

namespace settings {

void set_run_parallel(bool parallel) {
  run_parallel = parallel;
}
bool get_run_parallel() {
  return run_parallel;
}

void set_eval_path(std::string path){
  eval_path = path;
}
std::string get_eval_path() {
  return eval_path;
}
}
