#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "libft/libft.h"

int main(int argc, char *argv[])
{
    char path_list[2][10] = {"/bin/", "/usr/bin/"};
    char *joined;
    int     i = 0;
    
    joined = ft_strjoin(path_list[0], argv[1]);

    while( access(joined, X_OK) == -1 )
    {
        i++;
        joined = ft_strjoin(path_list[i], argv[1]);
        free (joined);
    }

    printf("'%s' is %d\n", joined, access(joined, X_OK));

}