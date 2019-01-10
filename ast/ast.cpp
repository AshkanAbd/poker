/*
 * main AST file and AST structure for AST and AST_IF and...
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cstring>

struct ast {
    void *left;
    int left_flag;
    void *right;
    int right_flag;
    int condition;
    int if_statement;
    int for_statement;
    int while_statement;

    ast(void *left, size_t left_size, void *right, size_t right_size, int condition) {
        ast::left = malloc(left_size);
        memcpy(ast::left, left, left_size);
        ast::right = malloc(right_size);
        memcpy(ast::right, right, right_size);
        ast::right_flag = 1;
        ast::condition = condition;
        ast::left_flag = 1;
        ast::if_statement = 0;
        ast::for_statement = 0;
        ast::while_statement = 0;
    }

    ast(void *left, size_t left_size) {
        ast::left = malloc(left_size);
        memcpy(ast::left, left, left_size);
        ast::left_flag = 1;
        ast::right = malloc(0);
        ast::right_flag = 0;
        ast::condition = -1;
        ast::if_statement = 0;
        ast::for_statement = 0;
        ast::while_statement = 0;
    }

    ast(void *left, size_t left_size, int condition) {
        ast::left = malloc(left_size);
        memcpy(ast::left, left, left_size);
        ast::left_flag = 1;
        ast::condition = condition;
        ast::right = malloc(0);
        ast::right_flag = 0;
        ast::if_statement = 0;
        ast::for_statement = 0;
        ast::while_statement = 0;
    }

    ast(int condition) {
        ast::condition = condition;
        ast::left = malloc(0);
        ast::left_flag = 0;
        ast::right = malloc(0);
        ast::right_flag = 0;
        ast::if_statement = 0;
        ast::for_statement = 0;
        ast::while_statement = 0;
    }

    ast() {
        ast::left = malloc(0);
        ast::left_flag = 0;
        ast::right = malloc(0);
        ast::right_flag = 0;
        ast::condition = -1;
        ast::if_statement = 0;
        ast::for_statement = 0;
        ast::while_statement = 0;
    }

    void set_left(void *left, size_t size) {
        ast::left = malloc(size);
        memcpy(ast::left, left, size);
        ast::left_flag = 1;
    }

    void set_right(void *right, size_t size) {
        ast::right = malloc(size);
        memcpy(ast::right, right, size);
        ast::right_flag = 1;
    }

    void set_condition(int condition) {
        ast::condition = condition;
    }

    void free_left() {
        free(ast::left);
        ast::left = malloc(0);
        ast::left_flag = 0;
    }

    void free_right() {
        free(ast::right);
        ast::right = malloc(0);
        ast::right_flag = 0;
    }

    virtual ~ast() {
    }
};

// AST structure for if statement
struct ast_if : ast {
    void *if_condition;
    void *if_program;
    int if_program_flag;
    void *else_if_program;
    int else_if_flag;

    ast_if() : ast() {
        ast::if_statement = 1;
        this->if_program_flag = 0;
        this->else_if_flag = 0;
        this->if_condition = malloc(0);
        this->if_program = malloc(0);
        this->else_if_program = malloc(0);
    }

    void set_if_condition(void *condition, size_t condition_size) {
        this->if_condition = malloc(condition_size);
        memcpy(this->if_condition, condition, condition_size);
    }

    void set_if_program(void *program, size_t program_size) {
        this->if_program = malloc(program_size);
        memcpy(this->if_program, program, program_size);
        this->if_program_flag = 1;
    }

    void set_else_if(void *else_if_program, size_t else_if_size) {
        this->else_if_program = malloc(else_if_size);
        memcpy(this->else_if_program, else_if_program, else_if_size);
        this->else_if_flag = 1;
    }
};

// AST structure for WHILE statement
struct ast_while : ast {
    void *program;
    int program_flag;
    void *while_condition;

    ast_while() : ast() {
        this->while_statement = 1;
        this->program = malloc(0);
        this->while_condition = malloc(0);
        this->program_flag = 0;
    };

    void set_while_program(void *program, size_t program_size) {
        this->program_flag = 1;
        this->program = malloc(program_size);
        memcpy(this->program, program, program_size);
    }

    void set_while_condition(void *condition, size_t condition_size) {
        this->while_condition = malloc(condition_size);
        mempcpy(this->while_condition, condition, condition_size);
    }
};
