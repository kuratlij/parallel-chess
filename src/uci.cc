/*
 * uci.cc
 *
 *  Created on: Nov 4, 2016
 *      Author: jonathan
 */

#include "uci.h"
#include "general/test.h"

namespace {

std::vector<std::string> &splits(const std::string &s, char delimeter,
    std::vector<std::string> &elements) {
    std::stringstream string_stream(s);
    std::string item;
    while (std::getline(string_stream, item, delimeter)) {
        elements.push_back(item);
    }
    return elements;
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    splits(s, delim, elems);
    return elems;
}

const std::string kEngineIsReady = "readyok";
const std::string kEngineNamePrefix = "id name ";
const std::string kEngineAuthorPrefix = "id author ";
const std::string kOk = "uciok";


}

namespace uci {

bool Equals(std::string string_a, std::string string_b) {
  return string_a.compare(string_b) == 0;
}

void Reply(std::string message) {
  std::cout << message << std::endl;
}

void Loop() {
  Board board;
  while (true) {
    std::string in;
    std::getline(std::cin, in);
    std::vector<std::string> tokens = split(in, ' ');
    unsigned int index = 0;
    std::string command = tokens[index++];
    if (Equals(command, "quit")) {
      break;
    }
    else if (Equals(command, "isready")) {
      Reply(kEngineIsReady);
    }
    else if (Equals(command, "print")) {
      board.Print();
    }
    else if (Equals(command, "uci")) {
      Reply(kEngineNamePrefix + settings::engine_name + " "
          + settings::engine_version);
      Reply(kEngineAuthorPrefix + settings::engine_author);
      Reply(kOk);
    }
    else if (Equals(command, "print_moves")) {
      std::vector<Move> moves = board.GetMoves();
      for (unsigned int i = 0; i < moves.size(); i++) {
        std::cout << parse::MoveToString(moves[i]) << std::endl;
      }
    }
    else if (Equals(command, "position")) {
      if (index < tokens.size()) {
        std::string arg = tokens[index++];
        if (Equals(arg, "startpos")) {
          board.SetStartBoard();
        }
        else if (Equals(arg, "fen")) {
          if(index < tokens.size()+2) {
            std::string fen_pos = tokens[index++];
            std::string turn = tokens[index++];
            std::string castling_rights = tokens[index++];
            board.SetBoard(fen_pos, turn, castling_rights);
          }
        }
        if (index < tokens.size()){
          arg = tokens[index++];
          while (Equals(arg,"-") && index < tokens.size()) {
            arg = tokens[index++];
          }
          if (Equals(arg, "moves")) {
            while (index < tokens.size()) {
              Move move = parse::StringToMove(tokens[index++]);
              std::vector<Move> moves = board.GetMoves();
              for (unsigned int i = 0; i < moves.size(); i++) {
                if (GetMoveSource(moves[i]) == GetMoveSource(move)
                    && GetMoveDestination(moves[i]) == GetMoveDestination(move)
                    && (GetMoveType(moves[i]) < kQueenPromotion
                        || GetMoveType(moves[i]) == GetMoveType(move))) {
                  board.Make(moves[i]);
                  break;
                }
              }
            }
          }
        }
      }
    }
    else if (Equals(command, "go")) {
      Move move;
      if (tokens.size() == index+2) {
        std::string arg = tokens[index++];
        if(Equals(arg, "depth")){
          Depth depth = atoi(tokens[index++].c_str());
          move = search::DepthSearch(board, depth);
        } else if(Equals(arg, "movetime")){
          Milliseconds duration = Milliseconds(atoi(tokens[index++].c_str()));
          move = search::TimeSearch(board, duration);
        }
      }else{
        move = search::DepthSearch(board, 6);
      }
      board.Make(move);
      std::cout << "bestmove " << parse::MoveToString(move) << std::endl;
    }
    else if (Equals(command, "perft")) {
      Depth depth = atoi(tokens[index++].c_str());
      std::vector<Move> moves = board.GetMoves();
      long sum = 0;
      for (Move move : moves) {
        board.Make(move);
        long perft_result = search::Perft(board, depth-1);
        board.UnMake();
        std::cout << parse::MoveToString(move) << " depth: " << (depth-1)
            << " perft: " << perft_result << std::endl;
        sum += perft_result;
      }
      std::cout << "depth: " << depth << " perft: " << sum << std::endl;
    }
    else if (Equals(command, "parallel")) {
      int run_parallel = atoi(tokens[index++].c_str());
      settings::set_run_parallel(run_parallel == 1);
    }
    else if (Equals(command, "testplay")) {
        int run_parallel = atoi(tokens[index++].c_str());
        settings::set_run_parallel(run_parallel == 1);
        int game_nr_or_fens = atoi(tokens[index++].c_str());
      int desired_depth = -1;
        if(tokens.size() > 2){
          desired_depth = atoi(tokens[index++].c_str());
        }
        Test test;
//        test.run(game_nr, 5);
//        test.test_fens(16,5,game_nr_or_fens, desired_depth);
      test.big_test(1, game_nr_or_fens, desired_depth);
    }
    else {
      Reply("Received unknown command: " + command);
    }
  }
}

}
