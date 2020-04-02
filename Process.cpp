#include "Process.h"

Process::Process(const std::string& path) {
    int pipefdTo[2];
    int pipefdFrom[2];

    try{
        if (pipe(pipefdTo) == -1 || pipe(pipefdFrom) == -1) {
            throw std::exception();
        }

        write_to_fd = pipefdTo[1];
        read_from_fd = pipefdFrom[0];

        pid = fork();
        if (pid == -1) {
            throw std::exception();
        }

        if (pid == 0) {
            ::close(pipefdTo[1]);
            ::close(pipefdFrom[0]);
            dup2(pipefdTo[0], STDIN_FILENO);
            dup2(pipefdFrom[1], STDOUT_FILENO);

            if (execl(path.c_str(), path.c_str(), nullptr) == -1) {
                throw std::exception();
            }
        } else {
            ::close(pipefdTo[0]);
            ::close(pipefdFrom[1]);
        }

    } catch (std::exception & e) {
        std::cerr << std::strerror(errno);
        throw;
    }
}

Process::~Process() {
    if(kill(pid, SIGTERM)) {
        std::cerr << std::strerror(errno);
    }
    if (waitpid(pid, nullptr, 0) == -1) {
        std::cerr << std::strerror(errno);
    }
}

size_t Process::write(const void* data, size_t len) {
    return ::write(write_to_fd, data, len);
}

size_t Process::read(void* data, size_t len) {
    return ::read(read_from_fd, data, len);
}

void Process::writeExact(const void* data, size_t len){
    ::write(write_to_fd, data, len);
}

void Process::readExact(void* data, size_t len){
    ::read(read_from_fd, data, len);
}

void Process::closeStdin() {
    if (::close(write_to_fd)) {
        std::cerr << std::strerror(errno);
    }
}

void Process::close() {
    if (::close(write_to_fd)) {
        std::cerr << std::strerror(errno);
    }
    if (::close(read_from_fd)) {
        std::cerr << std::strerror(errno);
    }
}