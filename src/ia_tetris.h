#ifndef IA_TETRIS_H
#define IA_TETRIS_H

#include "model_tetris.h"
#include "iatable.h"

void TestIA();
IATable* FindBestMove(Block* piece,Block** grid);
void ExecuteIAMove(IATable* iaTable,Block* piece,Block** grid);

#endif // IA_TETRIS_H
