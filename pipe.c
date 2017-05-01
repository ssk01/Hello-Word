#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>
int fork1(void);

int main()
{
    errno = 0;
    int p[2];
    int r;
    if (pipe(p) < 0)
    {
        fprintf(stderr, "fail to pipe%s\n", strerror(errno));
        exit(0);
    }

    if(fork1() == 0)
    {
        close(p[0]);
        printf("Child process 1 is sending a message!\n");
        write(p[1],"Child process 1 is sending a message!\n",100);
        exit(EXIT_SUCCESS);
    }
    else 
    {
        wait(&r);
        if(fork1() == 0)
        {
            close(p[0]);
            printf("Child process 2 is sending a message!\n");
            write(p[1],"Child process 2 is sending a message!\n",100);
            exit(EXIT_SUCCESS);
        }
        else 
        {
             wait(&r);
             close(p[1]);
         
             char c[100];
             char d[100];

             read(p[0], c, 100);
             printf("\tparent read first   :%s", c);
             read(p[0], d, 100);
             printf("\tparent read second  :%s", d);
        }
    }
    printf("happy ending!!!!!!!!!!!\n");
}

int fork1(void)
{
    int pid;
  
    pid = fork();
    if(pid == -1)
        perror("fork");
    return pid;
}