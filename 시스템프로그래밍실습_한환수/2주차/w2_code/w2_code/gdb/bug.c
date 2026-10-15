//Compile with:
//gcc -ggdb bug.c -o bug.out

#include <stdio.h>
#include <stdlib.h>

void foo() {
    char* temp = "Paras";

    int i = 0;
    temp[3] = 'F';

    for(i=0; i<5; i++)
        printf("%c", temp[i]);
}

int main() {
    foo();
    return EXIT_SUCCESS;
}
