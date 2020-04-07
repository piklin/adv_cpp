#ifndef HW1_PROCESS_H
#define HW1_PROCESS_H
#include <string>
#include <unistd.h>
#include <cerrno>
#include <sys/types.h>
#include <csignal>
#include <iostream>
#include <cstdio>

class PipeError: public std::exception {
public:
    explicit PipeError(std::string);
    virtual const char* what() const throw();
private:
    std::string what_str;
};


class ProcessError: public std::exception {
public:
    explicit ProcessError(std::string);
    virtual const char* what() const throw();
private:
    std::string what_str;
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
        pid_t pid;
        Pipe pipe_to;
        Pipe pipe_from;
        int read_from_fd;
        int write_to_fd;
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
