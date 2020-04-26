#include "Server.h"

namespace mp {
    Server::Server(const std::string &ip_address, size_t port,
            size_t clients_count, const Callback &handler) :
            srv_ip_addr_(ip_address), srv_port_(port), handler_(handler) {

        socket_fd_ = ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);

        sockaddr_in srv_addr {};
        srv_addr.sin_family = AF_INET;
        srv_addr.sin_port = htons(port);
        if (inet_aton(ip_address.data(), &srv_addr.sin_addr) == 0) {
            throw NetErrors("inet_aton error");
        }

        socklen_t addr_size = sizeof(srv_addr);

        if (::bind(socket_fd_.to_int(), (sockaddr *)&srv_addr, addr_size) < 0) {
            throw NetErrors("bind error");
        }

        if (::listen(socket_fd_.to_int(), clients_count) < 0) {
            throw NetErrors("listen error");
        }

        epoll_fd_ = epoll_create(1);
        if (epoll_fd_.to_int() < 0) {
            throw NetErrors("epoll_create error");
        }

        add_epoll(socket_fd_, EPOLLIN);
    }

    Server::~Server() {
        socket_fd_.close();
        epoll_fd_.close();
    }

    void Server::loop() {
        epoll_event events[events_count];

        while(true) {
            int nfds = epoll_wait(epoll_fd_.to_int(), events, events_count, -1);
            if (nfds < 0) {
                if (errno == EINTR) {
                    continue;
                } else {
                    throw NetErrors("epoll_wait error");
                }
            }
            for (size_t i = 0; i < nfds; i++) {
                if (socket_fd_.to_int() == events[i].data.fd) {
                    accept_clients();
                } else {
                    handle_client(events[i].data.fd, events[i].events);
                }
            }
        }
    }

    void Server::accept_clients() {
        sockaddr_in client_addr{};
        socklen_t addr_len = sizeof(client_addr);
        while(true) {
            FileDescriptor client_fd = accept4(socket_fd_.to_int(), (sockaddr *)&client_addr, &addr_len, SOCK_NONBLOCK);
            if (client_fd.to_int() < 0) {
                if (errno == EINTR) {
                    continue;
                } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
                    return;
                } else {
                    throw NetErrors("accept error");
                }
            }

            add_epoll(client_fd, EPOLLIN);

            size_t cl_fd = client_fd.to_int();
            char *client_ip_str = inet_ntoa(client_addr.sin_addr);
            connections.insert(
                    std::pair<size_t, Connection>
                            (cl_fd, Connection(client_fd,
                                    srv_ip_addr_,
                                    std::string(client_ip_str),
                                    srv_port_,
                                    ntohs(client_addr.sin_port))));
        }
    }

    void Server::handle_client(size_t client_fd, uint event) {

        Connection &conn = connections.at(client_fd);
        conn.last_event_ = event;
        handler_(conn);

        if (event & EPOLLERR || event & EPOLLHUP) {
            connections.erase(client_fd);
        }
    }

    void Server::add_epoll(const FileDescriptor &fd, int flags) const {
        epoll_ctl(fd, flags, EPOLL_CTL_ADD);
    }

    void Server::del_epoll(const FileDescriptor &fd, int flags) const {
        epoll_ctl(fd, flags, EPOLL_CTL_DEL);
    }

    void Server::epoll_ctl(const FileDescriptor &fd, int flags, int what) const {
        epoll_event event{};

        event.data.fd = fd.to_int();
        event.events = flags;

        if (::epoll_ctl(epoll_fd_.to_int(), what, fd.to_int(), &event) < 0) {
            throw NetErrors("epoll_ctl error");
        }
    }
}

