//
// Created by jonas on 16.12.16.
//

#include "test.h"
#include "../search.h"
#include <fstream>
#include <thread>

bool running = true;


Test::Test(){

    board = Board();

}

void position(Move move){
}

Move go(Board &board, std::string file_path) {
    Depth depth = 5;

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





