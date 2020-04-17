#include "Logger.h"

///errors
log::BaseLoggerError::BaseLoggerError(const std::string &string) {
    what_string_ = string;
}

const char* log::BaseLoggerError::what() const noexcept {
    return what_string_.c_str();
}

///BaseLogger class

log::BaseLogger::BaseLogger(log::Level level) : level_(level) {
}

void log::BaseLogger::set_level(log::Level level) {
    level_ = level;
}

log::Level log::BaseLogger::level() const {
    return level_;
}

void log::BaseLogger::error(const std::string &str_error) {
    check_level(str_error, log::Level::ERROR);
}

void log::BaseLogger::warning(const std::string &str_error) {
    check_level(str_error, log::Level::WARNING);
}

void log::BaseLogger::info(const std::string &str_error) {
    check_level(str_error, log::Level::INFO);
}

void log::BaseLogger::debug(const std::string &str_error) {
    check_level(str_error, log::Level::DEBUG);
}

void log::BaseLogger::check_level(const std::string &str, log::Level l) {
    if (level_ >= l){
        log(str, l);
    }
}
