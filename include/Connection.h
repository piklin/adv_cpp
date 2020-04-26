#ifndef HW4_CONNECTION_H
#define HW4_CONNECTION_H

#include <iostream>
#include <sys/socket.h>


#include "FileDescriptor.h"
#include "NetErrors.h"
#include "Server.h"

namespace mp {
    class Connection {
    public:
        Connection();
        ~Connection();

        Connection(const Connection &) = delete;

        Connection &operator=(const Connection &) = delete;

        Connection(Connection &&) noexcept;

        Connection &operator=(Connection &&) noexcept;

        void close();

        void swap(Connection &);

        int write(const std::string &, int);

        int read(std::string &, int);

        void clear_input();

        std::string &input();

        size_t input_size();

        void clear_output_size();

        size_t output_size();

        const std::string &get_dst_addr() const;

        const std::string &get_src_addr() const;

        size_t get_src_port() const;

        size_t get_dst_port() const;

        uint get_last_event() const;

        friend class Server;
    private:
        Connection(FileDescriptor &, const std::string &, const std::string &, size_t, size_t);
        FileDescriptor fd_;
        std::string dst_addr_;
        std::string src_addr_;
        size_t dst_port_;
        size_t src_port_;

        uint last_event_;
        std::string read_buf_;
        size_t writed_;
    };

}

#endif //HW4_CONNECTION_H
