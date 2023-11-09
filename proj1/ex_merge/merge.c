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
        
        if (pthread_create(t1, NULL, mergeSort, p1) || pthread_create(t2, NULL, mergeSort, p2))
            perror("SAKAI TRWS BEO");

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
    // csr is the cursor for the "exchanged" values that are
    // temporary stored in the old possition of the sorted numbers.
    int i = 0, j = 0, csr = ar2->csr;
    int *k = (int *) malloc(sizeof(int)), *int1 = (int *) malloc(sizeof(int)), 
    *int2 = (int *) malloc(sizeof(int));
    *k = 0;

    while (i < ar1->size && j < ar2->size)
    {
        // Set possition of cursor to block of numbers assigned to thread.
        fseek(ar1->fd, ar1->csr, SEEK_SET);
        if (fread(int1, sizeof(int), 1, ar1->fd) == -1)
            perror("SAKAI DIAVAZE");
        fseek(ar2->fd, ar2->csr, SEEK_SET);
        if (fread(int2, sizeof(int), 1, ar2->fd) == -1)
            perror("SAKAI DIAVAZE 2");
        // Intialization for first time.
        if(!*k)
            *k = *int2;
        // Since no extra space, three pointers at file, int1 to the left side,
        // int2 to right and k the pointer for the "exchanged" numbers possition.
        if (*int1 > *int2 && *k >= *int2)
        {
            // Case where right element smaller than the left.
            *int1 ^= *int2;
            *int2 ^= *int1;
            *int1 ^= *int2;
            fseek(ar1->fd, ar1->csr, SEEK_SET);
            if (fwrite(int1, sizeof(int), 1, ar1->fd) != 1)
                perror("SAKAI GRAFE 1");
            fseek(ar2->fd, ar2->csr, SEEK_SET);
            if (fwrite(int2, sizeof(int), 1, ar2->fd) != 1)
                perror("SAKAI GRAFE 2");
            // Fist "exchanged" number.
            if (*k == *int1)
                *k = *int2;
            i++;
            ar1->csr += 4;
            j++;
            ar2->csr += 4;
        }
        else if (*int1 < *int2 && *k < *int1)
        {
            // Case were "exchanged" value is smaller of all.
            *int1 ^= *k;
            *k ^= *int1;
            *int1 ^= *k;
            fseek(ar1->fd, ar1->csr, SEEK_SET);
            if (fwrite(int1, sizeof(int), 1, ar1->fd) != 1)
                perror("SAKAI GRAFE 3");
            fseek(ar2->fd, csr, SEEK_SET);
            if (fwrite(k, sizeof(int), 1, ar2->fd) != 1)
                perror("SAKAI GRAFE 4");
            i++;
            ar1->csr += 4;
        }
        else
        {
            // Case where left element is the smallest.
            i++;
            ar1->csr += 4;
        }
    }

    // Calculating possition of array cursor and array2 j index.
    j = ((csr - (ar2->csr - j * 4)) / 4) + 1;
    ar2->csr = csr + j * 4;

    // Sort the remain "exchanged" numbers that are all located in array2. 
    while (*k  && j < ar2->size)
    {
        fseek(ar2->fd, csr, SEEK_SET);
        if (fread(k, sizeof(int), 1, ar1->fd) == -1)
            perror("SAKAI DIAVAZE 3");
        fseek(ar2->fd, ar2->csr, SEEK_SET);
        if (fread(int2, sizeof(int), 1, ar2->fd) == -1)
            perror("SAKAI DIAVAZE 4");
        if (*k > *int2)
        {
            *k ^= *int2;
            *int2 ^= *k;
            *k ^= *int2;
            fseek(ar1->fd, csr, SEEK_SET);
            if (fwrite(k, sizeof(int), 1, ar1->fd) != 1)
                perror("SAKAI GRAFE 5");
            fseek(ar2->fd, ar2->csr, SEEK_SET);
            if (fwrite(int2, sizeof(int), 1, ar2->fd) != 1)
                perror("SAKAI GRAFE 6");
        }
        csr += 4;
        j++;
        ar2->csr += 4;
    }

    free(int1);
    free(k);
    free(int2);
}

void bubblesort(arg_t *ar)
{
    int tmp, swap = 1, i;
    int *int_buf = (int *) malloc(sizeof(int) * ar->size);
    // Set cursor to possition, read all chunk, sort and return.
    fseek(ar->fd, ar->csr, SEEK_SET);
    if (fread(int_buf, sizeof(int), ar->size, ar->fd) == -1)
        perror("SAKAI 2 THELEI SPERMA");
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
    if (fwrite(int_buf, sizeof(int), ar->size, ar->fd) != ar->size)
        perror("SAKAI STAMATA NA TON TRWS");

    free(int_buf);
}