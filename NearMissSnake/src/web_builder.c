#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>

#define MAX_COMMAND_LENGTH 1024
#define MAX_FILES 100

// Define target platform
#define PLATFORM "PLATFORM_WEB"

// Define default values for project variables
#ifndef PROJECT_NAME
#define PROJECT_NAME "raylib_game"
#endif

#ifndef PROJECT_VERSION
#define PROJECT_VERSION "1.0"
#endif

#ifndef PROJECT_BUILD_PATH
#define PROJECT_BUILD_PATH "."
#endif

#ifndef RAYLIB_PATH
#define RAYLIB_PATH "../../raylib"
#endif

#ifndef BUILD_MODE
#define BUILD_MODE "DEBUG"
#endif

#ifndef BUILD_WEB_ASYNCIFY
#define BUILD_WEB_ASYNCIFY "FALSE"
#endif

#ifndef BUILD_WEB_SHELL
#define BUILD_WEB_SHELL "minshell.html"
#endif

#ifndef BUILD_WEB_HEAP_SIZE
#define BUILD_WEB_HEAP_SIZE "128MB"
#endif

#ifndef BUILD_WEB_RESOURCES
#define BUILD_WEB_RESOURCES "TRUE"
#endif

#ifndef BUILD_WEB_RESOURCES_PATH
#define BUILD_WEB_RESOURCES_PATH "resources"
#endif

// Define environment paths for Emscripten
#ifndef EMSDK_PATH
#define EMSDK_PATH "O:/compilers/emsdk"
#endif

#define EMSCRIPTEN_PATH EMSDK_PATH "/upstream/emscripten"
#define CLANG_PATH EMSDK_PATH "/upstream/bin"
#define PYTHON_PATH EMSDK_PATH "/python/3.9.2-nuget_64bit"
#define NODE_PATH EMSDK_PATH "/node/16.20.0_64bit/bin"

void compile_sources(const char *sources[], int source_count);
void link_objects(const char *project_name, const char *output_ext);
void clean(void);
void delete_files_by_extension(const char *path, const char *ext);
int get_source_files(const char *path, const char *extension, char source_files[][MAX_COMMAND_LENGTH]);


int main(int argc, char *argv[]) {
    // Define source code files
    const char *project_source_files[] = {"raylib_game"};
    int source_count = sizeof(project_source_files) / sizeof(project_source_files[0]);

    //char source_files[MAX_FILES][MAX_COMMAND_LENGTH];
    //int source_count = get_source_files(".", ".c", source_files);

    if (argc > 1 && strcmp(argv[1], "clean") == 0) {
        clean();
    } else {
        compile_sources(project_source_files, source_count);
        link_objects(PROJECT_NAME, ".html");
    }

    return 0;
}

void compile_sources(const char *sources[], int source_count) {
    char command[MAX_COMMAND_LENGTH];

    for (int i = 0; i < source_count; i++) {
        snprintf(command, sizeof(command), "emcc -c %s.c -o %s.o -Wall -std=c99 -D_DEFAULT_SOURCE -Wno-missing-braces -Wunused-result -D%s",
                 sources[i], sources[i], PLATFORM);

        if (strcmp(BUILD_MODE, "DEBUG") == 0) {
            strncat(command, " -g -D_DEBUG", sizeof(command) - strlen(command) - 1);
        } else {
            if (strcmp(BUILD_WEB_ASYNCIFY, "TRUE") == 0) {
                strncat(command, " -O3", sizeof(command) - strlen(command) - 1);
            } else {
                strncat(command, " -Os", sizeof(command) - strlen(command) - 1);
            }
        }

        strncat(command, " -I. -I" RAYLIB_PATH "/src -I" RAYLIB_PATH "/src/external", sizeof(command) - strlen(command) - 1);
        system(command);
    }
}

void link_objects(const char *project_name, const char *output_ext) {
    char command[MAX_COMMAND_LENGTH];

    snprintf(command, sizeof(command),
             "emcc -o %s/%s%s raylib_game.o -Wall -std=c99 -D_DEFAULT_SOURCE -Wno-missing-braces -Wunused-result -D%s -L. -L%s/src -s USE_GLFW=3 -s TOTAL_MEMORY=%s -s FORCE_FILESYSTEM=1",
             PROJECT_BUILD_PATH, project_name, output_ext, PLATFORM, RAYLIB_PATH, BUILD_WEB_HEAP_SIZE);

    if (strcmp(BUILD_WEB_ASYNCIFY, "TRUE") == 0) {
        strncat(command, " -s ASYNCIFY", sizeof(command) - strlen(command) - 1);
    }

    if (strcmp(BUILD_WEB_RESOURCES, "TRUE") == 0) {
        strncat(command, " --preload-file " BUILD_WEB_RESOURCES_PATH, sizeof(command) - strlen(command) - 1);
    }

    if (strcmp(BUILD_MODE, "DEBUG") == 0) {
        strncat(command, " -s ASSERTIONS=1 --profiling", sizeof(command) - strlen(command) - 1);
    }

    strncat(command, " --shell-file " BUILD_WEB_SHELL " " RAYLIB_PATH "/src/libraylib.a", sizeof(command) - strlen(command) - 1);
    system(command);
}

void clean(void) {
    delete_files_by_extension(".", ".o");
    delete_files_by_extension(".", ".html");
    delete_files_by_extension(".", ".js");
    delete_files_by_extension(".", ".wasm");
    delete_files_by_extension(".", ".data");
    printf("Cleaning done\n");
}

void delete_files_by_extension(const char *path, const char *ext) {
    struct dirent *entry;
    char filepath[MAX_COMMAND_LENGTH];
    DIR *dir = opendir(path);

    if (dir == NULL) {
        perror("Unable to open directory");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strstr(entry->d_name, ext)) {
            snprintf(filepath, sizeof(filepath), "%s/%s", path, entry->d_name);
            if (unlink(filepath) == 0) {
                printf("Deleted: %s\n", filepath);
            } else {
                perror("Failed to delete file");
            }
        }
    }
    closedir(dir);
}

int get_source_files(const char *path, const char *extension, char source_files[][MAX_COMMAND_LENGTH]) {
    struct dirent *entry;
    int count = 0;
    DIR *dir = opendir(path);

    if (dir == NULL) {
        perror("Unable to open directory");
        return 0;
    }

    while ((entry = readdir(dir)) != NULL && count < MAX_FILES) {
        if (strstr(entry->d_name, extension)) {
            snprintf(source_files[count], MAX_COMMAND_LENGTH, "%s/%s", path, entry->d_name);
            count++;
        }
    }
    closedir(dir);
    return count;
}
