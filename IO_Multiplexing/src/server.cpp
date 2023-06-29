#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>

#define SERV_PORT 6666
#define SERB_IP "127.1.1.1"

using namespace std;

int main(int argc, char** argv)
{
    int serverFd,clientFd;

    struct sockaddr_in server_addr,client_addr; //地址和端口结构体，存放服务端和客户端的地址信息

    memset(&server_addr, 0, sizeof(server_addr));
    memset(&client_addr, 0, sizeof(client_addr));

    // 创建套接字 AF_INET：网络协议为IPV4, SOCK_STREAM：套接口的类型为字节流， 0:默认传输协议，TCP/IP
    if((serverFd = socket(AF_INET, SOCK_STREAM, 0)) == -1) { 
        cout << "Created socket failed : " << strerror(errno) << endl;
        return 0;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERV_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERB_IP); 

    // 绑定到监听套接字上
    if((bind(serverFd, (sockaddr*)&server_addr, sizeof(server_addr))) == -1) {
        cout << "Bind to socket failed : " << strerror(errno) << endl;
        return 0;
    }

    // 将套接字设置为监听状态
    if(listen(serverFd, 1024) == -1) {
        cout << "Listen failed : " << strerror(errno) << endl;
        return 0;
    }

    cout << "Soceket init success ! " << endl;
    cout << "Ip -> " << inet_ntoa(server_addr.sin_addr) << " Port -> " << ntohs(server_addr.sin_port) << endl;

    socklen_t client_size = 0; // 保存客户端的地址结构体大小

    if((clientFd == accept(serverFd, (sockaddr*)&client_addr, &client_size)) == -1) {
        cout << "Accept failed : " << strerror(errno) << endl;
        return 0;
    }

    cout << "Client access : " << inet_ntoa(client_addr.sin_addr) << " " << ntohs(client_addr.sin_port) << endl;

    char buf[1024]; // 数据接收缓冲区

    while(1) {
        int rc;
        if((rc = read(clientFd, buf, sizeof(buf))) > 0) {
            int num = 0;
            cout << "Server recv : ";
            for(num = 0; num < rc; num++) {
                cout << buf[num];
                buf[num] = toupper(buf[num]);
            }
            buf[num] = '\0';
            cout << endl;
            write(clientFd, buf, strlen(buf));
        } else if(rc == 0) {
            cout << "Client exit " << endl;
            return 0; 
        }
    }

    close(serverFd);
    close(clientFd);
    return 0;

}