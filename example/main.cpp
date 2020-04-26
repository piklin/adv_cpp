#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <sys/epoll.h>

#include "Server.h"
#include "Connection.h"

int main(){

    auto handle = [](mp::Connection &conn) {
        constexpr uint str_size = 8;
        uint event = conn.get_last_event();
        if (event & EPOLLERR || event & EPOLLHUP) {
            std::cout << conn.get_dst_addr() << ":" << conn.get_dst_port() << " closed" << std::endl;
        } else if (event & EPOLLIN) {
            std::string str(str_size - conn.input_size(), '\0');
            conn.read(str, str_size - conn.input_size());

            if (conn.input_size() == str_size) {
                conn.write(conn.input(), conn.input_size());
                conn.clear_input();
            }

        }
    };
    try {
        mp::Server s("127.0.0.1", 34567, 10, handle);
        s.loop();
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }
    return 0;
}

