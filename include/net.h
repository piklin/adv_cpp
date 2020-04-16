#ifndef HW3_NET_H
#define HW3_NET_H

#include <iostream>
#include <unistd.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

int default_max_connect = 16;

namespace net {
    class Connection;

    class NetError : public std::exception {
    public:
        explicit NetError(std::string);
        const char *what() const noexcept override;
    private:
        std::string what_str_;
    };


    class Server {
    public:
        Server(std::string, size_t);
        ~Server();

        void close();
        Connection accept();
        bool is_opened() const;
        void open(std::string, size_t);
        void set_max_connect(int);
    private:
        int fd_;
        bool is_opened_;
        std::string src_addr_;
        size_t src_port_;
    };

    class Connection {
    public:
        Connection(std::string, size_t);

        ~Connection();

        size_t write(std::string &, size_t);
        size_t read(std::string &, size_t);
        void close();
        bool is_opened();
        void writeExact(std::string, size_t);
        void readExact(std::string, size_t);
        void set_timeout(int);
        void connect(std::string, size_t);

        std::string get_dst_addr();
        std::string get_src_addr();
        size_t get_src_port();
        size_t get_dst_port();

        friend Connection net::Server::accept();
    private:
        Connection(int, std::string, std::string, size_t, size_t);

        int fd_;
        bool is_readable_;
        std::string dst_addr_;
        std::string src_addr_;
        size_t dst_port_;
        size_t src_port_;
    };

}
#endif //HW3_NET_H

