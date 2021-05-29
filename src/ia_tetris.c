#include <stdlib.h>
#include <stdio.h>

#include "model_tetris.h"
#include "iatable.h"

#define BLOCKS_PER_PIECES 4
#define ROW 24 //18 lines plus 4 hidden at the top of the grid
#define COLUMN 10
#define SHAPE_NUMBER_TYPE 7 // The 7 shape, the empty is not count

/*
THE IA MECANISM ARE INSPIRED BY THE GREAT WEB ARTICLE : "Tetris AI – The (Near) Perfect Bot",Yiyuan Lee , April 14 2013
https://codemyroad.wordpress.com/2013/04/14/tetris-ai-the-near-perfect-player/
*/

/* Return a ptr of a special storage data.
* Structure have been choosen because of the clarity but a table of int could works too
* [0][1][2][3]
*  |  |  |  |
*  |  |  |  the quantity of right movement at the intantiation needed for this turn
*  |  | the quantity of left movement at the intantiation needed for this turn
*  | the quantity of rotation at the intantiation needed for this turn
* The score of the grid regading 4 factors such as Aggregate height + Complete line + holes + bumpiness     */
IATable* InitialiseIATable(enum MoveType moveType,int qtyMove,int qtyRotation){
  IATable* tableIA = (IATable*)malloc(1*sizeof(IATable));
  // init Score
  tableIA->score = 9999;
  // init qty move regading move type
  switch(moveType){
    case Left : tableIA->qtyMoveLeft = qtyMove;
                tableIA->qtyMoveRight = 0;
                break;
    case Right :tableIA->qtyMoveRight = qtyMove;
                tableIA->qtyMoveLeft = 0;
                break;
    default : printf("ERROR: wrong type move for IA Table");
  }
  // init rotation number
  tableIA->rotation = qtyRotation;

  return tableIA;
}

// Debug purpose
void PrintIATAble(IATable* input){
  printf("[%d][%d][%d][%d]\n",input->score,input->rotation,input->qtyMoveLeft,input->qtyMoveRight);
}

IATable* GetIATable(Block* piece,Block** grid,enum MoveType moveType,int qtyMove,int qtyRotation){
  IATable* resultIATable = InitialiseIATable(moveType,qtyMove,qtyRotation);
  Block** gridCopy = InitialiseCopyGrid(grid);
  Block pieceCopy[4];
  int agregateScore = 0;
  int completeLineScore = 0;
  int holedScore = 0;
  int bumpinessScore = 0;
  int score= 0 ;
  for(int i=0; i < BLOCKS_PER_PIECES; i++){
    pieceCopy[i] = piece[i];
  }
  RotatePieceIA(qtyRotation,pieceCopy,gridCopy);
  MovePieceIA(moveType,qtyMove,pieceCopy,gridCopy);
  MoveDownPieceIA(pieceCopy,gridCopy);

  resultIATable->score = GetAgregateScore(gridCopy) + GetCompleteLineScore(pieceCopy,gridCopy) + GetHolesScore(gridCopy) + GetBumpinessScore(gridCopy);
  PrintGrid(gridCopy);

  free(gridCopy);  // WARNING : CREATE HAZARDOUS BUG OCCURENCE ON THE EXE : QUIT PROG SOMETIME


  return resultIATable;
}


/* Function to provide a sum regarding heights of grid blocks
 *
 *  3   2   3   2   = 10
 * [ ]     [ ]
 * [ ] [ ] [ ] [ ]
 * [ ]     [ ] [ ]        */
int GetAgregateScore(Block** grid){
  int result = 0;
  int columnHeight;
  for(int column = 0 ; column < COLUMN ; column++){
    columnHeight = GetColumnHeight(column,grid);
    result = result + columnHeight  ;  // +1 for convert columnHeight (index row height) to row height
    }
  return result;
}

/* Function to provide a score regarding complete line of grid blocks
 *
 * [ ]     [ ]
 * [ ] [ ] [ ] [ ]  =  1
 * [ ]     [ ] [ ]        */
int GetCompleteLineScore(Block* piece,Block** grid){
  int* specialRowsTable =  GetNumberOfRowComplete(piece,grid); //dedicated table for management of complete row
  int nbLinecomplete = specialRowsTable[0];  // index 0 of this table provide the number of completed line (cf model_tetris.c)
  return nbLinecomplete;
}

/* Function to provide a score regarding hole line of grid blocks
 * [ ]     [ ]      = 0
 * [ ] [ ] [ ] [ ]
 * [ ]  x  [ ] [ ]  = 1
 * [ ]  x  [ ]  x   = 2  */
int GetHolesScore(Block** grid){
  int result=0;
  bool firstBlockOfColumnHaveBeenMeet;
  for(int column = 0 ; column < COLUMN ; column++){
    firstBlockOfColumnHaveBeenMeet = false;
    for(int row = 0 ; row < ROW ; row++){  //REMINDER : top of the grid with index 0, botom of the grid with index ROW -1,
      if(grid[row][column].type != EMPTY) firstBlockOfColumnHaveBeenMeet = true;
      if( firstBlockOfColumnHaveBeenMeet && (grid[row][column].type == EMPTY) ) result++;
    }
  }
  return result;
}

/* Function to provide a score regarding delta height of grid blocks
 *  3   2   3   1   6    =>  | 3 - 2 | + | 2 - 3 | + | 3 - 1 | + | 1 - 6 | = 9
 *                 [ ]
 * [ ]     [ ]     [ ]
 * [ ] [ ] [ ]     [ ]
 * [ ]     [ ] [ ] [ ]     */
int GetBumpinessScore(Block** grid){
  int result=0;
  for(int column = 0 ; column < (COLUMN-1) ; column++){
    result+=abs(GetColumnHeight(column,grid) - GetColumnHeight(column+1,grid));
  }
  return result;
}

/* */
void MovePieceIA(enum MoveType moveType,int qtyMove,Block* piece,Block** grid){
  for(int i=0; i< qtyMove;i++) MovePiece(moveType,piece,grid);
}

/* */
void RotatePieceIA(int qtyRotation,Block* piece,Block** grid){
  for(int iRotation = 0; iRotation < qtyRotation; iRotation++) RotatePiece(piece,grid);
}

/* */
void MoveDownPieceIA(Block* piece,Block** grid){
  while(MovePiece(Down,piece,grid)){}
}

/* */
int GetColumnHeight(int column,Block** grid){
  int result=0;
  for(int row = ROW -1; row >0 ; row--){ //REMINDER : top of the grid with index 0, botom of the grid with index ROW -1,
    if(grid[row][column].type != EMPTY) result++;
  }
  return result;
}

/* */
void ExecuteIAMove(IATable* iaTable,Block* piece,Block** grid){
  RotatePieceIA(iaTable->rotation,piece,grid);
  MovePieceIA(Left,iaTable->qtyMoveLeft,piece,grid);
  MovePieceIA(Right,iaTable->qtyMoveRight,piece,grid);
  MoveDownPieceIA(piece,grid);
}

/* */
IATable* FindBestMove(Block* piece,Block** grid){
  IATable* iaTable = InitialiseIATable(Left,0,0);
  IATable* tmp;
  for(int rotation = 0; rotation < 4; rotation++){
    for(int moveLeft=0; moveLeft < COLUMN/2; moveLeft++){
      tmp = GetIATable(piece,grid,Left,moveLeft,rotation);
      printf("tmp :\t");
      PrintIATAble(tmp);
      if (tmp->score < iaTable->score) iaTable = tmp;
      printf("iaTable :\t");
      PrintIATAble(iaTable);
    }
    for(int moveRight=0; moveRight < COLUMN/2; moveRight++){
      tmp = GetIATable(piece,grid,Right,moveRight,rotation);
      printf("tmp :\t");
      PrintIATAble(tmp);
      if (tmp->score < iaTable->score) iaTable = tmp;
      printf("iaTable :\t");
      PrintIATAble(iaTable);
    }
  }

  return iaTable;
}

void TestIA(){
  Block** grid = InitialiseGrid();
  Block* pieces;

  PrintGrid(grid);
  pieces = InitialiseRandomPieces(grid);
  for(int i=0;i<23;i++){
       MovePiece(Down,pieces,grid);
  }
  SetPiecePlaced(pieces,grid);
  ProceedCompleteLine(pieces,grid);

  pieces = InitialisePieces(I,grid);

  IATable* bestMove = FindBestMove(pieces,grid);
  PrintIATAble(bestMove);

  ExecuteIAMove(bestMove,pieces,grid);
  PrintGrid(grid);
  free(bestMove);

}
