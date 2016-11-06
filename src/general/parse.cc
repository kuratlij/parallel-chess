/*
 * parse.cc
 *
 *  Created on: Nov 2, 2016
 *      Author: jonathan
 */

#include "parse.h"

namespace {

const std::string piece_abreviations[4] = {"q", "r", "b", "n"};

}

namespace parse {

void PrintStandardRow(std::string first_delim, std::string mid_delim, std::string last_delim) {
  for (int column = 0; column < kBoardLength; column++) {
    if (column == 0) {
      std::cout << std::string(2,' ') << first_delim;
    }
    else {
      std::cout << mid_delim;
    }
      std::cout << unicode::e_w_vec << unicode::e_w_vec << unicode::e_w_vec;
    if (column == kBoardLength-1) {
      std::cout << last_delim;
    }
  }
  std::cout << std::endl;
}


void PrintBitboard(const BitBoard bitboard) {
  PrintStandardRow(unicode::s_e_vec, unicode::s_e_w_vec, unicode::s_w_vec);
  for (int row = kBoardLength; row > 0; row--) {
    std::cout << row << ' ' << unicode::n_s_vec;
    for (int column = 0; column < kBoardLength; column++) {
      std::cout << ' ' <<
          ((bitboard>>((row-1)*kBoardLength + column)) & 0x1)
          << ' ' << unicode::n_s_vec;
    }
    std::cout << std::endl;
    if (row > 1) {
      PrintStandardRow(unicode::n_s_e_vec, unicode::n_s_e_w_vec,
                       unicode::n_s_w_vec);
    }
  }
  PrintStandardRow(unicode::n_e_vec, unicode::n_e_w_vec, unicode::n_w_vec);
  std::cout << " ";
  for (char column = 'a'; column <= 'h'; column++) {
    std::cout << "   " << column;
  }
  std::cout << std::endl;
}

Square StringToSquare(std::string square_name) {
  return (square_name[0]-'a') + (square_name[1]-'1') * 8;
}

std::string SquareToString(Square square) {
  return std::string(1,(square % 8) + 'a') + std::string(1,(square / 8) + '1');
}

BitBoard StringToBitBoard(std::string square_name) {
  return GetSquareBitBoard(StringToSquare(square_name));
}

std::string MoveToString(Move move) {
  if (GetMoveType(move) >= kQueenPromotion) {
    return SquareToString(GetMoveSource(move))
        + SquareToString(GetMoveDestination(move))
        + piece_abreviations[GetMoveType(move)-kQueenPromotion];
  }
  return SquareToString(GetMoveSource(move))
      + SquareToString(GetMoveDestination(move));
}

Move StringToMove(std::string move) {
  Square source = (move[0] - 'a') + (move[1] - '1') * 8;
  Square destination = (move[2] - 'a') + (move[3] - '1') * 8;
  if (move.size() == 4) {
    return GetMove(source, destination);
  }
  for (int i = 0; i < 4; i++) {
    if (piece_abreviations[i][0] == move[4]) {
      return GetMove(source, destination, kQueenPromotion + i);
    }
  }
  debug::Error("illegal promotion piece string! Parsed move length: "
      + std::to_string(move.size()) +" Parsed move: " + move);
  return 0;
}

}

