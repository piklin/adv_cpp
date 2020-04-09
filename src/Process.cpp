#include "Process.h"

//fork
Fork::Fork() {
    pid = fork();
    if (pid == -1) {
        throw ForkError("Fork error");
    }
}

Fork::~Fork() {
    kill(pid, SIGTERM);
    waitpid(pid, nullptr, 0);
}

size_t Fork::get_pid() {
    return static_cast<size_t>(pid);
}


//pipe
Pipe::Pipe() {
    if(pipe(fd) == -1) {
        throw PipeError("Error creating pipe");
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


//process exception
ProcessError::ProcessError(std::string err) {
    what_str = err;
}
const char* ProcessError::what() const throw() {
    return what_str.c_str();
}


//class process
Process::Process(const std::string &path) {
    try {

        if (pr.get_pid() == 0) {
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
            is_readable = true;
        }

    } catch (std::exception &e) {
        throw;
    }
}

    Process::~Process() {
        pipe_to.~Pipe();
        pipe_from.~Pipe();
        pr.~Fork();
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
        } else if (res == 0) {
            is_readable = false;
        }
        return static_cast<size_t>(res);
    }

    void Process::writeExact(const void *data, size_t len) {
        try {
            size_t res = this->write(data, len);
            if (res != len) {
                throw ProcessError("writeExact error");
            }
        } catch (std::exception &e) {
            throw;
        }
    }

    void Process::readExact(void *data, size_t len) {
        try {
            size_t res = this->read(data, len);
            if (res != len) {
                throw ProcessError("readExact error");
            }
        } catch (std::exception &e) {
            throw;
        }
    }

    void Process::closeStdin() {
        pipe_from.~Pipe();
    }

    void Process::close() {
        this->~Process();
    }

    bool Process::isReadable() const {
        return is_readable;
    }
