//
// Created by jonas on 16.12.16.
//

#include "test.h"
#include "../search.h"
#include <fstream>
#include <thread>

bool running = true;
Depth depth = 8;


Test::Test(){

    board = Board();

}

void position(Move move){
}

Move go(Board &board, std::string file_path) {

    Move move = search::TestDepthSearch(board, depth, file_path);
    board.Make(move);
    board.Print();
    std::cout << "bestmove " << parse::MoveToString(move) << std::endl;
}

void check_break(std::string foo){
  while(running){
    std::string input;
    std::cin >> input;
    if(input == "stop" || input == "break"){
      running = false;
      return;
    }
  }
}

void fen(Board b, std::string fen_code, std::string turn, std::string castling_rights){
  b.SetBoard(fen_code, turn, castling_rights);
}

void Test::test_fens(int pieces, int moves, int fens){
  //TODO: Set parameters here
  pieces = 16;
  moves = 1;
  fens = 5;

  std::string path = settings::get_eval_path() + std::to_string(pieces) + "_" + std::to_string(moves) + "_" + std::to_string(depth) + "_" + std::to_string(settings::get_run_parallel()) +  ".csv";

  // Delete existing file
  remove(path.c_str());

  // Load fens
  std::ifstream infile(settings::get_fen_path() + "random_fens_" + std::to_string(pieces));
  std::cout << settings::get_fen_path() + "random_fens_" + std::to_string(pieces) << std::endl;
  std::string fen, turn, castling, enpassant, foo, bar;
  for(int i = 0; i < fens; i++){
    // Load fen position
    infile >> fen >> turn >> castling >> enpassant >> foo >> bar;
    std::cout << fen << std::endl;
    board.SetBoard(fen, turn, castling);
    board.Print();


    int cur_move = 0;
    while(running && cur_move < moves){
      if(board.GetMoves().size() > 0) {
        Move white_move = go(board, path);
      }else{
        running = false;
      }
      /*if(board.GetMoves().size() > 0) {
        Move black_move = go(board, path);
      }else{
        running = false;
      }*/

      cur_move++;
    }
  }

  // Calculate avg time per turn and write to file
  std::ifstream timings;
  timings.open(path);
  std::string time, line;
  int pos;

  double total_time = 0.0;
  int total_moves = 0;

  while(timings >> line){
    pos = line.find(","); // Move string, not needed
    line.erase(0, pos + 1);
    pos = line.find(","); // Time string, needed
    time = line.substr(0, pos);
    total_moves++;

    total_time += atof(time.c_str());
  }

  std::ofstream csv;
  csv.open(path, std::ofstream::app);
  std::cout << total_time << " " << total_moves << " " << std::endl;
  csv << "Avg time," << std::to_string(total_time / (1.0 * total_moves)) << "," << depth << ";";
  csv.flush();
  csv.close();


}

void Test::run(int game_number, int search_depth){
//    std::thread c1(check_break, "foo");
//    depth = search_depth;
//
//    c1.join();


    std::string white_path = settings::get_eval_path() + std::to_string(game_number) + "_" + "white.csv";
    std::string black_path = settings::get_eval_path() + std::to_string(game_number) + "_" + "white.csv";
//    std::fstream black_file;
//    std::fstream white_file;
//
//    black_file.open(settings::get_eval_path() + "_" + std::to_string(game_number) + "_" + "black");
//    white_file.open(settings::get_eval_path() + "_" + std::to_string(game_number) + "_" + "white");


    while(running){
      if(board.GetMoves().size() > 0) {
        Move white_move = go(board, white_path);
      }else{
        running = false;
      }
      if(board.GetMoves().size() > 0) {
        Move black_move = go(board, black_path);
      }else{
        running = false;
      }
    }

}





