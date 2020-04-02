#ifndef PROCESS_H
#define PROCESS_H
#include <string>
#include <unistd.h>
#include <cerrno>
#include <sys/types.h>
#include <signal.h>
#include <iostream>
#include <cstdio>


class Process {
private:
    pid_t pid;
    int write_to_fd;
    int read_from_fd;
public:
    explicit Process(const std::string& path);
    ~Process();
    size_t write(const void* data, size_t len);
    void writeExact(const void* data, size_t len);
    size_t read(void* data, size_t len);
    void readExact(void* data, size_t len);
    bool isReadable() const;
    void closeStdin();
    void close();
};

#endif //PROCESS_H
