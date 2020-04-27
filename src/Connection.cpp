#include "Connection.h"

namespace net {
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
}
