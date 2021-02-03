:: Helper for compilation of project
@ECHO OFF
TITLE Comiplator
:tryToCompile
gcc src/arkanoid.c -o bin/Tetris -I include -L lib -lmingw32 -lSDL2main -lSDL2 || pause && CLS && GOTO tryToCompile
