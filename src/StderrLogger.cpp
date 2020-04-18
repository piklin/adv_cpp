#include "StderrLogger.h"

void log::StderrLogger::log(const std::string &str_error, const log::Level level) {
    std::cerr << "[" << get_str_level(level) << "] " << str_error << std::endl;
}

void log::StderrLogger::flush() {
    std::cerr.flush();
}