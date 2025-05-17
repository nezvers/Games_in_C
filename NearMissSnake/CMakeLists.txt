cmake_minimum_required(VERSION 3.28)
include(FetchContent)


# Raylib
FetchContent_Declare(
    raylib
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    URL https://github.com/raysan5/raylib/archive/refs/tags/5.5.zip
)
#FetchContent_GetProperties(raylib)
set(FETCHCONTENT_QUIET NO)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # don't build the supplied example games
FetchContent_MakeAvailable(raylib)

set(ProjectName NearMissSnake)
project(${ProjectName} LANGUAGES C)

include_directories(${CMAKE_SOURCE_DIR}/include)

set(CMAKE_C_STANDARD 99)
file(GLOB APP_SOURCE_FILES src/*.c src/*.h)
add_executable(${ProjectName} ${APP_SOURCE_FILES})

target_link_libraries(${ProjectName} raylib)
