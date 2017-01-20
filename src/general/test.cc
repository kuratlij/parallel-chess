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

Move go(Board &board, std::string file_path, Depth d_depth, int thds) {

    Move move = search::TestDepthSearch(board, d_depth, file_path, thds);
    board.Make(move);
//    board.Print();
//    std::cout << "bestmove " << parse::MoveToString(move) << std::endl;
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

void Test::big_test(int moves, int fens, int d_depth, bool parallel){
//  int threads[] = {1,2,4,8,12,16,24,32,64,96};
  if(parallel){
    settings::set_run_parallel(true);
  }else{
    settings::set_run_parallel(false);
  }
  int threads[] = {1,2,4,8,16,32,64,128,256,512};
  for(int pieces = 16; pieces <= 20; pieces += 8){
    if(pieces == 36){
      pieces = 32;
    }
    for (int d = 4; d <= d_depth; d++) {
      int start_ts = 0;
      if(d == 6){
        start_ts = 0;
      }if(d == 7){
        start_ts = 3;
      }if(d == 8){
        start_ts = 7;
      }
      start_ts = 0;
      for(int ts = start_ts; ts < 10; ts++) {


        int ts_for_test = threads[ts];
        settings::set_num_threads(ts_for_test);
        test_fens(pieces, ts_for_test, fens, d);

        if(!parallel){
          ts = 100;
        }

        // Collect all averages in single file
        std::string path = settings::get_eval_path() + std::to_string(settings::get_num_threads()) + "__" + std::to_string(d) + "_" + std::to_string(settings::get_run_parallel()) +  ".csv";
        std::cout << path << std::endl;
        std::string out_path = settings::get_eval_path() + "AVG" + std::to_string(d) + "_" + std::to_string(fens) + ".csv";
        std::ifstream infile(path);

        std::ofstream csv;
        csv.open(out_path, std::ofstream::app);

        // Find the row with the avg time
        std::string line,time;
        while(infile >> line) {
//          std::cout << first << std::endl;

          if (line.find("Avg") != std::string::npos) {
            std::cout << "found";
            int pos = 0;
            pos = line.find(","); // Move string, not needed
            line.erase(0, pos + 1);
            pos = line.find(","); // Time string, needed
            time = line.substr(0, pos);
            // Found row, time contains avg time over all fens for current parameters
            csv << ts_for_test << "," << time << ";" << std::endl;
            break;
          }
        }
        csv.flush();
        csv.close();
      }
    }



  }
}

void Test::test_fens(int pieces, int moves, int fens, int d_depth){
  std::cout << "Testing " << pieces << " " << moves << " " << d_depth << " " << fens << std::endl;
  if(d_depth != -1){
    depth = d_depth;
  }
  int thds = moves;

  //TODO: Set parameters here
//  pieces = 32;
  moves = 1;
//  fens = 100;

  std::string path = settings::get_eval_path() + std::to_string(settings::get_num_threads()) + "__" + std::to_string(d_depth) + "_" + std::to_string(settings::get_run_parallel()) +  ".csv";

  // Delete existing file
//  remove(path.c_str());

  // Load fens
  std::ifstream infile(settings::get_fen_path() + "random_fens_" + std::to_string(pieces));
//  std::cout << settings::get_fen_path() + "random_fens_" + std::to_string(pieces) << std::endl;
  std::string fen, turn, castling, enpassant, foo, bar;
  for(int i = 0; i < fens; i++){
    table::ClearTable();
    // Load fen position
    infile >> fen >> turn >> castling >> enpassant >> foo >> bar;
//    std::cout << fen << std::endl;
    board.SetBoard(fen, turn, castling);
//    board.Print();


    int cur_move = 0;
    running = true;
    while(running && cur_move < moves){
      if(board.GetMoves().size() > 0) {
        Move white_move = go(board, path, d_depth, thds);
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
  std::cout << total_time << " " << total_moves << " " << d_depth <<  " " << thds << std::endl;
  csv << "Avg time," << std::to_string(total_time / (1.0 * total_moves)) << "," << thds << ";" << std::endl;
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
        Move white_move = go(board, white_path, search_depth, 2);
      }else{
        running = false;
      }
      if(board.GetMoves().size() > 0) {
        Move black_move = go(board, black_path, search_depth, 2);
      }else{
        running = false;
      }
    }

}





