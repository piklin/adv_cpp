#include "StdoutLogger.h"

void log::StdoutLogger::log(const std::string &str_error, const log::Level &level) {
    std::cout << str_error << std::endl;
}

void log::StdoutLogger::flush() {
    std::cout.flush();
}