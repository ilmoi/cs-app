#include <stdlib.h>
#include <stdio.h>

typedef unsigned long size_t;

void *basic_memset(void *s, int c, size_t n) {
    size_t cnt = 0;
    unsigned char *schar = s;
    while (cnt < n) {
        *schar++ = (unsigned  char) c;
        cnt++;
    }
    return s;
}

void *smarter_memset(void *s, int c, size_t n) {
    size_t cnt = 0;

    unsigned char *schar = s;
    int k = sizeof(long);

    //until address multiple of 8, proceed normally
    while ((unsigned long)(&s) % k != 0) {
        while (cnt < n) {
            *schar++ = (unsigned  char) c;
            cnt++;
        }
    }

    //check if even worth bothering
    if ((n-cnt) > 7) {

        //generate the filler
        unsigned long last_byte = c & 0xff;
        unsigned long filler = 0;
        int i;
        for (i=0; i<k; i++) {
            filler = filler | last_byte;
            last_byte = last_byte << 8;
        }

        //set a limit (we don't want to overrun the bounds)
        int limit = n-8+1;

        //we'll need a second pointer of a different type
        unsigned long *slong = s;

        while (cnt < limit) {
            printf("yay\n");
            *slong = filler;
            //increment both pointers
            slong +=1; //need to increment by 1 not 8 coz type long, be careful
            schar +=8;
            //increment count
            cnt +=8;
        }
    }

    //finish off the rest
    while (cnt < n) {
        *schar++ = (unsigned  char) c;
        cnt++;
    }

    return s;
}

void int_to_char(int x) {
    x = 0x12341234;
    x = (char)x;
    printf("%x", x);
}


int main() {

    size_t size = 100;

    char *s = (char*)malloc(size*sizeof(char));

    //set to all 0s with function we know works
    s = basic_memset(s, 0x65, size);
    s = smarter_memset(s, 0x66, size);

    //for (int i=0; i<size; i++) {
    //    printf("%x\n", s[i]);
    //}
    printf("%s", s);
}