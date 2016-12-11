/*
 * board.cc
 *
 *  Created on: Nov 1, 2016
 *      Author: jonathan
 */

#include "board.h"

namespace {

const BitBoard fourth_row = parse::StringToBitBoard("a4") | parse::StringToBitBoard("b4")
                      | parse::StringToBitBoard("c4") | parse::StringToBitBoard("d4")
                      | parse::StringToBitBoard("e4") | parse::StringToBitBoard("f4")
                      | parse::StringToBitBoard("g4") | parse::StringToBitBoard("h4");

const BitBoard fifth_row = fourth_row << 8;

const BitBoard castling_relevant_bbs[] = {
    parse::StringToBitBoard("h1") | parse::StringToBitBoard("e1"),
    parse::StringToBitBoard("a1") | parse::StringToBitBoard("e1"),
    parse::StringToBitBoard("h8") | parse::StringToBitBoard("e8"),
    parse::StringToBitBoard("a8") | parse::StringToBitBoard("e8")
};
const BitBoard castling_check_bbs[] = {
    parse::StringToBitBoard("e1") | parse::StringToBitBoard("f1") | parse::StringToBitBoard("g1"),
    parse::StringToBitBoard("c1") | parse::StringToBitBoard("d1") | parse::StringToBitBoard("e1"),
    parse::StringToBitBoard("e8") | parse::StringToBitBoard("f8") | parse::StringToBitBoard("g8"),
    parse::StringToBitBoard("c8") | parse::StringToBitBoard("d8") | parse::StringToBitBoard("e8")
};
const BitBoard castling_empty_bbs[] = {
    parse::StringToBitBoard("f1") | parse::StringToBitBoard("g1"),
    parse::StringToBitBoard("b1") | parse::StringToBitBoard("c1") | parse::StringToBitBoard("d1"),
    parse::StringToBitBoard("f8") | parse::StringToBitBoard("g8"),
    parse::StringToBitBoard("b8") | parse::StringToBitBoard("c8") | parse::StringToBitBoard("d8")
};

const BitBoard all_castling_squares = parse::StringToBitBoard("h1") | parse::StringToBitBoard("e1")
                                    | parse::StringToBitBoard("a1") | parse::StringToBitBoard("e1")
                                    | parse::StringToBitBoard("h8") | parse::StringToBitBoard("e8")
                                    | parse::StringToBitBoard("a8") | parse::StringToBitBoard("e8");

//Define helper functions for move history information
void SaveEnPassant(MoveHistoryInformation &info, Square ep) {
  info |= (ep << 4);
}
Square GetEnPassant(const MoveHistoryInformation info) {
  return (info >> 4) & 0x3F;
}
void SaveMovingPiece(MoveHistoryInformation &info, Piece piece) {
  info |= piece;
}
Piece GetMovingPiece(const MoveHistoryInformation info) {
  return info & 0xF;
}
void SaveCastlingRignts(MoveHistoryInformation &info, CastlingRights rights) {
  info |= rights << 10;
}
CastlingRights GetCastlingRights(const MoveHistoryInformation info) {
  return info >> 10 & 0xF;
}

}


namespace {

void AddMoves(std::vector<Move> &move_list, BitBoard source, BitBoard destinations,
    BitBoard enemy_pieces) {
  Square source_square = bitops::NumberOfTrailingZeros(source);
  BitBoard captures = destinations & enemy_pieces;
  BitBoard regulars = destinations ^ captures;
  while (captures) {
    BitBoard capture = bitops::GetLSB(captures);
    Square destination_square = bitops::NumberOfTrailingZeros(capture);
    move_list.emplace_back(GetMove(source_square, destination_square, kCapture));
    bitops::PopLSB(captures);
  }
  while (regulars) {
    BitBoard regular = bitops::GetLSB(regulars);
    Square destination_square = bitops::NumberOfTrailingZeros(regular);
    move_list.emplace_back(GetMove(source_square, destination_square, kNormalMove));
    bitops::PopLSB(regulars);
  }
}

}

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
  hash = 0;
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

void Board::SetStartBoard() {
  hash = 0;
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
  debug::Print("set starting position", debug::kDebugBoard);
}

void Board::AddPiece(Square square, Piece piece) {
  piece_bitboards[GetPieceColor(piece)][GetPieceType(piece)] |= GetSquareBitBoard(square);
  pieces[square] = piece;
  hash ^= hash::get_piece(piece, square);
}

Piece Board::RemovePiece(Square square) {
  Piece piece = pieces[square];
  pieces[square] = kNoPiece;
  piece_bitboards[GetPieceColor(piece)][GetPieceType(piece)] ^= GetSquareBitBoard(square);
  hash ^= hash::get_piece(piece, square);
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
  hash ^= hash::get_color_hash();
}

void Board::Make(Move move) {
  MoveHistoryInformation information = 0;
  SaveMovingPiece(information, RemovePiece(GetMoveDestination(move)));
  MovePiece(GetMoveSource(move),GetMoveDestination(move));
  SaveEnPassant(information, en_passant);
  SaveCastlingRignts(information, castling_rights);
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
  BitBoard srcdes = GetSquareBitBoard(GetMoveSource(move))
      | GetSquareBitBoard(GetMoveDestination(move));
  if (srcdes & all_castling_squares) {
    for (int right = 0; right < 4; right++) {
      if (castling_relevant_bbs[right] & srcdes) {
        castling_rights &= ~(0x1 << right);
      }
    }
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
  AddPiece(GetMoveDestination(move), GetMovingPiece(info));
  en_passant = GetEnPassant(info);
  castling_rights = GetCastlingRights(info);
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
    for (PieceType piece_type = kKing; piece_type <= kPawn; piece_type++) {
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
  BitBoard ne_diag = bitops::FillNorthEast(diag, empty);
  ne_diag |= bitops::NE(ne_diag);
  BitBoard nw_diag = bitops::FillNorthWest(diag, empty);
  nw_diag |= bitops::NW(nw_diag);
  BitBoard se_diag = bitops::FillSouthEast(diag, empty);
  se_diag |= bitops::SE(se_diag);
  BitBoard sw_diag = bitops::FillSouthWest(diag, empty);
  sw_diag |= bitops::SW(sw_diag);
  under_control |= ne_diag | nw_diag | se_diag | sw_diag;

  BitBoard vec = piece_bitboards[color][kQueen] | piece_bitboards[color][kRook];
  BitBoard n_vec = bitops::FillNorth(vec, empty);
  n_vec |= bitops::N(n_vec);
  BitBoard s_vec = bitops::FillSouth(vec, empty);
  s_vec |= bitops::S(s_vec);
  BitBoard e_vec = bitops::FillEast(vec, empty);
  e_vec |= bitops::E(e_vec);
  BitBoard w_vec = bitops::FillWest(vec, empty);
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
  //parse::PrintBitboard(under_control);
  return under_control;
}

std::vector<Move> Board::GetMoves() {
  std::vector<Move> moves;
  BitBoard own_pieces = piece_bitboards[turn][kKing];
  BitBoard enemy_pieces = piece_bitboards[turn^0x1][kKing];
  for (PieceType piece_type = kQueen; piece_type <= kPawn; piece_type++) {
    own_pieces |= piece_bitboards[turn][piece_type];
    enemy_pieces |= piece_bitboards[turn^0x1][piece_type];
  }
  BitBoard empty = (~own_pieces) & (~enemy_pieces);

  //King
  //TODO: Add castling
  BitBoard king = piece_bitboards[turn][kKing];
  king |= bitops::E(king) | bitops::W(king);
  king |= bitops::N(king) | bitops::S(king);
  king &= ~own_pieces;
  AddMoves(moves, piece_bitboards[turn][kKing], king, enemy_pieces);
  BitBoard in_check = PlayerBitBoardControl(turn^0x1);
  for (int right = 0 + 2*turn; right < 4-2*turn; right++) {
    if ((castling_rights & (0x1 << right))
        && !(castling_check_bbs[right] & in_check)
        && !(castling_empty_bbs[right] & (own_pieces | enemy_pieces))) {
      Square source = bitops::NumberOfTrailingZeros(piece_bitboards[turn][kKing]);
      Square destination = source + 2 - (right%2)*4;
      moves.emplace_back(GetMove(source, destination, kCastle));
    }
  }

  //Vertical/Horizontal Sliders
  BitBoard vertical_movers = piece_bitboards[turn][kQueen]
                           | piece_bitboards[turn][kRook];
  while (vertical_movers) {
    BitBoard mover = bitops::GetLSB(vertical_movers);
    BitBoard n_vec = bitops::FillNorth(mover, empty);
    n_vec |= bitops::N(n_vec);
    BitBoard s_vec = bitops::FillSouth(mover, empty);
    s_vec |= bitops::S(s_vec);
    BitBoard e_vec = bitops::FillEast(mover, empty);
    e_vec |= bitops::E(e_vec);
    BitBoard w_vec = bitops::FillWest(mover, empty);
    w_vec |= bitops::W(w_vec);
    BitBoard destinations = (n_vec | s_vec | e_vec | w_vec) & ~own_pieces;
    AddMoves(moves, mover, destinations, enemy_pieces);
    bitops::PopLSB(vertical_movers);
  }

  //Diagonal Sliders
  BitBoard diagonal_movers = piece_bitboards[turn][kQueen]
                           | piece_bitboards[turn][kBishop];
  while (diagonal_movers) {
    BitBoard mover = bitops::GetLSB(diagonal_movers);
    BitBoard ne_diag = bitops::FillNorthEast(mover, empty);
    ne_diag |= bitops::NE(ne_diag);
    BitBoard nw_diag = bitops::FillNorthWest(mover, empty);
    nw_diag |= bitops::NW(nw_diag);
    BitBoard se_diag = bitops::FillSouthEast(mover, empty);
    se_diag |= bitops::SE(se_diag);
    BitBoard sw_diag = bitops::FillSouthWest(mover, empty);
    sw_diag |= bitops::SW(sw_diag);
    BitBoard destinations = (ne_diag | nw_diag | se_diag | sw_diag) & ~own_pieces;
    AddMoves(moves, mover, destinations, enemy_pieces);
    bitops::PopLSB(diagonal_movers);
  }

  //Knight moves
  BitBoard knights = piece_bitboards[turn][kKnight];
  while (knights) {
    BitBoard knight = bitops::GetLSB(knights);
    BitBoard vec = bitops::N(bitops::N(knight)) | bitops::S(bitops::S(knight));
    BitBoard destinations = bitops::E(vec) | bitops::W(vec);
    vec = bitops::E(bitops::E(knight)) | bitops::W(bitops::W(knight));
    destinations |= bitops::N(vec) | bitops::S(vec);
    destinations &= ~own_pieces;
    AddMoves(moves, knight, destinations, enemy_pieces);
    bitops::PopLSB(knights);
  }

  //Pawns
  //TODO: Add promotions and en passant capturing
  if (turn == kWhite) {
    BitBoard single_push = bitops::N(piece_bitboards[kWhite][kPawn])
                         & empty;
    BitBoard double_push = bitops::N(single_push) & empty & fourth_row;
    while (single_push) {
      BitBoard destination = bitops::NumberOfTrailingZeros(
          bitops::GetLSB(single_push));
      moves.emplace_back(GetMove(destination-8, destination, kNormalMove));
      bitops::PopLSB(single_push);
    }
    while (double_push) {
      BitBoard destination = bitops::NumberOfTrailingZeros(
          bitops::GetLSB(double_push));
      moves.emplace_back(GetMove(destination-16, destination, kDoublePawnMove));
      bitops::PopLSB(double_push);
    }
    BitBoard ne_captures = bitops::NE(piece_bitboards[kWhite][kPawn])
                         & enemy_pieces;
    while (ne_captures) {
      BitBoard destination = bitops::NumberOfTrailingZeros(
          bitops::GetLSB(ne_captures));
      moves.emplace_back(GetMove(destination-9, destination, kCapture));
      bitops::PopLSB(ne_captures);
    }
    BitBoard nw_captures = bitops::NW(piece_bitboards[kWhite][kPawn])
                         & enemy_pieces;
    while (nw_captures) {
      BitBoard destination = bitops::NumberOfTrailingZeros(
          bitops::GetLSB(nw_captures));
      moves.emplace_back(GetMove(destination-7, destination, kCapture));
      bitops::PopLSB(nw_captures);
    }

  }
  else {
    BitBoard single_push = bitops::S(piece_bitboards[kBlack][kPawn])
                         & empty;
    BitBoard double_push = bitops::S(single_push) & empty & fifth_row;
    while (single_push) {
      BitBoard destination = bitops::NumberOfTrailingZeros(
          bitops::GetLSB(single_push));
      moves.emplace_back(GetMove(destination+8, destination, kNormalMove));
      bitops::PopLSB(single_push);
    }
    while (double_push) {
      BitBoard destination = bitops::NumberOfTrailingZeros(
          bitops::GetLSB(double_push));
      moves.emplace_back(GetMove(destination+16, destination, kDoublePawnMove));
      bitops::PopLSB(double_push);
    }
    BitBoard ne_captures = bitops::SE(piece_bitboards[kBlack][kPawn])
                         & enemy_pieces;
    while (ne_captures) {
      BitBoard destination = bitops::NumberOfTrailingZeros(
          bitops::GetLSB(ne_captures));
      moves.emplace_back(GetMove(destination+7, destination, kCapture));
      bitops::PopLSB(ne_captures);
    }
    BitBoard nw_captures = bitops::SW(piece_bitboards[kBlack][kPawn])
                         & enemy_pieces;
    while (nw_captures) {
      BitBoard destination = bitops::NumberOfTrailingZeros(
          bitops::GetLSB(nw_captures));
      moves.emplace_back(GetMove(destination+9, destination, kCapture));
      bitops::PopLSB(nw_captures);
    }
  }

  //Now we need to remove illegal moves.
  std::vector<Move> legal_moves;
  for (Move move : moves) {
    Make(move);
    if (!(piece_bitboards[turn^0x1][kKing] & PlayerBitBoardControl(turn))) {
      if (GetPieceType(pieces[GetMoveDestination(move)]) == kPawn
          && (GetSquareY(GetMoveDestination(move)) - 7*turn) == 0) {
        Square source = GetMoveSource(move);
        Square destination = GetMoveDestination(move);
        for (MoveType move_type = kQueenPromotion; move_type <= kKnightPromotion; move_type++) {
          legal_moves.emplace_back(GetMove(source, destination, move_type));
        }
      }
      else {
        legal_moves.emplace_back(move);
      }
    }
    UnMake();
  }
  return legal_moves;
}

HashType Board::get_hash() {
  return hash;
}


PieceBitboardSet Board::get_piece_bitboards() {
  PieceBitboardSet bitboards;
  for (Color color = kWhite; color <= kBlack; color++) {
    for (PieceType piece_type = kKing; piece_type < kNumPieceTypes; piece_type++) {
      bitboards.piece_bitboards[color][piece_type] = piece_bitboards[color][piece_type];
    }
  }
  return bitboards;
}

Color Board::get_turn() {
  return turn;
}

bool Board::InCheck() {
  return (PlayerBitBoardControl(turn^0x1L) & piece_bitboards[turn][kKing]);
}

