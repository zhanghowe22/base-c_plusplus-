#include "thread_pool_1.1.h"

pthread_mutex_t GMutex;
int GDone;

void do_task(void *arg) {
    thread_pool_t* pool = (thread_pool_t*)arg;
    pthread_mutex_lock(&GMutex);
    GDone++;
    printf("Doing %d task\n", GDone);
    pthread_mutex_unlock(&GMutex);
    if(GDone >= 10) {
        thread_pool_terminate(pool);
    }
}

int main(int argc, char** argv) {
    int thread_counts = 8;
    pthread_mutex_init(&GMutex, NULL);

    thread_pool_t* pool = thread_pool_create(thread_counts);
    if(pool == NULL) {
        perror("Thread pool crate error! \n");
        exit(-1);
    }
    while (thread_pool_post(pool, &do_task, pool) == 0) {

    }
    thread_pool_waitdone(pool);

    pthread_mutex_destroy(&GMutex);

    return 0;
}