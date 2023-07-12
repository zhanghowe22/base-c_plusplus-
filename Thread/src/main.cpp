#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <string>

using namespace std;

pthread_rwlock_t rwclock;

void* write_file(void* arg)
{
    const char* path = (char*)arg;

    char buf[15] = "Hello! ni hao";

    FILE* fd = NULL;
    size_t rc;
    
    pthread_rwlock_wrlock(&rwclock);

    fd = fopen(path, "wb");
    if(fd == NULL) {
        cout << "Open file failed!" << endl;
    }

    if((rc = fwrite(buf, 1, sizeof(buf), fd)) == 0) {
        cout << "Write error!" << endl;
    } else {
        cout << "write file success " << rc << "bytes" << endl;
    } 

    fclose(fd);

    pthread_rwlock_unlock(&rwclock);

    return NULL;
}

void* read_file(void* arg) 
{
    const char* path = (char*)arg;
    FILE* fd;

    pthread_rwlock_rdlock(&rwclock);

    fd = fopen(path, "rb");

    char buf[255] = {0};

    size_t rc;
    rc = fread(buf, 1, sizeof(buf), fd);

    if(rc == 0) {
        cout << "No data" << endl;
    } else {
        cout << "Read success " << endl;
        cout << buf << endl;
    }

    fclose(fd);

    pthread_rwlock_unlock(&rwclock);
    return NULL;

}

int main()
{
    // 加锁

    pthread_rwlock_init(&rwclock, NULL);

    pthread_t thread_read;
    pthread_t thread_write;

    char path[255] = "712.dat";

    pthread_create(&thread_write, NULL, write_file, path);

    pthread_create(&thread_read, NULL, read_file, path);

    pthread_join(thread_write, NULL);

    pthread_join(thread_read, NULL);

    pthread_rwlock_destroy(&rwclock);

    return 0;
}