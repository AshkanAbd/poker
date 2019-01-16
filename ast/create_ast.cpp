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

ast *null_ast() {
    Type *type = create_null();
    ast *ast_value = new ast(type, sizeof(*type), NULL_F);
    return ast_value;
}

ast *left_ast(ast *ast1, int flag) {
    return new ast(ast1, sizeof(*ast1), flag);
}

ast *left_ast1(char *str, int flag) {
    return new ast(str, sizeof(str), flag);
}

ast *full_ast(ast *ast1, ast *ast2, int flag) {
    ast *ast_value = new ast(flag);
    size_t ast1_size = sizeof(*ast1);
    if (dynamic_cast<ast_while *>(ast1)) {
        ast1_size = sizeof(*(dynamic_cast<ast_while *>(ast1)));
    } else if (dynamic_cast<ast_if *>(ast1)) {
        ast1_size = sizeof(*(dynamic_cast<ast_if *>(ast1)));
    } else if (dynamic_cast<ast_for *>(ast1)) {
        ast1_size = sizeof(*(dynamic_cast<ast_for *>(ast1)));
    }
    ast_value->set_left(ast1, ast1_size);
    if (ast2 != NULL) {
        ast_value->set_right(ast2, sizeof(*ast2));
    }
    return ast_value;
}

ast *full_ast1(char *str, ast *ast1, int flag) {
    return new ast(str, sizeof(str), ast1, sizeof(*ast1), flag);
}

ast *flag_ast(int flag) {
    return new ast(flag);
}

ast *if_ast(ast *program, int flag) {
    ast_if *ast_value = new ast_if;
    size_t program_size = sizeof(*program);
    // Set program size
    if (dynamic_cast<ast_while *>(program)) {
        program_size = sizeof(*(dynamic_cast<ast_while *>(program)));
    } else if (dynamic_cast<ast_if *>(program)) {
        program_size = sizeof(*(dynamic_cast<ast_if *>(program)));
    } else if (dynamic_cast<ast_for *>(program)) {
        program_size = sizeof(*(dynamic_cast<ast_for *>(program)));
    }
    // Set condition and program (if available)
    ast_value->set_condition(flag);
    if (program != NULL) {
        ast_value->set_if_program(program, program_size);
    }
    return ast_value;
}

ast *if_ast(ast *condition, ast *program, int flag) {
    ast_if *ast_value = new ast_if;
    size_t program_size = sizeof(*program);
    if (dynamic_cast<ast_while *>(program)) {
        program_size = sizeof(*(dynamic_cast<ast_while *>(program)));
    } else if (dynamic_cast<ast_if *>(program)) {
        program_size = sizeof(*(dynamic_cast<ast_if *>(program)));
    } else if (dynamic_cast<ast_for *>(program)) {
        program_size = sizeof(*(dynamic_cast<ast_for *>(program)));
    }
    ast_value->set_condition(flag);
    ast_value->set_if_condition(condition, sizeof(*condition));
    if (program != NULL) {
        ast_value->set_if_program(program, program_size);
    }
    return ast_value;
};

ast *if_ast(ast *condition, ast *program, ast *else_if_program, int flag) {
    ast_if *ast_value = new ast_if;
    size_t program_size = sizeof(*program);
    size_t else_if_size = sizeof(*else_if_program);
    if (dynamic_cast<ast_while *>(program)) {
        program_size = sizeof(*(dynamic_cast<ast_while *>(program)));
    } else if (dynamic_cast<ast_if *>(program)) {
        program_size = sizeof(*(dynamic_cast<ast_if *>(program)));
    } else if (dynamic_cast<ast_for *>(program)) {
        program_size = sizeof(*(dynamic_cast<ast_for *>(program)));
    }
    if (dynamic_cast<ast_if *>(else_if_program)) {
        else_if_size = sizeof(*(dynamic_cast<ast_if *>(else_if_program)));
    }
    ast_value->set_condition(flag);
    ast_value->set_if_condition(condition, sizeof(*condition));
    if (program != NULL) {
        ast_value->set_if_program(program, program_size);
    }
    if (else_if_program != NULL) {
        ast_value->set_else_if(else_if_program, else_if_size);
    }
    return ast_value;
}

ast *while_ast(ast *condition, ast *program, int flag) {
    ast_while *ast_value = new ast_while;
    ast_value->set_condition(flag);
    ast_value->set_while_condition(condition, sizeof(*condition));
    size_t program_size = sizeof(*program);
    if (dynamic_cast<ast_while *>(program)) {
        program_size = sizeof(*(dynamic_cast<ast_while *>(program)));
    } else if (dynamic_cast<ast_if *>(program)) {
        program_size = sizeof(*(dynamic_cast<ast_if *>(program)));
    } else if (dynamic_cast<ast_for *>(program)) {
        program_size = sizeof(*(dynamic_cast<ast_for *>(program)));
    }
    if (program != NULL) {
        ast_value->set_while_program(program, program_size);
    }
    return ast_value;
}

ast *for_ast(ast *start, ast *end, ast *step, ast *program, int flag) {
    ast_for *ast_value = new ast_for;
    ast_value->set_condition(flag);
    size_t program_size = sizeof(*program);
    if (dynamic_cast<ast_while *>(program)) {
        program_size = sizeof(*(dynamic_cast<ast_while *>(program)));
    } else if (dynamic_cast<ast_if *>(program)) {
        program_size = sizeof(*(dynamic_cast<ast_if *>(program)));
    } else if (dynamic_cast<ast_for *>(program)) {
        program_size = sizeof(*(dynamic_cast<ast_for *>(program)));
    }
    if (start != NULL) {
        ast_value->set_start(start, sizeof(*start));
    }
    if (end != NULL) {
        ast_value->set_end(end, sizeof(*end));
    }
    if (step != NULL) {
        ast_value->set_step(step, sizeof(*step));
    }
    if (program != NULL) {
        ast_value->set_program(program, program_size);
    }
    return ast_value;
}
