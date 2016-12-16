//
// Created by jonas on 16.12.16.
//

#include "../board.h"

#ifndef PARALLEL_CHESS_TEST_H
#define PARALLEL_CHESS_TEST_H

#endif //PARALLEL_CHESS_TEST_H

class Test{
public:
    Test();
    void run(int game_number, int search_depth);
  void check_break(std::string foo);


private:
Board board;//    Board whiteBoard;
};
