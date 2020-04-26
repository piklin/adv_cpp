#include "Connection.h"

namespace mp {
    Connection::Connection() : fd_(FileDescriptor()), src_port_(0), dst_port_(0){
    }

    Connection::Connection(FileDescriptor &fd, const std::string &src_addr,
                           const std::string &dst_addr, size_t src_port, size_t dst_port)
            : fd_(std::move(fd)), src_addr_(src_addr), dst_addr_(dst_addr), src_port_(src_port), dst_port_(dst_port) {}

    Connection::~Connection() {
        this->close();
    }

    Connection::Connection(Connection &&c) noexcept {
        swap(c);
    }

    Connection &Connection::operator=(Connection &&c) noexcept {
        swap(c);
        return *this;
    }

    void Connection::close() {
        fd_.close();
        src_port_ = 0;
        dst_port_ = 0;
        src_addr_.clear();
        dst_addr_.clear();
    }

    void Connection::swap(Connection &c) {
        std::swap(fd_, c.fd_);
        std::swap(dst_addr_, c.dst_addr_);
        std::swap(src_addr_, c.src_addr_);
        std::swap(dst_port_, c.dst_port_);
        std::swap(src_port_, c.src_port_);
    }

    int Connection::write(const std::string &str, int len) {
        ssize_t res = ::write(fd_.to_int(), str.data(), len);
        if (res < 0) {
            close();
        }
        return res;
    }

    int Connection::read(std:: string &str, int len) {
        ssize_t res = ::read(fd_.to_int(), str.data(), len);
        if (res < 0) {
            close();
        }
        read_buf_.append(str, 0, res);
        return res;
    }

    void Connection::clear_input() {
        read_buf_.clear();
    }

    std::string &Connection::input() {
        return read_buf_;
    }

    size_t Connection::input_size() {
        return read_buf_.size();
    }

    void Connection::clear_output_size() {
        writed_ = 0;
    }

    size_t Connection::output_size() {
        return writed_;
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

    uint Connection::get_last_event() const {
        return last_event_;
    }
}
