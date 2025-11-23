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
#include <stdlib.h>
#include <stdio.h>

void* xmalloc(size_t size) {
    void* ptr = malloc(size);
    if (!ptr) {
        PANIC("Out of memory: failed to allocate %zu bytes", size);
    }
    return ptr;
}

void* xcalloc(size_t count, size_t size) {
    void* ptr = calloc(count, size);
    if (!ptr) {
        PANIC("Out of memory: failed to allocate %zu bytes", count * size);
    }
    return ptr;
}

void* xrealloc(void* ptr, size_t size) {
    void* new_ptr = realloc(ptr, size);
    if (!new_ptr && size > 0) {
        PANIC("Out of memory: failed to reallocate %zu bytes", size);
    }
    return new_ptr;
}

void xfree(void* ptr) {
    if (ptr) {
        free(ptr);
    }
}