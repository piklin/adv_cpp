#include "Process.h"


PipeError::PipeError(std::string err) {
    what_str = err;
}
const char* PipeError::what() const throw() {
    return what_str.c_str();
}


Pipe::Pipe() {
    if(pipe(fd) == -1) {
        throw std::bad_exception();
    }
}

Pipe::~Pipe() {
    close(fd[0]);
    close(fd[1]);
}

int Pipe::fd_read() {
    return fd[0];
}

int Pipe::fd_write() {
    return fd[1];
}


ProcessError::ProcessError(std::string err) {
    what_str = err;
}
const char* ProcessError::what() const throw() {
    return what_str.c_str();
}


Process::Process(const std::string &path) {
    try {
        pid = fork();
        if (pid == -1) {
            throw ProcessError("Forking error");
        }

        if (pid == 0) {
            ::close(pipe_to.fd_write());
            ::close(pipe_from.fd_read());
            dup2(pipe_to.fd_read(), STDIN_FILENO);
            dup2(pipe_from.fd_write(), STDOUT_FILENO);

            if (execl(path.c_str(), path.c_str(), nullptr) == -1) {
                throw ProcessError("execl error");
            }
        } else {
            ::close(pipe_to.fd_read());
            ::close(pipe_from.fd_write());
            write_to_fd = pipe_to.fd_write();
            read_from_fd = pipe_from.fd_read();
        }

    } catch (std::exception &e) {
        throw;
    }
}

    Process::~Process() {
        kill(pid, SIGTERM);
        waitpid(pid, nullptr, 0);
    }

    size_t Process::write(const void *data, size_t len) {
        ssize_t res = ::write(write_to_fd, data, len);
        if (res < 0) {
            throw ProcessError("write error");
        }
        return static_cast<size_t>(res);
    }

    size_t Process::read(void *data, size_t len) {
        ssize_t res = ::read(read_from_fd, data, len);
        if (res < 0) {
            throw ProcessError("read error");
        }
        return static_cast<size_t>(res);
    }

    void Process::writeExact(const void *data, size_t len) {
        ::write(write_to_fd, data, len);
    }

    void Process::readExact(void *data, size_t len) {
        ::read(read_from_fd, data, len);
    }

    void Process::closeStdin() {
        if (::close(write_to_fd)) {
            std::cerr << std::strerror(errno);
        }
    }

    void Process::close() {
        ::close(write_to_fd);
        ::close(read_from_fd);
        this->~Process();
    }

    bool Process::isReadable() const {
        if (::read(read_from_fd, nullptr, 0)) {
            return false;
        } else {
            return true;
        }
    }
