/*
 * This file saves language variable structure such as VariableName , Variables ,...
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cstring>
#include "types.cpp"

// Structure for each letter of variable name
struct NameNode {
    NameNode *next;
    int next_flag;
    char value;

    NameNode(char value) {
        NameNode::value = value;
        NameNode::next = (NameNode *) malloc(0);
        NameNode::next_flag = 0;
    }

    // Set next letter pointer
    void set_next(NameNode *next) {
        free_next();
        NameNode::next = (NameNode *) malloc(sizeof(*next));
        memcpy(NameNode::next, next, sizeof(*next));
        NameNode::next_flag = 1;
    }

    // Set this letter
    void set_value(char value) {
        NameNode::value = value;
    }

    NameNode() {
        NameNode::value = 0;
        NameNode::next_flag = 0;
        NameNode::next = (NameNode *) malloc(0);
    }

    void free_next() {
        free(next);
        next_flag = 0;
    }
};

// Structure for variable name
// Something like linked list
struct VariableName {
    NameNode *head;
    size_t size;

    VariableName() {
        VariableName::head = new NameNode;
        VariableName::size = 0;
    }

    // Add new letter to variable name
    void add(char c) {
        if (VariableName::size == 0) {
            VariableName::head->set_value(c);
        } else {
            NameNode *tmp = head;
            while (tmp->next_flag != 0) {
                tmp = tmp->next;
            }
            NameNode *node = new NameNode(c);
            tmp->set_next(node);
        }
        VariableName::size++;
    }

    // Equals current variable name with an other variable name
    int equals(VariableName *var) {
        if (var->size != size) {
            return 0;
        }
        NameNode *tmp1 = var->head, *tmp2 = head;
        while (true) {
            if (tmp1->value != tmp2->value) {
                return 0;
            }
            if (tmp1->next_flag == 0) {
                break;
            }
            tmp1 = tmp1->next;
            tmp2 = tmp2->next;
        }
        return 1;
    }

};

// Structure for saving variables
struct Variable {
    size_t size;
    VariableName *var_names;
    Type *values;
    int end;

    Variable() {
        Variable::size = 1000;
        var_names = (VariableName *) malloc(1000 * sizeof(VariableName));
        values = (Type *) malloc(1000 * sizeof(Type));
        end = 0;
    }

    // Add new variable to variables
    void add(VariableName *var_name, Type *type) {
        if (end == size) {
            var_names = (VariableName *) realloc(var_names, size + 1000);
            size = size + 1000;
        }
        *(var_names + end) = *var_name;
        *(values + end) = *type;
        end++;
    }

    // Index of some a variable name
    int index(VariableName *var_name) {
        int i = 0;
        for (; i < end; i++) {
            if ((*(var_names + i)).equals(var_name)) {
                return i;
            }
        }
        return -1;
    }

    // Get value of a variable name
    Type *get_var(VariableName *var_name) {
        int var_index = index(var_name);
        if (var_index == -1) {
            Type *tmp = new Type;
            tmp->set_type(INVALID);
            return tmp;
        }
        return values + var_index;
    }

    // Delete a variable name
    void delete_var(VariableName *var_name) {
        int var_index = index(var_name);
        if (var_index == -1)
            return;
        memmove(var_names + var_index, var_names + var_index + 1, (end - var_index) * sizeof(Type));
        memmove(values + var_index, values + var_index + 1, (end - var_index) * sizeof(VariableName));
        end--;
    }
};

// Structure for each node of ReferenceVariable
struct ReferenceNode {
    Type *value;
    ReferenceNode *next;
    int next_flag;

    ReferenceNode() {
        ReferenceNode::value = (Type *) malloc(0);
        ReferenceNode::next = (ReferenceNode *) malloc(0);
        ReferenceNode::next_flag = 0;
    }

    ReferenceNode(Type *value) {
        ReferenceNode::value = (Type *) malloc(sizeof(*value));
        memcpy(ReferenceNode::value, value, sizeof(*value));
        ReferenceNode::next = (ReferenceNode *) malloc(0);
        ReferenceNode::next_flag = 0;
    }

    void set_value(Type *value) {
        ReferenceNode::value = (Type *) malloc(sizeof(*value));
    }

    void set_next(ReferenceNode *next) {
        ReferenceNode::next = (ReferenceNode *) malloc(sizeof(*next));
        ReferenceNode::next_flag = 1;
    }
};

// Structure like linked list for save r=send by reference variables
struct ReferenceVariable {
    ReferenceNode *head;
    size_t size;

    ReferenceVariable() {
        head = (ReferenceNode *) malloc(sizeof(ReferenceNode));
        size = 0;
    }

    void add(Type *type) {
        if (size == 0) {
            head = new ReferenceNode(type);
        } else {
            ReferenceNode *tmp = head;
            while (tmp->next_flag == 1) {
                tmp = tmp->next;
            }
            ReferenceNode *node = new ReferenceNode(type);
            tmp->set_next(node);
        }
        size++;
    }

    void clear() {
        if (size == 0)
            return;
        ReferenceNode *tmp = head;
        while (tmp->next_flag == 1) {
            tmp->value->reference = 0;
            tmp = tmp->next;
        }
        tmp->value->reference = 0;
    }
};


// Create new VariableName Object from char pointer
VariableName *create_variable_name(char *name) {
    VariableName *var_name = new VariableName;
    int i = 0;
    for (; i < strlen(name); i++) {
        var_name->add(*(name + i));
    }
    return var_name;
}

