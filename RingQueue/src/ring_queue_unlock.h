/*
// 实现无锁的线程安全(并发)的循环队列
*/
// TODO: 实现无锁并发环形队列

#ifndef __RING_QUEUE_UNLOCK__
#define __RING_QUEUE_UNLOCK__

#include <iostream>
using namespace std;

template <typename T>
class Ring_Queue
{
public:
    Ring_Queue(int capacity) : m_capacity(capacity), m_data(nullptr), m_front(0), m_rear(0) {
        if(capacity < 1) {
            exit(1);
        } else {
            m_data = new T(capacity * sizeof(T));
        }
    }

    ~Ring_Queue() {
        delete[] m_data;
    }

    // 入队
    void ring_push(const T* m_data, int len);

    // 出队
    void ring_pop();

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
    T* m_data;       // 顺序队列的存储数组
    int m_front;     // 队首索引
    int m_rear;      // 队尾索引
    int m_capacity;  // 队列容量
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
void Ring_Queue<T>::ring_push(const T* data, int len) {
    if(ring_is_full()) {
        m_front = (m_front + len) % m_capacity;
    }

    memcpy(m_data, data, len * sizeof(T));

    for(int i = 0; i < len; i++) {
        cout << *(m_data + i) << " ";
    }
    cout << endl;

    m_rear = (m_rear + len) % m_capacity;
}

template <typename T>
void Ring_Queue<T>::ring_pop() {
    if(!ring_is_empty()) {
        m_front = (m_front + 1) % m_capacity;
    } else {
        exit(1);
    }
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