/*
 * Only FLAGS that uses by AST or Type
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cstring>
#include "math.h"

#define UNKNOWN                     -1
#define INVALID                     -2

// AST Flags
#define PLUS_F                      10000
#define MINUS_F                     10001
#define MULTIPLY_F                  10002
#define DIVIDE_F                    10003
#define POW_F                       10004
#define LEFT_OVER_F                 10005
#define PRINT_F                     10006
#define SINGLE_MINUS_F              10007
#define SINGLE_PLUS_F               10008
#define DOUBLE_F                    10009
#define INT_F                       10010
#define VARIABLE_F                  10011
#define EQU_NEW_VAR_F               10012
#define EQU_EXIST_VAR_F             10013
#define TYPEOF_F                    10014
#define DEL_VAR_F                   10015
#define VARIABLE_REF_F              10016
#define CHARACTER_F                 10017
#define READ_CHAR_F                 10018
#define READ_INT_F                  10019
#define READ_DOUBLE_F               10020
#define READ_LINE_F                 10021
#define READ_WORD_F                 10022
#define PARENTHESES_F               10023
#define BOOLEAN_F                   10024
#define COMPARE_EQU_F               10025
#define COMPARE_NOT_EQU_F           10026
#define COMPARE_BIGGER_EQU_F        10027
#define COMPARE_LITTLE_EQU_F        10028
#define COMPARE_BIGGER_F            10029
#define COMPARE_LITTLE_F            10030
#define AND_F                       10031
#define OR_F                        10032
#define NOT_F                       10033
#define IF_F                        10034
#define ELSE_IF_F                   10035
#define ELSE_F                      10036

// Types Flags
#define INTEGER_TYPE                1000000
#define DOUBLE_TYPE                 1000001
#define CHAR_TYPE                   1000002
#define BOOLEAN_TYPE                1000003
