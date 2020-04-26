#ifndef HW4_FILEDESCRIPTOR_H
#define HW4_FILEDESCRIPTOR_H

#include <iostream>
#include <unistd.h>

namespace mp {
    class FileDescriptor {
    public:
        FileDescriptor();
        FileDescriptor(int fd);
        ~FileDescriptor();
        FileDescriptor(const FileDescriptor &) = delete;
        FileDescriptor &operator=(const FileDescriptor &) = delete;
        FileDescriptor(FileDescriptor &&) noexcept ;
        FileDescriptor &operator=(FileDescriptor &&) noexcept ;
        void close();
        int to_int() const;
    private:
        int fd_;
    };
}
#endif //HW4_FILEDESCRIPTOR_H
