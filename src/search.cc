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
inline bool is_null_window(Score alpha, Score beta) {
    return alpha+1 == beta;
}
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
        for (unsigned int i = 0; i < moves.size(); i++) {
            Move move = moves[i];
            board.Make(move);
            debug::SearchDebug(">>"+parse::MoveToString(move)+" t"+std::to_string(omp_get_thread_num()), depth);
            Score score;
            if (i == 0 || (is_null_window(alpha, beta))) {
              debug::SearchDebug("["+std::to_string(alpha)+","+std::to_string(beta)+"] t"+std::to_string(omp_get_thread_num()), depth);
              if(depthPattern(depth)){
                  score = -ParallelAlphaBeta(board, -beta, -alpha, depth - 1);
              }
              else{
                  score = -AlphaBeta(board, -beta, -alpha, depth - 1);
              }
            }
            else {
              debug::SearchDebug("["+std::to_string(alpha)+","+std::to_string(alpha+1)+"] t"+std::to_string(omp_get_thread_num()), depth);
              if(depthPattern(depth)){
                  score = -ParallelAlphaBeta(board, -(alpha+1), -alpha, depth - 1);
                  if (score >= (alpha+1)) {
                    debug::SearchDebug("["+std::to_string(alpha)+","+std::to_string(beta)+"] t"+std::to_string(omp_get_thread_num()), depth);
                    score = -ParallelAlphaBeta(board, -beta, -alpha, depth - 1);
                  }
              }
              else{
                  score = -AlphaBeta(board, -(alpha+1), -alpha, depth - 1);
                  if (score >= (alpha+1)) {
                    debug::SearchDebug("["+std::to_string(alpha)+","+std::to_string(beta)+"] t"+std::to_string(omp_get_thread_num()), depth);
                    score = -AlphaBeta(board, -beta, -alpha, depth - 1);
                  }
              }
            }
            debug::SearchDebug("<<"+parse::MoveToString(move)+" "+std::to_string(score)+" t"+std::to_string(omp_get_thread_num()), depth);
            board.UnMake();
            if (score >= beta) {
                best_move = move;
                table::SaveEntry(board.get_hash(), move, score);
                return beta;
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
        debug::SearchDebug("ps t"+std::to_string(omp_get_thread_num())+" "+std::to_string(alpha)+","+std::to_string(beta), depth);
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
        bool go = true;
        omp_set_num_threads(settings::num_threads);
        omp_set_nested(1);

#pragma omp parallel
        {
            std::vector<Move> private_moves = moves;
            Score privateAlpha = alpha;
            Board privateBoard = board.copy();
            Move private_best_local_move = private_moves[0];
            for (int i = omp_get_thread_num(); i < private_moves.size(); i+=settings::num_threads) {
                if (go) {//already found better score then beta, skip rest of computation
                    Move move = private_moves[i];
                    privateBoard.Make(move);
                    debug::SearchDebug(">>"+parse::MoveToString(move)+" pt"+std::to_string(omp_get_thread_num()), depth);
                    if (settings::alphaPropagation && (is_null_window(alpha, beta)) && (alpha > privateAlpha)) {
                        privateAlpha = alpha;
                    }
                    Score score;
                    if (i < settings::num_threads || (is_null_window(privateAlpha, beta))) {
                      debug::SearchDebug("["+std::to_string(privateAlpha)+","+std::to_string(beta)+"] pt"+std::to_string(omp_get_thread_num()), depth);
                      if (depthPattern(depth)) {
                            score = -ParallelAlphaBeta(privateBoard, -beta, -privateAlpha, depth - 1);
                        } else {
                            score = -AlphaBeta(privateBoard, -beta, -privateAlpha, depth - 1);
                        }
                    } else {
                      debug::SearchDebug("["+std::to_string(privateAlpha)+","+std::to_string(privateAlpha+1)+"] pt"+std::to_string(omp_get_thread_num()), depth);
                        if(depthPattern(depth)){
                            score = -ParallelAlphaBeta(privateBoard, -(privateAlpha+1), -privateAlpha, depth - 1);
                            if (score >= (privateAlpha+1)) {
                              debug::SearchDebug("["+std::to_string(privateAlpha)+","+std::to_string(beta)+"] pt"+std::to_string(omp_get_thread_num()), depth);
                                score = -ParallelAlphaBeta(privateBoard, -beta, -privateAlpha, depth - 1);
                            }
                        }
                        else{
                            score = -AlphaBeta(privateBoard, -(privateAlpha+1), -privateAlpha, depth - 1);
                            if (score >= (privateAlpha+1)) {
                              debug::SearchDebug("["+std::to_string(privateAlpha)+","+std::to_string(beta)+"] pt"+std::to_string(omp_get_thread_num()), depth);
                                score = -AlphaBeta(privateBoard, -beta, -privateAlpha, depth - 1);
                            }
                        }
                    }
                    debug::SearchDebug("<<"+parse::MoveToString(move)+" "+std::to_string(score)+" pt"+std::to_string(omp_get_thread_num()), depth);
                    privateBoard.UnMake();
                    if (score >= beta) {
                        best_move = move;
                        table::SaveEntry(privateBoard.get_hash(), move, score);
                        go = false;//set go to false to skip rest of the moves in this branch
                        //flush the values to make them visible to all
#pragma omp flush(go)
                    }
                    if (score > privateAlpha) {
                        privateAlpha = score;
                        if (settings::alphaPropagation && ((privateAlpha - settings::alphaChange) > alpha)) {//Todo what is relevant improvement to cut more(best so far 15, maybe us proportional improvement)
#pragma omp atomic write
                            alpha = privateAlpha;
#pragma omp flush(alpha)
                        }
                        private_best_local_move = move;
                    }
                }
            }
#pragma omp critical
            {
            if(privateAlpha>alpha){
                best_move = private_best_local_move;
                alpha=privateAlpha;
            }
            }
        }
        if(!go){
            return beta;
        }
        return alpha;
    }




Move DepthSearch(Board board, Depth depth) {
  if (settings::get_run_parallel()) {
    return ParallelSearch(board, depth);
  }
  return SequentialSearch(board, depth);
}

Move TestDepthSearch(Board board, Depth depth, std::string file_path) {
  if (settings::get_run_parallel()) {
    return TestParallelSearch(board, depth, file_path);
  }
  return TestSequentialSearch(board, depth, file_path);
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

Move TestSequentialSearch(Board board, Depth depth, std::string file_path){
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

  // Write time to file
  std::cout << "path" << file_path << std::endl;
  std::ofstream csv;
  csv.open(file_path, std::ofstream::app);
  csv << parse::MoveToString(best_move) << "," << elapsed_secs.count() << "," << depth << ";" << std::endl;
  csv.flush();
  csv.close();

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

    Move TestParallelSearch(Board board, Depth depth, std::string file_path){
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

      // Write time to file
      std::cout << "path" << file_path << std::endl;
        std::ofstream csv;
        csv.open(file_path, std::ofstream::app);
        csv << parse::MoveToString(best_move) << "," << elapsed_secs.count() << "," << depth << ";" << std::endl;
      csv.flush();
      csv.close();

        return best_move;
    }


    bool depthPattern(Depth depth){//still use false for sequential runs since there is a overhead in the parallel version
        //Todo maybe just define a 2-D array with depth and starting_depth coordinates
        //Don't parallelize in the starting depth
        //return depth>4&&depth<starting_depth;
        if(!parallel){
            return false;
        }
        return 3 == depth;//false;//starting_depth
    }

}
