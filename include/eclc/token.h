/* 
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
#ifndef ECLC_TOKEN_H
#define ECLC_TOKEN_H

#include "common.h"

typedef enum {
    // Literal quantity
    TOK_INTEGER, // Literal quantity : 123
    // Keywords
    TOK_INT, // Int
    TOK_RETURN, // Return
    // identifier
    TOK_IDENTIFIER,
    // Operators and delimiters
    TOK_LBRACE,        // {
    TOK_RBRACE,        // }
    TOK_LPAREN,        // (
    TOK_RPAREN,        // )
    TOK_SEMICOLON,     // ;
    // Special
    TOK_EOF, // End of file(s)
    TOK_ERROR // Error(s)
} TokenType;

typedef struct {
    TokenType type;
    char* value;        // token's string
    int line;          
    int column;        
} Token;

typedef struct {
    Token* tokens;
    size_t count;
    size_t capacity;
    size_t current;
} TokenStream;

// Lexical Analyzer API
TokenStream* tokenize(const char* source);
void token_stream_free(TokenStream* stream);
Token* token_stream_next(TokenStream* stream);
Token* token_stream_peek(TokenStream* stream);

#endif // ECLC_TOKEN_H