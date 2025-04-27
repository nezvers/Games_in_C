# NearMissSnake
![](https://github.com/nezvers/Games_in_C/raw/main/NearMissSnake/Preview.gif)

- Unorthodox snake game, where you need to push apple into snakes mouth using a solid block or your tail.
- Snake can warp to other side of screen.
- Tap in movement direction to move faster.
- Project uses my favorite game mechanic - color palette swap. Here it happens on death with nice interpolation to next random chosen color palette.
- For colored rectangles, text, input, music and sounds used [Raylib](https://github.com/raysan5/raylib) framework.
- XM mod music by [Drozerix/M0N50N](https://modarchive.org/index.php?request=view_by_moduleid&query=172898)

# Building
`git clone https://github.com/nezvers/Games_in_C.git`  
Or download repository as zip.

## Visual Studio
- Open folder with `Visual Studio`
- Wait until `Output` tab says `CMake generation finished`
- In dropdown menu next to "Play" button choose `NearMissSnake.exe`

## Cmake
- Cmake should be installed and accessible from cmd/powershell/terminal
```
cd Games_in_C/NearMissSnake
cmake -S . -B ./build -G "${YOUR_BUILD_TARGET_CHOICE}"
```

### Example - Windows Mingw with optional debug build
- Mingw64 should be installed and `mingw64/bin` folder added to environment `PATH` variable
```
cmake -S . -B ./build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug
mingw32-make -C ./build
./build/NearMissSnake.exe
```
