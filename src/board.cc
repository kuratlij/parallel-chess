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
  turn = kWhite;
  debug::Print("constructed board", debug::kDebugBoard);
}

void Board::AddPiece(Square square, Piece piece) {
  piece_bitboards[GetPieceColor(piece)][GetPieceType(piece)] |= GetSquareBitBoard(square);
  pieces[square] = piece;
}

Piece Board::RemovePiece(Square square) {
  Piece piece = pieces[square];
  pieces[square] = kNoPiece;
  piece_bitboards[GetPieceColor(piece)][GetPieceType(piece)] ^= GetSquareBitBoard(square);
  return piece;
}

Piece Board::MovePiece(Square source, Square destination) {
  Piece piece = RemovePiece(destination);
  AddPiece(destination, RemovePiece(source));
  if (debug::kDebugBoard) {
    debug::Print("moved piece from " + parse::SquareToString(source)
        + " to " + parse::SquareToString(destination));
  }
  return piece;
}

void Board::SwapTurn() {
  turn ^= 0x1;
}

void Board::Make(Move move) {
  MoveHistoryInformation information = 0;
  information |= RemovePiece(GetMoveDestination(move));
  MovePiece(GetMoveSource(move),GetMoveDestination(move));
  information |= (en_passant << 4);
  //We default our ep square to a place the opponent will never be able to ep.
  en_passant = 56 - (56 * turn);
  switch (GetMoveType(move)) {
  case kDoublePawnMove:
    en_passant = GetMoveDestination(move) - 8 + (2*8) * turn;
    break;
  case kCastle:
    if (GetMoveDestination(move) < GetMoveSource(move)) {
      //Queen-side castling
      MovePiece(GetMoveSource(move)-4, GetMoveSource(move)-1);
    }
    else {
      //King-side castling
      MovePiece(GetMoveSource(move)+3, GetMoveSource(move)+1);
    }
    break;
  case kEnPassant:
    RemovePiece(GetMoveDestination(move) - 8 + (2*8) * turn);
  default:
    if (GetMoveType(move) >= kQueenPromotion) {
      RemovePiece(GetMoveDestination(move));
      AddPiece(GetMoveDestination(move),
          GetPiece(turn, GetMoveType(move) - kQueenPromotion + kQueen));
    }
    break;
  }
  move_history_information.emplace_back(information);
  move_history.emplace_back(move);
  SwapTurn();
}

void Board::UnMake() {
  SwapTurn();
  Move move = move_history.back();
  move_history.pop_back();
  MoveHistoryInformation info = move_history_information.back();
  move_history_information.pop_back();
  AddPiece(GetMoveSource(move), RemovePiece(GetMoveDestination(move)));
  AddPiece(GetMoveDestination(move), info & 0xF);
  en_passant = info >> 4;
  switch(GetMoveType(move)) {
  case kEnPassant:
    RemovePiece(GetMoveDestination(move) - 8 + (2*8) * turn);
  case kCastle:
    if (GetMoveDestination(move) < GetMoveSource(move)) {
      //Queen-side castling
      MovePiece(GetMoveSource(move)-1, GetMoveSource(move)-4);
    }
    else {
      //King-side castling
      MovePiece(GetMoveSource(move)+1, GetMoveSource(move)+3);
    }
    break;
  default:
    if (GetMoveType(move) >= kQueenPromotion) {
      RemovePiece(GetMoveSource(move));
      AddPiece(GetMoveSource(move), GetPiece(turn, kPawn));
    }
    break;
  }
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

BitBoard Board::PlayerBitBoardControl(Color color) {
  BitBoard under_control = 0;
  BitBoard empty = ~0;
  for (Color player = kWhite; player <= kBlack; player++) {
    for (PieceType piece_type = kKing; piece_type < kPawn; piece_type++) {
      empty &= ~piece_bitboards[player][piece_type];
    }
  }

  if (color == kWhite) {
    under_control |= bitops::NE(piece_bitboards[color][kPawn]);
    under_control |= bitops::NW(piece_bitboards[color][kPawn]);
  }
  else {
    under_control |= bitops::SE(piece_bitboards[color][kPawn]);
    under_control |= bitops::SW(piece_bitboards[color][kPawn]);
  }
  BitBoard diag = piece_bitboards[color][kQueen] | piece_bitboards[color][kBishop];
  BitBoard ne_diag = bitops::FillNorthEast(diag, empty) | ~diag;
  ne_diag |= bitops::NE(ne_diag);
  BitBoard nw_diag = bitops::FillNorthWest(diag, empty) | ~diag;
  nw_diag |= bitops::NW(nw_diag);
  BitBoard se_diag = bitops::FillSouthEast(diag, empty) | ~diag;
  se_diag |= bitops::SE(se_diag);
  BitBoard sw_diag = bitops::FillSouthWest(diag, empty) | ~diag;
  sw_diag |= bitops::NW(sw_diag);
  under_control |= ne_diag | nw_diag | se_diag | sw_diag;

  BitBoard vec = piece_bitboards[color][kQueen] | piece_bitboards[color][kRook];
  BitBoard n_vec = bitops::FillNorth(vec, empty) | ~vec;
  n_vec |= bitops::N(n_vec);
  BitBoard s_vec = bitops::FillSouth(vec, empty) | ~vec;
  s_vec |= bitops::S(s_vec);
  BitBoard e_vec = bitops::FillEast(vec, empty) | ~vec;
  e_vec |= bitops::E(e_vec);
  BitBoard w_vec = bitops::FillWest(vec, empty) | ~vec;
  w_vec |= bitops::W(w_vec);
  under_control |= n_vec | s_vec | e_vec | w_vec;

  BitBoard knights = piece_bitboards[color][kKnight];
  vec = bitops::N(bitops::N(knights)) | bitops::S(bitops::S(knights));
  under_control |= bitops::E(vec) | bitops::W(vec);
  vec = bitops::E(bitops::E(knights)) | bitops::W(bitops::W(knights));
  under_control |= bitops::N(vec) | bitops::S(vec);
  BitBoard king = piece_bitboards[color][kKing];
  king |= bitops::E(king) | bitops::W(king);
  king |= bitops::N(king) | bitops::S(king);
  under_control |= king & ~piece_bitboards[color][kKing];
  return under_control;
}

std::vector<Move> Board::GetMoves() {
  std::vector<Move> moves;
  return moves;
}
