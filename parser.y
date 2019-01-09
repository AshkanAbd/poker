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

%token EOL PRINT VAR TYPEOF DEL
%token IF ELSE ELSE_IF
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
    PRINT '(' compare ')'                               {   $<ast_value>$ = new ast($<ast_value>3, sizeof(*$<ast_value>3));
                                                            $<ast_value>$->set_condition(PRINT_F);   }
    | vars                                              {   /* Do nothing vars block will handle it */  }
    | TYPEOF '(' compare ')'                            {   $<ast_value>$ = new ast($<ast_value>3, sizeof(*$<ast_value>3));
                                                            $<ast_value>$->set_condition(TYPEOF_F); }
    | DEL VAR_NAME                                      {   $<ast_value>$ = new ast($<ast_value>2, sizeof(*$<ast_value>2));
                                                            $<ast_value>$->set_condition(DEL_VAR_F);    }
    | if_statement                                          /* Nothing to do if_statement block will handle it */
    ;

/* IF statement grammar */

if_statement :
    IF '(' compare ')' else_if_statement
    | IF '(' compare ')' '{' program '}'
    else_if_statement ELSE '{' program '}'
    ;

else_if_statement :
    %empty
    | ELSE_IF '(' compare ')' '{' program '}'
    else_if_statement
    ;

/* READ input grammar */
read_input :
    READ_CHAR '(' ')'                                   {   $<ast_value>$ = new ast(READ_CHAR_F);    }
    | READ_INT '(' ')'                                  {   $<ast_value>$ = new ast(READ_INT_F);    }
    | READ_DOUBLE '(' ')'                               {   $<ast_value>$ = new ast(READ_DOUBLE_F); }
    | READ_LINE '(' ')'                                 {   $<ast_value>$ = new ast(READ_LINE_F);   }
    | READ '(' ')'                                      {   $<ast_value>$ = new ast(READ_WORD_F);   }
    ;

/* Variable grammar */
vars :
    VAR VAR_NAME '=' compare                            {   $<ast_value>$ = new ast($<str_value>2, sizeof($<str_value>2));
                                                            $<ast_value>$->set_right($<ast_value>4, sizeof(*$<ast_value>4));
                                                            $<ast_value>$->set_condition(EQU_NEW_VAR_F);    }
    | VAR_NAME '=' compare                              {   $<ast_value>$ = new ast($<str_value>1, sizeof($<str_value>1));
                                                            $<ast_value>$->set_right($<ast_value>3, sizeof(*$<ast_value>3));
                                                            $<ast_value>$->set_condition(EQU_EXIST_VAR_F);    }
    ;

/* compare grammar */
compare:
    compare COMPARE_EQU compare                         {   $<ast_value>$ = new ast();
                                                            $<ast_value>$->set_left($<ast_value>1, sizeof(*$<ast_value>1));
                                                            $<ast_value>$->set_right($<ast_value>3, sizeof(*$<ast_value>3));
                                                            $<ast_value>$->set_condition(COMPARE_EQU_F);    }
    | compare COMPARE_NOT_EQU compare                   {   $<ast_value>$ = new ast();
                                                            $<ast_value>$->set_left($<ast_value>1, sizeof(*$<ast_value>1));
                                                            $<ast_value>$->set_right($<ast_value>3, sizeof(*$<ast_value>3));
                                                            $<ast_value>$->set_condition(COMPARE_NOT_EQU_F);    }
    | compare COMPARE_BIGGER_EQU compare                {   $<ast_value>$ = new ast();
                                                            $<ast_value>$->set_left($<ast_value>1, sizeof(*$<ast_value>1));
                                                            $<ast_value>$->set_right($<ast_value>3, sizeof(*$<ast_value>3));
                                                            $<ast_value>$->set_condition(COMPARE_BIGGER_EQU_F);    }
    | compare COMPARE_LITTLE_EQU compare                {   $<ast_value>$ = new ast();
                                                            $<ast_value>$->set_left($<ast_value>1, sizeof(*$<ast_value>1));
                                                            $<ast_value>$->set_right($<ast_value>3, sizeof(*$<ast_value>3));
                                                            $<ast_value>$->set_condition(COMPARE_LITTLE_EQU_F);    }
    | compare COMPARE_BIGGER compare                    {   $<ast_value>$ = new ast();
                                                            $<ast_value>$->set_left($<ast_value>1, sizeof(*$<ast_value>1));
                                                            $<ast_value>$->set_right($<ast_value>3, sizeof(*$<ast_value>3));
                                                            $<ast_value>$->set_condition(COMPARE_BIGGER_F);    }
    | compare COMPARE_LITTLE compare                    {   $<ast_value>$ = new ast();
                                                            $<ast_value>$->set_left($<ast_value>1, sizeof(*$<ast_value>1));
                                                            $<ast_value>$->set_right($<ast_value>3, sizeof(*$<ast_value>3));
                                                            $<ast_value>$->set_condition(COMPARE_LITTLE_F);    }
    | compare OR compare                                {   $<ast_value>$ = new ast();
                                                            $<ast_value>$->set_left($<ast_value>1, sizeof(*$<ast_value>1));
                                                            $<ast_value>$->set_right($<ast_value>3, sizeof(*$<ast_value>3));
                                                            $<ast_value>$->set_condition(OR_F);    }
    | compare AND compare                               {   $<ast_value>$ = new ast();
                                                            $<ast_value>$->set_left($<ast_value>1, sizeof(*$<ast_value>1));
                                                            $<ast_value>$->set_right($<ast_value>3, sizeof(*$<ast_value>3));
                                                            $<ast_value>$->set_condition(AND_F);    }
    | '(' compare ')'                                   {   $<ast_value>$ = new ast($<ast_value>2, sizeof(*$<ast_value>2));
                                                            $<ast_value>$->set_condition(PARENTHESES_F);    }
    | '!' compare                                       {   $<ast_value>$ = new ast($<ast_value>2, sizeof(*$<ast_value>2));
                                                            $<ast_value>$->set_condition(NOT_F);    }
    | exp
    ;

exp :
    INT                                                 {   $<ast_value>$ = create_int_ast($<int_value>1);    }
    | DOUBLE                                            {   $<ast_value>$ = create_double_ast($<double_value>1);    }
    | CHAR                                              {   $<ast_value>$ = create_char_ast($<char_value>1);    }
    | BOOLEAN                                           {   Type *type = create_bool($<bool_value>1);
                                                            $<ast_value>$ = new ast(type, sizeof(*type));
                                                            $<ast_value>$->set_condition(BOOLEAN_F);    }
    | read_input
    | VAR_NAME                                          {   $<ast_value>$ = new ast($<str_value>1, sizeof($<str_value>1));
                                                            $<ast_value>$->set_condition(VARIABLE_F);   }
    | VAR_NAME_REF                                      {   $<ast_value>$ = new ast($<str_value>1, sizeof($<str_value>1));
                                                            $<ast_value>$->set_condition(VARIABLE_REF_F);   }
    | exp '+' exp                                       {   $<ast_value>$ = new ast($<ast_value>1, sizeof(*$<ast_value>1));
                                                            $<ast_value>$->set_right($<ast_value>3, sizeof(*$<ast_value>3));
                                                            $<ast_value>$->set_condition(PLUS_F);   }
    | exp '-' exp                                       {   $<ast_value>$ = new ast($<ast_value>1, sizeof(*$<ast_value>1));
                                                            $<ast_value>$->set_right($<ast_value>3, sizeof(*$<ast_value>3));
                                                            $<ast_value>$->set_condition(MINUS_F);   }
    | exp '/' exp                                       {   $<ast_value>$ = new ast($<ast_value>1, sizeof(*$<ast_value>1));
                                                            $<ast_value>$->set_right($<ast_value>3, sizeof(*$<ast_value>3));
                                                            $<ast_value>$->set_condition(DIVIDE_F);   }
    | exp '*' exp                                       {   $<ast_value>$ = new ast($<ast_value>1, sizeof(*$<ast_value>1));
                                                            $<ast_value>$->set_right($<ast_value>3, sizeof(*$<ast_value>3));
                                                            $<ast_value>$->set_condition(MULTIPLY_F);   }
    | exp '^' exp                                       {   $<ast_value>$ = new ast($<ast_value>1, sizeof(*$<ast_value>1));
                                                            $<ast_value>$->set_right($<ast_value>3, sizeof(*$<ast_value>3));
                                                            $<ast_value>$->set_condition(POW_F);   }
    | exp '%' exp                                       {   $<ast_value>$ = new ast($<ast_value>1, sizeof(*$<ast_value>1));
                                                            $<ast_value>$->set_right($<ast_value>3, sizeof(*$<ast_value>3));
                                                            $<ast_value>$->set_condition(LEFT_OVER_F);   }
    | '-' exp                                           {   $<ast_value>$ = new ast($<ast_value>2, sizeof(*$<ast_value>2));
                                                            $<ast_value>$->set_condition(SINGLE_MINUS_F);   }
    | '+' exp                                           {   $<ast_value>$ = new ast($<ast_value>2, sizeof(*$<ast_value>2));
                                                            $<ast_value>$->set_condition(SINGLE_PLUS_F);   }
    ;
