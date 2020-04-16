#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#include <cstdio>
#include "net.h"


int main() {
    net::Server s("127.0.0.1", 34567);

    net::Connection a = s.accept();


    //net::Connection a("127.0.0.1", 23456);
    std::string b("lol");
    a.write(b, 3);
    std::string buf(10, '\0');
    a.read(buf, 5);
    std::cout << buf << std::endl;



   /* int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(23456);
    inet_aton("127.0.0.1", &addr.sin_addr);

    bind(socket_fd, (sockaddr *)&addr, sizeof(addr));
    listen(socket_fd, 1);
    sockaddr_in client_addr {};
    socklen_t addr_size = sizeof(client_addr);
    int client_fd = accept(socket_fd, (sockaddr *)&client_addr, &addr_size);

    while(1) {
        char buf[10];
        std::cout << read(client_fd, &buf, 6);
        std::cout << std::string(buf) << std::endl;
        sleep(1);
    }



    connect(socket_fd, (sockaddr *)&addr, sizeof(addr));
    while(1) {
        write(socket_fd, "hello!", 6);
        sleep(1);
    }*/
}
