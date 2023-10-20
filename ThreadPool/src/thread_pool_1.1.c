#include "thread_pool_1.1.h"

// 创建任务队列
static task_queue_t* 
__task_queue_create() {
    task_queue_t* task_queue = (task_queue_t*)malloc(sizeof(*task_queue));
    if(!task_queue) return NULL;

    int ret;
    ret = pthread_mutex_init(&task_queue->mutex, NULL);
    if(ret == 0) {
        ret = pthread_cond_init(&task_queue->cond, NULL);
        if(ret == 0) {
            spinlock_init(&task_queue->lock);
            task_queue->head = NULL;
            task_queue->tail = &task_queue->head;
            task_queue->block = 1; // 默认为阻塞:1
            return task_queue;
        }
        pthread_cond_destroy(&task_queue->cond);
    }
    pthread_mutex_destroy(&task_queue->mutex);
    return NULL;
}

// 向任务队列中添加任务
static inline void
__add_task(task_queue_t* queue, void* task) {
    void** link = (void**)task;
    *link = NULL;
    spinlock_lock(&queue->lock);
    *queue->tail = link;
    queue->tail = link;
    spinlock_unlock(&queue->lock);
    pthread_cond_signal(&queue->cond);
}

// 获取任务队列中的任务
static inline void*
__pop_task(task_queue_t* queue) {
    spinlock_lock(&queue->lock);

    // 队列是否为空
    if(queue->head == NULL) { 
        spinlock_unlock(&queue->lock);
        return NULL;
    }

    task_t *task;
    task = queue->head;
    queue->head = task->next;

    // 判断队列中是否只有一个任务
    if(queue->head == NULL) { 
        queue->tail = &queue->head;
    }

    spinlock_unlock(&queue->lock);
    return task;
}

// 如果没有任务的话且为阻塞模式，让当前线程休眠
static inline void*
__get_task(task_queue_t* queue) {
    task_t* task;
    while ((task = __pop_task(queue)) == NULL )
    {
        pthread_mutex_lock(&queue->mutex);
        // 为非阻塞模式时，直接返回
        if(queue->block == 0) { 
            pthread_mutex_unlock(&queue->mutex);
            break;
        }
        // 线程休眠位置
        pthread_cond_wait(&queue->cond, &queue->mutex); 
        pthread_mutex_unlock(&queue->mutex);
    }
    return task;
}

// 设置任务队列为非阻塞
static void
__nonblock(task_queue_t* queue) {
    pthread_mutex_lock(&queue->mutex);
    queue->block = 0;
    pthread_mutex_unlock(&queue->mutex);

    // pthread_cond_broadcast 广播，将所有休眠的线程唤醒
    pthread_cond_broadcast(&queue->cond); 
}

// 销毁任务队列
static void
__task_queue_destory(task_queue_t* queue) {
    task_t* task;
    while(task = __pop_task(queue)) {
        // 销毁任务
        free(task);
    }

    pthread_cond_destroy(&queue->cond);
    pthread_mutex_destroy(&queue->mutex);
    spinlock_destroy(&queue->lock);

    // 销毁任务队列
    free(queue);
}

static void*
__thread_worker(void* arg) {
    thread_pool_t* pool = (thread_pool_t*) arg;
    task_t* task;
    void *ctx;
    while(atomic_load(&pool->qiut) == 0) {
        task = __get_task(pool->task_queue);
        if(!task) break;
        handler_pt func = task->func;
        ctx = task->arg;
        free(task);
        func(ctx);
    }
    return NULL;
}

// 线程安全退出
static void
__thread_terminate(thread_pool_t* pool) {
    // 通过原子操作，标记为退出
    atomic_store(&pool->qiut, 1);
    __nonblock(pool->task_queue);
    for(int i = 0; i < pool->thread_count; ++i) {
        pthread_join(pool->threads[i], NULL);
    }
}

static int
__thread_cteate(thread_pool_t* pool, size_t thread_count) {
    pthread_attr_t attr;
    int ret;
    ret = pthread_attr_init(&attr);

    if(ret == 0) {
        pool->threads = (pthread_t*)malloc(sizeof(pthread_t) * pool->thread_count);
        if(pool->threads) {
            int cnt;
            for (cnt = 0; cnt < thread_count; cnt++) {
                if(pthread_create(&pool->threads[cnt], &attr, __thread_worker, pool) != 0) {
                    break;
                }
            }
            pool->thread_count = cnt;
            pthread_attr_destroy(&attr);
            // 成功创建指定数量的线程
            if(cnt == thread_count) {
                return 0;
            } else { //未成功创建指定数量的线程，
                __thread_terminate(pool);
            }
        }
        ret = -1;
    }
    return ret;
}

void
thread_pool_terminate(thread_pool_t* pool) {
    atomic_store(&pool->qiut, 1);
    __nonblock(pool->task_queue);
}

// 创建线程池
thread_pool_t*
thread_pool_create(int thread_count) {
    thread_pool_t* pool;
    pool = (thread_pool_t*) malloc(sizeof(*pool));
    if(!pool) {
        printf("Malloc failed\n");
        return NULL;
    }

    task_queue_t* queue = __task_queue_create();
    if(queue) {
        pool->task_queue = queue;
        ATOM_INIT(&pool->qiut, 0);
        if(__thread_cteate(pool, thread_count) == 0) {
            return pool;
        }
        __task_queue_destory(pool->task_queue);
    }

    free(pool);
    pool = NULL;
    return NULL;
} 

int 
thread_pool_post(thread_pool_t* pool, handler_pt func, void* arg) {
    // 标识为退出的话，直接return -1
    if(atomic_load(&pool->qiut) == 1) {
        return -1;
    }
    task_t* task = malloc(sizeof(task_t));
    if(!task) {
        free(task);
        task = NULL;
        return -1;
    }
    task->func = func;
    task->arg = arg;
    __add_task(pool->task_queue, task);
    return 0;
}

void
thread_pool_waitdone(thread_pool_t* pool) {
    for(int i = 0; i < pool->thread_count; ++i) {
        pthread_join(pool->threads[i], NULL);
    }
    __task_queue_destory(pool->task_queue);
    free(pool->threads);
    pool->threads = NULL;
    free(pool);
    pool = NULL;
}