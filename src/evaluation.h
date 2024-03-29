/*
 * evaluation.h
 *
 * The evaluation source and header files with associated namespace is used
 * in order to score positions so the search function can differentiate how
 * good different positions are
 *
 *  Created on: Nov 1, 2016
 *      Author: jonathan
 */

#ifndef EVALUATION_H_
#define EVALUATION_H_

#include "general_headers.h"
#include "board.h"

namespace evaluation {

Score ScoreBoard(Board board);

}

#endif /* EVALUATION_H_ */
