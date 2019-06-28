//////////////////////////////////////////////
// Author: I-No Liao                        //
// Date of update: 2019/05/14               //
// Description: OS HW4 Dining Philosophers  //
//////////////////////////////////////////////

#include <pthread.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N 5
#define THINKING 0
#define HUNGRY 1
#define EATING 2
#define LEFT (philosopherId + 4) % N
#define RIGHT (philosopherId + 1) % N

// pthread declaration
pthread_mutex_t mutex;
pthread_cond_t cond_var[N];
pthread_t threadID[N];

// global variable
int state[N];
int philosophers[N] = {0, 1, 2, 3, 4};

void test(int philosopherId) {
    if (state[philosopherId] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING) {

        // EATING
        int eatingTime = rand() % 3 + 1;
        state[philosopherId] = EATING;
        printf("Philosopher %d is now EATING for %d seconds\n", philosopherId, eatingTime);
        sleep(eatingTime);

        // after EATING finishes, signal to let others know
        pthread_cond_signal(&cond_var[philosopherId]);

    } else if (state[philosopherId] == HUNGRY) {
        printf("Philosopher %d fails to pick up forks and start waiting.\n", philosopherId);
    }
}

void pickup_forks(int philosopherId) {

    // lock
    pthread_mutex_lock(&mutex); 

    // HUNGRY
    state[philosopherId] = HUNGRY;
    printf("Philosopher %d is now HUNGRY and trying to pick up forks.\n", philosopherId);

    // try EATING
    test(philosopherId);

    // wait for forks from left and/or right philosopher
    // namely, wait for signal on cond_var
    if (state[philosopherId] != EATING) {
        pthread_cond_wait(&cond_var[philosopherId], &mutex);
    }

    // unlock
    pthread_mutex_unlock(&mutex);
}

void return_forks(int philosopherId) {

    // lock
    pthread_mutex_lock(&mutex);

    // return to THINKING
    state[philosopherId] = THINKING;
    printf("Philosopher %d returns forks and then starts TESTING %d and %d.\n", philosopherId, LEFT, RIGHT);

    // test if left or right philosopher wants to eat
    test(LEFT);
    test(RIGHT);

    // unlock
    pthread_mutex_unlock(&mutex);
}

// thread function
void* philospher(void *n) {

    // THINKING
    int thinkingTime = rand() % 3 + 1;
    int *id = n;
    printf("Philosopher %d is now THINKING for %d seconds.\n", *id, thinkingTime);
    sleep(thinkingTime);

    // HUNGRY & try EATING
    pickup_forks(*id);

    // return to THINKING
    return_forks(*id);
    return NULL;
}

int main() {

    // variable declaration
    srand((unsigned) time(NULL));
    pthread_t *threads = malloc(N * sizeof(pthread_t));

    // initialization
    for(int i = 0; i < N; i++) {
        state[i] = THINKING;
        pthread_cond_init(&cond_var[i], NULL);
    }
 
    // create thread
    pthread_mutex_init(&mutex, NULL);
    for(int i = 0; i < N; i++) {
        pthread_create(&threads[i], NULL, philospher, &philosophers[i]);
    }

    // join thread
    for(int i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);
    }

    // free
    free(threads);
    pthread_mutex_destroy(&mutex);
}
