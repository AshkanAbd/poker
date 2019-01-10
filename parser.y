%{
#include "lex.yy.c"
%}

/* require imports */

%code requires {
    #include "ast/create_ast.cpp"
}

/* types */

%union{
    ast* ast_value;
    char* str_value;
    long int int_value;
    double double_value;
    char char_value;
    bool bool_value;
}

%debug
%define parse.error verbose

/* tokens */

%token EOL PRINT VAR TYPEOF DEL UNKNOWN_INPUT
%token IF ELSE ELSE_IF WHILE FOR
%token AND OR
%token COMPARE_EQU COMPARE_NOT_EQU COMPARE_BIGGER_EQU COMPARE_LITTLE_EQU COMPARE_BIGGER COMPARE_LITTLE
%token READ_CHAR READ_DOUBLE READ_INT READ_LINE READ
%token<double_value> DOUBLE
%token<int_value> INT
%token<char_value> CHAR
%token<str_value> VAR_NAME VAR_NAME_REF
%token<bool_value> BOOLEAN

/* left and right */

%right '='
%left AND
%left OR
%left COMPARE_EQU COMPARE_NOT_EQU
%left COMPARE_BIGGER_EQU COMPARE_LITTLE_EQU COMPARE_BIGGER COMPARE_LITTLE
%left '+' '-'
%left '*' '/' '%'
%left '^'
%left '(' ')'
%right '!'

/* grammar */
%%

/* start */
program :
	program term EOL                                    { read_ast($<ast_value>2);  }
	| program EOL
	| %empty
	;

term :
    PRINT '(' compare ')'                               {   $<ast_value>$ = left_ast($<ast_value>3, PRINT_F);    }
    | TYPEOF '(' compare ')'                            {   $<ast_value>$ = left_ast($<ast_value>3, TYPEOF_F);   }
    | DEL VAR_NAME                                      {   $<ast_value>$ = left_ast1($<str_value>2, DEL_VAR_F); }
    | vars                                                  /* Do nothing vars block will handle it */
    | if_statement                                          /* Nothing to do if_statement block will handle it */
    | while_statement                                       /* Nothing to do */
    ;

/* just for if, for and while statement */
program1:
    program1 term1 EOL                                  {   $<ast_value>$ = full_ast($<ast_value>2, $<ast_value>1, PROGRAM_F);              }
    | program1 EOL
    | %empty                                            {   $<ast_value>$ = NULL;                                                           }

term1 :
    PRINT '(' compare ')'                               {   $<ast_value>$ = left_ast($<ast_value>3, PRINT_F);    }
    | TYPEOF '(' compare ')'                            {   $<ast_value>$ = left_ast($<ast_value>3, TYPEOF_F);   }
    | DEL VAR_NAME                                      {   $<ast_value>$ = left_ast1($<str_value>2, DEL_VAR_F); }
    | vars                                                  /* Do nothing vars block will handle it */
    | if_statement                                          /* Nothing to do if_statement block will handle it */
    | while_statement                                       /* Nothing to do */
    ;

/* WHILE statement grammar */
while_statement:
    WHILE '(' compare ')' '{' program1 '}'              {   $<ast_value>$ = while_ast($<ast_value>3, $<ast_value>6, WHILE_F);               }
    ;

/* IF statement grammar */
if_statement:
    IF '(' compare ')' '{' program1 '}'
    else_if_statement                                   {   $<ast_value>$ = if_ast($<ast_value>3, $<ast_value>6, $<ast_value>8, IF_F);      }
    ;

else_if_statement:
    %empty                                              {   $<ast_value>$ = NULL;                                                           }
    | ELSE '{' program1 '}'                             {   $<ast_value>$ = if_ast($<ast_value>3, ELSE_F);                                  }
    | ELSE_IF '(' compare ')' '{' program1 '}'
    else_if_statement                                   {   $<ast_value>$ = if_ast($<ast_value>3, $<ast_value>6, $<ast_value>8, ELSE_IF_F); }
    ;

/* READ input grammar */
read_input :
    READ_CHAR '(' ')'                                   {   $<ast_value>$ = flag_ast(READ_CHAR_F);   }
    | READ_INT '(' ')'                                  {   $<ast_value>$ = flag_ast(READ_INT_F);    }
    | READ_DOUBLE '(' ')'                               {   $<ast_value>$ = flag_ast(READ_DOUBLE_F); }
    | READ_LINE '(' ')'                                 {   $<ast_value>$ = flag_ast(READ_LINE_F);   }
    | READ '(' ')'                                      {   $<ast_value>$ = flag_ast(READ_WORD_F);   }
    ;

/* Variable grammar */
vars :
    VAR VAR_NAME '=' compare                            {   $<ast_value>$ = full_ast1($<str_value>2, $<ast_value>4, EQU_NEW_VAR_F);     }
    | VAR_NAME '=' compare                              {   $<ast_value>$ = full_ast1($<str_value>1, $<ast_value>3, EQU_EXIST_VAR_F);   }
    ;

/* compare grammar */
compare:
    compare COMPARE_EQU compare                         {   $<ast_value>$ = full_ast($<ast_value>1, $<ast_value>3, COMPARE_EQU_F);          }
    | compare COMPARE_NOT_EQU compare                   {   $<ast_value>$ = full_ast($<ast_value>1, $<ast_value>3, COMPARE_NOT_EQU_F);      }
    | compare COMPARE_BIGGER_EQU compare                {   $<ast_value>$ = full_ast($<ast_value>1, $<ast_value>3, COMPARE_BIGGER_EQU_F);   }
    | compare COMPARE_LITTLE_EQU compare                {   $<ast_value>$ = full_ast($<ast_value>1, $<ast_value>3, COMPARE_LITTLE_EQU_F);   }
    | compare COMPARE_BIGGER compare                    {   $<ast_value>$ = full_ast($<ast_value>1, $<ast_value>3, COMPARE_BIGGER_F);       }
    | compare COMPARE_LITTLE compare                    {   $<ast_value>$ = full_ast($<ast_value>1, $<ast_value>3, COMPARE_LITTLE_F);       }
    | compare OR compare                                {   $<ast_value>$ = full_ast($<ast_value>1, $<ast_value>3, OR_F);                   }
    | compare AND compare                               {   $<ast_value>$ = full_ast($<ast_value>1, $<ast_value>3, AND_F);                  }
    | '(' compare ')'                                   {   $<ast_value>$ = left_ast($<ast_value>2, PARENTHESES_F);                         }
    | '!' compare                                       {   $<ast_value>$ = left_ast($<ast_value>2, NOT_F);                                 }
    | exp
    ;

exp :
    INT                                                 {   $<ast_value>$ = int_ast($<int_value>1);                                 }
    | DOUBLE                                            {   $<ast_value>$ = double_ast($<double_value>1);                           }
    | CHAR                                              {   $<ast_value>$ = char_ast($<char_value>1);                               }
    | BOOLEAN                                           {   $<ast_value>$ = bool_ast($<bool_value>1);                               }
    | read_input
    | VAR_NAME                                          {   $<ast_value>$ = left_ast1($<str_value>1, VARIABLE_F);                   }
    | VAR_NAME_REF                                      {   $<ast_value>$ = left_ast1($<str_value>1, VARIABLE_REF_F);               }
    | exp '+' exp                                       {   $<ast_value>$ = full_ast($<ast_value>1, $<ast_value>3, PLUS_F);         }
    | exp '-' exp                                       {   $<ast_value>$ = full_ast($<ast_value>1, $<ast_value>3, MINUS_F);        }
    | exp '/' exp                                       {   $<ast_value>$ = full_ast($<ast_value>1, $<ast_value>3, DIVIDE_F);       }
    | exp '*' exp                                       {   $<ast_value>$ = full_ast($<ast_value>1, $<ast_value>3, MULTIPLY_F);     }
    | exp '^' exp                                       {   $<ast_value>$ = full_ast($<ast_value>1, $<ast_value>3, POW_F);          }
    | exp '%' exp                                       {   $<ast_value>$ = full_ast($<ast_value>1, $<ast_value>3, LEFT_OVER_F);    }
    | '-' exp                                           {   $<ast_value>$ = left_ast($<ast_value>2, SINGLE_MINUS_F);                }
    | '+' exp                                           {   $<ast_value>$ = left_ast($<ast_value>2, SINGLE_PLUS_F);                 }
    ;
