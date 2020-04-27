#ifndef HW3_NETERROR_H
#define HW3_NETERROR_H

#include <iostream>
#include <unistd.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

namespace net {
    class NetError : public std::exception {
    public:
        explicit NetError(const std::string &);
        const char *what() const noexcept override;
    private:
        std::string what_str_;
    };
}

#endif //HW3_NETERROR_H
