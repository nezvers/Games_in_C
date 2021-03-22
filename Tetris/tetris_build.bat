echo off
set src=main.c game.c tetromino.c
set output=main.exe
set platform=-DPLATFORM_DESKTOP
set include=
set raylib=Raylib\
set lib=%raylib%

set compailer=gcc
set compFlags=-Wall %raylib%raylib.rc.data -s -static -Os -std=c99 %platform%
set linkFlags=-lraylib -lopengl32 -lgdi32 -luser32 -lwinmm 

del %output%
%compailer% -o %output% %src% %compFlags% -I%include% -L%lib% %linkFlags%

%output%
pause