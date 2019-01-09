/*
 * functions to create AST
 * functions called from parser.y
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cstring>
#include "read_ast.cpp"

ast *int_ast(int i) {
    Type *type = create_int(i);
    ast *ast_value = new ast(type, sizeof(*type));
    ast_value->set_condition(INT_F);
    return ast_value;
}

ast *double_ast(double d) {
    Type *type = create_double(d);
    ast *ast_value = new ast(type, sizeof(*type));
    ast_value->set_condition(DOUBLE_F);
    return ast_value;
}

ast *char_ast(char c) {
    Type *type = create_char(c);
    ast *ast_value = new ast(type, sizeof(*type));
    ast_value->set_condition(CHARACTER_F);
    return ast_value;
}

ast *bool_ast(bool b) {
    Type *type = create_bool(b);
    ast *ast_value = new ast(type, sizeof(*type));
    ast_value->set_condition(BOOLEAN_F);
    return ast_value;
}

ast *left_ast(ast *ast1, int flag) {
    return new ast(ast1, sizeof(*ast1), flag);
}

ast *left_ast1(char *str, int flag) {
    return new ast(str, sizeof(str), flag);
}

ast *full_ast(ast *ast1, ast *ast2, int flag) {
    return new ast(ast1, sizeof(*ast1), ast2, sizeof(*ast2), flag);
}

ast *full_ast1(char *str, ast *ast1, int flag) {
    return new ast(str, sizeof(str), ast1, sizeof(*ast1), flag);
}

ast *flag_ast(int flag) {
    return new ast(flag);
}

ast *if_ast(ast *condition, ast *program, int flag) {
    ast_if *ast_value = new ast_if;
    ast_value->set_condition(flag);
    ast_value->set_if_condition(condition, sizeof(*condition));
    if (program != NULL)
        ast_value->set_if_program(program, sizeof(*program));
    return ast_value;
};

ast *if_ast(ast *condition, ast *program, ast *else_program, int flag) {
    ast_if *ast_value = new ast_if;
    ast_value->set_condition(flag);
    ast_value->set_if_condition(condition, sizeof(*condition));
    if (program != NULL)
        ast_value->set_if_program(program, sizeof(*program));
    ast_value->set_else(else_program, sizeof(*else_program));
    return ast_value;
}

ast *if_ast1(ast *condition, ast *program, ast *else_if_program, int flag) {
    ast_if *ast_value = new ast_if;
    ast_value->set_condition(flag);
    ast_value->set_if_condition(condition, sizeof(*condition));
    if (program != NULL)
        ast_value->set_if_program(program, sizeof(*program));
    ast_value->set_else_if(else_if_program, sizeof(*else_if_program));
    return ast_value;
}

ast *if_ast(ast *condition, ast *program, ast *else_program, ast *else_if_program, int flag) {
    ast_if *ast_value = new ast_if;
    ast_value->set_condition(flag);
    ast_value->set_if_condition(condition, sizeof(*condition));
    if (program != NULL)
        ast_value->set_if_program(program, sizeof(*program));
    ast_value->set_else(else_program, sizeof(*else_program));
    ast_value->set_else_if(else_if_program, sizeof(*else_if_program));
    return ast_value;
}
