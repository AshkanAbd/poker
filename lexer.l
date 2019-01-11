%{

char *get_var_name(const char *name) {
    size_t len = strlen(name);
    char *res = (char *) malloc(len * sizeof(char));
    memcpy(res, name + 1, (len - 1) * sizeof(char));
    return res;
}

%}

%option noyywrap

/* lexical analyzer */

%%
[ \n\t]                                     { }                                     /* do nothing */
";"                                         { return (EOL); }                       /* End of line token */
var                                         { return (VAR); }                       /* Variable token */
typeof                                      { return (TYPEOF); }                    /* Typeof() token */
print                                       { return (PRINT); }                     /* Print token */
del                                         { return (DEL); }                       /* Del token */
read_char                                   { return (READ_CHAR); }                 /* Get input as char token */
read_int                                    { return (READ_INT); }                  /* Get input as int token */
read_double                                 { return (READ_DOUBLE); }               /* Get input as double token */
read                                        { return (READ); }                      /* Get input word token
read_line                                   { return (READ_LINE); }                 /* Get input line token */
true                                        { yylval.bool_value = 1;
                                              return (BOOLEAN); }                   /* True token */
false                                       { yylval.bool_value = 0;
                                              return (BOOLEAN); }                   /* False token */
if                                          { return (IF); }                        /* If token */
else                                        { return (ELSE); }                      /* Else token */
else" "if                                   { return (ELSE_IF); }                   /* Else if token */
or                                          { return (OR); }                        /* OR token */
and                                         { return (AND); }                       /* AND token */
for                                         { return (FOR); }                       /* FOR token */
while                                       { return (WHILE); }                     /* WHILE token */
break                                       { return (BREAK); }
continue                                    { return (CONTINUE); }
("\'")(.+)("\'")                            { yylval.char_value = yytext[1];
                                              return (CHAR); }                      /* Character token */
[0-9]+                                      { yylval.int_value = atoi(yytext);
                                              return (INT); }                       /* Integer token */
([0-9]+)(".")([0-9]+)                       { yylval.double_value = atof(yytext);
                                              return (DOUBLE); }                    /* Double token */
[a-zA-Z_][a-zA-Z0-9_]*                      { yylval.str_value = strdup(yytext);
                                              return (VAR_NAME); }                  /* Variable name token */
"&"([a-zA-Z_][a-zA-Z0-9_]*)                 { yylval.str_value = get_var_name(yytext);
                                              return (VAR_NAME_REF); }              /* Variable name with reference token */
"=="                                        { return (COMPARE_EQU); }
"!="                                        { return (COMPARE_NOT_EQU); }
">="                                        { return (COMPARE_BIGGER_EQU); }
"<="                                        { return (COMPARE_LITTLE_EQU); }
">"                                         { return (COMPARE_BIGGER); }
"<"                                         { return (COMPARE_LITTLE); }
"!"                                         { return (yytext[0]); }
"+"                                         { return (yytext[0]); }
"*"                                         { return (yytext[0]); }
"-"                                         { return (yytext[0]); }
"/"                                         { return (yytext[0]); }
"%"                                         { return (yytext[0]); }
"="                                         { return (yytext[0]); }
"^"                                         { return (yytext[0]); }
"("                                         { return (yytext[0]); }
")"                                         { return (yytext[0]); }
"{"                                         { return (yytext[0]); }
"}"                                         { return (yytext[0]); }
.                                           { return (UNKNOWN_INPUT); }                   /* Im=nvalid input token */
%%
