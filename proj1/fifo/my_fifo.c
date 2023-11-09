#include <my_fifo.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>


F_SETPIPE_SZ
int pipe_open(int size)
{
    int fd[2];
    int check = pipe(fd);
    if (check == -1)
        perror("");
    return check;
}

int pipe_write(int p, char c)
{
    int check = write(p, c, 1);

    return check;   
}

int pipe_writeDone(int p)
{
    close(p);

    return 0;   
}

int pipe_read(int p, char *c)
{
    int check;

    check = read(p, (char*)c, 1);

    return check;   
}

