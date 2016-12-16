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
    Move TestDepthSearch(Board board, Depth depth, std::string file_path);
Move SequentialSearch(Board board, Depth depth);
Move TestSequentialSearch(Board board, Depth depth, std::string file_path);
Move ParallelSearch(Board board, Depth depth);
    Move TestParallelSearch(Board board, Depth depth, std::string file_path);
bool depthPattern(Depth depth);
Score AlphaBeta(Board board, Score alpha, Score beta, Depth depth);
Score ParallelAlphaBeta(Board board, Score alpha, Score beta, Depth depth);
}

#endif /* SEARCH_H_ */
