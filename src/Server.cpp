#include "Server.h"

namespace net {
    Server::Server(const std::string &str_addr, size_t port) : fd_(-1) {
        this->open(str_addr, port);
    }

    Server::~Server() {
        this->close();
        is_opened_ = false;
    }

    Server::Server(net::Server &&s) noexcept{
        swap(s);
    }

    Server &Server::operator=(Server &&s) noexcept {
        swap(s);
        return *this;
    }

    void Server::swap(Server &s) {
        std::swap(fd_, s.fd_);
        std::swap(is_opened_, s.is_opened_);
        std::swap(src_addr_, s.src_addr_);
        std::swap(src_port_, s.src_port_);
    }

    void Server::open(const std::string &str_addr, size_t port) {
        if (fd_ != -1) {
            this->close();
        }
        try {
            fd_ = socket(AF_INET, SOCK_STREAM, 0);
            if (fd_ < 0) {
                throw NetError("Server: error creating socket");
            }

            sockaddr_in addr{};
            if (inet_aton(str_addr.data(), &addr.sin_addr) < 0) {
                throw NetError("Server: error parse ip address");
            }
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port);

            if (bind(fd_, (sockaddr *) &addr, sizeof(addr)) < 0) {
                throw NetError("Server: bind error");
            }
            if (listen(fd_, default_max_connect) < 0) {
                throw NetError("Server: listen error");
            }

            src_addr_ = str_addr;
            src_port_ = port;
            is_opened_ = true;
        } catch (NetError &e) {
            ::close(fd_);
            throw;
        }
    }

    void Server::close() {
        ::close(fd_);
    }

    void Server::set_max_connect(int count) {
        if (listen(fd_, count) < 0) {
            throw NetError("Server: listen error");
        }
    }

    bool Server::is_opened() const {
        return is_opened_;
    }

    Connection Server::accept() {
        sockaddr_in client_addr{};
        socklen_t addr_size = sizeof(client_addr);
        int client_fd = ::accept(fd_, (sockaddr *) &client_addr, &addr_size);

        char *result = inet_ntoa(client_addr.sin_addr);
        return Connection(client_fd, src_addr_, std::string(result), src_port_, ntohs(client_addr.sin_port));
    }
}

