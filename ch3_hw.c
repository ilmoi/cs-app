//
// Created by Ilja Moisejevs on 8/26/20.
//

#include <stdio.h>
#define BUF_SIZE 4

int good_echo() {
    //safely reads from stdin and writes to stdout

    //define a string as an array of size BUF_SIZE
    char buf[BUF_SIZE];
    while(1) {
        // as the user continues typing, new buf chunks are allocated,
        // which means the user can type an arbitrary-length string
        // and never go out of bound
        char *p = fgets(buf, BUF_SIZE, stdin);

        //null pointer is returned when user goes out of bounds
        if (p == NULL) {
            break;
        }
        printf("%s", p);
    }
    return 1;
}

double funct3(int *ap, double b, long c, float *dp) {
    if (b > *ap) {
        return (double) (*dp * c);
    }
    return (double) (*dp + *dp + c);

}

int main() {
    printf("yay");
    good_echo();
}