#include "../inc/cvector.h"

#include <stdio.h>

CVECTOR_DECLARE(int);
CVECTOR_DEFINE(int);

int vecprint(intVector *vec){
    for (int i = 0; i < vec->len; i++){
        printf("%d ", *intVector_get(vec, i));
    }
    printf("\n");

    return 0;
}

int inteq(int a, int b){
    if (a < b){
        return -1;
    }else if(a > b){
        return 1;
    }

    return 0;
}

int main(){
    intVector vec;

    intVector_init(&vec);

    for (int i = 0; i < 12; i++){
        intVector_append(&vec, i);
    }

    vecprint(&vec);

    intVector_ins(&vec, 3, 7);
    intVector_ins(&vec, 6, 10); 
    intVector_del(&vec, 3, NULL);
    printf("%d\n", intVector_find(&vec, inteq, 10));
    intVector_rem(&vec, inteq, 10);
    vecprint(&vec);
    
    for (int i = 0; i < 10; i++){
        intVector_append(&vec, i + 12);
    }

    vecprint(&vec);
}
