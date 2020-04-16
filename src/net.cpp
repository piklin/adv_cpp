#include "net.h"

net::NetError::NetError(std::string str) : what_str_(str){
}

const char *net::NetError::what() const noexcept {
    return what_str_.data();
}


///class Conection
net::Connection::Connection(std::string str_addr, size_t port){
    this->connect(str_addr, port);
}

net::Connection::Connection(int fd, std::string src_addr,
                std::string dst_addr, size_t src_port, size_t dst_port)
                : fd_(fd), src_addr_(src_addr), dst_addr_(dst_addr), src_port_(src_port), dst_port_(dst_port) {}

net::Connection::~Connection() {
    this->close();
    is_readable_ = false;
}

void net::Connection::close() {
    ::close(fd_);
}

size_t net::Connection::write(std::string &str, size_t len) {
    ssize_t res = ::write(fd_, str.data(), len);
    if (res < 0) {
        throw NetError("write error");
    }
    return static_cast<size_t>(res);
}

size_t net::Connection::read(std::string &str, size_t len) {
    ssize_t res = ::read(fd_, str.data(), len);
    if (res < 0) {
        throw NetError("read error");
    } else if (res == 0) {
        is_readable_ = false;
    }
    return static_cast<size_t>(res);
}

void net::Connection::writeExact(std::string data, size_t len) {
    size_t res = this->write(data, len);
    if (res != len) {
        throw NetError("writeExact error");
    }
}

void net::Connection::readExact(std::string data, size_t len) {
    size_t res = this->read(data, len);
    if (res != len) {
        throw NetError("readExact error");
    }
}

bool net::Connection::is_opened() {
    return is_readable_;
}

void net::Connection::set_timeout(int seconds) {
    //i have problem
}

void net::Connection::connect(std::string str_addr, size_t port) {
    this->close();
    fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (fd_ < 0) {
        throw NetError("error creating socket");
    }

    sockaddr_in addr {};
    if (inet_aton(str_addr.data(), &addr.sin_addr) < 0) {
        ::close(fd_);
        throw NetError("error parse ip address");
    }
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    if (::connect(fd_, (sockaddr *)&addr, sizeof(addr)) < 0) {
        ::close(fd_);
        throw NetError("connect error");
    }

    sockaddr_in my_addr {};
    socklen_t addr_size = sizeof(my_addr);
    getsockname(fd_, (sockaddr *)&my_addr, &addr_size);
    char *result = inet_ntoa(my_addr.sin_addr);
    src_addr_ = std::string(result);
    src_port_ = ntohs(my_addr.sin_port);
    dst_addr_ = str_addr;
    dst_port_ = port;
    is_readable_ = true;

}

///class Server

net::Server::Server(std::string str_addr, size_t port) : fd_(-1) {
    this->open(str_addr, port);
}

net::Server::~Server() {
    this->close();
    is_opened_ = false;
}

void net::Server::open(std::string str_addr, size_t port) {
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

void net::Server::close() {
    ::close(fd_);
}
void net::Server::set_max_connect(int count) {
    if (listen(fd_, count) < 0) {
        throw NetError("Server: listen error");
    }
}

bool net::Server::is_opened() const {
    return is_opened_;
}

net::Connection net::Server::accept() {
    sockaddr_in client_addr {};
    socklen_t addr_size = sizeof(client_addr);
    int client_fd = ::accept(fd_, (sockaddr *)&client_addr, &addr_size);

    char *result = inet_ntoa(client_addr.sin_addr);
    return Connection(client_fd, src_addr_, std::string(result), src_port_, ntohs(client_addr.sin_port));
}
