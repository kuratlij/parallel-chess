/*
 * bit_operations.h
 *
 *  Created on: Nov 4, 2016
 *      Author: jonathan
 */

#ifndef GENERAL_BIT_OPERATIONS_H_
#define GENERAL_BIT_OPERATIONS_H_

#include "types.h"
#include "parse.h"

namespace bitops {

const BitBoard a_file = parse::StringToBitBoard("a1") | parse::StringToBitBoard("a2")
                      | parse::StringToBitBoard("a3") | parse::StringToBitBoard("a4")
                      | parse::StringToBitBoard("a5") | parse::StringToBitBoard("a6")
                      | parse::StringToBitBoard("a7") | parse::StringToBitBoard("a8");

const BitBoard h_file = parse::StringToBitBoard("h1") | parse::StringToBitBoard("h2")
                      | parse::StringToBitBoard("h3") | parse::StringToBitBoard("h4")
                      | parse::StringToBitBoard("h5") | parse::StringToBitBoard("h6")
                      | parse::StringToBitBoard("h7") | parse::StringToBitBoard("h8");


BitBoard N(BitBoard bb);
BitBoard S(BitBoard bb);
BitBoard E(BitBoard bb);
BitBoard W(BitBoard bb);
BitBoard NE(BitBoard bb);
BitBoard NW(BitBoard bb);
BitBoard SE(BitBoard bb);
BitBoard SW(BitBoard bb);

BitBoard FillNorth(BitBoard bb, BitBoard empty);
BitBoard FillSouth(BitBoard bb, BitBoard empty);
BitBoard FillEast(BitBoard bb, BitBoard empty);
BitBoard FillWest(BitBoard bb, BitBoard empty);
BitBoard FillNorthEast(BitBoard bb, BitBoard empty);
BitBoard FillNorthWest(BitBoard bb, BitBoard empty);
BitBoard FillSouthEast(BitBoard bb, BitBoard empty);
BitBoard FillSouthWest(BitBoard bb, BitBoard empty);

// return the number of trailing zeros
inline int NumberOfTrailingZeros(BitBoard x) {
    return __builtin_ctzll(x);
}
// return the leading number of zeros
inline int NumberOfLeadingZeros(BitBoard x) {
    return __builtin_clzll(x);
}

// return number of set 1 bits
inline int PopCount(BitBoard x) {
    return __builtin_popcountll(x);
}

// return the least significant 1 bit
inline BitBoard GetLSB(BitBoard x) {
    return x & -x;
}

// remove the least significant 1 bit from the param
inline void PopLSB(BitBoard &x) {
    x &= x-1;
}

}

#endif /* GENERAL_BIT_OPERATIONS_H_ */
