#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>

#include "model_tetris.h"

#define CELL_PIXEL 32

#define VISIBLE_ROW 20  // [ !!! ]  lines but there is 4 hidden row at the top of the grid => meaning 24 ROW
#define COLUMN 10

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


int main(int argc, char** argv)
{
		Block** grid = InitialiseGrid();
		Block* pieces;
		bool continueGame = true;
		if (SDL_Init(SDL_INIT_VIDEO) != 0 )
    {
			return 1;
    }

		init();
		while (continueGame)
		{
			SDL_Event event;
			/*while (!quit && SDL_PollEvent(&event))
			{
				switch (event.type)
				{
				case SDL_QUIT:
					quit = true;
					break;
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym)
					{
						// touche clavier
						case SDLK_LEFT:  x_vault -= 10; break;
						case SDLK_RIGHT: x_vault +=10; break;
						case SDLK_ESCAPE: quit = true; break;
						default: break;
					}
					break;
				case SDL_MOUSEMOTION:	x_vault += event.motion.xrel;	break;
				case SDL_MOUSEBUTTONDOWN:
					printf("mouse click %d\n", event.button.button);
					break;
				default: break;
				}
			}*/
			//prev = now;
			//now = SDL_GetPerformanceCounter();
			//delta_t = (double)((now - prev) * 1000 / (double)SDL_GetPerformanceFrequency());

		  PrintGrid(grid);
		  while(continueGame) {
				drawBackground();
				drawGrid(grid,352,32);
		    pieces = InitialiseRandomPieces(grid);
		    for(int i=0;i<23;i++){
		         MovePiece(Down,pieces,grid);
		    }
		    SetPiecePlaced(pieces,grid);
		    ProceedCompleteLine(pieces,grid);
		    if(LostConditionMeet(grid)) continueGame = false;
		    PrintGrid(grid);
			}
			SDL_UpdateWindowSurface(pWindow);

			while(true){}

		}
}
