#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

typedef enum status = {READY=0, BUSY} status_t

typedef struct
{
    int th_id;
    status_t stat = READY;
    pthread_t *next;
    pthread_t thread;
} p_t;

void int_handler()
{
    //When a worker thread is ready, send signal to notify main thread
}

void worker_thread()
{
    


void main_thread()
{
    double num;
    while(scanf(&num))
    {


int main(int argc, char *argv)
{
    int N = atoi(argv[1]), i;
    p_t *ready_line = NULL, *busy_line = NULL;
    pthread_t threads[N];
    for (i = 0; i < N; i++)
        if (pthread_creare(threads + i, ))
            print("error");

    for (i = 0; i < N; i++)
    {
        p_t
    }
    

