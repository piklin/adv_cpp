#include "FileDescriptor.h"

namespace mp {
    FileDescriptor::FileDescriptor() : fd_(-1) {
    }
    FileDescriptor::FileDescriptor(int fd) {
        fd_ = fd;
    }
    FileDescriptor::~FileDescriptor() {
        close();
    }
    FileDescriptor::FileDescriptor(FileDescriptor &&f) noexcept {
        fd_ = f.fd_;
        f.fd_ = -1;
    }
    FileDescriptor &FileDescriptor::operator=(FileDescriptor &&f) noexcept {
        close();
        std::swap(fd_, f.fd_);
        return *this;
    }
    void FileDescriptor::close(){
        ::close(fd_);
        fd_ = -1;
    }
    int FileDescriptor::to_int() const {
        return fd_;
    }
}
