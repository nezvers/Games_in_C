set output=Tetris

del %output%.exe
mingw32-make OUTPUT=%output%
%output%.exe
pause