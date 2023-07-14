# 网络编程

## 文件描述符
文件描述符是操作系统中用于标识一个打开的文件的整数值。在C++中，我们通常使用文件描述符来进行文件的读写操作。
文件描述符是一个非负的整数，其中**0、1、2是系统保留的标准输入、标准输出和标准错误输出**的文件描述符。

## I/O多路复用
### 同步阻塞总结

单线程：某个socket阻塞，会影响到其他socket处理

多线程：客户端较多时，会造成资源浪费，全部socket中可能每个时刻只有几个就绪。同时，线程的调度、上下文切换乃至它们占用的内存，可能都成为瓶颈。
### 同步非阻塞总结

从操作系统层面解决了阻塞问题

优点：单个socket阻塞，不会影响到其他socket

缺点：需要不断的遍历进行系统调用，有一定开销

### select

```
/**
* 函数原型
* 获取就绪事件
*
* @param nfds      3个监听集合的文件描述符最大值+1
* @param readfds   要监听的可读文件描述符集合
* @param writefds  要监听的可写文件描述符集合
* @param exceptfds 要监听的异常文件描述符集合
* @param timeout   本次调用的超时时间 等于0时：不等待； 等于-1时：无限等待
* @return          大于0：已就绪的文件描述符数量； 等于0：超时； 小于0：出错
**/

int select(int nfds, fd_set* readfds, fd_set* writefds, fd_set* exceptfds, struct timeval* timeout);
```
总结：

将socket是否就绪检查逻辑下沉到操作系统层面，避免大量系统调用。告诉你有事件就绪，但没告诉你具体是哪个FD。

优点：
不需要每个FD都进行一次系统调用，解决了频繁的用户态内核态切换问题

缺点：
单进程监听的FD存在限制，默认1024
每次调用都需要将FD从用户态拷贝到内核态
不知道具体是哪个文件描述符就绪，需要遍历全部文件描述符
入参的3个 fd_set集合每次调用都需要重置

### poll

```
/**
* 获取就绪事件
*
* @param pollfd   要监听的文件描述符集合
* @param nfds     文件描述符数量
* @param timeout  本次调用的超时时间
* @return         大于0：已就绪的文件描述符数量；等于0：超时；小于0：出错
*/
int poll(struct pollfd* fds,
        unsigned int nfds,
        int timeout);

struct pollfd {
    int fd;        // 监听的文件描述符
    short events;  // 监听的事件
    short revents; // 就绪的事件
} 
```

总结：

跟select基本类似，主要优化了监听1024的限制

优点：

不需要每个FD都进行一次系统调用，导致频繁的用户态内核态切换

缺点：

每次调用都需要将FD从用户态拷贝到内核态

不知道具体是哪个文件描述符就绪，需要遍历全部文件描述符

### epoll
```
/**
* 创建一个epoll
*
* @param size epoll要监听的文件描述符数量
* @return epoll的文件描述符
*/

int epoll_create(int size);

/**
* 事件注册
*
* @param epfd        epoll的文件描述符，epoll_create创建时返回
* @param op          操作类型：新增(1)、删除(2)、更新(3)
* @param fd          本次要操作的文件描述符
* @param epoll_event 需要监听的事件：读事件、写事件等
* @return            如果调用成功返回0，不成功返回-1
*/

int epoll_ctl(int epfd, int op, int fd, struct epoll_event* event);

/**
* 获取就绪事件
* 
* @param epfd       epoll的文件描述符，epoll_create创建时返回
* @param events     用于回传就绪的事件
* @param maxevents  每次能处理的最大事件数
* @param timeout    等待I/O时间发生的超时时间，-1相当于阻塞，0相当于非阻塞
* @return           大于0：已就绪的文件描述符数；等于0：超时；小于0：出错
*/

int epoll_wait(int epfd, struct epoll_event* events, int maxevents, int timeout);
```
总结：

高效处理高并发下的大量连接，同事拥有非常优异的性能。

优点：

支持水平触发和边缘触发两种工作模式；

内部采用红黑树组织事件集合，查找速度快；

只需要调用一次epoll_ctl函数将所有需要监听的socket加入到内核中，不必每次调用都传递整个事件集合；

当有新的连接到达或者已经就绪的socket出现读写事件时会产生EPOLLIN或EPOLLOUT等类型的事件，并通过epoll_wait函数返回，从而避免了轮询的过程；

支持边缘触发模式，只有在socket状态发生改变时才会返回事件，可以减少事件通知次数，提高效率。

缺点：

跨平台性不够好，只支持linux；

相较于select，epoll没有select更轻量和可移植性强；

在监听连接数和事件较少的场景下，select可能更优。

LE VS ET

LT：Level-triggered，水平（条件）触发，默认。epoll_wait检测到事件后，如果该事件没有被处理完，后续每次epoll_wait都会返回该事件。

ET: Edge-triggered，边缘触发。epoll_wait检测到事件后，只会在当次返回该事件，不管该事件是否被处理完毕。


## select/poll 与 epoll
[源码地址](www.kernel.org)
