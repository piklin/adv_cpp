#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <sys/epoll.h>

#ifndef SOCK_NONBLOCK
#include <fcntl.h>
#define SOCK_NONBLOCK O_NONBLOCK
#endif


int main(){
    std::string ip_addr("127.0.0.1");
    size_t port = 34567;

    int socket_fd = ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);

    sockaddr_in srv_addr {};
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_port = htons(port);
    if (inet_aton(ip_addr.data(), &srv_addr.sin_addr) == 0) {
        exit(EXIT_FAILURE);
    }

    socklen_t addr_size = sizeof(srv_addr);

    if (::bind(socket_fd, (sockaddr *)&srv_addr, addr_size) < 0) {
        exit(EXIT_FAILURE);
    }

    if (::listen(socket_fd, 256) < 0) {
        exit(EXIT_FAILURE);
    }

    int epoll_fd = epoll_create(1);



    return 0;
}

