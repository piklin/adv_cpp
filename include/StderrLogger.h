#ifndef HW2_SRDERRLOGGER_H
#define HW2_SRDERRLOGGER_H

#include "Logger.h"

namespace log {

    class StderrLogger : public BaseLogger {
    public:
        using log::BaseLogger::BaseLogger;

        void flush() override;

    private:
        void log(const std::string &, log::Level) override;
    };

}
#endif //HW2_SRDERRLOGGER_H
