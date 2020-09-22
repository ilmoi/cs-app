/* $begin counterprob */
#include "csapp.h"

int counter = 0;

void handler(int sig)
{
    while (waitpid(-1, NULL, 0) > 0) {
        counter++;
        printf("counter incremented to %d\n", counter);
    }
    if (errno != ECHILD)
        printf("waitpid error");
    sleep(1); /* Do some work in the handler */
    return;
}

int main()
{
    int i;
    signal(SIGUSR2, handler);

    for (i = 0; i < 5; i++) {
        if (fork() == 0) {  /* Child */
            kill(getppid(), SIGUSR2);
            printf("sent SIGUSR2 to parent\n");
            exit(0);
        }
    }

    sleep(20);
    //wait(NULL);
    printf("counter=%d\n", counter);
    exit(0);
}
/* $end counterprob */