/*
 * settings.h
 *
 *  Created on: Nov 4, 2016
 *      Author: jonathan
 */

#ifndef GENERAL_SETTINGS_H_
#define GENERAL_SETTINGS_H_

#include <string>

namespace settings {

const std::string engine_name = "DPHPC Engine";
const std::string engine_version = "Alpha 0.1";
const std::string engine_author = "Jonas Kuratli, Jonathan Maurer and Jonathan Rosenthal";
const int num_threads = 2;
const bool alphaPropagation = false;
const int alphaChange = 15;
void set_run_parallel(bool parallel);
bool get_run_parallel();
const bool use_YBWC = true;
void set_eval_path(std::string path);
std::string get_eval_path();
std::string get_fen_path();
}



#endif /* GENERAL_SETTINGS_H_ */
