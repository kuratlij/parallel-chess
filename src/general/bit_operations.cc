/*
 * bit_operations.cc
 *
 *  Created on: Nov 4, 2016
 *      Author: jonathan
 */

#include "bit_operations.h"

namespace bitops {

BitBoard N(BitBoard bb) {
  return bb << 8;
}

BitBoard S(BitBoard bb) {
  return bb >> 8;
}

BitBoard E(BitBoard bb) {
  return (bb >> 1) & ~a_file;
}

BitBoard W(BitBoard bb) {
  return (bb << 1) & ~h_file;
}

BitBoard NE(BitBoard bb) {
  return (bb << 7) & ~a_file;
}

BitBoard NW(BitBoard bb) {
  return (bb << 9) & ~h_file;
}

BitBoard SE(BitBoard bb) {
  return (bb >> 9) & ~a_file;
}

BitBoard SW(BitBoard bb) {
  return (bb >> 7) & ~h_file;
}

BitBoard FillNorth(BitBoard bb, BitBoard empty) {
  bb |= empty & (bb << 8);
  empty &= empty << 8;
  bb |= empty & (bb << 16);
  empty &= empty << 16;
  bb |= empty & (bb << 32);
  return bb;
}

BitBoard FillSouth(BitBoard bb, BitBoard empty) {
  bb |= empty & (bb >> 8);
  empty &= empty >> 8;
  bb |= empty & (bb >> 16);
  empty &= empty >> 16;
  bb |= empty & (bb >> 32);
  return bb;
}

BitBoard FillEast(BitBoard bb, BitBoard empty) {
  empty &= ~a_file;
  bb |= empty & (bb >> 1);
  empty &= empty >> 1;
  bb |= empty & (bb >> 2);
  empty &= empty >> 2;
  bb |= empty & (bb >> 4);
  return bb;
}

BitBoard FillWest(BitBoard bb, BitBoard empty) {
  empty &= ~h_file;
  bb |= empty & (bb << 1);
  empty &= empty << 1;
  bb |= empty & (bb << 2);
  empty &= empty << 2;
  bb |= empty & (bb << 4);
  return bb;
}

BitBoard FillNorthEast(BitBoard bb, BitBoard empty) {
  empty &= ~a_file;
  bb |= empty & (bb << 7);
  empty &= empty << 7;
  bb |= empty & (bb << 14);
  empty &= empty << 14;
  bb |= empty & (bb << 28);
  return bb;
}

BitBoard FillNorthWest(BitBoard bb, BitBoard empty) {
  empty &= ~h_file;
  bb |= empty & (bb << 9);
  empty &= empty << 9;
  bb |= empty & (bb << 18);
  empty &= empty << 18;
  bb |= empty & (bb << 36);
  return bb;
}

BitBoard FillSouthEast(BitBoard bb, BitBoard empty) {
  empty &= ~h_file;
  bb |= empty & (bb >> 9);
  empty &= empty >> 9;
  bb |= empty & (bb >> 18);
  empty &= empty >> 18;
  bb |= empty & (bb >> 36);
  return bb;
}

BitBoard FillSouthWest(BitBoard bb, BitBoard empty) {
  empty &= ~a_file;
  bb |= empty & (bb >> 7);
  empty &= empty >> 7;
  bb |= empty & (bb >> 14);
  empty &= empty >> 14;
  bb |= empty & (bb >> 28);
  return bb;
}


}


