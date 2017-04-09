#!/bin/bash

bison -d svg.ypp
flex svg.lex
g++ -std=c++11 -Wall -c lex.yy.c -o obj/lex.yy.o -lfl
g++ -std=c++11 -Wall -c svg.tab.cpp -o obj/svg.tab.o
g++ -std=c++11 -Wall -o svg.out obj/lex.yy.o obj/svg.tab.o -lfl

