#ifndef HW3_CONNECTION_H
#define HW3_CONNECTION_H

#include <iostream>
#include <unistd.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

#include "Server.h"
#include "NetError.h"

namespace net {

    class Connection {
    public:
        Connection(const std::string &, size_t);
        ~Connection();
        Connection(const Connection &) = delete;
        Connection &operator=(const Connection &) = delete;
        Connection(Connection &&) noexcept;
        Connection &operator=(Connection &&) noexcept;

        size_t write(const std::string &, size_t);
        size_t read(std::string &, size_t);
        void close();
        bool is_opened();
        void swap(Connection &);
        void writeExact(const std::string &, size_t);
        void readExact(std::string &, size_t);
        void set_timeout(int);
        void connect(const std::string &, size_t);

        const std::string &get_dst_addr() const;
        const std::string &get_src_addr() const;
        size_t get_src_port() const;
        size_t get_dst_port() const;

        friend Connection net::Server::accept();
    private:
        Connection(int, const std::string &, const std::string &, size_t, size_t);

        int fd_;
        bool is_readable_;
        std::string dst_addr_;
        std::string src_addr_;
        size_t dst_port_;
        size_t src_port_;
    };
}

#endif //HW3_CONNECTION_H
