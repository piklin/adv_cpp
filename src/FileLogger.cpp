#include "FileLogger.h"

log::FileLogger::FileLogger(std::string path, Level level) : BaseLogger(level) {
    out_stream_.open(path);
    if (!out_stream_.is_open()) {
        throw BaseLoggerError("file open error");
    }
}

log::FileLogger::~FileLogger() {
    out_stream_.close();
}

void log::FileLogger::log(const std::string &str_error, const log::Level &level) {
    out_stream_ << str_error << std::endl;
}

void log::FileLogger::flush() {
    out_stream_.flush();
}