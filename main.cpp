#include "y.tab.h"
#include "y.tab.c"
#include <fstream>

int main(int argc, char **argv) {
    if (argc > 1) {
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