#include "net.h"

namespace net {

    NetError::NetError(const std::string &str) : what_str_(str) {
    }

    const char *NetError::what() const noexcept {
        return what_str_.data();
    }


///class Conection
    Connection::Connection(const std::string &str_addr, size_t port) {
        this->connect(str_addr, port);
    }

    Connection::Connection(int fd, const std::string &src_addr,
                                const std::string &dst_addr, size_t src_port, size_t dst_port)
            : fd_(fd), src_addr_(src_addr), dst_addr_(dst_addr), src_port_(src_port), dst_port_(dst_port) {}

    Connection::~Connection() {
        this->close();
        is_readable_ = false;
    }

    Connection::Connection(Connection &&c) noexcept {
        swap(c);
    }

    Connection &Connection::operator=(Connection &&c) noexcept {
        swap(c);
        return *this;
    }

    void Connection::close() {
        ::close(fd_);
    }

    size_t Connection::write(const std::string &str, size_t len) {
        ssize_t res = ::write(fd_, str.data(), len);
        if (res < 0) {
            throw NetError("write error");
        }
        return static_cast<size_t>(res);
    }

    size_t Connection::read(std::string &str, size_t len) {
        ssize_t res = ::read(fd_, str.data(), len);
        if (res < 0) {
            throw NetError("read error");
        } else if (res == 0) {
            is_readable_ = false;
        }
        return static_cast<size_t>(res);
    }

    void Connection::writeExact(const std::string &data, size_t len) {
        size_t res = this->write(data, len);
        if (res != len) {
            throw NetError("writeExact error");
        }
    }

    void Connection::readExact(std::string &data, size_t len) {
        size_t res = this->read(data, len);
        if (res != len) {
            throw NetError("readExact error");
        }
    }

    bool Connection::is_opened() {
        return is_readable_;
    }

    void Connection::swap(Connection & c) {
        std::swap(fd_, c.fd_);
        std::swap(is_readable_, c.is_readable_);
        std::swap(dst_addr_, c.dst_addr_);
        std::swap(src_addr_, c.src_addr_);
        std::swap(dst_port_, c.dst_port_);
        std::swap(src_port_, c.src_port_);
    }

    void Connection::set_timeout(int seconds) {
        timeval timeout {
                .tv_sec = seconds,
        };

        if (setsockopt(fd_, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout)) < 0) {
            throw(NetError("connection set_timeout: setsockopt error"));
        }
        if (setsockopt(fd_, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
            throw(NetError("connection set_timeout: setsockopt error"));
        }
    }

    void Connection::connect(const std::string &str_addr, size_t port) {
        this->close();
        fd_ = socket(AF_INET, SOCK_STREAM, 0);
        if (fd_ < 0) {
            throw NetError("error creating socket");
        }

        sockaddr_in addr{};
        if (inet_aton(str_addr.data(), &addr.sin_addr) < 0) {
            ::close(fd_);
            throw NetError("error parse ip address");
        }
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);

        if (::connect(fd_, (sockaddr *) &addr, sizeof(addr)) < 0) {
            ::close(fd_);
            throw NetError("connect error");
        }

        sockaddr_in my_addr{};
        socklen_t addr_size = sizeof(my_addr);
        getsockname(fd_, (sockaddr *) &my_addr, &addr_size);
        char *result = inet_ntoa(my_addr.sin_addr);
        src_addr_ = std::string(result);
        src_port_ = ntohs(my_addr.sin_port);
        dst_addr_ = str_addr;
        dst_port_ = port;
        is_readable_ = true;

    }

    const std::string &Connection::get_dst_addr() const {
        return dst_addr_;
    }

    const std::string &Connection::get_src_addr() const {
        return src_addr_;
    }

    size_t Connection::get_dst_port() const {
        return dst_port_;
    }

    size_t Connection::get_src_port() const {
        return src_port_;
    }

///class Server

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
