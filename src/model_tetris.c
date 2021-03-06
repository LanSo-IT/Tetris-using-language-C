#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "shapes_type.h"
#include "move_type.h"
#include "block.h"
#include "piece.h"

#define BLOCKS_PER_PIECES 4
#define ROW 24 //18 lines plus 4 hidden at the top of the grid
#define COLUMN 10
#define SHAPE_NUMBER_TYPE 7 // The 7 shape, the empty is not count

/* Initialise grid ROW(24)xCOLUMN(10), 4 first row will be hidden : it's the spawning area
                                                                  (y)
      EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY  0
      EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY  1
      EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY  2
      EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY  3
      ---   ---   ---   ---   ---   ---   ---   ---   ---   ---
      EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY  4
      EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY  5
      EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY  6
      EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY  7
      EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY  8
      EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY  9
      EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY  10
      EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY  11
      EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY  12
      EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY  13
      EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY  14
      EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY  15
      EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY  16
      EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY  17
      EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY  18
      EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY  19
      EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY  20
      EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY  21
      EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY  22
      EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY  23
  (x)   0     1     2     3     4     5     6     7     8     9

      [!] TAKE CARE [!] it's grid[Y][X] <=> grid[row][column]  */
Block** InitialiseGrid(){
    Block** grid = (Block**)malloc(ROW*sizeof(Block*));
    for(int i=0;i<ROW;i++){
        grid[i]= (Block*)malloc(COLUMN*sizeof(Block));
    }
    InitialiseAllBlockEmpty(grid);
    return grid;
}

/* Initialise a copy  of an existing grid with a size ROW(24)xCOLUMN(10) */
Block** InitialiseCopyGrid(Block** gridOrigin){
    Block** grid = (Block**)malloc(ROW*sizeof(Block*));
    for(int i=0;i<ROW;i++){
        grid[i]= (Block*)malloc(COLUMN*sizeof(Block));
    }
    InitialiseWithCopy(gridOrigin,grid);
    return grid;
}

/* Initialise a grid by setting the different data of each block
 * Will provide for each block :
 *  - the correct row number
 *  - collumn number
 *  - activity state
 *  - the disable center
 *  - the kind of blocks : empty at the beginning */
void InitialiseAllBlockEmpty(Block** grid){
    for(int row = 0 ; row<ROW; row++){
        for(int column = 0; column< COLUMN; column++){
            grid[row][column].row = row;
            grid[row][column].column = column;
            grid[row][column].active = false;
            grid[row][column].center = false;
            grid[row][column].type = EMPTY;
        }
    }
}

/* function that initialise the second input grid like the first one
 * It will go throught every blocks of the grid and copy every aspect for the target one
 * It will result of two different pointer of a grid having the same properties     */
void InitialiseWithCopy(Block** gridOrigin, Block** gridToCopy){
    for(int row = 0 ; row<ROW; row++){
        for(int column = 0; column< COLUMN; column++){
            gridToCopy[row][column].row = gridOrigin[row][column].row;
            gridToCopy[row][column].column = gridOrigin[row][column].column;
            gridToCopy[row][column].active = gridOrigin[row][column].active;
            gridToCopy[row][column].center = gridOrigin[row][column].center;
            gridToCopy[row][column].type =   gridOrigin[row][column].type;
        }
    }
}

/* L Piece, center of rotation is R
 * Will be created in the hidden area
 * _ _ _ _ _ _ _ _ _ _
 * _ _ _ _ _ X _ _ _ _
 * _ _ _ _ _ R _ _ _ _
 * _ _ _ _ X X _ _ _ _
 * Return a ptr of the pieces for an easier management of the piece (the pieces have to be in the hidden area : 4x10)
 * [0][1][2][3]
 *  |  |  |  |
 *  |  |  |  A pieces found (not center)
 *  |  |  A pieces found (not center)
 *  |  A pieces found (not center)
 * THE CENTER BLOCK */
Block* InitialiseLPieces(Block** grid){
    Block* pieces = (Block*)malloc(BLOCKS_PER_PIECES*sizeof(Block));

    grid[2][5].active = true;
    grid[2][5].type = L;
    grid[2][5].center = true;
    pieces[0] = grid[2][5];

    grid[1][5].active = true;
    grid[1][5].type = L;
    pieces[1] = grid[1][5];

    grid[3][5].active = true;
    grid[3][5].type = L;
    pieces[2] = grid[3][5];

    grid[3][4].active = true;
    grid[3][4].type = L;
    pieces[3] = grid[3][4];

    return pieces;
}

/* J Piece, center of rotation is R
 * Will be created in the hidden area
 * _ _ _ _ _ _ _ _ _ _
 * _ _ _ _ X _ _ _ _ _
 * _ _ _ _ R _ _ _ _ _
 * _ _ _ _ X X _ _ _ _
 * Return a ptr of the pieces for an easier management of the piece (the pieces have to be in the hidden area : 4x10)
 * [0][1][2][3]
 *  |  |  |  |
 *  |  |  |  A pieces found (not center)
 *  |  |  A pieces found (not center)
 *  |  A pieces found (not center)
 * THE CENTER BLOCK */
Block* InitialiseJPieces(Block** grid){
    Block* pieces = (Block*)malloc(BLOCKS_PER_PIECES*sizeof(Block));

    grid[2][4].active = true;
    grid[2][4].type = J;
    grid[2][4].center = true;
    pieces[0] = grid[2][4];

    grid[1][4].active = true;
    grid[1][4].type = J;
    pieces[1] = grid[1][4];

    grid[3][5].active = true;
    grid[3][5].type = J;
    pieces[2] = grid[3][5];

    grid[3][4].active = true;
    grid[3][4].type = J;
    pieces[3] = grid[3][4];

    return pieces;
}

/* O Piece, center of rotation is none
 * Will be created in the hidden area
 * _ _ _ _ _ _ _ _ _ _
 * _ _ _ _ _ _ _ _ _ _
 * _ _ _ _ X X _ _ _ _
 * _ _ _ _ X X _ _ _ _
 * Return a ptr of the pieces for an easier management of the piece (the pieces have to be in the hidden area : 4x10)
 * [0][1][2][3]
 *  |  |  |  |
 *  |  |  |  A pieces found (not center)
 *  |  |  A pieces found (not center)
 *  |  A pieces found (not center)
 * THE CENTER BLOCK */
Block* InitialiseOPieces(Block** grid){
    Block* pieces = (Block*)malloc(BLOCKS_PER_PIECES*sizeof(Block));

    grid[2][4].active = true;
    grid[2][4].type = O;
    pieces[0] = grid[2][4];

    grid[2][5].active = true;
    grid[2][5].type = O;
    pieces[1] = grid[2][5];

    grid[3][5].active = true;
    grid[3][5].type = O;
    pieces[2] = grid[3][5];

    grid[3][4].active = true;
    grid[3][4].type = O;
    pieces[3] = grid[3][4];

    return pieces;
}

/* I Piece, center of rotation is R
 * Will be created in the hidden area
 * _ _ _ _ _ _ _ _ _ _
 * _ _ _ _ _ _ _ _ _ _
 * _ _ _ _ _ _ _ _ _ _
 * _ _ _ X X R X _ _ _
 * Return a ptr of the pieces for an easier management of the piece (the pieces have to be in the hidden area : 4x10)
 * [0][1][2][3]
 *  |  |  |  |
 *  |  |  |  A pieces found (not center)
 *  |  |  A pieces found (not center)
 *  |  A pieces found (not center)
 * THE CENTER BLOCK */
Block* InitialiseIPieces(Block** grid){
    Block* pieces = (Block*)malloc(BLOCKS_PER_PIECES*sizeof(Block));
    grid[3][5].active = true;
    grid[3][5].type = I;
    grid[3][5].center = true;
    pieces[0] = grid[3][5];

    grid[3][3].active = true;
    grid[3][3].type = I;
    pieces[1] = grid[3][3];

    grid[3][4].active = true;
    grid[3][4].type = I;
    pieces[2] = grid[3][4];

    grid[3][6].active = true;
    grid[3][6].type = I;
    pieces[3] = grid[3][6];
    return pieces;
}
/* Z Piece, center of rotation is R
 * Will be created in the hidden area
 * _ _ _ _ _ _ _ _ _ _
 * _ _ _ _ _ _ _ _ _ _
 * _ _ _ X X _ _ _ _ _
 * _ _ _ _ R X _ _ _ _
 * Return a ptr of the pieces for an easier management of the piece (the pieces have to be in the hidden area : 4x10)
 * [0][1][2][3]
 *  |  |  |  |
 *  |  |  |  A pieces found (not center)
 *  |  |  A pieces found (not center)
 *  |  A pieces found (not center)
 * THE CENTER BLOCK */
Block* InitialiseZPieces(Block** grid){
    Block* pieces = (Block*)malloc(BLOCKS_PER_PIECES*sizeof(Block));
    grid[3][4].active = true;
    grid[3][4].center = true;
    grid[3][4].type = Z;
    pieces[0] = grid[3][4];

    grid[2][3].active = true;
    grid[2][3].type = Z;
    pieces[1] = grid[2][3];

    grid[3][5].active = true;
    grid[3][5].type = Z;
    pieces[2] = grid[3][5];

    grid[2][4].active = true;
    grid[2][4].type = Z;
    pieces[3] = grid[2][4];

    return pieces;
}

/* S Piece, center of rotation is R
 * Will be created in the hidden area
 * _ _ _ _ _ _ _ _ _ _
 * _ _ _ _ _ _ _ _ _ _
 * _ _ _ _ X X _ _ _ _
 * _ _ _ X R _ _ _ _ _
 * Return a ptr of the pieces for an easier management of the piece (the pieces have to be in the hidden area : 4x10)
 * [0][1][2][3]
 *  |  |  |  |
 *  |  |  |  A pieces found (not center)
 *  |  |  A pieces found (not center)
 *  |  A pieces found (not center)
 * THE CENTER BLOCK */
Block* InitialiseSPieces(Block** grid){
    Block* pieces = (Block*)malloc(BLOCKS_PER_PIECES*sizeof(Block));

    grid[3][4].active = true;
    grid[3][4].type = S;
    grid[3][4].center = true;
    pieces[0] = grid[3][4];

    grid[2][4].active = true;
    grid[2][4].type = S;
    pieces[1] = grid[2][4];

    grid[2][5].active = true;
    grid[2][5].type = S;
    pieces[2] = grid[2][5];

    grid[3][3].active = true;
    grid[3][3].type = S;
    pieces[3] = grid[3][3];

    return pieces;

}

/* T Piece, center of rotation is R
 * Will be created in the hidden area
 * _ _ _ _ _ _ _ _ _ _
 * _ _ _ _ _ _ _ _ _ _
 * _ _ _ _ X _ _ _ _ _
 * _ _ _ X R X _ _ _ _
 * Return a ptr of the pieces for an easier management of the piece (the pieces have to be in the hidden area : 4x10)
 * [0][1][2][3]
 *  |  |  |  |
 *  |  |  |  A pieces found (not center)
 *  |  |  A pieces found (not center)
 *  |  A pieces found (not center)
 * THE CENTER BLOCK   */
Block* InitialiseTPieces(Block** grid){
    Block* pieces = (Block*)malloc(BLOCKS_PER_PIECES*sizeof(Block));

    grid[3][4].active = true;
    grid[3][4].center = true;
    grid[3][4].type = T;
    pieces[0] = grid[3][4];

    grid[3][5].active = true;
    grid[3][5].type = T;
    pieces[1] = grid[3][5];

    grid[2][4].active = true;
    grid[2][4].type = T;
    pieces[2] = grid[2][4];

    grid[3][3].active = true;
    grid[3][3].type = T;
    pieces[3] = grid[3][3];

    return pieces;
}

/* Create an empty pieces
 * Piece must bu use only for help to manage pieces player storage
 * Return a ptr of the pieces for an easier management of the piece
 * Grid is not updated with empty pieces
 * [0][1][2][3]
 *  |  |  |  |
 *  |  |  |  EMPTY, Active, (0,0)
 *  |  |  EMPTY, Active, (0,0)
 *  |  EMPTY, Active, (0,0)
 * EMPTY, Active, (0,0)   */
Block* InitialiseEmptyPieces(){
    Block* pieces = (Block*)malloc(BLOCKS_PER_PIECES*sizeof(Block));
    for(int i=0 ; i< BLOCKS_PER_PIECES; i++){
      pieces[i].active = true;
      pieces[i].center = true;
      pieces[i].type = EMPTY;
      pieces[i].row = 0;
      pieces[i].column = 0;
    }
    return pieces;
}

/* Initialise a given pieces to the grid
 * Return a pieces pointor having provided type and in the hidden area */
Block* InitialisePieces(enum ShapeType type,Block** grid){
  switch(type){
    case L : return InitialiseLPieces(grid);
    case J : return InitialiseJPieces(grid);
    case T : return InitialiseTPieces(grid);
    case S : return InitialiseSPieces(grid);
    case Z : return InitialiseZPieces(grid);
    case I : return InitialiseIPieces(grid);
    case O : return InitialiseOPieces(grid);
    case EMPTY: return InitialiseEmptyPieces(); // for pieces storage location, no associated with grid
    default : printf("ERROR : enum not in range [L,J,T,S,Z,I,O]");
  }
}

/* Provide a random type
 * Function for get a random type among the SHAPE_NUMBER_TYPE */
enum ShapeType GetRandomType(){
  enum ShapeType type;
  int randomValue = rand()% SHAPE_NUMBER_TYPE +1 ; //provide random from 1 to SHAPE_NUMBER_TYPE
  switch(randomValue){
    case 1 : return type = L;
    case 2 : return type = J;
    case 3 : return type = T;
    case 4 : return type = S;
    case 5 : return type = Z;
    case 6 : return type = I;
    case 7 : return type = O;
    default : printf("ERROR : random value not in range [L,J,T,S,Z,I,O]");
  }
}

/* Initialise a pieces to the grid (pieces will be genreated randomly)
 * Return a pieces pointor having random type and in the hidden area */
Block* InitialiseRandomPieces(Block** grid){
  enum ShapeType type = GetRandomType() ;
  return InitialisePieces(type,grid);
}

bool IsOutOfColumnRange(int targetColumn){
    if( (targetColumn > (COLUMN-1)) || (targetColumn < 0)) return true; //index  of column
    else return false;
}
bool IsOutOfRowRange(int targetRow){
    if( (targetRow > (ROW-1))) return true; //index  of row
    else return false;
}
bool IsOutOfRange(int targetRow,int targetColumn){
    if(IsOutOfRowRange(targetRow) || IsOutOfColumnRange(targetColumn)) return true;
    else return false;
}
bool IsAlreadyPlacedBlock(Block** grid,int targetRow,int targetColumn){
    if((grid[targetRow][targetColumn].active == false) && (grid[targetRow][targetColumn].type != EMPTY)) return true;
    else return false;
}
bool IsNotMovableBlock(Block** grid,int targetRow,int targetColumn){
    if( IsOutOfRange(targetRow,targetColumn) || IsAlreadyPlacedBlock(grid,targetRow,targetColumn) ) return true;
    else return false;
}

/* Debug purpose */
void PrintGrid(Block** grid){
    char tmp = '*';
    printf("==========\n");
    for(int i=0; i < ROW; i++ ){
        if(i == 4){
            printf("----------\n");
        }
        for(int j=0; j < COLUMN; j++){
            switch(grid[i][j].type){
                case L : tmp = 'L';
                    break;
                case J : tmp = 'J';
                    break;
                case T : tmp = 'T';
                    break;
                case S : tmp = 'S';
                    break;
                case Z : tmp = 'Z';
                    break;
                case I : tmp = 'I';
                    break;
                case O : tmp = 'O';
                    break;
                case EMPTY : tmp = ' ';
                    break;
                default : tmp = '?';
                    break;

            }
            printf("%c",tmp);
        }
        printf("\n");
    }
    printf("==========\n");
}

 /* To perform a rotation regarding the center of the pieces into the grid, we must perform :
 *  - TRANSLATION to a new coordinate system with the center of the pieces as the origin
 *  - ROTATION
 *  - TRANSLATION of the coordonate to the original coordinate system
 * Mathematical Rotation is :
 *  Column_new = Column_old*cos(@) - Row_old*sin(@)
 *  Row_new (row) = Column_old*sin(@) + Row_old*cos(@)
 * Using @ = +90??, cos(@) = 0 and sin(@)= 1 thus
 *  Column_new = - Row_old
 *  Row_new = Column_old
 * [!] PLEASE MIND: The center of rotation is set to the center of the pieces, not the origin of the grid thus we use phiX and phiY
 * Return :s
 * - False if the piece cannot be moved
 * - True otherwise (the pieces have been moved) */
bool RotatePiece(Block* piece,Block** grid){
    if (piece[0].type == O) return; // Square have not the ability to rotate, interupt function

    Block targetPieces[4];

    int phiRow = piece[0].row;
    int phiCol = piece[0].column;
    enum ShapeType typePieces = piece[0].type;
    int tmpCol,tmpRow,tmp;
    for(int i=0; i< BLOCKS_PER_PIECES; i++){ // Create a copy piece for testing the new location of the piece
        targetPieces[i] = piece[i];
        tmpRow = targetPieces[i].row - phiRow;          // TRANSLATION
        tmpCol = targetPieces[i].column - phiCol;       // TRANSLATION
        tmp = tmpCol;
        tmpCol = - tmpRow;                              // ROTATION
        tmpRow = tmp;                                   // ROTATION

        tmpCol = tmpCol + phiCol;                       // TRANSLATION
        tmpRow = tmpRow + phiRow;                       // TRANSLATION
        targetPieces[i].row = tmpRow;                   // Keep in memory for later use if block indeed rotable
        targetPieces[i].column = tmpCol;                // Keep in memory for later use if block indeed rotable
    }

    for(int i=0; i< BLOCKS_PER_PIECES; i++){    // Verify block can be moved
        if(IsNotMovableBlock(grid,targetPieces[i].row,targetPieces[i].column)) return false;
    }

    for(int i=0; i< BLOCKS_PER_PIECES; i++){    // Uptdate new piece location
        grid[piece[i].row][piece[i].column].type = EMPTY;
        grid[piece[i].row][piece[i].column].active = false;
        piece[i] = targetPieces[i];
        grid[piece[i].row][piece[i].column].type = typePieces;
        grid[piece[i].row][piece[i].column].active = true;
    }
    return true;
}

/* Attempt to move to one direction the piece(among MoveType). Checks of conformity are done prior to move the piece
 * Return :
 * - False if the piece cannot be moved
 * - True otherwise (the pieces have been moved) */
bool MovePiece(enum MoveType moveType,Block* piece,Block** grid){
    Block targetPieces[4];
    for(int i=0; i< BLOCKS_PER_PIECES; i++){ // Create a copy piece for testing the new location of the piece
        targetPieces[i] = piece[i];
        switch (moveType){
            case Left : targetPieces[i].column -= 1; break;
            case Right : targetPieces[i].column += 1; break;
            case Down : targetPieces[i].row += 1; break;
            default : printf("[!] Error, not expected move type");
        }
    }
    for(int i=0; i< BLOCKS_PER_PIECES; i++){ // Verify block can be moved
        switch (moveType){
            case Left : if(IsNotMovableBlock(grid,piece[i].row,targetPieces[i].column)) return false; break;    //Abort if piece cannot be move
            case Right : if(IsNotMovableBlock(grid,piece[i].row,targetPieces[i].column)) return false; break;   //Abort if piece cannot be move
            case Down : if(IsNotMovableBlock(grid,targetPieces[i].row,piece[i].column)) return false; break;    //Abort if piece cannot be move
        }
    }
    RemovePiecesFromGrid(piece,grid);
    PlacePieceIntoGrid(piece,targetPieces,grid);

    return true;
}

/* Remove from the piece from the grid .
 * It remove the blocks (remove any type) and the state (meaning usable by the player) */
void RemovePiecesFromGrid(Block* piece,Block** grid){
    for(int i=0; i< BLOCKS_PER_PIECES; i++){ // Remove from the grid the previous pieces
        SetBlockType(EMPTY,piece[i].row,piece[i].column,grid);
    }
}

/* Place a piece into the grid
 * It will add a pieces to the grid at a targetPieces Location */
void PlacePieceIntoGrid(Block* piece,Block* targetPieces,Block** grid){
  for(int i=0; i< BLOCKS_PER_PIECES; i++){                              // Uptdate new piece location
      piece[i] = targetPieces[i];                                       //Update of piece
      grid[piece[i].row][piece[i].column]= piece[i];                  //Update grid block
  }
}

/* Set a block with the required type
 * Block will be set as disable from the player ownership */
void SetBlockType(enum ShapeType type,int row,int column,Block** grid){
        grid[row][column].type = type;               //Disable previous block type
        grid[row][column].active = false;             //Disable previous block ownership
}

/* Check if a row in the grid is a complete, no matter player ownership
 * Return:
 *  - False if the line have a gap <=> not full
 *  - True if line have only block <=> full line */
bool IsComleteLine(int row,Block** grid){
    for(int column=0; column< COLUMN; column++){
        if(grid[row][column].type == EMPTY) {
            return false;
        }
    }
    return true;
}

/* Loop throught a piece's row for return a list of rows index wich are complete
 * [!] PLEASE MIND: Value are sort in INCREASING ORDER and there is NO REPETITION of value
 * Return :
 *  - Liste of index
 *     [Nb][0][1][2][3]
        |    \ |  |  /
        |     \|  | /
        |    index of rows, set at -1 if other row aren't complete
      Number of line complete, value beetween 0 and BLOCKS_PER_PIECES  */
int* GetNumberOfRowComplete(Block* piece,Block** grid){
    int sizeList = BLOCKS_PER_PIECES+1;
    int* listCompleteRow = (int*)malloc(sizeList*sizeof(int));
    int numberOfLineComplete = 0;
    int tempValue;
    int NextIndexList = 1;

    for(int i =0; i< sizeList; i++){ // Set all value at -1  [-1][-1][-1][-1][-1]
        listCompleteRow[i] = -1;
    }
    for(int i=0; i < BLOCKS_PER_PIECES; i++){
        if(IsComleteLine(piece[i].row,grid)){
            numberOfLineComplete++;
            for(int j=1 ; j < sizeList ;j++){ // Start at the 2nd positions [Nb][X][-][-][-] and because of sort stop before end
                if(listCompleteRow[j] == piece[i].row) {
                    numberOfLineComplete--;
                    break;
                }
                if ( (listCompleteRow[j] < piece[i].row) && (listCompleteRow[j] != -1)){
                    continue;
                }
                if ( (listCompleteRow[j] > piece[i].row)&&(listCompleteRow[j] != -1) ) {
                    for(int k = sizeList-1 ; k > j ; k--){
                        listCompleteRow[k] = listCompleteRow[k-1];
                    }
                    listCompleteRow[j] = piece[i].row;
                    break;
                }
                else if ( listCompleteRow[j] == -1 ) {
                    listCompleteRow[j] = piece[i].row;
                    break;
                }
            }
        }
    }
    listCompleteRow[0] = numberOfLineComplete;

}

/* Remove from the grid the completed lines and move down the block of the player
 * Using a dedicated table using this format :
 *     [Nb][0][1][2][3]
        |    \ |  |  /
        |    index of rows, set at -1 if other row aren't complete, sort using INCREASING ORDER
      Number of line complete, value beetween 0 and BLOCKS_PER_PIECES
 * Block will be moved down where a line have been deleted
 * Function Return the number of completed row
 * [!] PLEASE MIND: piece coordinate will not be updated */
int ProceedCompleteLine(Block* piece,Block** grid){
    int* specialRowsTable =  GetNumberOfRowComplete(piece,grid);//dedicated table for management of complete row
    int nbLineToDelete = specialRowsTable[0];
    for(int i=1; i <= nbLineToDelete; i++){
            RemoveLine(grid,specialRowsTable[i]);
            MoveGridBlocksDown(grid,specialRowsTable[i]);
        }
    free(specialRowsTable);
    return nbLineToDelete; // Number of line complete, value beetween 0 and BLOCKS_PER_PIECES at the location of index 0 [Nb][..][..][..][..]

}

/* Function for multiplayer game only.
 * A certain qty of rows (almost complete : with one hole somewhere) will be added to a grid.
 * It will move up every block already placed and then add the holed line */
void CreatHoledLineInGrid(Block* piece,Block** grid,int qty){
  for(int i=0 ; i< qty; i++){
      MoveAllGridBlocksUp(piece,grid);
      SetHoledLine(grid,ROW -1 );
  }
}

/* Set a line at an index of a grid as almost complete
 * Using a row index, a line will fill with blocks on the entire row EXEPT for one block choosen randomly */
void SetHoledLine(Block** grid,int row){
  int randomColumnHole = rand()% COLUMN +1 ;  //provide random from 1 to COLUMN
  for(int column = 0; column < COLUMN; column++){
      if(column != randomColumnHole) grid[row][column].type = I;               //anythipe would fit
      else grid[row][column].type = EMPTY;
      grid[row][column].active = false;
      }
}

/* Remove an entier empty
 * Blocks will be set as EMPTY type and Disable from player */
void RemoveLine(Block** grid,int row){
    for(int column = 0; column < COLUMN; column++){
        SetBlockType(EMPTY,row,column,grid);
    }
}

/* Move down every blocks based on a row index
 * Block will be moved down from the index row (to the first row (<=> row at the top of the grid)
 * [!] PLEASE MIND: piece coordinate will not be updated
 * [!] PLEASE MIND: no check of line emptyness are done
 * [!] PLEASE MIND: Row index must be an empty line   */
void MoveGridBlocksDown(Block** grid,int row){
    while (row >0){
        for(int column = 0; column < COLUMN; column++){
            grid[row][column].type = grid[row-1][column].type;
            grid[row][column].active = grid[row-1][column].active;
        }
        row--;
    }
}

/* Move up every blocks based on a row index
 * Block will be moved up from the index row (to the first row (<=> row at the top of the grid)
 * [!] PLEASE MIND: no checks of line are done
 * [!] PLEASE MIND: Row index must be a empty line   */
void MoveAllGridBlocksUp(Block* piece,Block** grid){
    int row = 0;
    while (row <ROW -1){
        for(int column = 0; column < COLUMN; column++){
            grid[row][column].type = grid[row+1][column].type;
            grid[row][column].active = grid[row+1][column].active;
        }
        row++;
    }
    for(int i=0 ; i< BLOCKS_PER_PIECES; i++){
      piece[i].row--;
    }
}

/* Set the status of the pieces to false
 * Piece status aspect (piece active) is set to false */
void SetPiecePlaced(Block* piece,Block** grid){
    for(int i=0 ; i< BLOCKS_PER_PIECES; i++) grid[piece[i].row][piece[i].column].active = false;
}

/* Function allowing a management of the stocked pieces
 * Because a piece is alway on the grid only two case can occur :
 *   - a pieces is on the grid AND stock is empty
 *       => store and instanciate a new piece randomly
 *   - a pieces is on the grid AND stock is full ( i.e. on piece is stored)
 *       => switch the stored piece and instantiate starting from the hidden area previous stored pieces   */
Block* ManagePlayerPiecesSwitch(enum ShapeType* storedPiecesType,Block* piece,Block** grid){
  if ((*storedPiecesType == EMPTY)){
      *storedPiecesType = piece[0].type;
      RemovePiecesFromGrid(piece,grid);
      piece = InitialiseRandomPieces(grid);
      return piece;
  }
  else {
      enum ShapeType typeToInstantiate;
      typeToInstantiate = *storedPiecesType;
      *storedPiecesType = piece[0].type;
      RemovePiecesFromGrid(piece,grid);
      piece = InitialisePieces(typeToInstantiate,grid);
      return piece;
  }
}

/* Check if a piece is present (not EMPTY) and not active(not usable by the player anymore) inside the row no 3
 * The hidden area is consider as the forbidden area, thus player have the lost the game
 * Return :
 * - True if piece not inside the ban area
 * - False if piece is inside the ban area
 *                                                                (i)
 *    EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY  0
 *    EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY  1
 *    EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY  2
 *    CHECK CHECK CHECK CHECK CHECK CHECK CHECK CHECK CHECK CHECK  3   <=
 *    ---   ---   ---   ---   ---   ---   ---   ---   ---   ---
 *    EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY EMPTY  4
 *    [...] [...] [...] [...] [...] [...] [...] [...] [...] [...]
 *(j)   0     1     2     3     4     5     6     7     8     9
 *
 *    [!] TAKE CARE [!] Only row 3 will be check because a piece placed in the hidden area have at least one block inside that row
 *   */
bool LostConditionMeet(Block** grid){
    for(int iColumn=0; iColumn < COLUMN; iColumn++){
        if( (grid[3][iColumn].active == false) && (grid[3][iColumn].type != EMPTY) ) return true;
    }
    return false;
}

/* Function return the speed of the piece (block per milis <=> 1000 is equivalent to 1s)
 * One level is pass each 5 rows completed (try 10 maybe)
 * The speed link to the level use an existing graph. Documentation is availbale into the folder doc
 *    [!] TAKE CARE [!] Input MUST BE the TOTAL amount of row achieve by the player
 * graph availbable at : https://gitlab.com/lburckert/projet-tetris-c/-/blob/master/doc/Speed_Lvl_Graph.png */
long GetBlockSpeed(int rowCompleted){
  int NumberRowBetweenEachLevel = 5;
  int level = rowCompleted/NumberRowBetweenEachLevel; // Euclidian division
  switch (level){
    case 0 : return 798; break;
    case 1 : return 715; break;
    case 2 : return 632; break;
    case 3 : return 549; break;
    case 4 : return 465; break;
    case 5 : return 382; break;
    case 6 : return 299; break;
    case 7 : return 216; break;
    case 8 : return 133; break;
    case 9 : return 133; break;
    case 10 : return 83; break;
    case 11 : return 83; break;
    case 12 : return 83; break;
    case 13 : return 83; break;
    case 14 : return 83; break;
    case 15 : return 83; break;
    case 16 : return 49; break;
    case 17 : return 49; break;
    case 18 : return 49; break;
    case 19 : return 33; break;
    case 20 : return 33; break;
    case 21 : return 33; break;
    case 22 : return 33; break;
    case 23 : return 33; break;
    case 24 : return 33; break;
    case 25 : return 33; break;
    case 26 : return 33; break;
    case 27 : return 33; break;
    case 28 : return 33; break;
    case 29 : return 33; break;
    default :
      printf("[WARNNING]: Lvl cannot be used ");
      return 1;
    }
}

/* Function return the speed of the piece (block per milis <=> 1000 is equivalent to 1s)
 * One level is pass each 5 rows completed (try 10 maybe)
 * The speed link to the level use an existing graph. Documentation is availbale into the folder doc
 *    [!] TAKE CARE [!] Input MUST BE the TOTAL amount of row achieve by the player   */
long GetBlockSpeedForIA(int rowCompleted){
  int NumberRowBetweenEachLevel = 5;
  int level = rowCompleted/NumberRowBetweenEachLevel; // Euclidian division
  switch (level){
    case 0 : return 216; break;
    case 1 : return 216; break;
    case 2 : return 200; break;
    case 3 : return 200; break;
    case 4 : return 190; break;
    case 5 : return 190; break;
    case 6 : return 180; break;
    case 7 : return 150; break;
    case 8 : return 133; break;
    case 9 : return 133; break;
    case 10 : return 133; break;
    case 11 : return 83; break;
    case 12 : return 83; break;
    case 13 : return 83; break;
    case 14 : return 83; break;
    case 15 : return 83; break;
    case 16 : return 49; break;
    case 17 : return 49; break;
    case 18 : return 49; break;
    case 19 : return 33; break;
    case 20 : return 33; break;
    case 21 : return 33; break;
    case 22 : return 33; break;
    case 23 : return 33; break;
    case 24 : return 33; break;
    case 25 : return 33; break;
    case 26 : return 33; break;
    case 27 : return 33; break;
    case 28 : return 33; break;
    case 29 : return 33; break;
    default :
      printf("[WARNNING]: Lvl cannot be used ");
      return 1;
    }
}

/* Function returning the score to print on the screen
 * The current score and the row completed must be provided
 * TAKE CARE : there is no break, score will be update in a waterfall manner*/
int UpdateScoreTetris(int score,int rowCompleted){
  switch(rowCompleted){
    case 4 : score = score + 20;
    case 3 : score = score + 15;
    case 2 : score = score + 10;
    case 1 : score = score + 5;
    case 0 : score++;
  }
  return score ;
}

/*
 *
 */
int demo(){
  Block** grid = InitialiseGrid();
  Block* pieces;
  bool continueGame = true;
  PrintGrid(grid);
  while(continueGame) {
    pieces = InitialiseRandomPieces(grid);
    for(int i=0;i<ROW-1;i++){
         MovePiece(Down,pieces,grid);
    }
    SetPiecePlaced(pieces,grid);
    ProceedCompleteLine(pieces,grid);
    if(LostConditionMeet(grid)) continueGame = false;
    PrintGrid(grid);
  }
  return 0;
}
