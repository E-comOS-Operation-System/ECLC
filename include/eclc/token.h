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
    TOK_INTEGER,       // 123
    TOK_STRING,        // "hello"
    TOK_CHAR,          // 'a'
    // Keywords
    TOK_INT,           // int
    TOK_RETURN,        // return
    TOK_INCLUDE,       // include (for #include)
    // identifier
    TOK_IDENTIFIER,
    // Operators
    TOK_ASSIGN,        // =
    TOK_PLUS,          // +
    TOK_MINUS,         // -
    TOK_MULTIPLY,      // *
    TOK_DIVIDE,        // /
    TOK_MODULO,        // %
    TOK_AMPERSAND,     // &
    TOK_PIPE,          // |
    TOK_CARET,         // ^
    TOK_TILDE,         // ~
    TOK_EXCLAMATION,   // !
    TOK_QUESTION,      // ?
    TOK_COLON,         // :
    // Comparison
    TOK_EQ,            // ==
    TOK_NE,            // !=
    TOK_LT,            // <
    TOK_LE,            // <=
    TOK_GT,            // >
    TOK_GE,            // >=
    // Delimiters
    TOK_LBRACE,        // {
    TOK_RBRACE,        // }
    TOK_LPAREN,        // (
    TOK_RPAREN,        // )
    TOK_LBRACKET,      // [
    TOK_RBRACKET,      // ]
    TOK_SEMICOLON,     // ;
    TOK_COMMA,         // ,
    TOK_DOT,           // .
    TOK_ARROW,         // ->
    // Preprocessor
    TOK_HASH,          // #
    // Special
    TOK_EOF,           // End of file
    TOK_ERROR          // Error
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