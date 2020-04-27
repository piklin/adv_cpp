#ifndef HW3_SERVER_H
#define HW3_SERVER_H

#include <iostream>
#include <unistd.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

//#include "Connection.h"
#include "NetError.h"

namespace net {

    int default_max_connect = 16;

    class Server {
    public:
        Server(const std::string &, size_t);
        ~Server();
        Server(const Server &) = delete;
        Server &operator=(const Server &) = delete;
        Server(Server &&) noexcept;
        Server &operator=(Server &&) noexcept;

        void swap(Server &);
        void close();
        Connection accept();
        bool is_opened() const;
        void open(const std::string &, size_t);
        void set_max_connect(int);
    private:
        int fd_;
        bool is_opened_;
        std::string src_addr_;
        size_t src_port_;
    };
}

#endif //HW3_SERVER_H
