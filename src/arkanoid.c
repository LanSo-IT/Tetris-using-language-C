#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

#include "model_tetris.h"
#include "ia_tetris.h"

#define CELL_PIXEL 32

#define VISIBLE_ROW 20  // [ !!! ]  only visible lines because there is 4 hidden row at the top of the grid => meaning a total of 24 ROW
#define COLUMN 10

#define KEY_PAV_NUM_0 48
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
SDL_Surface* MenuBackground = NULL;
SDL_Surface* buttonSprite1P = NULL;
SDL_Surface* buttonSprite2P = NULL;
SDL_Surface* buttonSpriteIA = NULL;
SDL_Rect srcBlock = { 0, 0, 32, 32 }; // x,y, w,h (0,0) en haut a gauche  0,0,32,32
SDL_Rect srcMenuBlock = { 0, 0, 960, 672 };
SDL_Rect srcButton1P = {30, 406, 172,207}; // BUTTON 1P
SDL_Rect srcButton2P = {394, 406, 172,207}; // BUTTON 2P
SDL_Rect srcButtonIA = {758, 406, 172,207}; // BUTTON IA
/*  . - - - w - - - .
	  |               |
		h               h
	  |               |
	  . - - - w - - - .  */

SDL_Rect srcGridSolo = { 352,160, CELL_PIXEL,CELL_PIXEL }; // x,y, w,h (0,0) en haut a gauche  0,0,32,32

/* Function to initialise SDL basic needs
 * it will create a windows with the correct name and load the picture needed for further prinitng */
void init(){
	pWindow = SDL_CreateWindow("Tetris", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 960, 672, SDL_WINDOW_SHOWN);
	win_surf = SDL_GetWindowSurface(pWindow);
	BackgroundSprites = SDL_LoadBMP("../bin/wall_cell.bmp");
	MenuBackground = SDL_LoadBMP("../bin/menu.bmp");
	buttonSprite1P = SDL_LoadBMP("../bin/buttonMenu1P.bmp");
	buttonSprite2P = SDL_LoadBMP("../bin/buttonMenu2P.bmp");
	buttonSpriteIA = SDL_LoadBMP("../bin/buttonMenuIA.bmp");
	SDL_SetColorKey(plancheSprites, true, 0);  // 0: 00/00/00 noir -> transparent
}

/* Function to draw the all background of tetris game
 * It will print several time the same basic picture saved as BackgroundSprites */
void drawBackground(){
	SDL_Rect dest = { 0,0,0,0 };
	for (int j = 0; j < win_surf->h; j+=CELL_PIXEL){
		for (int i = 0; i < win_surf->w; i += CELL_PIXEL)
		{
			dest.x = i;
			dest.y = j;
			SDL_BlitSurface(BackgroundSprites, &srcBlock, win_surf, &dest);
		}
	}
}

/* Function that draw a grid of blocks
 * Starting of the draw have to be input as pixel starting at an index row and index column */
void drawGrid(Block** grid,int startPixelWidth,int StartPixelHeight){
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
	SDL_UpdateWindowSurface(pWindow);
}

// TODO
/* funciton to print the score on the screen with the input integer */
void drawScore(int score,int startPixelWidth,int StartPixelHeight){

}
/* Function to provide a menu for Tetris game
 * It will print the default image and then add the 3 buttons */
void drawMenu(){
	SDL_Rect srcMenuBlock = { 0,0,960,670 };
	SDL_BlitSurface(MenuBackground, &srcMenuBlock, win_surf, &srcMenuBlock);
	SDL_Rect srcButton = {0, 0, 172,207};
	SDL_BlitSurface(buttonSprite1P,&srcButton,win_surf,&srcButton1P);
	SDL_BlitSurface(buttonSprite2P,&srcButton,win_surf,&srcButton2P);
	SDL_BlitSurface(buttonSpriteIA,&srcButton,win_surf,&srcButtonIA);
	SDL_UpdateWindowSurface(pWindow);
}

/* function to provide the current time of the pc*/
long currentTimeMillis() {
  struct timeval time;
  gettimeofday(&time, NULL);
  return time.tv_sec * 1000 + time.tv_usec / 1000;
}

/* funciton allowing to know if the input time is later than the current time
 * It will be used for the tetris game */
bool IsTimerOver(long inputTimeOver){
	return inputTimeOver > currentTimeMillis();
}

/* function returning the next time before the pieces will move done as a mendatory move
 * It use the curent time and add the duration calculated by GetBlockSpeed (see model_tetris.c) */
long GetNextTimeOver(int contorRows){
	return currentTimeMillis() + GetBlockSpeed(contorRows);
}

/* Funciton allowing to know if an input x and y are indeed in the inputed SDL Rect */
bool XYInButton(SDL_Rect rect, int x, int y ){
	if( ( rect.x <= x ) && ( (rect.x + rect.w) >= x)
		&& ( rect.y <= y ) && ( (rect.y + rect.h) >= y)) return true;
	else return false;
}

/* Tetris for only one player is managed at hight level in the following code  */
bool GameSolo()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0 ) return 1;
	// Initialise variable
	Block** grid = InitialiseGrid();
	Block* pieces;
	enum ShapeType* storedPiece;
	enum ShapeType testInit = EMPTY;
	storedPiece = &testInit;
	bool continueGame = true;
	bool quit = false;
	bool timeIsnotOverAtTheRow;
	bool canBeMovedLower = true;
	int rowsCompleted = 0;
	int contorRows = 0;
	bool canBeSwitch = true;
	long timeBeforeMoveToNextRow;
	SDL_Event PlayerEvent;
	int press = 0;

	init();
	drawBackground();

	while (continueGame)
	{
		SDL_Event event;
	  while(continueGame && !quit) {
			// Start of a turn : init value for the turn
			SDL_PumpEvents();
			drawGrid(grid,352,32);
			SDL_UpdateWindowSurface(pWindow);
			free(pieces);
	    pieces = InitialiseRandomPieces(grid);
			canBeMovedLower = true;
			// Pieces will move down until it cannot go lower
	    while(canBeMovedLower){
					timeBeforeMoveToNextRow = currentTimeMillis() + GetBlockSpeed(contorRows) ; //speed of blocks regarding completed rows
					while ( timeBeforeMoveToNextRow > currentTimeMillis() ){
								if(kbhit()){
									switch ((press = getch())) {
										case KEY_UP: RotatePiece(pieces,grid); break;
										case KEY_DOWN: timeBeforeMoveToNextRow = currentTimeMillis(); break; // speed up pieces
										case KEY_LEFT: MovePiece(Left,pieces,grid); break;
										case KEY_RIGHT:MovePiece(Right,pieces,grid); break;
										case KEY_PAV_NUM_0 : printf("zero !\n" );
											if( canBeSwitch) {
												pieces = ManagePlayerPiecesSwitch(storedPiece,pieces,grid);
												canBeSwitch = false;// can be switch only once per turn
											}
											break;
									}
									drawGrid(grid,352,32);
									SDL_UpdateWindowSurface(pWindow);
								}
					}
					canBeMovedLower = MovePiece(Down,pieces,grid);
					drawGrid(grid,352,32);
					SDL_UpdateWindowSurface(pWindow);
	    }
			// Once pieces cannot go further down we set the pieces as placed
	    SetPiecePlaced(pieces,grid);
			canBeSwitch = true;
			// In addition we remove from the grid the completed row an record the score
	    rowsCompleted = ProceedCompleteLine(pieces,grid);
			printf("rowsCompleted afected by ProceedCompleteLine is %d\n",rowsCompleted);
			if (rowsCompleted > 0) {
				contorRows = contorRows +  rowsCompleted ;
				printf("contorRows = %d\t rowsCompleted = %d\n",contorRows,rowsCompleted);
				rowsCompleted = 0;
			}
			// finaly we check if the victory condition are reach
	    if(LostConditionMeet(grid)) continueGame = false;
			if( event.type == SDL_QUIT ) quit = true;
		}
		SDL_UpdateWindowSurface(pWindow);
	}
	// Wait 3 second after player have lost
	long shortWaitBeforeQuit = currentTimeMillis() + 3000; // 3000 is 3 second
	while(shortWaitBeforeQuit > currentTimeMillis() ) {}

	return true;
}

/* Tetris playable for two human players
 * It's the hight level game management */
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
		drawBackground();
		while (continueGame)
		{
			SDL_Event event;
		  while(continueGame&& !quit) {
				// Start of a turn : init value for the turn
				SDL_PumpEvents();
				drawGrid(gridPlayer1,32,32);
				drawGrid(gridPlayer2,608,32);
				SDL_UpdateWindowSurface(pWindow);
				// Once pieces cannot go further down we set the pieces as placed for grid 1
				if( ! canBeMovedLowerPlayer1 ) {
					SetPiecePlaced(piecesPlayer1,gridPlayer1);
					rowsCompletedPlayer1 = ProceedCompleteLine(piecesPlayer1,gridPlayer1);
					CreatHoledLineInGrid(piecesPlayer2,gridPlayer2,rowsCompletedPlayer1-1);
					piecesPlayer1 = InitialiseRandomPieces(gridPlayer1);
					canBeMovedLowerPlayer1 = true;
				}
				// Once pieces cannot go further down we set the pieces as placed for grid 2
				if ( ! canBeMovedLowerPlayer2 ){
					SetPiecePlaced(piecesPlayer2,gridPlayer2);
					rowsCompletedPlayer2 = ProceedCompleteLine(piecesPlayer2,gridPlayer2);
					CreatHoledLineInGrid(piecesPlayer1,gridPlayer1,rowsCompletedPlayer2-1);
					piecesPlayer2 = InitialiseRandomPieces(gridPlayer2);
					canBeMovedLowerPlayer2 = true;
				}
				// Pieces of two player will move down until it cannot go lower
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
				// Record the score of lines
				if (rowsCompletedPlayer1 > 0 || rowsCompletedPlayer2 >0 ) {
					contorRows = contorRows + rowsCompletedPlayer1 + rowsCompletedPlayer2 ;
					rowsCompletedPlayer1  = 0;
					rowsCompletedPlayer2 = 0;
				}
				// finaly we check if the victory condition are reach
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

/* Tetris playable for a human player and the IA
 * It's the hight level game management */
bool GameIA(){
		if (SDL_Init(SDL_INIT_VIDEO) != 0 ) return 1;
		// Start of a turn : init value for the turn
		Block** gridPlayer1 = InitialiseGrid();
		Block** gridPlayer2 = InitialiseGrid();
		Block* piecesPlayer1 = InitialiseRandomPieces(gridPlayer1);
		Block* piecesPlayer2 = InitialiseRandomPieces(gridPlayer2);
		bool continueGame = true;
		bool quit = false;
		bool timeIsnotOverAtTheRow;
		bool canBeMovedLowerPlayer1 = true;
		bool canBeMovedLowerPlayer2 = true;
		bool IAMustFindTheLocation = true;
		int rowsCompletedPlayer1 = 0;
		int rowsCompletedPlayer2 = 0;
		int contorRows = 0;
		long timerPlayer1 = currentTimeMillis();
		long timerPlayer2 = currentTimeMillis();
		SDL_Event PlayerEvent;
		int press = 0;

		init();
		drawBackground();
		while (continueGame)
		{
			SDL_Event event;
		  while(continueGame&& !quit) {
				SDL_PumpEvents();
				drawGrid(gridPlayer1,32,32);
				drawGrid(gridPlayer2,608,32);
				SDL_UpdateWindowSurface(pWindow);
				// Once pieces cannot go further down we set the pieces as placed for grid 1
				if( ! canBeMovedLowerPlayer1 ) {
					SetPiecePlaced(piecesPlayer1,gridPlayer1);
					rowsCompletedPlayer1 = ProceedCompleteLine(piecesPlayer1,gridPlayer1);
					CreatHoledLineInGrid(piecesPlayer2,gridPlayer2,rowsCompletedPlayer1-1);
					free(piecesPlayer1);
					piecesPlayer1 = InitialiseRandomPieces(gridPlayer1);
					canBeMovedLowerPlayer1 = true;
				}
				// Once pieces cannot go further down we set the pieces as placed for grid 2
				if ( ! canBeMovedLowerPlayer2 ){
					SetPiecePlaced(piecesPlayer2,gridPlayer2);
					rowsCompletedPlayer2 = ProceedCompleteLine(piecesPlayer2,gridPlayer2);
					CreatHoledLineInGrid(piecesPlayer1,gridPlayer1,rowsCompletedPlayer2-1);
					free(piecesPlayer2);
					piecesPlayer2 = InitialiseRandomPieces(gridPlayer2);
					canBeMovedLowerPlayer2 = true;
					IAMustFindTheLocation = true;
				}
				// Pieces of two player will move down until it cannot go lower
		    while(canBeMovedLowerPlayer1 && canBeMovedLowerPlayer2){
						if (timerPlayer1 <= currentTimeMillis() ) {
							timerPlayer1 = currentTimeMillis() + GetBlockSpeed(contorRows); //speed regarding completed rows
							canBeMovedLowerPlayer1 = MovePiece(Down,piecesPlayer1,gridPlayer1);
						}
						if (timerPlayer2 <= currentTimeMillis() )  {
							timerPlayer2 = currentTimeMillis() + GetBlockSpeedForIA(contorRows);
							canBeMovedLowerPlayer2 = MovePiece(Down,piecesPlayer2,gridPlayer2);
						}
						while ( (timerPlayer1 > currentTimeMillis()) && (timerPlayer2 > currentTimeMillis()) ){
									if(kbhit()){
										switch ((press = getch())) {
											// P1
											case KEY_UP : RotatePiece(piecesPlayer1,gridPlayer1);break;
											case KEY_DOWN : timerPlayer1 = currentTimeMillis(); break;
											case KEY_LEFT : MovePiece(Left,piecesPlayer1,gridPlayer1); break;
											case KEY_RIGHT : MovePiece(Right,piecesPlayer1,gridPlayer1); break;
											// P2 is IA
										}
										drawGrid(gridPlayer1,32,32);
										drawGrid(gridPlayer2,608,32);
										SDL_UpdateWindowSurface(pWindow);
									}

						}
						// P2 Move
						if(IAMustFindTheLocation){
							IATable* bestMove = FindBestMove(piecesPlayer2,gridPlayer2);
							printf("Best table :");
							PrintIATAble(bestMove);
							ExecuteIAMove(bestMove,piecesPlayer2,gridPlayer2);
							IAMustFindTheLocation = false;
						}
						drawGrid(gridPlayer1,32,32);
						drawGrid(gridPlayer2,608,32);
						SDL_UpdateWindowSurface(pWindow);
		    }
				// Record the score of lines
				if (rowsCompletedPlayer1 > 0 || rowsCompletedPlayer2 >0 ) {
					contorRows = contorRows + rowsCompletedPlayer1 + rowsCompletedPlayer2 ;
					rowsCompletedPlayer1  = 0;
					rowsCompletedPlayer2 = 0;
				}
				// finaly we check if the victory condition are reach
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


int main(int argc, char** argv)
{
	while (true){
		int press = 0;
		bool quit = false;
		int choice = 0;
		SDL_Event e;
		int xMouse, yMouse;
		SDL_PumpEvents();
		init();
		drawMenu();
		// If a certain button is press, start the corresponding game
		while(quit == false){
	    while(SDL_PollEvent(&e) != 0){
	        if((e.type == SDL_MOUSEBUTTONDOWN)){
	            SDL_GetMouseState(&xMouse,&yMouse);
							if(XYInButton(srcButton1P,xMouse,yMouse)){
								choice = 1;
								quit = true;
							}
							else if (XYInButton(srcButton2P,xMouse,yMouse)){
								choice = 2;
								quit = true;
							}
							else if (XYInButton(srcButtonIA,xMouse,yMouse)) {
								choice = 3;
								quit = true;
							}
	        }
	    }
		}
		SDL_DestroyWindow(pWindow);

		switch(choice){
			case 1 : GameSolo(); break;
			case 2 : GameDuo(); break;
			case 3 : GameIA(); break;
		}

		SDL_DestroyWindow(pWindow);
	}
}
