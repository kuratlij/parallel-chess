/*
 * search.cc
 *
 *  Created on: Nov 6, 2016
 *      Author: jonathan
 */

#include "search.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include "omp.h"

namespace {
struct Sorter {
  Sorter(Move move) { this->best_move = move; }
  bool operator() (Move i, Move j) {
    return (i == this->best_move) || (j!=best_move && GetMoveType(i) >= kCapture
        && GetMoveType(j) < kCapture);
  }

  Move best_move;
};

std::mt19937_64 rng;
Move best_move = 0;
bool parallel = true;

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
        table::Entry entry = table::GetEntry(board.get_hash());
        if (table::ValidateHash(entry,board.get_hash())) {
            std::sort(moves.begin(), moves.end(), Sorter(entry.best_move));
        }
        Move best_local_move = moves[0];
        for (Move move : moves) {
            board.Make(move);Score score;
            if(depthPattern(depth)){
                score = -ParallelAlphaBeta(board, -beta, -alpha, depth - 1);
            }
            else{
                score = -AlphaBeta(board, -beta, -alpha, depth - 1);
            }
            board.UnMake();
            if (score >= beta) {
                best_move = move;
                table::SaveEntry(board.get_hash(), move, score);
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

    Score ParallelAlphaBeta(Board board, Score alpha, Score beta, Depth depth) {
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
        table::Entry entry = table::GetEntry(board.get_hash());
        if (table::ValidateHash(entry,board.get_hash())) {
            std::sort(moves.begin(), moves.end(), Sorter(entry.best_move));
        }
        std::vector<Move> best_local_move = std::vector<Move>(settings::num_threads);
        std::vector<Board> parallelBoard = std::vector<Board>(settings::num_threads);
        std::vector<Score> parallelAlpha = std::vector<Score>(settings::num_threads);
        for(int i = 0; i<settings::num_threads;i++){
            best_local_move[i] = moves[0];
            parallelBoard[i] = board.copy();
            parallelAlpha[i] = alpha;
        }
        omp_set_num_threads(settings::num_threads);
        omp_set_nested(1);
        bool go = true;
        Score end_score;
#pragma omp parallel for if(parallel)
        for ( int i = 0;i<moves.size();i++) {
            if(go){//already found better score then beta, skip rest of computation
                Move move = moves[i];
                parallelBoard[omp_get_thread_num()].Make(move);
                if(settings::alphaPropagation&& (alpha > parallelAlpha[omp_get_thread_num()])) {
                    parallelAlpha[omp_get_thread_num()] = alpha;
                }
                Score score;
                if(depthPattern(depth)){
                    score = -ParallelAlphaBeta(parallelBoard[omp_get_thread_num()], -beta, -parallelAlpha[omp_get_thread_num()], depth - 1);
                }
                else{
                    score = -AlphaBeta(parallelBoard[omp_get_thread_num()], -beta, -parallelAlpha[omp_get_thread_num()], depth - 1);
                }
                parallelBoard[omp_get_thread_num()].UnMake();
                if (score >= beta) {
                    best_move = move;
                    table::SaveEntry(parallelBoard[omp_get_thread_num()].get_hash(), move, score);
                    go=false;//set go to false to skip rest of the moves in this branch
                    end_score = score;//return value, since we can't exit a parallel for with a return.
                    //flush the values to make them visible to all
                    #pragma omp flush(go,end_score)
                }
                if (score > parallelAlpha[omp_get_thread_num()]) {
                    parallelAlpha[omp_get_thread_num()] = score;
                    if(settings::alphaPropagation&&((parallelAlpha[omp_get_thread_num()]-settings::alphaChange)>alpha)){//Todo what is relevant improvement to cut more(best so far 15, maybe us proportional improvement)
                        alpha = parallelAlpha[omp_get_thread_num()];
                        #pragma omp flush(alpha)
                    }
                    best_local_move[omp_get_thread_num()] = move;
                }
            }
        }
        if(!go){
            return end_score;
        }
        for(int i = 0; i<settings::num_threads;i++){
            if(parallelAlpha[i]>alpha){
                best_move = best_local_move[i];
                alpha=parallelAlpha[i];
            }
        }

        return alpha;
    }





Move DepthSearch(Board board, Depth depth) {
    Move bestmove = SequentialSearch(board, depth);
  //Move bestmove = ParallelSearch(board, depth);
  return bestmove;
}

    Move SequentialSearch(Board board, Depth depth){
      // Measure complete search time
      parallel = false;
        Time complete_begin = now();

      for (Depth current_depth = 1; current_depth <= depth; current_depth++) {
        std::cout << std::endl << "depth: " << current_depth << std::endl;
        // Measure search time
        Time begin = now();

        Score score = AlphaBeta(board, kMinScore, kMaxScore, current_depth);

        Time end = now();
          std::chrono::duration<double> elapsed_secs = std::chrono::duration_cast<std::chrono::duration<double> >(end-begin);

          //    std::fstream file;
        //    file.open("/home/jonas/parallel-log.txt");
        //    file << "Move: " << parse::MoveToString(best_move) << std::endl;
        //    file.close();
        std::cout << "info " << "cp " << score << " pv "
                  << parse::MoveToString(best_move) << std::endl;
        std::cout << "Elapsed time (depth " << current_depth << "): " << elapsed_secs.count() << std::endl;
      }

      // Print elapsed search time
      Time complete_end = now();
        std::chrono::duration<double> elapsed_secs = std::chrono::duration_cast<std::chrono::duration<double> >(complete_end-complete_begin);
        std::cout << "SEQUENTIAL Elapsed time total: " << elapsed_secs.count() << std::endl;

      return best_move;
    }
Depth starting_depth;
    Move ParallelSearch(Board board, Depth depth){
      // Measure complete search time
      parallel = true;
      Time complete_begin = now();

      for (Depth current_depth = 1; current_depth <= depth; current_depth++) {

        std::cout << std::endl << "depth: " << current_depth << std::endl;
        // Measure search time
          starting_depth=current_depth;
        Time begin = now();

        Score score = AlphaBeta(board, kMinScore, kMaxScore, current_depth);

        Time end = now();
        std::chrono::duration<double> elapsed_secs = std::chrono::duration_cast<std::chrono::duration<double> >(end-begin);

        //    std::fstream file;
        //    file.open("/home/jonas/parallel-log.txt");
        //    file << "Move: " << parse::MoveToString(best_move) << std::endl;
        //    file.close();
        std::cout << "info " << "cp " << score << " pv "
                  << parse::MoveToString(best_move) << std::endl;
        std::cout << "Elapsed time (depth " << current_depth << "): " << elapsed_secs.count() << std::endl;
      }

      // Print elapsed search time
      Time complete_end = now();
        std::chrono::duration<double> elapsed_secs = std::chrono::duration_cast<std::chrono::duration<double> >(complete_end-complete_begin);
        std::cout << "PARALLEL Elapsed time total: " << elapsed_secs.count() << std::endl;

      return best_move;
    }


    bool depthPattern(Depth depth){
        return depth==(starting_depth-1)||depth==(starting_depth-2);//starting_depth
    }

}
