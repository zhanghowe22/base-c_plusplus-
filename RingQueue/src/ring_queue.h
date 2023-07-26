#include <iostream>
#include <cstdlib>
#include <memory.h>

class RingQueue
{
public:
    int m_maxCapacity; // 队列的最大容量

    // 队列是先进先出，故头索引位置为读，尾索引为写
    int m_headIndex; // 头索引（读）
    int m_tailIndex; // 尾索引（写）

    int *m_dataBuf; // 数据buf

    bool m_tag; // 队列是否满的标志

public:
    // 构造函数
    RingQueue(int maxCapacity) {
        m_maxCapacity = maxCapacity;
        m_headIndex = 0;
        m_tailIndex = 0;
        m_dataBuf = new int[maxCapacity];
        memset(m_dataBuf, 0, sizeof(int) * m_maxCapacity);
        m_tag = false;
    }

    // 析构函数
    ~RingQueue() {
        delete m_dataBuf;
        m_dataBuf = NULL;
    }

    // 入队
    void enQueue(int value);

    // 出队
    void deQueue();

    // 判断队列是否满
    bool isFull();

    // 判断队列是否为空
    bool isEmpty();

    // 队列中元素个数
    int getSize();

    // 打印队列中元素
    void showQueue();

    // 显示队列头元素
    void showQueueHead();
};