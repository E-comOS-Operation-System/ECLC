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
#include "eclc/ast.h"
#include "eclc/common.h"
#include <stdio.h>
#include <string.h>

// Create new AST node
static ASTNode* create_node(NodeType type, Token token) {
    ASTNode* node = xcalloc(1, sizeof(ASTNode));
    node->type = type;
    node->token = token;
    return node;
}

// Get current token
static Token* current_token(Parser* parser) {
    if (parser->current_pos >= parser->tokens->count) {
        return NULL;
    }
    return &parser->tokens->tokens[parser->current_pos];
}

// Advance to next token
static void advance(Parser* parser) {
    if (parser->current_pos < parser->tokens->count) {
        parser->current_pos++;
    }
}

// Check if current token matches expected type
static bool match(Parser* parser, TokenType type) {
    Token* token = current_token(parser);
    return token && token->type == type;
}

// Consume token if it matches expected type
static bool consume(Parser* parser, TokenType type) {
    if (match(parser, type)) {
        advance(parser);
        return true;
    }
    return false;
}

// Parse integer literal
static ASTNode* parse_integer(Parser* parser) {
    Token* token = current_token(parser);
    if (!token || token->type != TOK_INTEGER) {
        return NULL;
    }
    ASTNode* node = create_node(NODE_INTEGER_LITERAL, *token);
    advance(parser);
    return node;
}

// Parse identifier
static ASTNode* parse_identifier(Parser* parser) {
    Token* token = current_token(parser);
    if (!token || token->type != TOK_IDENTIFIER) {
        return NULL;
    }
    ASTNode* node = create_node(NODE_IDENTIFIER, *token);
    advance(parser);
    return node;
}

// Parse return statement: return <expression>;
static ASTNode* parse_return_stmt(Parser* parser) {
    if (!consume(parser, TOK_RETURN)) {
        return NULL;
    }
    
    Token return_token = parser->tokens->tokens[parser->current_pos - 1];
    ASTNode* node = create_node(NODE_RETURN_STMT, return_token);
    
    // Parse return value (integer for now)
    node->left = parse_integer(parser);
    
    if (!consume(parser, TOK_SEMICOLON)) {
        fprintf(stderr, "Error: Expected ';' after return statement\n");
        return NULL;
    }
    
    return node;
}

// Parse function body: { <statements> }
static ASTNode* parse_block(Parser* parser) {
    if (!consume(parser, TOK_LBRACE)) {
        return NULL;
    }
    
    // For now, just parse a single return statement
    ASTNode* stmt = parse_return_stmt(parser);
    
    if (!consume(parser, TOK_RBRACE)) {
        fprintf(stderr, "Error: Expected '}' to close block\n");
        return NULL;
    }
    
    return stmt;
}

// Parse function definition: int <name>() { <body> }
static ASTNode* parse_function(Parser* parser) {
    if (!consume(parser, TOK_INT)) {
        return NULL;
    }
    
    Token* name_token = current_token(parser);
    if (!name_token || name_token->type != TOK_IDENTIFIER) {
        fprintf(stderr, "Error: Expected function name\n");
        return NULL;
    }
    
    ASTNode* node = create_node(NODE_FUNCTION_DEF, *name_token);
    advance(parser);
    
    if (!consume(parser, TOK_LPAREN)) {
        fprintf(stderr, "Error: Expected '(' after function name\n");
        return NULL;
    }
    
    if (!consume(parser, TOK_RPAREN)) {
        fprintf(stderr, "Error: Expected ')' after parameters\n");
        return NULL;
    }
    
    // Parse function body
    node->left = parse_block(parser);
    
    return node;
}

// Parse program (top-level)
static ASTNode* parse_program(Parser* parser) {
    Token dummy_token = {0};
    ASTNode* program = create_node(NODE_PROGRAM, dummy_token);
    
    // Parse single function for now
    program->left = parse_function(parser);
    
    return program;
}

// Create parser instance
Parser* parser_create(TokenStream* tokens, const char* filename) {
    Parser* parser = xcalloc(1, sizeof(Parser));
    parser->tokens = tokens;
    parser->current_pos = 0;
    parser->filename = filename ? strdup(filename) : NULL;
    return parser;
}

// Parse tokens into AST
ASTNode* parser_parse(Parser* parser) {
    if (!parser || !parser->tokens) {
        return NULL;
    }
    
    parser->root = parse_program(parser);
    return parser->root;
}

// Clean up parser
void parser_destroy(Parser* parser) {
    if (parser) {
        if (parser->filename) {
            xfree(parser->filename);
        }
        xfree(parser);
    }
}

// Print AST for debugging
void ast_print(ASTNode* node, int indent) {
    if (!node) return;
    
    for (int i = 0; i < indent; i++) printf("  ");
    
    switch (node->type) {
        case NODE_PROGRAM:
            printf("Program\n");
            break;
        case NODE_FUNCTION_DEF:
            printf("Function: %s\n", node->token.value);
            break;
        case NODE_RETURN_STMT:
            printf("Return\n");
            break;
        case NODE_INTEGER_LITERAL:
            printf("Integer: %s\n", node->token.value);
            break;
        case NODE_IDENTIFIER:
            printf("Identifier: %s\n", node->token.value);
            break;
        default:
            printf("Unknown node\n");
    }
    
    if (node->left) ast_print(node->left, indent + 1);
    if (node->right) ast_print(node->right, indent + 1);
}