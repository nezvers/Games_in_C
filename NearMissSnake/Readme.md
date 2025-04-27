# NearMissSnake
![](https://github.com/nezvers/Games_in_C/raw/main/NearMissSnake/Preview.gif)

- Unorthodox snake game, where you need to push apple into snakes mouth using a solid block or your tail.
- Snake can warp to other side of screen.
- Tap in movement direction to move faster.
- Project uses my favorite game mechanic - color palette swap. Here it happens on death with nice interpolation to next random chosen color palette.
- For colored rectangles, text, input, music and sounds used [Raylib](https://github.com/raysan5/raylib) framework.

# Building
`git clone https://github.com/nezvers/Games_in_C.git`  
Or download repository as zip.

## Visual Studio
- Open folder with `Visual Studio`
- Wait until `Output` tab says `CMake generation finished`
- In dropdown menu next to "Play" button choose `NearMissSnake.exe`

## Cmake
```
cd Games_in_C/NearMissSnake
cmake -S . -B ./build -G "${YOUR_BUILD_TARGET_CHOICE}"
```
#### Windows Mingw example with optional debug build
```
cmake -S . -B ./build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug
mingw32-make -C ./build
./build/NearMissSnake.exe
```
