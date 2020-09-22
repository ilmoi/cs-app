//
// Created by Ilja Moisejevs on 8/30/20.
//

#include <stdio.h>

void bubble_a(long *data, long count) {
    long i, j;
    for (i=0; i<count; i++) {
        for (j = 0; j<i; j++)
            if (data[j+1] < data[j]) {
                long t = data[j+1];
                data[j+1] = data[j];
                data[j] = t;
            }
    }
}

void bubble_b(long *data, long count) {
    //reimplementation of the above but using pointers
    long i, j;
    for (i=0; i<count; i++) {
        for (j=0; j<i; j++)
            if (*(data+j+1) < *(data+j)) {
                long t = *(data+j+1);
                *(data+j+1) = *(data+j);
                *(data+j) = t;
            }
    }
}

int main() {
    printf("hello world\n");
    long data[] = {11,2,33,4,55};

    //had to make sure I create a pointer and pass the pointer rather than actual array
    long *data_ptr = data;
    bubble_b(data_ptr, 5);

    int i;
    for (i=0; i<5; i++) {
        printf("%ld\n", data[i]);
    }
}

//to compile to assembly use gcc -S -fverbose-asm -O1 ch4.c