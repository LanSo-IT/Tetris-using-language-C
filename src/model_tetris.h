#ifndef MODEL_TETRIS_H
#define MODEL_TETRIS_H

Block** InitialiseGrid();
Block* InitialiseRandomPieces(Block** grid);
bool MovePiece(enum MoveType moveType,Block* piece,Block** grid);
void SetPiecePlaced(Block* piece,Block** grid);
bool ProceedCompleteLine(Block* piece,Block** grid);
bool LostConditionMeet(Block** grid);

void PrintGrid(Block** grid); //debug purpose
Block* InitialisePieces(enum ShapeType type,Block** grid);// debug purpose

#endif // MODEL_TETRIS_H
