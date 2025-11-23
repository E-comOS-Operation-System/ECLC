/** 
    ECLC - E-comOS C/C++ Language Compiler
    Copyright (C) 2025  Saladin5101

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "eclc/common.h"
#include <string.h>
#include <stdio.h>

typedef struct {
    char** input_files;
    int file_count;
    bool c_mode;
    bool cpp_mode;
    bool folder_mode;
    char* folder_path;
    char* output_file;
    int optimization_level;
    bool debug_info;
    bool show_help;
    bool show_version;
} CompilerConfig;

CompilerConfig parse_arguments(int argc, char** argv) {
    CompilerConfig config = {0};
    config.optimization_level = 0; // Default: no optimization
    
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            // Language specification
            if (strcmp(argv[i], "--c-code") == 0) {
                config.c_mode = true;
                config.cpp_mode = false;
            }
            else if (strcmp(argv[i], "--cpp-code") == 0) {
                config.cpp_mode = true;
                config.c_mode = false;
            }
            // Folder mode
            else if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) {
                config.folder_mode = true;
                config.folder_path = argv[++i];
            }
            // Output file
            else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
                config.output_file = argv[++i];
            }
            // Optimization levels
            else if (strncmp(argv[i], "-O", 2) == 0) {
                config.optimization_level = argv[i][2] - '0';
            }
            // Debug info
            else if (strcmp(argv[i], "-g") == 0) {
                config.debug_info = true;
            }
            // Help and version
            else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
                config.show_help = true;
            }
            else if (strcmp(argv[i], "--version") == 0 || strcmp(argv[i], "-v") == 0) {
                config.show_version = true;
            }
        } else {
            // Input file
            config.input_files = realloc(config.input_files, 
                                       (config.file_count + 1) * sizeof(char*));
            config.input_files[config.file_count++] = argv[i];
        }
    }
    
    return config;
}

void print_help() {
    printf("ECLC - E-comOS C/C++ Compiler\n\n");
    printf("Usage: eclc [options] <file1> [file2 ...]\n\n");
    printf("Basic Usage:\n");
    printf("  eclc hello.c                 # Compile C file\n");
    printf("  eclc hello.cpp              # Compile C++ file\n");
    printf("  eclc -f project/src         # Compile folder\n");
    printf("  eclc main.c -o myapp        # Specify output\n\n");
    printf("Language Options:\n");
    printf("  --c-code                    # Force C mode\n");
    printf("  --cpp-code                  # Force C++ mode\n");
    printf("  (Usually auto-detected from file extension)\n\n");
    printf("Full help: eclc --help\n");
}