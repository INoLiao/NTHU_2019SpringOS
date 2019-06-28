#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <string.h>

// define
#define MAX_SIZE 15000
#define MAX_LINE 20 
#define MAX_THREAD 2

// global variables
int nums[MAX_LINE][MAX_SIZE];
char buffer[MAX_LINE][MAX_SIZE];
int rowSize = 0;
int colSize[MAX_SIZE];

// thread input datatype
struct threadData {
    int thread_id;
    int row;
};

// merge - serial
void merge(int nums[], int start, int mid, int end) {

    // copy to temporary arrays
    int sizeL = mid - start + 1;
    int sizeR = end - (mid + 1) + 1;
    int left[sizeL], right[sizeR];
    for (int i = 0; i < sizeL; i++) {
        left[i] = nums[start + i];
    }
    for (int i = 0; i < sizeR; i++) {
        right[i] = nums[mid + 1 + i];
    }

    // merge left and right into nums
    int i = 0, j = 0, count = start;

    // merge
    while (i < sizeL && j < sizeR) {
        if (left[i] < right[j]) {
            nums[count++] = left[i++];
        } else {
            nums[count++] = right[j++];
        }
    }

    // copy the rest
    while (i < sizeL) nums[count++] = left[i++];
    while (j < sizeR) nums[count++] = right[j++];

    return;
}

// merge sort - serial
void mergeSort(int nums[], int start, int end) {
    if (start >= end) {
        return;
    }

    // divide
    int mid = start + (end - start) / 2;
    mergeSort(nums, start, mid);
    mergeSort(nums, mid + 1, end);

    // conquer
    merge(nums, start, mid, end);

    return;
}

// merge sort - thread fundtion
void* mergeSortThread(void *arguments) {

    // retrieve arguments
    struct threadData* arg = (struct threadData*) arguments;
    int thread_id = arg->thread_id;
    int row = arg->row;

    // merge sort initialization 
    int start, end;
    if (colSize[row] % 2 == 0) {
        start = thread_id * (colSize[row] / MAX_THREAD);
        end = (thread_id + 1) * (colSize[row] / MAX_THREAD) - 1;
    } else {
        if (thread_id == 0) {
            start = 0;
        } else {
            start = thread_id * (colSize[row] / MAX_THREAD) + 1;
        }
        end = (thread_id + 1) * (colSize[row] / MAX_THREAD);
    }

    if (start >= end) {
        return NULL;
    }

    // divide
    int mid = start + (end - start) / 2;
    mergeSort(nums[row], start, mid);
    mergeSort(nums[row], mid + 1, end);

    // conquer
    merge(nums[row], start, mid, end);
    
    return NULL;
}

int main(int argc, char *argv[]) {
    printf("\n========== program start ==========\n\n");
    
    // parse arguments
    if (argc != 3) {
        printf("Incorrect # of arguments!\n");
        return 0;
    }

    // open files
    FILE *inputFile = fopen(argv[1], "r");
    FILE *outputFile = fopen(argv[2], "wb");

    // read inputFile
    if (inputFile == NULL) {
        printf("Cannot open %s file\n", argv[1]);
        printf("\n=========== program end ===========\n\n");
        return 0;
    }
    while (fgets(buffer[rowSize], sizeof buffer, inputFile)) {
        char *p = buffer[rowSize];
        char *q;
        int i = 0;
        while (i < MAX_SIZE) {
            nums[rowSize][i] = strtol(p, &q, 0);
            if (p == q) {
                break;
            }
            p = q;
            i++;
        }
        colSize[rowSize] = i;
        rowSize++;
        if (rowSize >= MAX_LINE) {
            printf("Line size exceeds limit\n");
        }
    }
    fclose(inputFile);

    // pthread
    struct threadData* threadArray = malloc(MAX_THREAD * sizeof(struct threadData));
    pthread_t threads[MAX_THREAD];

    printf("--------------- merge sort ---------------\n");
    clock_t startTime, endTime;
    for (int i = 0; i < rowSize; i++) {

        // time stamp - start
        startTime = clock();

        // create threads
        for (int thread = 0; thread < MAX_THREAD; thread++) {

            // thread arguments
            threadArray[thread].thread_id = thread;
            threadArray[thread].row = i;

            // merge sort
            pthread_create(&threads[thread], NULL, mergeSortThread, (void*) &threadArray[thread]);
        }

        // join threads
        for (int thread = 0; thread < MAX_THREAD; thread++) {
            pthread_join(threads[thread], NULL);
        }

        // merge by main thread
        merge(nums[i], 0, (colSize[i] - 1) / 2, colSize[i] - 1);

        // time stamp - end
        endTime = clock();
        printf("case #%d: size = %d, time = %fs\n", i + 1, colSize[i], (endTime - startTime) / (double) CLOCKS_PER_SEC);
        
        for (int j = 0; j < colSize[i]; j++) {
            // check sorting results
            if (j > 0 && nums[i][j - 1] > nums[i][j]) {
                printf("\n>> incorrect sorting @row = %d\n", i);
            }

            if (j < colSize[i] - 1) {
                fprintf(outputFile, "%d ", nums[i][j]);
            } else {
                fprintf(outputFile, "%d", nums[i][j]);
            }
        }
        fprintf(outputFile, "\n");
    }
    printf("------------------------------------------\n");

    free(threadArray);
    fclose(outputFile);
    printf("\n=========== program end ===========\n\n");

    return 0;
}
