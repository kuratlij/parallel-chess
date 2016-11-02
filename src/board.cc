/*
 * board.cc
 *
 *  Created on: Nov 1, 2016
 *      Author: jonathan
 */

#include "board.h"

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

Board::Board() {
  for (int player = kWhite; player <= kBlack; player++) {
    for (int piece_type = 0; piece_type < kNumPieceTypes; piece_type++) {
      piece_bitboards[player][piece_type] = 0;
    }
  }
  for (Square square = parse::StringToSquare("a1");
      square <= parse::StringToSquare("h8"); square++) {
    pieces[square] = kNoPiece;
  }
  for (Color color = kWhite; color <= kBlack; color++) {
    AddPiece(parse::StringToSquare("a1") + (56*color), GetPiece(color, kRook));
    AddPiece(parse::StringToSquare("b1") + (56*color), GetPiece(color, kKnight));
    AddPiece(parse::StringToSquare("c1") + (56*color), GetPiece(color, kBishop));
    AddPiece(parse::StringToSquare("d1") + (56*color), GetPiece(color, kQueen));
    AddPiece(parse::StringToSquare("e1") + (56*color), GetPiece(color, kKing));
    AddPiece(parse::StringToSquare("f1") + (56*color), GetPiece(color, kBishop));
    AddPiece(parse::StringToSquare("g1") + (56*color), GetPiece(color, kKnight));
    AddPiece(parse::StringToSquare("h1") + (56*color), GetPiece(color, kRook));
    for (int i = 0; i < 8; i++) {
      AddPiece(parse::StringToSquare("a2") + i + (40*color), GetPiece(color, kPawn));
    }
  }
  castling_rights = 15;
  debug::Print("constructed board", debug::kDebugBoard);
}

void Board::AddPiece(Square square, Piece piece) {
  piece_bitboards[GetPieceColor(piece)][GetPieceType(piece)] |= GetSquareBitBoard(square);
  pieces[square] = piece;
  debug::Print("added piece to square " + parse::SquareToString(square), debug::kDebugBoard);
}

void Board::Print() {
  PrintStandardRow(unicode::s_e_vec, unicode::s_e_w_vec, unicode::s_w_vec);
  for (int row = kBoardLength; row > 0; row--) {
    std::cout << row << ' ' << unicode::n_s_vec;
    for (int column = 0; column < kBoardLength; column++) {
      Square square = (row-1)*kBoardLength + column;
      std::cout << ' ' <<
          (unicode::pieces[GetPieceColor(pieces[square])][GetPieceType(pieces[square])])
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


