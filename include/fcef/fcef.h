/** 
 * ECLC - E-comOS C/C++ Language Compiler
 * Copyright (C) 2025  Saladin5101
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#ifndef ECLC_FCEF_H
#define ECLC_FCEF_H

#include "fcef.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// ==================== ECLC maked data typedef ====================
typedef struct {
    uint8_t *code;         
    size_t code_size;      
    uint8_t *data;          
    size_t data_size;       
    uint8_t *rodata;        
    size_t rodata_size;     
    uint8_t *bss;           
    size_t bss_size;        
    uint32_t entry_point;   
    uint32_t text_addr;     
    uint32_t data_addr;     
    uint32_t rodata_addr;   
} eclc_output_t;

// ==================== Function ====================

// From ECLC ouput create FCEF file
void *eclc_to_fcef(const eclc_output_t *output, size_t *out_size);

// save ECLC output FCEF file
bool eclc_save_fcef(const eclc_output_t *output, const char *filename);

// load ECLC compiltion FCEF file
eclc_output_t *eclc_load_fcef(const char *filename);

// Free ECLC output structure
void eclc_free_output(eclc_output_t *output);

// Output ECLC information for debugging
void eclc_print_output(const eclc_output_t *output);

#ifdef __cplusplus
}
#endif

#endif // ECLC_FCEF_H