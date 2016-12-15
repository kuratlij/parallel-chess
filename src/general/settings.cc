/*
 * settings.cc
 *
 *  Created on: Dec 15, 2016
 *      Author: jonathan
 */

#include "settings.h"

namespace {

bool run_parallel = true;

}

namespace settings {

void set_run_parallel(bool parallel) {
  run_parallel = parallel;
}
bool get_run_parallel() {
  return run_parallel;
}

}
