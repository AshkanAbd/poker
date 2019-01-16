/*
 * functions for reading AST that created by create_ast.cpp in parser.y
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cstring>
#include "../libs/functions.cpp"

// Return 1 for continue
// Return -1 for break
// For other Flags return 0
int read_ast(ast *ast_value) {
    set_line_number(get_line_number() + 1);
    if (ast_value == NULL)
        return 0;
    if (ast_value->condition == PROGRAM_F) {
        read_ast((ast *) ast_value->left);
    } else if (ast_value->condition == BREAK_F) {
        if (!ast_value->left_flag)
            return -1;
        Type *type = read_exp((ast *) ast_value->left);
        if (type->type != INTEGER_TYPE) {
            yyerror("INVALID BREAK");
            return -100000;
        }
        return -1 * (*(int *) type->value);
    } else if (ast_value->condition == CONTINUE_F) {
        return 1;
    } else if (dynamic_cast<ast_if *>(ast_value)) {
        return if_statement_ast(dynamic_cast<ast_if *>(ast_value));
    } else if (dynamic_cast<ast_while *>(ast_value)) {
        return while_statement_ast(dynamic_cast<ast_while *>(ast_value));
    } else if (dynamic_cast<ast_for *>(ast_value)) {
        return for_statement_ast(dynamic_cast<ast_for *>(ast_value));
    } else if (ast_value->condition == PRINT_F) {
        // Print Info saved in left of AST
        return print((ast *) ast_value->left);
    } else if (ast_value->condition == EQU_NEW_VAR_F) {
        // Add new variable
        return add_new_variable(ast_value);
    } else if (ast_value->condition == EQU_EXIST_VAR_F) {
        // Change exist variable
        return change_variable(ast_value);
    } else if (ast_value->condition == DEL_VAR_F) {
        // Delete variable
        return delete_variable(ast_value);
    } else if (ast_value->condition == TYPEOF_F) {
        // Print type of an exp
        Type *type = read_exp((ast *) ast_value->left);
        return print_type(type);
    }
    return 0;
}

int for_statement_ast(ast_for *ast_value) {
    ast *program = ast_value->program_flag ? (ast *) ast_value->program : NULL;
    ast *end = ast_value->end_condition ? (ast *) ast_value->end_condition : NULL;
    ast *start = ast_value->start_flag ? (ast *) ast_value->start : NULL;
    ast *step = ast_value->step_flag ? (ast *) ast_value->step : NULL;
    Type *end_result;
    int status;
    if (read_ast(start) < 0) {
        yyerror("Invalid for start");
        return -100000;
    }
    while (true) {
        if (ast_value->program_flag != 0) {
            status = read_loop_statement_ast(program);
            if (status < 0) {
                break;
            }
        }
        if (read_ast(step) < 0) {
            yyerror("Invalid for step");
            return -100000;
        }
        if (ast_value->end_flag != 0) {
            end_result = read_exp(end);
            if (end_result->type != BOOLEAN_TYPE) {
                yyerror("Invalid for condition");
                return -100000;
            }
            if (!(*(bool *) end_result->value)) {
                return 0;
            }
        }
    }
    return status + 1;
}

int while_statement_ast(ast_while *ast_value) {
    ast *program = ast_value->program_flag ? (ast *) ast_value->program : NULL;
    ast *condition = (ast *) ast_value->while_condition;
    Type *condition_result;
    int status;
    while (true) {
        condition_result = read_exp(condition);
        if (condition_result->type != BOOLEAN_TYPE) {
            yyerror("Invalid while condition");
            return -100000;
        }
        if (!(*(bool *) condition_result->value)) {
            return 0;
        }
        if (program != NULL) {
            status = read_loop_statement_ast(program);
            if (status < 0) {
                break;
            }
        }
    }
    return status + 1;
}

int if_statement_ast(ast_if *ast_value) {
    // Check AST IF is ELSE or NOT
    if (ast_value->condition == ELSE_F) {
        // If is ELSE then run and return
        return read_if_statement_ast((ast *) ast_value->if_program);
    }
    // Try to read if (else if) condition and get result
    Type *condition_result = read_exp((ast *) ast_value->if_condition);
    if (condition_result->type != BOOLEAN_TYPE) {
        yyerror("Invalid if condition");
        return -100000;
    }
    bool b = *(bool *) condition_result->value;
    if (b) {
        // If condition is true then run if (else if) program
        if (ast_value->if_program_flag) {
            return read_if_statement_ast((ast *) ast_value->if_program);
        }
    } else {
        // If condition is false then read else (else if) AST
        if (ast_value->else_if_flag) {
            return read_if_statement_ast((ast *) ast_value->else_if_program);
        }
    }
    return 0;
}

int read_if_statement_ast(ast *ast_value) {
    if (ast_value->right_flag) {
        int status = read_if_statement_ast((ast *) ast_value->right);
        if (status != 0) {
            return status;
        }
    }
    if (ast_value->left_flag) {
        return read_ast((ast *) ast_value->left);
    } else {
        return read_ast(ast_value);
    }
}

int read_loop_statement_ast(ast *ast_value) {
    if (ast_value->right_flag) {
        int status = read_loop_statement_ast((ast *) ast_value->right);
        if (status != 0) {
            return status;
        }
    }
    if (ast_value->left_flag) {
        return read_ast((ast *) ast_value->left);
    } else {
        return read_ast(ast_value);
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
        || ast_value->condition == BOOLEAN_F || ast_value->condition == NULL_F) {
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
    // todo add variable reference
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