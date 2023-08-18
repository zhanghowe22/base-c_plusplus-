/*
// 实现无锁的线程安全(并发)的循环队列
*/
// TODO: 实现无锁并发环形队列

#ifndef __RING_QUEUE_UNLOCK__
#define __RING_QUEUE_UNLOCK__

#include <iostream>
#include <atomic>
using namespace std;

template <typename T>
class Ring_Queue
{
public:
    Ring_Queue(unsigned int capacity) : m_capacity(capacity), m_data(nullptr), m_front(0), m_rear(0), m_write(0) {
        if(m_capacity < 1) {
            exit(1);
        } else if(m_capacity & (m_capacity - 1)) {
            // 必须确保队列的容量是2的幂
            // 这样才可以使用&替换%求出当前读写索引的位置，&操作比%操作更高效，是在内存中进行的
            cout << "[Error] queue capacity must be a power of 2" << endl;
            exit(1);
        } else {
            m_data = new T(capacity * sizeof(T));
        }
    }

    ~Ring_Queue() {
        delete[] m_data;
    }

    // 入队
    unsigned int ring_push(const T* m_data,unsigned int len);

    // 出队
    unsigned int ring_pop(T *data,unsigned int len);

    // 返回队首元素
    T& ring_front();

    // 返回队尾元素
    T& ring_rear();

    // 队列判空
    bool ring_is_empty();

    // 队列判满
    bool ring_is_full();

    // 队列内元素个数
    int ring_size();

private:
    T* m_data;     // 顺序队列的存储数组
    unsigned int m_capacity;  // 队列容量
    atomic<unsigned int> m_front; // 队首索引
    atomic<unsigned int> m_rear; // 队尾索引
    atomic<unsigned int> m_write;     // 引入一个写指针，避免一个线程Push后rear后移未写入数据时，另一个线程Pop数据引起的问题
};

template <typename T>
bool Ring_Queue<T>::ring_is_empty() {
    if(m_front == m_rear) {
        return true;
    } else {
        return false;
    }
}

template <typename T>
bool Ring_Queue<T>::ring_is_full() {
    if((m_rear + 1) % m_capacity == m_front) {
        return true;
    } else {
        return false;
    }
}

template <typename T>
unsigned int Ring_Queue<T>::ring_push(const T* val, unsigned int len)
{
    unsigned int l;
    unsigned int length;

    unsigned int rId,wId;
    do {
        rId = m_rear.load();
    } while (!m_rear.compare_exchange_weak(rId, (rId + len) & (m_capacity - 1)));

    length = min(len, m_capacity - rId + m_front);

    /* 首先从m_rear处开始写数据，到缓冲区末端结束*/
    l = min(length, m_capacity - (rId & (m_capacity - 1)));
    memcpy(m_data + (rId & (m_capacity - 1)), val, l);

    /* 然后将剩余数据从缓冲区起始位置开始写入*/

    memcpy(m_data, val + l, len - l);

    do {
        wId = rId;
    } while (!m_write.compare_exchange_weak(wId, (wId + len) & (m_capacity - 1)));

    return len;
}

template <typename T>
unsigned int Ring_Queue<T>::ring_pop(T* val, unsigned int len)
{
    unsigned int l;
    unsigned int length;
    unsigned int fId;

    do {
        fId = m_front.load();
        if(fId == m_write.load()) {
            exit(1);
        }
    } while (!m_front.compare_exchange_weak(fId, (fId + len) & (m_capacity - 1)));

    length = min(len, m_rear - fId);

    /* 首先从m_front获取数据，直到缓冲区结束 */
    l = min(len, m_capacity - (fId & (m_capacity - 1)));
    memcpy(val, m_data + (fId & (m_capacity - 1)), l);

    /* 然后从缓冲区起始位置获取剩余数据 */
    memcpy(val + l, m_data, len - l);

    return len;
}

template <typename T>
T& Ring_Queue<T>::ring_front()
{
    if(!ring_is_empty()) {
        return m_data[(m_front) % m_capacity];
    } else {
        exit(1);
    }
}

template <typename T>
T& Ring_Queue<T>::ring_rear()
{
    if(!ring_is_empty()) {
        return m_data[m_rear - 1];
    } else {
        exit(1);
    }
}

template <typename T>
int Ring_Queue<T>::ring_size()
{
    return (m_rear - m_front + m_capacity) % m_capacity;
}


#endif