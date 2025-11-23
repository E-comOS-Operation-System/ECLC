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
#include "eclc/token.h"
#include "eclc/ast.h"
#include "eclc/common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

// Read file content
static char* read_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Cannot open file '%s'\n", filename);
        return NULL;
    }
    
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    char* content = xmalloc(size + 1);
    fread(content, 1, size, file);
    content[size] = '\0';
    
    fclose(file);
    return content;
}

// Check if file has C/C++ extension
static bool is_c_file(const char* filename) {
    const char* ext = strrchr(filename, '.');
    return ext && (strcmp(ext, ".c") == 0 || strcmp(ext, ".cpp") == 0);
}

// Compile single file (quiet mode for folder compilation)
static int compile_file_quiet(const char* filename) {
    char* source = read_file(filename);
    if (!source) {
        return 1;
    }
    
    TokenStream* tokens = tokenize(source);
    if (!tokens) {
        xfree(source);
        return 1;
    }
    
    Parser* parser = parser_create(tokens, filename);
    ASTNode* ast = parser_parse(parser);
    if (!ast) {
        parser_destroy(parser);
        token_stream_free(tokens);
        xfree(source);
        return 1;
    }
    
    parser_destroy(parser);
    token_stream_free(tokens);
    xfree(source);
    
    return 0;
}

// Compile single file (verbose mode)
static int compile_file(const char* filename) {
    printf("Compiling: %s\n", filename);
    
    char* source = read_file(filename);
    if (!source) {
        return 1;
    }
    
    TokenStream* tokens = tokenize(source);
    if (!tokens) {
        fprintf(stderr, "Error: Tokenization failed for %s\n", filename);
        xfree(source);
        return 1;
    }
    
    Parser* parser = parser_create(tokens, filename);
    ASTNode* ast = parser_parse(parser);
    if (!ast) {
        fprintf(stderr, "Error: Parsing failed for %s\n", filename);
        parser_destroy(parser);
        token_stream_free(tokens);
        xfree(source);
        return 1;
    }
    
    printf("AST for %s:\n", filename);
    ast_print(ast, 0);
    printf("\n");
    
    parser_destroy(parser);
    token_stream_free(tokens);
    xfree(source);
    
    return 0;
}

// Print cargo-style progress bar
static void print_progress(int current, int total, const char* current_file, bool success) {
    int percent = (current * 100) / total;
    int bar_width = 20;
    int filled = (current * bar_width) / total;
    
    printf("\r");
    if (success) {
        printf("\033[32m   Compiling\033[0m"); // Green "Compiling"
    } else {
        printf("\033[31m     Failed\033[0m"); // Red "Failed"
    }
    
    printf(" [%2d/%2d] ", current, total);
    
    // Progress bar
    for (int i = 0; i < bar_width; i++) {
        if (i < filled) {
            printf("\033[32m=\033[0m"); // Green filled
        } else if (i == filled && current < total) {
            printf("\033[33m>\033[0m"); // Yellow arrow
        } else {
            printf(" ");
        }
    }
    
    printf(" %s", current_file ? current_file : "");
    fflush(stdout);
}

// Count C files in directory
static int count_c_files(const char* folder_path) {
    DIR* dir = opendir(folder_path);
    if (!dir) return 0;
    
    struct dirent* entry;
    int count = 0;
    
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.') continue;
        if (is_c_file(entry->d_name)) count++;
    }
    
    closedir(dir);
    return count;
}

// Compile folder
static int compile_folder(const char* folder_path) {
    DIR* dir = opendir(folder_path);
    if (!dir) {
        fprintf(stderr, "Error: Cannot open directory '%s'\n", folder_path);
        return 1;
    }
    
    int total_files = count_c_files(folder_path);
    if (total_files == 0) {
        printf("No C/C++ files found in '%s'\n", folder_path);
        closedir(dir);
        return 0;
    }
    
    struct dirent* entry;
    int failed_count = 0;
    int current = 0;
    
    rewinddir(dir);
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.') continue;
        
        if (is_c_file(entry->d_name)) {
            current++;
            print_progress(current, total_files, entry->d_name, true);
            
            char filepath[1024];
            snprintf(filepath, sizeof(filepath), "%s/%s", folder_path, entry->d_name);
            
            if (compile_file_quiet(filepath) != 0) {
                print_progress(current, total_files, entry->d_name, false);
                failed_count++;
                printf("\n\033[31mError:\033[0m Failed to compile %s\n", entry->d_name);
            }
        }
    }
    
    printf("\n");
    if (failed_count == 0) {
        printf("\033[32m    Finished\033[0m compiling %d files\n", total_files);
    } else {
        printf("\033[31m    Finished\033[0m with %d errors out of %d files\n", failed_count, total_files);
    }
    
    closedir(dir);
    return failed_count > 0 ? 1 : 0;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <source_file> | -f <folder>\n", argv[0]);
        return 1;
    }
    
    // Check for folder compilation flag
    if (argc >= 3 && strcmp(argv[1], "-f") == 0) {
        return compile_folder(argv[2]);
    }
    
    // Single file compilation
    return compile_file(argv[1]);
}