#!/usr/bin/env bash

flex lexer.l && bison -y -d parser.y && g++ -o poker main.cpp -lm
