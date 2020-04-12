#include "Logger.h"

///errors
BaseLoggerError::BaseLoggerError(std::string string) {
    what_string = string;
}

const char* BaseLoggerError::what() const noexcept {
    return what_string.c_str();
}


///Level class
Level::Level(std::string level) {
    if (levels.count(level) == 0) {
        throw LoggerLevelError("no level");
    } else {
        str_l = level;
    }
}

size_t Level::get_level() const {
    return levels.at(str_l);
}

std::string Level::get_str_level() const {
    return str_l;
}

bool operator>=(Level const &left, Level const &right) {
    if (left.get_level() >= right.get_level()) {
        return true;
    } else {
        return false;
    }
}

///BaseLogger class
BaseLogger::BaseLogger(std::string str_level) : level_(Level(str_level)) {
}

BaseLogger::BaseLogger(Level level) : level_(level) {
}

void BaseLogger::set_level(Level &level) {
    level_ = level;
}

Level BaseLogger::level() const {
    return level_;
}

void BaseLogger::error(std::string str_error) {
    std::string str_level = "error";
    Level l(str_level);
    if (level_ >= l){
        log(str_error, l);
    }
}

void BaseLogger::warning(std::string str_error) {
    std::string str_level = "warning";
    Level l(str_level);
    if (level_ >= l){
        log(str_error, l);
    }
}

void BaseLogger::info(std::string str_error) {
    std::string str_level = "info";
    Level l(str_level);
    if (level_ >= l){
        log(str_error, l);
    }
}

void BaseLogger::debug(std::string str_error) {
    std::string str_level = "debug";
    Level l(str_level);
    if (level_ >= l){
        log(str_error, l);
    }
}


///BaseLogger's children
void StdoutLogger::log(std::string &str_error, Level &level) {
    std::cout << "[" + level.get_str_level() + "] " + str_error << std::endl;
}

void StdoutLogger::flush() {
    std::cout.flush();
}


void StderrLogger::log(std::string &str_error, Level &level) {
    std::cerr << "[" + level.get_str_level() + "] " + str_error << std::endl;
}

void StderrLogger::flush() {
    std::cerr.flush();
}


FileLogger::FileLogger(std::string path, Level level) : BaseLogger(level) {
    ostream.open(path);
    if (!ostream.is_open()) {
        throw BaseLoggerError("file open error");
    }
}

FileLogger::~FileLogger() {
    ostream.close();
}

void FileLogger::log(std::string &str_error, Level &level) {
    ostream << "[" + level.get_str_level() + "] " + str_error << std::endl;
}

void FileLogger::flush() {
    ostream.flush();
}
