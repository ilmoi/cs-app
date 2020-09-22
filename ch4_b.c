#include <stdio.h>

void bubble_b(long *data, long count) {
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
    long data[] = {5,4,3,2,1};
    long *data_ptr = data;
    bubble_b(data_ptr, 5);
}

//to compile to assembly use gcc -S -fverbose-asm -O1 ch4.c