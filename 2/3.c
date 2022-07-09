#include <stdio.h>


int main(int argc, const char * argv[]) {

    int array[11]={1,0,1,0,1,0,1,0,1,0,1};
    int i;
    for (i=0; i<11; ++i) {
        printf("%x %i \n", i,array[i]);
    }
    return 0;
}