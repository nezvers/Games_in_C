@echo off
set source=NearMissSnake.c
set output=NearMissSnake.exe
set raylib_src=C:\raylib\raylib\src
set compF=-Wall -std=c99 -Wno-missing-braces
set libF=-lopengl32 -lgdi32 -lwinmm
::delete previous compiled file
cmd /c IF EXIST %output% del /F %output%
::compile
gcc %source% -o %output% %compF% -I %raylib_src%/ -L %raylib_src% -lraylib %libF%
%output%
pause