#include "y.tab.h"
#include "y.tab.c"
#include <fstream>

int get_line_number() {
    return yyget_lineno();
}

void set_line_number(int n) {
    yyset_lineno(n);
}

int main(int argc, char **argv) {
    set_line_number(0);
    if (argc > 1) {
        RUN_MODE = 1;
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            printf("file not found\n");
        }
        // system("clear");
        yyparse();
    } else {
        system("clear");
        yyparse();
    }
}