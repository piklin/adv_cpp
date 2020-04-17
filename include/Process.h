#ifndef HW1_PROCESS_H
#define HW1_PROCESS_H
#include <string>
#include <unistd.h>
#include <cerrno>
#include <sys/types.h>
#include <csignal>
#include <iostream>
#include <cstdio>


class ProcessError: public std::exception {
public:
    explicit ProcessError(std::string);
    virtual const char* what() const throw();
private:
    std::string what_str;
};

class ForkError: public ProcessError {
    using ProcessError::ProcessError;
};

class PipeError: public ProcessError {
    using ProcessError::ProcessError;
};

class Pipe {
private:
    int fd[2];
public:
    explicit Pipe();
    ~Pipe();
    int fd_read();
    int fd_write();
};


class Process {
    private:
        Pipe pipe_to;
        Pipe pipe_from;
        int read_from_fd;
        int write_to_fd;
        bool is_readable;
        int pid;
    public:
        explicit Process(const std::string &path);

        ~Process();

        size_t write(const void *data, size_t len);

        void writeExact(const void *data, size_t len);

        size_t read(void *data, size_t len);

        void readExact(void *data, size_t len);

        bool isReadable() const;

        void closeStdin();

        void close();
};

#endif //HW1_PROCESS_H
