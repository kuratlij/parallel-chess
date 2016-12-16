/*
 * search.h
 *
 * The search namespace is intended to handle all tree search operations.
 * Specifically our program should implement perft, timed alpha-beta and depth
 * limited alpha-beta search algorithms.
 *
 *  Created on: Nov 1, 2016
 *      Author: jonathan
 */

#ifndef SEARCH_H_
#define SEARCH_H_

#include <random>
#include <algorithm>
#include "board.h"
#include "evaluation.h"
#include "transposition.h"

namespace search {

Move GetRandomMove(Board board);

long Perft(Board board, Depth depth);
Move DepthSearch(Board board, Depth depth);
Move TimeSearch(Board board, Milliseconds depth);
    Move TestDepthSearch(Board board, Depth depth, std::string file_path);
Move SequentialSearch(Board board, Depth depth, Time end_time);
Move TestSequentialSearch(Board board, Depth depth, std::string file_path);
Move ParallelSearch(Board board, Depth depth, Time end_time);
    Move TestParallelSearch(Board board, Depth depth, std::string file_path);
bool depthPattern(Depth depth);
bool finished(Time end_time);
Score AlphaBeta(Board board, Score alpha, Score beta, Depth depth, Time end_time);
Score ParallelAlphaBeta(Board board, Score alpha, Score beta, Depth depth, Time end_time);
Time createEndTime();
}

#endif /* SEARCH_H_ */
