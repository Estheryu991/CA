#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "memory.h"
#include "mips.h"
#include "compiler.h"

/* The file we store the compiled code in */
FILE* file;
/* To save the expression in postfix notation */
char* postfix;

/* tokens points to the beginning of the token list, currentToken to the token that is currently used */
TokenNode *tokens=NULL, *currentToken=NULL; 

void expression();

/* Stores a word to the file --- big endian */
void store(word w) {
    int i;
    char buf[4];
    for (i=0; i<4; i++) {
        buf[i] = (w >> (8*(4-(i+1)))) & 0xFF;
    }
    fwrite(buf, 1, 4, file);
}

/* Stores a loadi instruction to the file*/
void storeLoadi(word value) {
    store(create_itype_hex((value & 0xFFFF0000)>>16, I_T0, I_ZERO, OC_LUI));
    store(create_itype_hex(value & 0x0000FFFF, I_T0, I_T0, OC_ORI));
}

/* Stores a pop instruction to the file */
void storePop(int registerIndex) {
    store(create_itype_hex( 0x0000, registerIndex, I_SP ,OC_LW));
    store(create_itype_hex(0x0004, I_SP, I_SP, OC_ADDI));
}

/* Stores a push instruction to the file */
void storePush() {
    store(create_itype_hex(0xFFFC, I_SP, I_SP, OC_ADDI));
    store(create_itype_hex(0x0000, I_T0, I_SP, OC_SW));
}

/* Stores an operation to the file */
void storeOperation(SymbolCode symcode) {
	int functionCode;
        switch (symcode) {
            case times: functionCode=FC_MULT; break;
            case slash: functionCode=FC_DIV; break;
            case plus: functionCode=FC_ADD; break;
            case minus: functionCode=FC_SUB; break;
            default: ERROR("unknown operation, symcode %i", symcode);
        }
        store(create_rtype_hex(functionCode, 0x0000, I_T0, I_T0, I_T1, OC_ZERO));
}

/* - initializes the token list if necessary
 * - adds a new token with the arguments given
 * - make currentToken point to the newly added token 
 */
void addNewToken(SymbolCode symcode, Lexeme lexeme) {
     TokenNode *newToken = (TokenNode*) malloc(sizeof(TokenNode));
     if (!newToken) {
         ERROR("not enough memory");
     }
     newToken->symcode=symcode;
     newToken->lexeme=lexeme;
     newToken->next=NULL;
     if (!tokens) {
        tokens = newToken;
     } else {
        currentToken->next=newToken;
     }
     currentToken=newToken;
}

/* releases the memory allocated for the token list */
void freeTokens() {
     TokenNode* token;
     while (tokens) {
         token = tokens;
         tokens = tokens->next;
         free(token);
     }
}

/* sets currentToken to the next token */
void getToken() {
    currentToken = currentToken->next;
}


/* checks whether the current symcode matches to the symcode given as argument and gets the next token */
int accept(SymbolCode symcode) {
    if (currentToken && currentToken->symcode == symcode) {
        getToken();
        return TRUE;
    }
    return FALSE;
}
 
/* checks whether the current symcode matches the expected symcode given as argument */
int expect(SymbolCode symcode) {
    if (accept(symcode)) {
        return TRUE;
    }
    ERROR("expected symcode %i", symcode);
    return FALSE;
}

/* recursive descent parser method for factor */
void factor() {
    TokenNode* savedToken = currentToken;
    if (accept(number)) {
        sprintf(postfix,"%s %i", postfix, savedToken->lexeme.value);
        storeLoadi(savedToken->lexeme.value);
        storePush();
    } else if (accept(lparen)) {
        expression();
        expect(rparen);
    } else {
        ERROR("syntax error, expected number or left bracket");
        getToken();
    }
}

/* recursive descent parser method for term */
void term() {
  TokenNode* savedToken;
  factor();
  while (currentToken && (currentToken->symcode == times || currentToken->symcode == slash)) {
    savedToken = currentToken;
    getToken();
    factor();
    sprintf(postfix, "%s %c", postfix, savedToken->lexeme.symbol);
    storePop(I_T0);
    storePop(I_T1);
    storeOperation(savedToken->symcode);
    storePush();
  }
}

/* recursive descent parser method for expression */
void expression() {
  TokenNode* savedToken;
  term();
  while (currentToken && (currentToken->symcode == plus || currentToken->symcode == minus)) {
    savedToken = currentToken;
    getToken();
    term();
    sprintf(postfix,"%s %c", postfix, savedToken->lexeme.symbol);
    storePop(I_T0);
    storePop(I_T1);
    storeOperation(savedToken->symcode);
    storePush();

  }
}

/* the lexical scanner transform the input to a token list */
void lexer(char *expression) {
    SymbolCode symcode;
    Lexeme lexeme;
    char *numberString, symbol;
    int whitespace;
    while ( (symbol=*expression++) ) {
          whitespace = FALSE;
          lexeme.symbol=symbol;
          switch ( symbol ) {
              case '+': symcode=plus; break;
              case '-': symcode=minus; break;
              case '*': symcode=times; break;
              case '/': symcode=slash; break;
              case '(': symcode=lparen; break;
              case ')': symcode=rparen; break;
              case '0': case '1': case '2': case '3': case '4':
              case '5': case '6': case '7': case '8': case '9':
                  symcode=number;
                  numberString = (char*) calloc(MAX_NUMBER_LENGTH, sizeof(char));
                  if (!numberString) {
                      ERROR("not enough memory");
                  }
                  do {
                      strncat(numberString,&symbol,1);
                  } while ( (*expression>='0') && (*expression<='9') &&  (symbol=*expression++) );
                  lexeme.value=atoi(numberString);
                  free(numberString);
                  break;
             case ' ': case '\n': case '\t':
                 whitespace=TRUE;
                 break;
             default: ERROR("unknown symbol '%c'",symbol);
        }
        if (!whitespace) {
             addNewToken(symcode, lexeme);
        }

    }
}

/* Compiler:
 * - makes a lexical analysis of the input and thereby generates a token list
 * - parses and compiles the expression
 * - saves the result in a file named by filename
 */  
void compiler(char* exp, char *filename) {
    lexer(exp);
    file = fopen(filename,"w");
    if (!file) { 
        ERROR("can't open file %s", filename);
    }
    postfix = (char*) calloc(2*strlen(exp)+1, sizeof(char));
    if (!postfix) {
        ERROR("not enough memory");
    }
    currentToken=tokens;
    expression();
    store(create_jtype_hex(0x000000,OC_STOP));
    if (verbose) {
        printf("%s\n", postfix);
    }
    freeTokens();
    fclose(file);
    free(postfix);
}

