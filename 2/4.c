#include <stdio.h>


int main(int argc, const char * argv[]) {



    L1: #do something
        add $s2, $s2, $s1
        beq $s2, $s3, L2 j L1
    L2:

    int s1 = 1;
    int s2 = 4;
    int s3 = 20;

    while(s2==s3){
        s2 = s2+s1;
    }



    return 0;
}