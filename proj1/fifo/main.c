#include <my_fifo.h>
#include <unistd.h>
#include <pthreads.h>
#include <fcntl.h>
#include <stdio.h>


int main(int argc, char *argv)
{
    char *f_name = argv[1];
    pthread_t thread[2];
    if (pthread_create())
        printf("threads\n");
    // str manip
    fout_name = ;
    int fd = open(fout_name, O_CREAT);



    close(fd);

    return 0;
}

