#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void try() {
    fork();
    printf("yay\n");
    fork(); //this wont print

}

int counter = 1;

void incrementer() {
    if (fork() == 0) {
        counter++; //increments but inside of private child's space, so doesn't transfer to parent
        exit(0);
    } else {
        wait(NULL);
        printf("%d", counter);
    }
}

int main() {
    //fork(); //this will double try
    //try();
    //fork(); //this wont affect try
    //incrementer();
    printf("yes");
    fflush(stdout);

}