/* TODO: Task (b) Please fill in the following lines, then remove this line.
 *
 * author(s):   Tobias Brunner
 *              Olivier Cathrein
 * modified:    2010-01-07
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include "memory.h"
#include "mips.h"
#include "compiler.h"
 
int main ( int argc, char** argv ) {
    /* TODO: Task (c) implement main */
    /* Task (c) implement main */
    if (argc != 3){
        printf("usage:\t%s %s %s",argv[0],argv[1], argv[2]);
    }
    else{
        printf("Input:  \t%s\n",argv[1]);
        verbose = TRUE;
        printf("Postfix:\t");
        compiler(argv[1],argv[2]);
        printf("MIPS binary saved to %s\n", argv[2]);
        verbose = FALSE;
    }
    return EXIT_SUCCESS;
}

