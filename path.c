#include<stdio.h>
#include <unistd.h>

int main(void)
{
    char *argv[] = { "bin/ls", 0 };
    char *envp[] =
    {
        "HOME=/",
        "PATH=/bin:/usr/bin",
        "TZ=UTC0",
        "USER=beelzebub",
        "LOGNAME=tarzan",
        0
    };
    execve(argv[0], argv, envp);
    fprintf(stderr, "Oops!\n");
    return -1;
}