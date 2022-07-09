#ifndef MIPS_COMPILER_H
#define MIPS_COMPILER_H

#define MAX_NUMBER_LENGTH 5

/* Enumeration of all possible symbols */
typedef enum {number, lparen, rparen, times, slash, plus, minus} SymbolCode;

/* Stores the value of a number or the symbol itself else */
typedef union {
    int value;
    char symbol;
} Lexeme;

/* A simple linked list to manage the tokens */
typedef struct TokenNode_ {
   SymbolCode symcode;
   Lexeme lexeme;
   struct TokenNode_ *next;
} TokenNode;

void compiler(char*, char*);

#endif /* MIPS_COMPILER_H */
