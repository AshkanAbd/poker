/*
 * important function of poker language
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cstring>
#include "variables.cpp"
#include "../ast/ast.cpp"

static Variable variables;

void read_ast(ast *ast_value);

Type *read_exp(ast *ast_value);

Type *read_input_exp(ast *ast_value);

int is_compare(ast *ast_value);

void if_statement_ast(ast_if *ast_value);

void while_statement_ast(ast_while *ast_value);

void read_statement_ast(ast* ast_value);

void print(const char *data) {
    fprintf(stdout, "$> %s\n", data);
}

void print(ast *ast_value) {
    // Read exp
    Type *type = read_exp(ast_value);
    if (type->type == INVALID)
        return;
    if (type->type == DOUBLE_TYPE) {
        print_double(type);
    } else if (type->type == INTEGER_TYPE) {
        print_int(type);
    } else if (type->type == CHAR_TYPE) {
        print_char(type);
    } else if (type->type == BOOLEAN_TYPE) {
        print_boolean(type);
    } else {
        // For unknown type
        print_unknown(type);
    }
}

void print_type(Type *type) {
    switch (type->type) {
        case DOUBLE_TYPE:
            print("Double");
            break;
        case INTEGER_TYPE:
            print("Integer");
            break;
        case BOOLEAN_TYPE:
            print("Boolean");
            break;
        case CHAR_TYPE:
            print("Character");
            break;
        case UNKNOWN:
            print("UNKNOWN");
    }
}

int divide_on_zero(Type *type) {
    if (type->type == DOUBLE_TYPE) {
        double d = *(double *) type->value;
        if (d == 0) {
            return 1;
        }
    } else if (type->type == INTEGER_TYPE) {
        long int i = *(long int *) type->value;
        if (i == 0) {
            return 1;
        }
    }
    return 0;
}

void add_new_variable(ast *ast_value) {
    // Create VariableName and find index of it in variables
    VariableName *variableName = create_variable_name((char *) ast_value->left);
    int var_index = variables.index(variableName);
    if (var_index != -1) {
        // if variable exist throw exception
        yyerror(strcat((char *) ast_value->left, " : Variable was defined"));
        return;
    }
    // else create variable
    Type *type = read_exp((ast *) ast_value->right);
    if (type->type != INVALID) {
        variables.add(variableName, type);
    }
}

void change_variable(ast *ast_value) {
    // Create VariableName and find index of it in variables
    VariableName *variableName = create_variable_name((char *) ast_value->left);
    int var_index = variables.index(variableName);
    if (var_index == -1) {
        // if variable not exist throw exception
        yyerror(strcat((char *) ast_value->left, " : Undefined variable"));
        return;
    }
    // if variable exist update it
    Type *type = read_exp((ast *) ast_value->right);
    if (type->type != INVALID) {
        *((variables.values) + var_index) = *type;
    }
}

// Delete a variable from variables
void delete_variable(ast *ast_value) {
    VariableName *variableName = create_variable_name((char *) ast_value->left);
    if (variables.index(variableName) == -1) {
        yyerror(strcat((char *) ast_value->left, " : Undefined variable"));
        return;
    }
    variables.delete_var(variableName);
}

// Read variable from gives ast
Type *read_variable(ast *ast_value) {
    Type *type;
    // Create VariableName and find index of it in variables
    VariableName *var_name = create_variable_name((char *) ast_value->left);
    int var_index = variables.index(var_name);
    if (var_index == -1) {
        // VariableName not in variables
        yyerror(strcat((char *) ast_value->left, " : Undefined variable"));
        type = new Type;
        type->set_type(INVALID);
    } else {
        // VariableName in variables
        type = variables.values + var_index;
    }
    return type;
}

// Read single character from input
Type *read_char() {
    char *c = (char *) malloc(1 * sizeof(char));
    fprintf(stdout, "--> ");
    fscanf(stdin, "%s", c);
    char c2 = *c;
    return new Type(&c2, CHAR_TYPE);
}

// Read int from input
Type *read_int() {
    long int i;
    fprintf(stdout, "--> ");
    fscanf(stdin, "%ld", &i);
    return new Type(&i, INTEGER_TYPE);
}

// Read double from input
Type *read_double() {
    double d;
    fprintf(stdout, "--> ");
    fscanf(stdin, "%lf", &d);
    return new Type(&d, DOUBLE_TYPE);
}

// todo complete these function after array support
Type *read_word() {

}

Type *read_line() {

}

int is_compare(ast *ast_value) {
    if (ast_value->condition == COMPARE_EQU_F || ast_value->condition == COMPARE_NOT_EQU_F
        || ast_value->condition == COMPARE_LITTLE_EQU_F || ast_value->condition == COMPARE_BIGGER_EQU_F
        || ast_value->condition == COMPARE_BIGGER_F || ast_value->condition == COMPARE_LITTLE_F) {
        return 1;
    }
    return 0;
}

// logical not ("!") for boolean type only
Type *not_function(Type *type) {
    {
        if (invalid_type(type))
            return type;
    }
    if (type->type != BOOLEAN_TYPE) {
        return new Type(INVALID);
    }
    bool b = !(*(bool *) type->value);
    return new Type(&b, BOOLEAN_TYPE);
}

// Single operator for types
Type *single_operator(Type *type1, int flag) {
    {
        if (invalid_type(type1))
            return type1;
    }
    if (flag == SINGLE_PLUS_F) {
        return type1;
    }
    if (type1->type == DOUBLE_TYPE) {
        double d = (*(double *) type1->value);
        d = d * -1;
        type1->set_value(&d);
        return type1;
    } else if (type1->type == INTEGER_TYPE) {
        long int i = (*(long long int *) type1->value);
        i = i * -1;
        type1->set_value(&i);
        return type1;
    }
    yyerror("Invalid operation");
    return new Type(INVALID);
}

// For change type without reference
Type *number_operators(Type *type1, Type *type2, int flag) {
    {
        if (invalid_type(type1))
            return type1;
        if (invalid_type(type2))
            return type2;
    }
    switch (flag) {
        case PLUS_F:
            if (type1->type == DOUBLE_TYPE && type2->type == DOUBLE_TYPE) {
                double d = (*(double *) type1->value) + (*(double *) type2->value);
                return new Type(&d, DOUBLE_TYPE);
            } else if (type1->type == INTEGER_TYPE && type2->type == INTEGER_TYPE) {
                long int d = (*(long int *) type1->value) + (*(long int *) type2->value);
                return new Type(&d, INTEGER_TYPE);
            } else if (type1->type == INTEGER_TYPE && type2->type == DOUBLE_TYPE) {
                double d = (*(long int *) type1->value) + (*(double *) type2->value);
                return new Type(&d, DOUBLE_TYPE);
            } else if (type1->type == DOUBLE_TYPE && type2->type == INTEGER_TYPE) {
                double d = (*(double *) type1->value) + (*(long int *) type2->value);
                return new Type(&d, DOUBLE_TYPE);
            }
            break;
        case MINUS_F:
            if (type1->type == DOUBLE_TYPE && type2->type == DOUBLE_TYPE) {
                double d = (*(double *) type1->value) - (*(double *) type2->value);
                return new Type(&d, DOUBLE_TYPE);
            } else if (type1->type == INTEGER_TYPE && type2->type == INTEGER_TYPE) {
                long int d = (*(long int *) type1->value) - (*(long int *) type2->value);
                return new Type(&d, INTEGER_TYPE);
            } else if (type1->type == INTEGER_TYPE && type2->type == DOUBLE_TYPE) {
                double d = (*(long int *) type1->value) - (*(double *) type2->value);
                return new Type(&d, DOUBLE_TYPE);
            } else if (type1->type == DOUBLE_TYPE && type2->type == INTEGER_TYPE) {
                double d = (*(double *) type1->value) - (*(long int *) type2->value);
                return new Type(&d, DOUBLE_TYPE);
            }
            break;
        case MULTIPLY_F:
            if (type1->type == DOUBLE_TYPE && type2->type == DOUBLE_TYPE) {
                double d = (*(double *) type1->value) * (*(double *) type2->value);
                return new Type(&d, DOUBLE_TYPE);
            } else if (type1->type == INTEGER_TYPE && type2->type == INTEGER_TYPE) {
                long int d = (*(long int *) type1->value) * (*(long int *) type2->value);
                return new Type(&d, INTEGER_TYPE);
            } else if (type1->type == INTEGER_TYPE && type2->type == DOUBLE_TYPE) {
                double d = (*(long int *) type1->value) * (*(double *) type2->value);
                return new Type(&d, DOUBLE_TYPE);
            } else if (type1->type == DOUBLE_TYPE && type2->type == INTEGER_TYPE) {
                double d = (*(double *) type1->value) * (*(long int *) type2->value);
                return new Type(&d, DOUBLE_TYPE);
            }
            break;
        case DIVIDE_F:
            if (divide_on_zero(type2)) {
                yyerror("Divide on zero");
                return new Type(INVALID);
            } else {
                if (type1->type == DOUBLE_TYPE && type2->type == DOUBLE_TYPE) {
                    double d = (*(double *) type1->value) / (*(double *) type2->value);
                    return new Type(&d, DOUBLE_TYPE);
                } else if (type1->type == INTEGER_TYPE && type2->type == INTEGER_TYPE) {
                    long int d = (*(long int *) type1->value) / (*(long int *) type2->value);
                    return new Type(&d, INTEGER_TYPE);
                } else if (type1->type == INTEGER_TYPE && type2->type == DOUBLE_TYPE) {
                    double d = (*(long int *) type1->value) / (*(double *) type2->value);
                    return new Type(&d, DOUBLE_TYPE);
                } else if (type1->type == DOUBLE_TYPE && type2->type == INTEGER_TYPE) {
                    double d = (*(double *) type1->value) / (*(long int *) type2->value);
                    return new Type(&d, DOUBLE_TYPE);
                }
                break;
            }
        case LEFT_OVER_F:
            if (type1->type == INTEGER_TYPE && type2->type == INTEGER_TYPE) {
                long int d = (*(long int *) type1->value) % (*(long int *) type2->value);
                return new Type(&d, INTEGER_TYPE);
            } else {
                yyerror("\"%\" operator is only for Integers");
                return new Type(INVALID);
            }
        case POW_F:
            if (type1->type == DOUBLE_TYPE && type2->type == DOUBLE_TYPE) {
                double d = pow((*(double *) type1->value), (*(double *) type2->value));
                return new Type(&d, DOUBLE_TYPE);
            } else if (type1->type == INTEGER_TYPE && type2->type == INTEGER_TYPE) {
                long int d = static_cast<long int>(pow((*(long int *) type1->value), (*(long int *) type2->value)));
                return new Type(&d, INTEGER_TYPE);
            } else if (type1->type == INTEGER_TYPE && type2->type == DOUBLE_TYPE) {
                double d = pow((*(long int *) type1->value), (*(double *) type2->value));
                return new Type(&d, DOUBLE_TYPE);
            } else if (type1->type == DOUBLE_TYPE && type2->type == INTEGER_TYPE) {
                double d = pow((*(double *) type1->value), (*(long int *) type2->value));
                return new Type(&d, DOUBLE_TYPE);
            }
            break;
    }
    yyerror("Invalid operation");
    return new Type(INVALID);
}

// Change type with reference
Type *number_operators_with_reference(Type *type1, Type *type2, int flag, Type *ref) {
    {
        if (invalid_type(type1))
            return type1;
        if (invalid_type(type2))
            return type2;
    }
    switch (flag) {
        case PLUS_F:
            if (type1->type == DOUBLE_TYPE && type2->type == DOUBLE_TYPE) {
                double d = (*(double *) type1->value) + (*(double *) type2->value);
                ref->set_value(&d);
                return ref;
            } else if (type1->type == INTEGER_TYPE && type2->type == INTEGER_TYPE) {
                long int d = (*(long int *) type1->value) + (*(long int *) type2->value);
                ref->set_value(&d);
                return ref;
            } else if (type1->type == INTEGER_TYPE && type2->type == DOUBLE_TYPE) {
                double d = (*(long int *) type1->value) + (*(double *) type2->value);
                type1->set_value(&d);
                return ref;
            } else {
                double d = (*(double *) type1->value) + (*(long int *) type2->value);
                ref->set_value(&d);
                return ref;
            }
        case MINUS_F:
            if (type1->type == DOUBLE_TYPE && type2->type == DOUBLE_TYPE) {
                double d = (*(double *) type1->value) - (*(double *) type2->value);
                ref->set_value(&d);
                return ref;
            } else if (type1->type == INTEGER_TYPE && type2->type == INTEGER_TYPE) {
                long int d = (*(long int *) type1->value) - (*(long int *) type2->value);
                ref->set_value(&d);
                return ref;
            } else if (type1->type == INTEGER_TYPE && type2->type == DOUBLE_TYPE) {
                double d = (*(long int *) type1->value) - (*(double *) type2->value);
                ref->set_value(&d);
                return ref;
            } else {
                double d = (*(double *) type1->value) - (*(long int *) type2->value);
                ref->set_value(&d);
                return ref;
            }
        case MULTIPLY_F:
            if (type1->type == DOUBLE_TYPE && type2->type == DOUBLE_TYPE) {
                double d = (*(double *) type1->value) * (*(double *) type2->value);
                ref->set_value(&d);
                return ref;
            } else if (type1->type == INTEGER_TYPE && type2->type == INTEGER_TYPE) {
                long int d = (*(long int *) type1->value) * (*(long int *) type2->value);
                ref->set_value(&d);
                return ref;
            } else if (type1->type == INTEGER_TYPE && type2->type == DOUBLE_TYPE) {
                double d = (*(long int *) type1->value) * (*(double *) type2->value);
                ref->set_value(&d);
                return ref;
            } else {
                double d = (*(double *) type1->value) * (*(long int *) type2->value);
                ref->set_value(&d);
                return ref;
            }
        case DIVIDE_F:
            if (divide_on_zero(type2)) {
                yyerror("Divide on zero");
                return new Type(INVALID);
            } else {
                if (type1->type == DOUBLE_TYPE && type2->type == DOUBLE_TYPE) {
                    double d = (*(double *) type1->value) / (*(double *) type2->value);
                    ref->set_value(&d);
                    return ref;
                } else if (type1->type == INTEGER_TYPE && type2->type == INTEGER_TYPE) {
                    long int d = (*(long int *) type1->value) / (*(long int *) type2->value);
                    ref->set_value(&d);
                    return ref;
                } else if (type1->type == INTEGER_TYPE && type2->type == DOUBLE_TYPE) {
                    double d = (*(long int *) type1->value) / (*(double *) type2->value);
                    ref->set_value(&d);
                    return ref;
                } else {
                    double d = (*(double *) type1->value) / (*(long int *) type2->value);
                    ref->set_value(&d);
                    return ref;
                }
            }
        case LEFT_OVER_F:
            if (type1->type == INTEGER_TYPE && type2->type == INTEGER_TYPE) {
                long int d = (*(long int *) type1->value) % (*(long int *) type2->value);
                ref->set_value(&d);
                return ref;
            } else {
                yyerror("\"%\" operator is only for Integers");
                Type *t = new Type();
                t->set_type(INVALID);
                return t;
            }
        case POW_F:
            if (type1->type == DOUBLE_TYPE && type2->type == DOUBLE_TYPE) {
                double d = pow((*(double *) type1->value), (*(double *) type2->value));
                ref->set_value(&d);
                return ref;
            } else if (type1->type == INTEGER_TYPE && type2->type == INTEGER_TYPE) {
                long int d = static_cast<long int>(pow((*(long int *) type1->value), (*(long int *) type2->value)));
                ref->set_value(&d);
                return ref;
            } else if (type1->type == INTEGER_TYPE && type2->type == DOUBLE_TYPE) {
                double d = pow((*(long int *) type1->value), (*(double *) type2->value));
                ref->set_value(&d);
                return ref;
            } else {
                double d = pow((*(double *) type1->value), (*(long int *) type2->value));
                ref->set_value(&d);
                return ref;
            }
    }
}

// Compare two Types
// Only integer and double is supported
// Only == and != is valid for boolean
Type *compare(Type *type1, Type *type2, int flag) {
    {
        if (invalid_type(type1))
            return type1;
        if (invalid_type(type2))
            return type2;
    }
    bool b = false;
    if (type1->type == BOOLEAN_TYPE && type2->type == BOOLEAN_TYPE) {
        if (flag == COMPARE_EQU_F) {
            b = (*(bool *) type1->value) == (*(bool *) type2->value);
        } else if (flag == COMPARE_NOT_EQU_F) {
            b = (*(bool *) type1->value) == (*(bool *) type2->value);
        }
        return new Type(&b, BOOLEAN_TYPE);
    }
    if (type1->type != DOUBLE_TYPE && type1->type != INTEGER_TYPE) {
        yyerror("Invalid type for this operation");
        return new Type(INVALID);
    }
    if (type2->type != DOUBLE_TYPE && type2->type != INTEGER_TYPE) {
        yyerror("Invalid type for this operation");
        return new Type(INVALID);
    }
    switch (flag) {
        case COMPARE_EQU_F:
            if (type1->type == DOUBLE_TYPE && type2->type == DOUBLE_TYPE) {
                b = (*(double *) type1->value) == (*(double *) type2->value);
            } else if (type1->type == DOUBLE_TYPE && type2->type == INTEGER_TYPE) {
                b = (*(double *) type1->value) == (*(long int *) type2->value);
            } else if (type1->type == INTEGER_TYPE && type2->type == DOUBLE_TYPE) {
                b = (*(long int *) type1->value) == (*(double *) type2->value);
            } else if (type1->type == INTEGER_TYPE && type2->type == INTEGER_TYPE) {
                b = (*(long int *) type1->value) == (*(long int *) type2->value);
            }
            break;
        case COMPARE_NOT_EQU_F:
            if (type1->type == DOUBLE_TYPE && type2->type == DOUBLE_TYPE) {
                b = (*(double *) type1->value) != (*(double *) type2->value);
            } else if (type1->type == DOUBLE_TYPE && type2->type == INTEGER_TYPE) {
                b = (*(double *) type1->value) != (*(long int *) type2->value);
            } else if (type1->type == INTEGER_TYPE && type2->type == DOUBLE_TYPE) {
                b = (*(long int *) type1->value) != (*(double *) type2->value);
            } else if (type1->type == INTEGER_TYPE && type2->type == INTEGER_TYPE) {
                b = (*(long int *) type1->value) != (*(long int *) type2->value);
            }
            break;
        case COMPARE_BIGGER_EQU_F:
            if (type1->type == DOUBLE_TYPE && type2->type == DOUBLE_TYPE) {
                b = (*(double *) type1->value) >= (*(double *) type2->value);
            } else if (type1->type == DOUBLE_TYPE && type2->type == INTEGER_TYPE) {
                b = (*(double *) type1->value) >= (*(long int *) type2->value);
            } else if (type1->type == INTEGER_TYPE && type2->type == DOUBLE_TYPE) {
                b = (*(long int *) type1->value) >= (*(double *) type2->value);
            } else if (type1->type == INTEGER_TYPE && type2->type == INTEGER_TYPE) {
                b = (*(long int *) type1->value) >= (*(long int *) type2->value);
            }
            break;
        case COMPARE_LITTLE_EQU_F:
            if (type1->type == DOUBLE_TYPE && type2->type == DOUBLE_TYPE) {
                b = (*(double *) type1->value) <= (*(double *) type2->value);
            } else if (type1->type == DOUBLE_TYPE && type2->type == INTEGER_TYPE) {
                b = (*(double *) type1->value) <= (*(long int *) type2->value);
            } else if (type1->type == INTEGER_TYPE && type2->type == DOUBLE_TYPE) {
                b = (*(long int *) type1->value) <= (*(double *) type2->value);
            } else if (type1->type == INTEGER_TYPE && type2->type == INTEGER_TYPE) {
                b = (*(long int *) type1->value) <= (*(long int *) type2->value);
            }
            break;
        case COMPARE_LITTLE_F:
            if (type1->type == DOUBLE_TYPE && type2->type == DOUBLE_TYPE) {
                b = (*(double *) type1->value) < (*(double *) type2->value);
            } else if (type1->type == DOUBLE_TYPE && type2->type == INTEGER_TYPE) {
                b = (*(double *) type1->value) < (*(long int *) type2->value);
            } else if (type1->type == INTEGER_TYPE && type2->type == DOUBLE_TYPE) {
                b = (*(long int *) type1->value) < (*(double *) type2->value);
            } else if (type1->type == INTEGER_TYPE && type2->type == INTEGER_TYPE) {
                b = (*(long int *) type1->value) < (*(long int *) type2->value);
            }
            break;
        case COMPARE_BIGGER_F:
            if (type1->type == DOUBLE_TYPE && type2->type == DOUBLE_TYPE) {
                b = (*(double *) type1->value) > (*(double *) type2->value);
            } else if (type1->type == DOUBLE_TYPE && type2->type == INTEGER_TYPE) {
                b = (*(double *) type1->value) > (*(long int *) type2->value);
            } else if (type1->type == INTEGER_TYPE && type2->type == DOUBLE_TYPE) {
                b = (*(long int *) type1->value) > (*(double *) type2->value);
            } else if (type1->type == INTEGER_TYPE && type2->type == INTEGER_TYPE) {
                b = (*(long int *) type1->value) > (*(long int *) type2->value);
            }
            break;
    }
    return new Type(&b, BOOLEAN_TYPE);
}

// and , or operators only for boolean type
Type *get_and_or_result(Type *type1, Type *type2, int flag) {
    {
        if (invalid_type(type1))
            return type1;
        if (invalid_type(type2))
            return type2;
    }
    if (type1->type != BOOLEAN_TYPE || type2->type != BOOLEAN_TYPE) {
        yyerror("Invalid operation");
        return new Type(INVALID);
    }
    bool b = false;
    if (flag == AND_F) {
        b = (*(bool *) type1->value) && (*(bool *) type2->value);
    } else {
        b = (*(bool *) type1->value) || (*(bool *) type2->value);
    }
    return new Type(&b, BOOLEAN_TYPE);
}

