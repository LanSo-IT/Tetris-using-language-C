#ifndef MODEL_TETRIS_H
#define MODEL_TETRIS_H

#include "shapes_type.h"
#include "move_type.h"
#include "block.h"
#include "piece.h"

Block** InitialiseGrid();
Block** InitialiseCopyGrid(Block** gridOrigin);
Block* InitialiseRandomPieces(Block** grid);
Block* InitialisePieces(enum ShapeType type,Block** grid);
bool MovePiece(enum MoveType moveType,Block* piece,Block** grid);
bool RotatePiece(Block* piece,Block** grid);
void SetPiecePlaced(Block* piece,Block** grid);
int ProceedCompleteLine(Block* piece,Block** grid);
bool LostConditionMeet(Block** grid);
long GetBlockSpeed(int rowCompleted);
int UpdateScoreTetris(int score,int rowCompleted);

void CreatHoledLineInGrid(Block* piece,Block** grid,int qty); // 2P Game

Block* ManagePlayerPiecesSwitch(enum ShapeType* storedPiecesType,Block* piece,Block** grid);

void PrintGrid(Block** grid); //debug purpose
#endif // MODEL_TETRIS_H
