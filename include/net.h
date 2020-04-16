#ifndef HW3_NET_H
#define HW3_NET_H


    class NetError : public std::exception {
    public:
        explicit NetError(std::string);
        override const char *what() const noexcept;
    private:
        std::string what_str_;
    };


    class Connection {
    public:
        Connection(std::string, size_t);
        ~Connection();

        write(std::string, size_t);
        read(std::string, size_t);
        close();
        is_opened();
        writeExact(std::string, size_t);
        readExact(std::string, size_t);
        set_timeout(int);
        connect(std::string, size_t);
    private:
        size_t fd_;
        std::string dst_addr_;
        std::string src_addr_;
        std::string dst_port_;
        std::string src_port_;
    };


    class Server {

    };

#endif //HW3_NET_H

