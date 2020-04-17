#ifndef HW2_FILELOGGER_H
#define HW2_FILELOGGER_H

#include "Logger.h"

namespace log {

    class FileLogger : public BaseLogger {
    public:
        FileLogger(const std::string &, log::Level);

        ~FileLogger();

        void flush() override;

    private:
        void log(const std::string &, log::Level) override;

        std::ofstream out_stream_;
    };
}

#endif //HW2_FILELOGGER_H
