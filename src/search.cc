/*
 * search.cc
 *
 *  Created on: Nov 6, 2016
 *      Author: jonathan
 */

#include "search.h"

namespace {

std::mt19937_64 rng;
Move best_move = 0;

const Score kMaxScore = 100000;
const Score kMinScore = -100000;

}

namespace search {

Move GetRandomMove(Board board) {
  std::vector<Move> moves = board.GetMoves();
  return moves[rng() % moves.size()];
}

long Perft(Board board, Depth depth) {
  if (depth <= 0) {
    return 1;
  }
  if (depth == 1) {
    return board.GetMoves().size();
  }
  long perft_sum = 0;
  std::vector<Move> moves = board.GetMoves();
  for (Move move : moves) {
    board.Make(move);
    perft_sum += Perft(board, depth-1);
    board.UnMake();
  }
  return perft_sum;
}

Score AlphaBeta(Board board, Score alpha, Score beta, Depth depth) {
  if (depth == 0) {
    return evaluation::ScoreBoard(board);
  }
  std::vector<Move> moves = board.GetMoves();
  if (moves.size() == 0) {
    if (board.InCheck()) {
      return kMinScore;
    }
    return 0;
  }
  Move best_local_move = moves[0];
  for (Move move : moves) {
    board.Make(move);
    Score score = -AlphaBeta(board, -beta, -alpha, depth-1);
    board.UnMake();
    if (score >= beta) {
      best_move = move;
      return score;
    }
    if (score > alpha) {
      alpha = score;
      best_local_move = move;
    }
  }
  best_move = best_local_move;
  return alpha;
}

Move DepthSearch(Board board, Depth depth) {
  for (Depth current_depth = 1; current_depth <= depth; current_depth++) {
    Score score = AlphaBeta(board, kMinScore, kMaxScore, current_depth);
    std::cout << "info " << "cp " << score << " pv "
        << parse::MoveToString(best_move) << std::endl;
  }
  return best_move;
}

}
