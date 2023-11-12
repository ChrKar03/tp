#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 2

typedef struct
{
    FILE *fd;
    int size;   // Number of ints in "array".
    int *done;  // Flag to block & release threads above.
    int csr;    // Cursor position.
} arg_t;

// Thread function to initialize and decide actions.
void *mergeSort(void *arg);
// Function that merges the two "arrays".
void merge(arg_t *ar1, arg_t *ar2);
// Function to sort the small block of numbers in memory.
void bubblesort(arg_t *ar);
// Swaps the values of a & b.
void swap(int *a, int *b);

int main(int argc, char **argv)
{
    // Initialization.
    int done = 0;
    arg_t arg = {.done = &done};
    arg.fd = fopen(argv[1], "r+b");
    // File size and # of ints.
    fseek(arg.fd, 0, SEEK_END);
    arg.size = ftell(arg.fd) / 4;
    fseek(arg.fd, 0, SEEK_SET);
    arg.csr = 0;

    pthread_t *t = (pthread_t*) malloc(sizeof(pthread_t));
    void *p = &arg;

    pthread_create(t, NULL, mergeSort, p);

    while (!done);

    free(t);
    fclose(arg.fd);
    return 0;
}

void *mergeSort(void *arg)
{
    // To avoid casting.
    arg_t *tmp = (arg_t*)arg;
    // If doesn't exceed size, sort the numbers with bubblesort,
    // else standard merge sort method.
    if (tmp->size > N)
    {
        // Check variables.
        int done1 = 0;
        int done2 = 0;
        arg_t temp1 = {.fd = tmp->fd, .size = tmp->size/2, .done = &done1, .csr = tmp->csr};
        arg_t temp2 = {.fd = tmp->fd, .size = tmp->size/2, .done = &done2, .csr = tmp->csr + (tmp->size/2) * 4};

        if (tmp->size % 2)
            temp2.size += 1;

        void *p1 = &temp1;
        void *p2 = &temp2;

        pthread_t *t1 = (pthread_t*) malloc(sizeof(pthread_t));
        pthread_t *t2 = (pthread_t*) malloc(sizeof(pthread_t));
        
        pthread_create(t1, NULL, mergeSort, p1);
        pthread_create(t2, NULL, mergeSort, p2);

        while (!(done1 && done2));

        free(t1);
        free(t2);
        merge(&temp1, &temp2);
    }
    else
        bubblesort(tmp);

    *tmp->done = 1;
    return NULL;
}

void merge(arg_t *ar1, arg_t *ar2)
{
    // We will implement the gap method, so as the complexity of space is O(1).
    int i, j, csr1, csr2, g = (ar1->size + ar2->size) / 2 + (ar1->size + ar2->size) % 2;
    int *int1 = (int *) malloc(sizeof(int)), *int2 = (int *) malloc(sizeof(int));

    while (g)
    {
        i = 0;
        j = g;
        csr1 = ar1->csr, csr2 = ar1->csr + 4 * j;
        while (j < ar1->size + ar2->size)
        {
            // Set possition of cursors to block of numbers assigned to thread.
            fseek(ar1->fd, csr1, SEEK_SET);
            fread(int1, sizeof(int), 1, ar1->fd);
            fseek(ar2->fd, csr2, SEEK_SET);
            fread(int2, sizeof(int), 1, ar2->fd);
            // Gap method for merging the two arrays.
            if (i < ar1->size && j >= ar1->size && *int1 > *int2)
                swap(int1, int2);
            else if (i > ar1->size && *int1 > * int2)
                swap(int1, int2);
            else if (*int1 > *int2) 
                swap(int1, int2);
            // Store values to file.
            fseek(ar1->fd, csr1, SEEK_SET);
            fwrite(int1, sizeof(int), 1, ar1->fd);
            fseek(ar2->fd, csr2, SEEK_SET);
            fwrite(int2, sizeof(int), 1, ar2->fd);
            csr1 += 4;
            csr2 += 4;
            i++;
            j++;
        }
        if (g == 1)
            break;
        g = g / 2 + g % 2;
    }
    free(int1);
    free(int2);
}

void bubblesort(arg_t *ar)
{
    int tmp, swap = 1, i;
    int *int_buf = (int *) malloc(sizeof(int) * ar->size);
    // Set cursor to possition, read all chunk, sort and return.
    fseek(ar->fd, ar->csr, SEEK_SET);
    fread(int_buf, sizeof(int), ar->size, ar->fd);
    fseek(ar->fd, ar->csr, SEEK_SET);
    while (swap)
    {
        swap = 0;
        for (i = 0; i < ar->size - 1; i++)
        {
            if (ar->size > 1 && *(int_buf + i) > *(int_buf + i + 1))
            {
                tmp = *(int_buf + i);
                *(int_buf + i) = *(int_buf + i + 1);
                *(int_buf + i + 1) = tmp;
                swap++;
            }
        }
    }
    fwrite(int_buf, sizeof(int), ar->size, ar->fd);
    free(int_buf);
}

void swap(int *a, int *b)
{
    *a ^= *b;
    *b ^= *a;
    *a ^= *b;
}