#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <stdio.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "spinlock.h"
#include "atomic.h"

typedef struct spinlock spinlock_t;
typedef void(*handler_pt)(void*);

// 任务
typedef struct task_s {
    void* next;
    handler_pt func;
    void* arg;
} task_t;

// 任务队列
typedef struct task_queue_s {
    void *head;
    void **tail;
    int block;
    spinlock_t lock; // 操作队列临界资源的自旋锁 为什么使用自旋锁：锁的时间比较短
    pthread_mutex_t mutex; // 管理任务队列中任务的锁
    pthread_cond_t cond;
} task_queue_t;

// 线程池
typedef struct thread_pool_s {
    task_queue_t* task_queue; // 任务队列
    ATOM_INT qiut; // 标记是否退出 原子类型
    int thread_count; // 线程池中线程数量
    pthread_t* threads; // 若干个线程数据
} thread_pool_t;

thread_pool_t* thread_pool_create(int thread_count);

void thread_pool_terminate(thread_pool_t* pool);

int thread_pool_post(thread_pool_t* pool, handler_pt func, void* arg);

void thread_pool_waitdone(thread_pool_t* pool);


#endif