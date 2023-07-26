#include "ring_queue.h"

bool RingQueue::isFull()
{
    if((m_tailIndex + 1) % m_maxCapacity == m_headIndex) {
        return true;
    }

    return false;
}

void RingQueue::enQueue(int value)
{
    if(isFull()) {
        m_headIndex = (m_headIndex + 1) % m_maxCapacity;
    }
    m_dataBuf[m_tailIndex] = value;
    std::cout << "Push to queue : " << m_dataBuf[m_tailIndex] << std::endl;
    m_tailIndex = (m_tailIndex + 1) % m_maxCapacity;
}

int RingQueue::getSize()
{
    return (m_tailIndex - m_headIndex + m_maxCapacity) % m_maxCapacity;
}

void RingQueue::showQueue()
{
    std::cout << "Num in queue  is : ";
    for(int i = m_headIndex; i < m_headIndex + getSize(); i++) {
        std::cout << m_dataBuf[i] << " "; 
    }
    std::cout << std::endl;
}

void RingQueue::deQueue()
{
    std::cout << "Pop from queue : ";
    std::cout << m_dataBuf[m_headIndex] << std::endl;
    m_headIndex = (m_headIndex + 1) % m_maxCapacity; 
}

int main()
{
    RingQueue my_queue(5);
    for(int i = 0; i < 5; i++) {
        my_queue.enQueue(i);
    }

    std::cout << "-------------------------------" << std::endl;

    my_queue.showQueue();

    std::cout << "-------------------------------" << std::endl;

    my_queue.deQueue();

    std::cout << "-------------------------------" << std::endl;

    my_queue.showQueue();

    return 0;
}