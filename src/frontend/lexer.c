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
#include "eclc/common.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>

static bool is_identifier_char(char c) {
    return isalnum(c) || c == '_';
}

static Token* create_token(TokenType type, const char* value, int line, int column) {
    Token* token = xmalloc(sizeof(Token));
    token->type = type;
    token->value = value ? strdup(value) : NULL;
    token->line = line;
    token->column = column;
    return token;
}

static void token_stream_add(TokenStream* stream, Token* token) {
    if (stream->count >= stream->capacity) {
        stream->capacity = stream->capacity ? stream->capacity * 2 : 16;
        stream->tokens = xrealloc(stream->tokens, stream->capacity * sizeof(Token));
    }
    stream->tokens[stream->count++] = *token;
    xfree(token);
}

TokenStream* tokenize(const char* source) {
    TokenStream* stream = xcalloc(1, sizeof(TokenStream));
    int pos = 0;
    int line = 1;
    int column = 1;
    
    while (source[pos] != '\0') {
        char c = source[pos];
        
        // Skip space char
        if (isspace(c)) {
            if (c == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            pos++;
            continue;
        }
        
        // Know number digit
        if (isdigit(c)) {
            int start = pos;
            while (isdigit(source[pos])) {
                pos++;
                column++;
            }
            int length = pos - start;
            char* value = xmalloc(length + 1);
            strncpy(value, source + start, length);
            value[length] = '\0';
            
            token_stream_add(stream, create_token(TOK_INTEGER, value, line, column - length));
            xfree(value);
            continue;
        }
        
        // Type and keywords
        if (isalpha(c) || c == '_') {
            int start = pos;
            while (is_identifier_char(source[pos])) {
                pos++;
                column++;
            }
            int length = pos - start;
            char* value = xmalloc(length + 1);
            strncpy(value, source + start, length);
            value[length] = '\0';
            
            // Check Keywords
            TokenType type = TOK_IDENTIFIER;
            if (strcmp(value, "int") == 0) type = TOK_INT;
            else if (strcmp(value, "return") == 0) type = TOK_RETURN;
            
            token_stream_add(stream, create_token(type, value, line, column - length));
            xfree(value);
            continue;
        }
        
        // A char's token
        TokenType type = TOK_ERROR;
        switch (c) {
            case '{': type = TOK_LBRACE; break;
            case '}': type = TOK_RBRACE; break;
            case '(': type = TOK_LPAREN; break;
            case ')': type = TOK_RPAREN; break;
            case ';': type = TOK_SEMICOLON; break;
            default:
                fprintf(stderr, "Error: Unknown character '%c' at line %d, column %d\n", 
                       c, line, column);
                type = TOK_ERROR;
        }
        
        if (type != TOK_ERROR) {
            char char_str[2] = {c, '\0'};
            token_stream_add(stream, create_token(type, char_str, line, column));
        }
        
        pos++;
        column++;
    }
    
    // Add EOF token
    token_stream_add(stream, create_token(TOK_EOF, NULL, line, column));
    return stream;
}

void token_stream_free(TokenStream* stream) {
    for (size_t i = 0; i < stream->count; i++) {
        if (stream->tokens[i].value) {
            xfree(stream->tokens[i].value);
        }
    }
    xfree(stream->tokens);
    xfree(stream);
}

Token* token_stream_next(TokenStream* stream) {
    if (stream->current >= stream->count) {
        return NULL;
    }
    return &stream->tokens[stream->current++];
}

Token* token_stream_peek(TokenStream* stream) {
    if (stream->current >= stream->count) {
        return NULL;
    }
    return &stream->tokens[stream->current];
}