#ifndef MODEL_TETRIS_H
#define MODEL_TETRIS_H

#include "shapes_type.h"
#include "move_type.h"
#include "block.h"
#include "piece.h"

Block** InitialiseGrid();
Block* InitialiseRandomPieces(Block** grid);
bool MovePiece(enum MoveType moveType,Block* piece,Block** grid);
bool RotatePiece(Block* piece,Block** grid);
void SetPiecePlaced(Block* piece,Block** grid);
bool ProceedCompleteLine(Block* piece,Block** grid);
bool LostConditionMeet(Block** grid);
long GetBlockSpeed(int rowCompleted);

void PrintGrid(Block** grid); //debug purpose
Block* InitialisePieces(enum ShapeType type,Block** grid);// debug purpose

#endif // MODEL_TETRIS_H
