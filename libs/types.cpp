/*
 * main Type structure file where defines important Types and print or create them
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cstring>
#include "flags.cpp"

static int line_counter = 0;

void yyerror(const char *s) {
    fprintf(stdout, "$> [%d]: \"%s\"\n", line_counter, s);
}

// Structure for basic language Types
struct Type {
    void *value;
    int type;
    int reference;

    Type(void *value, int type) {
        Type::type = type;
        Type::value = malloc(sizeof(value));
        reference = 0;
        memcpy(Type::value, value, sizeof(value));
    }

    Type(int type) {
        Type::type = type;
        reference = 0;
        value = malloc(0);
    }

    Type() {
        type = -1;
        reference = 0;
        value = malloc(0);
    }

    void set_type(int type) {
        Type::type = type;
    }

    void set_value(void *value) {
        free_value();
        Type::value = malloc(sizeof(value));
        memcpy(Type::value, value, sizeof(value));
    }

    void free_value() {
        free(Type::value);
    }
};

int divide_on_zero(Type *type);


// Create double type
Type *create_double(double d) {
    return new Type(&d, DOUBLE_TYPE);
}

// Create int type
Type *create_int(long int i) {
    return new Type(&i, INTEGER_TYPE);
}

// Create char type
Type *create_char(char c) {
    return new Type(&c, CHAR_TYPE);
}

// Create boolean type
Type *create_bool(int i) {
    return new Type(&i, BOOLEAN_TYPE);
}

void print_double(Type *type) {
    double d = *(double *) type->value;
    fprintf(stdout, "$> %.12g\n", d);
}

void print_int(Type *type) {
    long int i = *(long long int *) type->value;
    fprintf(stdout, "$> %ld\n", i);
}

void print_char(Type *type) {
    char c = *(char *) type->value;
    fprintf(stdout, "$> %c\n", c);
}

void print_boolean(Type *type) {
    bool b = *(bool *) type->value;
    if (b) {
        fprintf(stdout, "$> true\n");
    } else {
        fprintf(stdout, "$> false\n");
    }
}

void print_unknown(Type *type) {
    fprintf(stdout, "$> %p\n", type->value);
}
