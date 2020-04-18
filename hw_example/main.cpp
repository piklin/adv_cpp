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

    std::string b("pam");
    a.write(b, 3);
    std::string buf(10, '\0');
    a.read(buf, 5);
    net::Connection a2(std::move(a));
    a.close();
    a2.write(b, 3);

    net::Server ss = std::move(s);

    net::Connection aa = ss.accept();
    aa.write(b, 3);

    std::cout << buf << std::endl;

    std::cout << a2.get_dst_port() << "  " << a2.get_src_addr() << std::endl;
}
