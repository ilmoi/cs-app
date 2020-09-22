#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

int mysystem(char *command) {
    char *params[] = {"", "-c", command, NULL}; //we can just skip the first variable and make it empty, since we're passing the filename below
    pid_t pid; //process id

    if ((pid = fork()) == 0) {
        //child runs the below
        if (execve("/bin/sh", params, NULL) < 0) { //we need to point to the shell executable, not just to the folder
            printf("%s: command not found. \n", command);
            exit(0);
        }
    }

    //removing zombies
    int status; //this is where the child's status will be written
    if (waitpid(pid, &status, 0) < 0)
        printf("waitpid error\n");
    else
        printf("%d, %s\n", pid, command);

        //the below basically applies functions on top of child's status to extract meaning out of it

        /* exit normally */
        if (WIFEXITED(status))
            return WEXITSTATUS(status);

        /* exit by signal */
        if (WIFSIGNALED(status))
            return WTERMSIG(status);

    //executes the passed command, returning either the exit status or the error

}

int main() {
    printf("yay\n");
    int code = mysystem("./prog");
    printf("exit code is %d", code);
    return 0;
}