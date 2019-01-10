/*
 * functions for reading AST that created by create_ast.cpp in parser.y
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cstring>
#include "../libs/functions.cpp"

void read_ast(ast *ast_value) {
    set_line_number(get_line_number() + 1);
    if (ast_value == NULL)
        return;
    if (ast_value->condition == PROGRAM_F) {
        read_ast((ast *) ast_value->left);
    } else if (dynamic_cast<ast_if *>(ast_value)) {
        if_statement_ast(dynamic_cast<ast_if *>(ast_value));
        return;
    } else if (dynamic_cast<ast_while *>(ast_value)) {
        while_statement_ast(dynamic_cast<ast_while *>(ast_value));
    } else if (ast_value->condition == PRINT_F) {
        // Print Info saved in left of AST
        print((ast *) ast_value->left);
    } else if (ast_value->condition == EQU_NEW_VAR_F) {
        // Add new variable
        add_new_variable(ast_value);
    } else if (ast_value->condition == EQU_EXIST_VAR_F) {
        // Change exist variable
        change_variable(ast_value);
    } else if (ast_value->condition == DEL_VAR_F) {
        // Delete variable
        delete_variable(ast_value);
    } else if (ast_value->condition == TYPEOF_F) {
        // Print type of an exp
        Type *type = read_exp((ast *) ast_value->left);
        print_type(type);
    }
}

void while_statement_ast(ast_while *ast_value) {
    ast *program = ast_value->program_flag ? (ast *) ast_value->program : NULL;
    ast *condition = (ast *) ast_value->while_condition;
    Type *condition_result;
    while (true) {
        condition_result = read_exp(condition);
        if (condition_result->type != BOOLEAN_TYPE) {
            yyerror("Invalid condition");
            return;
        }
        if (!(*(bool *) condition_result->value)) {
            return;
        }
        read_statement_ast(program);
    }
}

void if_statement_ast(ast_if *ast_value) {
    // Check AST IF is ELSE or NOT
    if (ast_value->condition == ELSE_F) {
        // If is ELSE then run and return
        read_statement_ast((ast *) ast_value->if_program);
        return;
    }
    // Try to read if (else if) condition and get result
    Type *condition_result = read_exp((ast *) ast_value->if_condition);
    if (condition_result->type != BOOLEAN_TYPE) {
        yyerror("Invalid condition");
        return;
    }
    bool b = *(bool *) condition_result->value;
    if (b) {
        // If condition is true then run if (else if) program
        if (ast_value->if_program_flag) {
            read_statement_ast((ast *) ast_value->if_program);
        }
    } else {
        // If condition is false then read else (else if) AST
        if (ast_value->else_if_flag) {
            read_statement_ast((ast *) ast_value->else_if_program);
        }
    }
}

void read_statement_ast(ast *ast_value) {
    if (ast_value->right_flag) {
        read_statement_ast((ast *) ast_value->right);
    }
    if (ast_value->left_flag) {
        read_ast((ast *) ast_value->left);
    } else {
        read_ast(ast_value);
    }
}

Type *read_exp(ast *ast_value) {
    // check is a compare ast
    if (is_compare(ast_value)) {
        Type *tmp_left = read_exp((ast *) ast_value->left);
        Type *tmp_right = read_exp((ast *) ast_value->right);
        return compare(tmp_left, tmp_right, ast_value->condition);
    }
    // check is a and-or ast
    if (ast_value->condition == OR_F || ast_value->condition == AND_F) {
        Type *tmp_left = read_exp((ast *) ast_value->left);
        Type *tmp_right = read_exp((ast *) ast_value->right);
        return get_and_or_result(tmp_left, tmp_right, ast_value->condition);
    }
    // check is a not ast
    if (ast_value->condition == NOT_F) {
        Type *type = read_exp((ast *) ast_value->left);
        return not_function(type);
    }
    if (ast_value->condition == DOUBLE_F || ast_value->condition == INT_F || ast_value->condition == CHARACTER_F
        || ast_value->condition == BOOLEAN_F) {
        return (Type *) ast_value->left;
    }
    if (ast_value->condition == READ_CHAR_F || ast_value->condition == READ_INT_F
        || ast_value->condition == READ_DOUBLE_F || ast_value->condition == READ_LINE_F) {
        return read_input_exp(ast_value);
    }
    // Variable without reference
    if (ast_value->condition == VARIABLE_F) {
        return read_variable(ast_value);
    }
    // Variable with reference
    // todo add vairable reference
    if (ast_value->condition == VARIABLE_REF_F) {
        return read_variable(ast_value);
    }
    if (ast_value->condition == PARENTHESES_F) {
        return read_exp((ast *) ast_value->left);
    }
    if (ast_value->condition == SINGLE_MINUS_F || ast_value->condition == SINGLE_PLUS_F) {
        // For single minus and single plus like -1
        Type *res = read_exp((ast *) ast_value->left);
        return single_operator(res, ast_value->condition);
    }
    Type *tmp_left = read_exp((ast *) ast_value->left);
    Type *tmp_right = read_exp((ast *) ast_value->right);
    // Check valid Types
    if (tmp_left->type == INVALID) {
        return tmp_left;
    }
    if (tmp_right->type == INVALID) {
        return tmp_right;
    }
    Type *res = number_operators(tmp_left, tmp_right, ast_value->condition);
    return res;
}

Type *read_input_exp(ast *ast_value) {
    switch (ast_value->condition) {
        case READ_CHAR_F:
            return read_char();
        case READ_INT_F:
            return read_int();
        case READ_DOUBLE_F:
            return read_double();
        case READ_LINE_F:
            break;
        case READ_WORD_F:
            break;
    }
}