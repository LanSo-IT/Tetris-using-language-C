:: Helper for compilation of project
@ECHO OFF
TITLE Comiplator
:tryToCompile
gcc src/arkanoid.c src/model_tetris.c -o bin/Tetris -I include -L lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image || pause && CLS && GOTO tryToCompile
