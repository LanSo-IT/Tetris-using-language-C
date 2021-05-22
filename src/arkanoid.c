#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <SDL_image.h>

#include "model_tetris.h"

#define CELL_PIXEL 32

#define VISIBLE_ROW 20  // [ !!! ]  lines but there is 4 hidden row at the top of the grid => meaning 24 ROW
#define COLUMN 10

#define KEY_PAV_NUM_1 49
#define KEY_PAV_NUM_2 50
#define KEY_PAV_NUM_3 51

//PLayer 1
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
//Player 2
#define KEY_Z 122
#define KEY_Q 113
#define KEY_S 115
#define KEY_D 100

SDL_Window* pWindow = NULL;
SDL_Surface* win_surf = NULL;
SDL_Surface* plancheSprites = NULL;
SDL_Surface* BackgroundSprites = NULL;
SDL_Surface* BlocksSprites = NULL;

SDL_Rect srcBlock = { 0,0, 32,32 }; // x,y, w,h (0,0) en haut a gauche  0,0,32,32
/*  . - - - w - - - .
	  |               |
		h               h
	  |               |
	  . - - - w - - - .  */

SDL_Rect srcGridSolo = { 352,160, 32,32 }; // x,y, w,h (0,0) en haut a gauche  0,0,32,32

void init()
{
	pWindow = SDL_CreateWindow("Tetris", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 960, 672, SDL_WINDOW_SHOWN);
	win_surf = SDL_GetWindowSurface(pWindow);
	//plancheSprites = SDL_LoadBMP("../bin/default_cell.bmp");
	BackgroundSprites = SDL_LoadBMP("../bin/wall_cell.bmp");
	//SDL_Surface *image;
	//image=IMG_Load("../bin/metal-background.png");
	BackgroundSprites = IMG_Load("../bin/metal-background.png");
	//SDL_SetColorKey(plancheSprites, true, 0);  // 0: 00/00/00 noir -> transparent
}

void drawBackground()
{
	// remplit le fond
	SDL_Rect dest = { 0,0,0,0 };
	for (int j = 0; j < win_surf->h; j+=32)
		for (int i = 0; i < win_surf->w; i += 32)
		{
			dest.x = i;
			dest.y = j;
			SDL_BlitSurface(BackgroundSprites, &srcBlock, win_surf, &dest);
		}

	SDL_FreeSurface(BackgroundSprites);
}

/* Function that draw a grid of blocks
 * Starting of the draw have to be input as pixel  starting at an index row and index column */
void drawGrid(Block** grid,int startPixelWidth,int StartPixelHeight)
{
 SDL_Rect dest = { 0,0,0,0 };
 int rowOffset = 4;  //  acces visible pieces starting at the index 4 of the grid
 for(int row =0 ; row < VISIBLE_ROW ; row ++){
	 	for(int column =0 ; column < COLUMN; column++){
			switch (grid[row + rowOffset][column].type){
					case EMPTY: BlocksSprites =  SDL_LoadBMP("../bin/basic_cell.bmp"); break;
					case I: BlocksSprites =  SDL_LoadBMP("../bin/I_cell.bmp"); break;
					case O: BlocksSprites =  SDL_LoadBMP("../bin/O_cell.bmp"); break;
					case L :  BlocksSprites =  SDL_LoadBMP("../bin/L_cell.bmp"); break;
					case J :  BlocksSprites =  SDL_LoadBMP("../bin/J_cell.bmp"); break;
					case T :  BlocksSprites =  SDL_LoadBMP("../bin/T_cell.bmp"); break;
					case S :  BlocksSprites =  SDL_LoadBMP("../bin/S_cell.bmp"); break;
					case Z :  BlocksSprites =  SDL_LoadBMP("../bin/Z_cell.bmp"); break;
				  default : BlocksSprites = SDL_LoadBMP("../bin/error_cell.bmp");
			}
			dest.x = startPixelWidth+column*CELL_PIXEL;
			dest.y = StartPixelHeight+row*CELL_PIXEL;
			SDL_BlitSurface(BlocksSprites, &srcBlock, win_surf, &dest);
		}
 }
}

void drawAwaitingPieces(Block** pieces,int startPixelWidth,int StartPixelHeight)
{
 //SDL_Rect dest = { 0,0,0,0 };
 //int rowOffset = 4;  //  acces visible pieces starting at the index 4 of the grid
 /*for(int row =0 ; row < VISIBLE_ROW ; row ++){
	 	for(int column =0 ; column < COLUMN; column++){
			switch (grid[row + rowOffset][column].type){
					case EMPTY: BlocksSprites =  SDL_LoadBMP("../bin/basic_cell.bmp"); break;
					case I: BlocksSprites =  SDL_LoadBMP("../bin/I_cell.bmp"); break;
					case O: BlocksSprites =  SDL_LoadBMP("../bin/O_cell.bmp"); break;
					case L :  BlocksSprites =  SDL_LoadBMP("../bin/L_cell.bmp"); break;
					case J :  BlocksSprites =  SDL_LoadBMP("../bin/J_cell.bmp"); break;
					case T :  BlocksSprites =  SDL_LoadBMP("../bin/T_cell.bmp"); break;
					case S :  BlocksSprites =  SDL_LoadBMP("../bin/S_cell.bmp"); break;
					case Z :  BlocksSprites =  SDL_LoadBMP("../bin/Z_cell.bmp"); break;
				  default : BlocksSprites = SDL_LoadBMP("../bin/error_cell.bmp");
			}
			dest.x = startPixelWidth+column*CELL_PIXEL;
			dest.y = StartPixelHeight+row*CELL_PIXEL;
			SDL_BlitSurface(BlocksSprites, &srcBlock, win_surf, &dest);
		}
 }*/
}

long currentTimeMillis() {
  struct timeval time;
  gettimeofday(&time, NULL);

  return time.tv_sec * 1000 + time.tv_usec / 1000;
}
bool IsTimerOver(long inputTimeOver){
	return inputTimeOver > currentTimeMillis();
}
long GetNextTimeOver(int contorRows){
	return currentTimeMillis() + GetBlockSpeed(contorRows);
}

bool GameSolo()
{
		if (SDL_Init(SDL_INIT_VIDEO) != 0 ) return 1;

		Block** grid = InitialiseGrid();
		Block* pieces;
		bool continueGame = true;
		bool quit = false;
		bool timeIsnotOverAtTheRow;
		bool canBeMovedLower = true;
		int rowsCompleted = 0;
		int contorRows = 0;
		long timeBeforeMoveToNextRow;
		SDL_Event PlayerEvent;
		int press = 0;

		init();

		while (continueGame)
		{
			SDL_Event event;
		  while(continueGame&& !quit) {
				SDL_PumpEvents();
				drawBackground();
				drawGrid(grid,352,32);
				SDL_UpdateWindowSurface(pWindow);
		    pieces = InitialiseRandomPieces(grid);
				canBeMovedLower = true;
		    while(canBeMovedLower){
						timeBeforeMoveToNextRow = currentTimeMillis() + GetBlockSpeed(contorRows) ; //speed regarding completed rows
					while ( timeBeforeMoveToNextRow > currentTimeMillis() ){
									if(kbhit()){
										switch ((press = getch())) {
											case KEY_UP: RotatePiece(pieces,grid); break;
											case KEY_DOWN: timeBeforeMoveToNextRow = currentTimeMillis(); break;
											case KEY_LEFT: MovePiece(Left,pieces,grid); break;
											case KEY_RIGHT:MovePiece(Right,pieces,grid); break;
										}
										drawGrid(grid,352,32);
										PrintGrid(grid);
										SDL_UpdateWindowSurface(pWindow);
									}
						}
						canBeMovedLower = MovePiece(Down,pieces,grid);
						drawGrid(grid,352,32);
						SDL_UpdateWindowSurface(pWindow);
		    }
		    SetPiecePlaced(pieces,grid);
		    rowsCompleted = ProceedCompleteLine(pieces,grid);
				if (rowsCompleted > 0) {
					contorRows =+ rowsCompleted ;
					printf("Row done :%d\n",rowsCompleted);
					PrintGrid(grid);
				}
		    if(LostConditionMeet(grid)) continueGame = false;
				if( event.type == SDL_QUIT ) quit = true;
			}
			SDL_UpdateWindowSurface(pWindow);

			while(true){}

			return true;

		}
}

bool GameDuo()
{
		if (SDL_Init(SDL_INIT_VIDEO) != 0 ) return 1;

		Block** gridPlayer1 = InitialiseGrid();
		Block** gridPlayer2 = InitialiseGrid();
		Block* piecesPlayer1 = InitialiseRandomPieces(gridPlayer1);
		Block* piecesPlayer2 = InitialiseRandomPieces(gridPlayer2);
		bool continueGame = true;
		bool quit = false;
		bool timeIsnotOverAtTheRow;
		bool canBeMovedLowerPlayer1 = true;
		bool canBeMovedLowerPlayer2 = true;
		int rowsCompletedPlayer1 = 0;
		int rowsCompletedPlayer2 = 0;
		int contorRows = 0;
		long timerPlayer1 = currentTimeMillis();
		long timerPlayer2 = currentTimeMillis();
		SDL_Event PlayerEvent;
		int press = 0;

		init();

		while (continueGame)
		{
			SDL_Event event;
		  while(continueGame&& !quit) {
				SDL_PumpEvents();
				drawBackground();
				drawGrid(gridPlayer1,32,32);
				drawGrid(gridPlayer2,608,32);
				SDL_UpdateWindowSurface(pWindow);

				if( ! canBeMovedLowerPlayer1 ) {
					SetPiecePlaced(piecesPlayer1,gridPlayer1);
					rowsCompletedPlayer1 = ProceedCompleteLine(piecesPlayer1,gridPlayer1);
					CreatHoledLineInGrid(gridPlayer2,rowsCompletedPlayer1-1);
					piecesPlayer1 = InitialiseRandomPieces(gridPlayer1);
					canBeMovedLowerPlayer1 = true;
				}
				if ( ! canBeMovedLowerPlayer2 ){
					SetPiecePlaced(piecesPlayer2,gridPlayer2);
					rowsCompletedPlayer2 = ProceedCompleteLine(piecesPlayer2,gridPlayer2);
					CreatHoledLineInGrid(gridPlayer1,rowsCompletedPlayer2-1);
					piecesPlayer2 = InitialiseRandomPieces(gridPlayer2);
					canBeMovedLowerPlayer2 = true;
				}

		    while(canBeMovedLowerPlayer1 && canBeMovedLowerPlayer2){
						if (timerPlayer1 <= currentTimeMillis() ) {
							timerPlayer1 = currentTimeMillis() + GetBlockSpeed(contorRows); //speed regarding completed rows
							canBeMovedLowerPlayer1 = MovePiece(Down,piecesPlayer1,gridPlayer1);
						}
						if (timerPlayer2 <= currentTimeMillis() )  {
							timerPlayer2 = currentTimeMillis() + GetBlockSpeed(contorRows);
							canBeMovedLowerPlayer2 = MovePiece(Down,piecesPlayer2,gridPlayer2);
						}

						while ( (timerPlayer1 > currentTimeMillis()) && (timerPlayer2 > currentTimeMillis()) ){
									if(kbhit()){
										switch ((press = getch())) {
											// P1
											case KEY_Z : RotatePiece(piecesPlayer1,gridPlayer1);break;
											case KEY_S : timerPlayer1 = currentTimeMillis(); break;
											case KEY_Q : MovePiece(Left,piecesPlayer1,gridPlayer1); break;
											case KEY_D : MovePiece(Right,piecesPlayer1,gridPlayer1); break;
											// P2
											case KEY_UP: RotatePiece(piecesPlayer2,gridPlayer2); break;
											case KEY_DOWN: timerPlayer2 = currentTimeMillis(); break;
											case KEY_LEFT: MovePiece(Left,piecesPlayer2,gridPlayer2); break;
											case KEY_RIGHT:MovePiece(Right,piecesPlayer2,gridPlayer2); break;
										}
										drawGrid(gridPlayer1,32,32);
										drawGrid(gridPlayer2,608,32);
										SDL_UpdateWindowSurface(pWindow);
									}
						}
						drawGrid(gridPlayer1,32,32);
						drawGrid(gridPlayer2,608,32);
						SDL_UpdateWindowSurface(pWindow);
		    }
				if (rowsCompletedPlayer1 > 0 || rowsCompletedPlayer2 >0 ) {
					contorRows = contorRows + rowsCompletedPlayer1 + rowsCompletedPlayer2 ;
					rowsCompletedPlayer1  = 0;
					rowsCompletedPlayer2 = 0;
				}
		    if(LostConditionMeet(gridPlayer1) || LostConditionMeet(gridPlayer2)) continueGame = false;
				if( event.type == SDL_QUIT ) quit = true;
			}
			SDL_UpdateWindowSurface(pWindow);

			// Wait 3 second after player have lost
			long shortWaitBeforeQuit = currentTimeMillis() + 3000; // 3000 is 3 second
			while(shortWaitBeforeQuit > currentTimeMillis() ) {}

			return true;
		}
}


void test(){
	while (true){}
}

int main(int argc, char** argv)
{
	printf("Terminal Tetris Menu\n\nEnter (1) from the numerical pad for a solo game\nEnter (2) from the numerical pad for duo game\n\n");
	int press = 0;
	SDL_PumpEvents();

	while(press == 0){
			if(kbhit())press = getch();
	}
	printf("%d\n",press );
	switch (press) {
		case KEY_PAV_NUM_1: GameSolo(); break;
		case KEY_PAV_NUM_2: GameDuo(); break;
		case KEY_PAV_NUM_3 : test(); break;
	}

	while(true){}
}
