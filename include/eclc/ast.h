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
#ifndef ECLC_AST_H
#define ECLC_AST_H

#include "token.h"

typedef enum {
    NODE_PROGRAM,
    NODE_FUNCTION_DEF,
    NODE_VARIABLE_DECL,
    NODE_RETURN_STMT,
    NODE_INTEGER_LITERAL,
    NODE_BINARY_OP,
    NODE_IDENTIFIER
} NodeType; // AST node types

typedef struct ASTNode {
    NodeType type;
    Token token;
    struct ASTNode* left;
    struct ASTNode* right;
} ASTNode;

typedef struct {
    ASTNode base;
    char* name;
    ASTNode return_type;
    ASTNode* parameters;
    ASTNode* body;
} FunctionDefNode;

// Status of Token
typedef struct {
    TokenStream* tokens;        // Unit stream
    int current_pos;            // local
    ASTNode* root;              // AST root node
    char* filename;             // Source filename
} Parser;

// API function
Parser* parser_create(TokenStream* tokens, const char* filename);
ASTNode* parser_parse(Parser* parser);
void parser_destroy(Parser* parser);
void ast_print(ASTNode* node, int indent);

#endif // ECLC_AST_H
