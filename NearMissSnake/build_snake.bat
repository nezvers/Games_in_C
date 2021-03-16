::@echo off
::
:: Set path variables
set source=NearMissSnake.c
set output=NearMissSnake.exe
::set raylib_src=C:\raylib\raylib\src
set raylib_src=%cd%\Raylib
set compF=-Wall -std=c99 -Wno-missing-braces
set libF=-lraylib -lopengl32 -lgdi32 -lwinmm
::
:: Delete previous compiled file
cmd /c IF EXIST %output% del /F %output%
::
:: Compile
gcc %source% -o %output% %compF% -I%raylib_src% -L%raylib_src% %libF%
::
:: Launch exe
%output%
::
pause