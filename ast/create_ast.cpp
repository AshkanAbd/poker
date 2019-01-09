/*
 * functions to create AST
 * functions called from parser.y
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cstring>
#include "read_ast.cpp"

ast *create_int_ast(int i) {
    Type *type = create_int(i);
    ast *ast_value = new ast(type, sizeof(*type));
    ast_value->set_condition(INT_F);
    return ast_value;
}

ast *create_double_ast(double d) {
    Type *type = create_double(d);
    ast *ast_value = new ast(type, sizeof(*type));
    ast_value->set_condition(DOUBLE_F);
    return ast_value;
}

ast *create_char_ast(char c) {
    Type *type = create_char(c);
    ast *ast_value = new ast(type, sizeof(*type));
    ast_value->set_condition(CHARACTER_F);
    return ast_value;
}