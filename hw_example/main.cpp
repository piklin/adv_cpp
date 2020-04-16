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
    std::cout << buf << std::endl;

}
