/*
 * zobrist.h
 *
 *  Created on: Dec 10, 2016
 *      Author: jonathan
 */

#ifndef ZOBRIST_H_
#define ZOBRIST_H_

#include <random>
#include "types.h"

namespace hash {

void initialize();
HashType get_piece(Piece piece, Square square);
HashType get_color_hash();

}


#endif /* ZOBRIST_H_ */
