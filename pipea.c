#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

// ./pipex infile "grep a1" "wc -w" outfile
// < infile grep a1 | wc -w > outfile
// ./pipex infile "ls -l" "wc -l" outfile
// < infile ls -l | wc -l > outfile

int pipe_fd[2];

void do_child()
{
    close(pipe_fd[0]);
    dup2(pipe_fd[1], 1);
    close(pipe_fd[1]);

    char *argv[] = {"/usr/bin/grep", "1", NULL};
    char **environ;
    
    execve(argv[0], argv, environ);
}

void do_parent()
{
    // char outf[] = "outf";
    close(pipe_fd[1]);
    dup2(pipe_fd[0], 0);
    close(pipe_fd[0]);

    // int fileout_fd;
    // fileout_fd =  open("bin", O_CREAT|O_RDWR|O_TRUNC, 0644);
    // if(fileout_fd < 0)
    //     printf("for erro ");
    // dup2(fileout_fd, 1);
    // close(fileout_fd);

    char *argv[] = {"/usr/bin/wc", "-w", NULL};
    char **environ;
    execve(argv[0], argv, environ);
}

//./pipex infile "grep a1" "wc -w" outfile
int main(int argc, char *argv[])
{
    pipe(pipe_fd);

    int fd;
    fd = open(argv[1], O_RDONLY);
    close(0);
    dup2(fd, 0);
    close(fd);

    int pid = fork();

    if (pid == 0)
        do_child();
    else
    {
    int fileout_fd;
    fileout_fd =  open(argv[4], O_CREAT|O_RDWR|O_TRUNC, 0644);
    if(fileout_fd < 0)
        printf("for erro ");
    dup2(fileout_fd, 1);
    close(fileout_fd);
    do_parent();
    }
}