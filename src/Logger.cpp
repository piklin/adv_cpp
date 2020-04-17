#include "Logger.h"

///errors
log::BaseLoggerError::BaseLoggerError(const std::string &string) {
    what_string_ = string;
}

const char* log::BaseLoggerError::what() const noexcept {
    return what_string_.c_str();
}


///Level class
log::Level::Level(log::log_level l) : level_(l) {
}

log::log_level log::Level::get_level() const {
    return level_;
}

bool operator>=(const log::Level &left, const log::Level &right) {
    if (left.get_level() >= right.get_level()) {
        return true;
    } else {
        return false;
    }
}

///BaseLogger class

log::BaseLogger::BaseLogger(log::Level level) : level_(level) {
}

void log::BaseLogger::set_level(log::Level &level) {
    level_ = level;
}

log::Level log::BaseLogger::level() const {
    return level_;
}

void log::BaseLogger::error(const std::string &str_error) {
    check_level(str_error, ERROR);
}

void log::BaseLogger::warning(const std::string &str_error) {
    check_level(str_error, WARNING);
}

void log::BaseLogger::info(const std::string &str_error) {
    check_level(str_error, INFO);
}

void log::BaseLogger::debug(const std::string &str_error) {
    check_level(str_error, DEBUG);
}

void log::BaseLogger::check_level(const std::string &str, log::log_level l) {
    Level level(l);
    if (level_ >= level){
        log(str, level);
    }
}






