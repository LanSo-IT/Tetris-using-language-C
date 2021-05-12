#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

#include "model_tetris.h"

#define CELL_PIXEL 32

#define VISIBLE_ROW 20  // [ !!! ]  lines but there is 4 hidden row at the top of the grid => meaning 24 ROW
#define COLUMN 10

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77

//struct { double x; double y; } ball_speed;
struct { double x; double y;  double vx; double vy;} ball;

Uint64 prev, now; // timers
double delta_t;  // durée frame en ms
int x_vault;

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


SDL_Rect srcBall = { 0,96,24,24 };
SDL_Rect scrVaiss = { 128,0,128,32 };



void init()
{
	pWindow = SDL_CreateWindow("Tetris", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 960, 672, SDL_WINDOW_SHOWN);
	win_surf = SDL_GetWindowSurface(pWindow);
	//plancheSprites = SDL_LoadBMP("../bin/default_cell.bmp");
	BackgroundSprites = SDL_LoadBMP("../bin/wall_cell.bmp");
	SDL_SetColorKey(plancheSprites, true, 0);  // 0: 00/00/00 noir -> transparent
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

long currentTimeMillis() {
  struct timeval time;
  gettimeofday(&time, NULL);

  return time.tv_sec * 1000 + time.tv_usec / 1000;
}



int main(int argc, char** argv)
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
					PrintGrid(grid);
				}
		    if(LostConditionMeet(grid)) continueGame = false;
				if( event.type == SDL_QUIT ) quit = true;
			}
			SDL_UpdateWindowSurface(pWindow);

			while(true){}

		}
}
