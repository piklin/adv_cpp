#ifndef HW2_LOGGER_H
#define HW2_LOGGER_H

#include <iostream>
#include <fstream>
#include <map>

namespace log {

    enum class Level : size_t {
        ERROR,
        WARNING,
        INFO,
        DEBUG
    };

    class BaseLoggerError : public std::exception {
    public:
        explicit BaseLoggerError(const std::string &);

        const char *what() const noexcept override;

    private:
        std::string what_string_;
    };

    class LoggerLevelError : public BaseLoggerError {
        using log::BaseLoggerError::BaseLoggerError;
    };


    class BaseLogger {
    public:
        explicit BaseLogger(log::Level);

        void set_level(log::Level);

        log::Level level() const;

        void check_level(const std::string &, log::Level);

        std::string get_str_level(log::Level);

        virtual void flush() = 0;

        void error(const std::string &);

        void warning(const std::string &);

        void info(const std::string &);

        void debug(const std::string &);

    private:
        virtual void log(const std::string &, log::Level) = 0;

        log::Level level_;
    };


}

#endif //HW2_LOGGER_H
