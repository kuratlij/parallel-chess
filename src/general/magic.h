/*
 * magic.h
 *
 *  Created on: Dec 16, 2016
 *      Author: jonathan
 */

#ifndef GENERAL_MAGIC_H_
#define GENERAL_MAGIC_H_

#include "types.h"
#include "bit_operations.h"


namespace magic {

void Initialize();

template<PieceType pt>
BitBoard GetAttackMap(const int &index, BitBoard AllPieces);
BitBoard GetAttackVectors(BitBoard src, BitBoard des);

}

#endif /* GENERAL_MAGIC_H_ */
