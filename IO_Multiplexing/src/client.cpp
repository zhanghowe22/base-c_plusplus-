#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>

using namespace std;

int main(int argc, char** argv)
{
    int clientFd;
    struct sockaddr_in server_addr;

    memset(&server_addr, 0, sizeof(server_addr));

    if(argc != 3) {
        cout << "Parameter illegal ! Should include : " << argv[0] << " xxx.xxx.xxx.xxx(ip) 1234(port)" <<endl;
        return 0; 
    }

    if((clientFd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        cout << "Create socket failed : " << strerror(errno) << endl;
        return 0;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(atoi(argv[2]));

    cout << "Could to connect..." << endl;

    if((connect(clientFd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)) {
        cout << "Connect failed : " << strerror(errno) << endl;
        return 0;
    }

    cout << "Connect success ! " << endl;

    while(1) {
        char sendBuf[1024];
        char recvBuf[1024];
        int sendLen,recvLen,num=0;

        cout << "Client send : ";

        while ((sendBuf[num] = getchar()) != '\n')
        {
            num++;
        }
        if(num == 0) {
            continue;
        }

        sendLen = write(clientFd, sendBuf, num);

        recvLen = 0;

        while (recvLen < sendLen)
        {
            int readCnt = read(clientFd, &recvBuf[recvLen], sizeof(recvBuf));
            if(readCnt == -1) {
                perror(NULL);
                continue;
            }
            recvLen += readCnt;
        }

        if(recvLen) {
            recvBuf[recvLen] = '\0';
            cout << "Client recv : " << recvBuf << endl;
        } else {
            cout << "Server is closed ! " << endl;
            break;
        }
    }

    close(clientFd);
}
