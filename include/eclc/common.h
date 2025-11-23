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
#ifndef ECLC_COMMON_H
#define ECLC_COMMON_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

// Basic 
typedef int32_t i32;
typedef int64_t i64;
typedef uint32_t u32;
typedef uint64_t u64;

// Memory
void* xmalloc(size_t size);
void* xcalloc(size_t count, size_t size);
void* xrealloc(void* ptr, size_t size);
void xfree(void* ptr);

// Error
#define PANIC(...) do { \
    fprintf(stderr, "PANIC: " __VA_ARGS__); \
    fprintf(stderr, " [%s:%d]\n", __FILE__, __LINE__); \
    exit(1); \
} while(0)

#define ASSERT(cond, ...) do { \
    if (!(cond)) { \
        fprintf(stderr, "ASSERT FAILED: " __VA_ARGS__); \
        fprintf(stderr, " [%s:%d]\n", __FILE__, __LINE__); \
        exit(1); \
    } \
} while(0)

#endif