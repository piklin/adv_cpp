#ifndef HW4_SERVER_H
#define HW4_SERVER_H

#include <iostream>
#include <functional>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <map>

#include "Connection.h"
#include "NetErrors.h"
#include "FileDescriptor.h"


namespace mp {
    class Connection;
    using Callback = std::function<void(Connection&)>;
    constexpr size_t events_count = 64;

    class Server {
    public:
        Server(const std::string &ip_address, size_t port, size_t, const Callback &handler);
        ~Server();
        Server(const Server &) = delete;
        Server &operator=(const Server &) = delete;
        Server(Server &&) = delete;
        Server &operator=(Server &&) = delete;

        void loop();
    private:
        void accept_clients();
        void handle_client(size_t, uint);
        void epoll_ctl(const FileDescriptor &, int, int) const;
        void add_epoll(const FileDescriptor &, int) const;
        void del_epoll(const FileDescriptor &, int) const;

        FileDescriptor socket_fd_;
        FileDescriptor epoll_fd_;
        std::map<size_t, Connection> connections;
        std::string srv_ip_addr_;
        size_t srv_port_;
        Callback handler_;
    };
}

#endif //HW4_SERVER_H
