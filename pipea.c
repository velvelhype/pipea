#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include "libft/libft.h"
#include <string.h>

// ./pipex infile "grep a1" "wc -w" outfile
// < infile grep a1 | wc -w > outfile
// ./pipex infile "ls -l" "wc -l" outfile
// < infile ls -l | wc -l > outfile

int pipe_fd[2];

void    ft_exit(char *error)
{
    write(2, error, ft_strlen(error));
    exit(0);
}

char* make_path(char *file)
{
    char path_list[8][30] = {"/bin/", "/usr/bin/", "/Users/eval/.brew/bin",
     "/usr/local/bin", "/usr/sbin", "/sbin", "/usr/local/go/bin", "/usr/local/munki"};
    char *joined;
    int  i = 0;
    
    joined = ft_strjoin(path_list[0], file);

    while(access(joined, X_OK) == -1)
    {
        free(joined);
        i++;
        if (i == 8)
            ft_exit("command not found\n");
        joined = ft_strjoin(path_list[i], file);
    }

    free(file);
    return (joined);
}

void do_child(char *arg[])
{
    char **splitted_away = ft_split(arg[2], ' ');
    *splitted_away = make_path(*splitted_away);

    close(pipe_fd[0]);
    dup2(pipe_fd[1], 1);
    close(pipe_fd[1]);

    char *argv[] = {*splitted_away, *(splitted_away + 1), NULL};
    char *environ[] = {NULL};

    execve(argv[0], argv, environ);

   perror("execve\n");
}

void do_parent(char *arg[])
{   
    char **splitted_away = ft_split(arg[3], ' ');
    *splitted_away = make_path(*splitted_away);

    close(pipe_fd[1]);
    dup2(pipe_fd[0], 0);
    close(pipe_fd[0]);

    char *argv[] = {*splitted_away, *(splitted_away + 1), NULL};
    char *environ[] = {NULL};
    execve(argv[0], argv, environ);
        
   perror("execve\n");
}

int main(int argc, char *argv[])
{
    pipe(pipe_fd);

    if(argc != 5)
        exit(1);
    
    int filein_fd;
    filein_fd = open(argv[1], O_RDONLY);
    close(0);
    dup2(filein_fd, 0);
    close(filein_fd);

    int fileout_fd;
    fileout_fd = open(argv[4], O_CREAT|O_RDWR|O_TRUNC, 0644);
    close(1);
    dup2(fileout_fd, 1);
    close(fileout_fd);

    int pid = fork();

    if (pid == 0)
        do_child(argv);
    else
        do_parent(argv);
        
    return (0);
}