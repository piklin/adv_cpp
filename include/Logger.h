#ifndef HW2_LOGGER_H
#define HW2_LOGGER_H

#include <iostream>
#include <fstream>
#include <map>

class BaseLoggerError : public std::exception {
public:
    explicit BaseLoggerError(std::string);
    virtual const char* what() const noexcept;
private:
    std::string what_string;
};

class LoggerLevelError : public BaseLoggerError {
    using BaseLoggerError::BaseLoggerError;
};



class Level {
public:
    explicit Level(std::string);
    size_t get_level() const;
    std::string get_str_level() const;
private:
    std::map<std::string, size_t> levels =  {{"error", 0},
                                             {"warning", 1},
                                             {"info", 2},
                                             {"debug", 3}};
    std::string str_l;
};


class BaseLogger {
public:
    explicit BaseLogger(std::string);
    explicit BaseLogger(Level);

    void set_level(Level &);
    Level level() const;
    virtual void flush() = 0;

    void error(std::string);
    void warning(std::string);
    void info(std::string);
    void debug(std::string);

    friend bool operator>=(Level const &, Level const &);
private:
    virtual void log(std::string &, Level &) = 0;
    Level level_;
};


class StdoutLogger : public BaseLogger {
public:
    using BaseLogger::BaseLogger;

    void flush();
private:
    void log(std::string &, Level &);
};

class StderrLogger : public BaseLogger {
public:
    using BaseLogger::BaseLogger;

    void flush();
private:
    void log(std::string &, Level &);
};

class FileLogger : public BaseLogger {
public:
    FileLogger(std::string, Level);
    ~FileLogger();

    void flush();
private:
    void log(std::string &, Level &);
    std::ofstream ostream;
};

#endif //HW2_LOGGER_H
