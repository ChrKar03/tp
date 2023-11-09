#ifndef MY_FIFO
#define MY_FIFO 1

int pipe_open(int size);
int pipe_write(int p, char c);
int pipe_writeDone(int p);
int pipe_read(int p, char *c);
