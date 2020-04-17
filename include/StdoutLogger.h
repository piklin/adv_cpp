#ifndef HW2_STDOUTLOGGER_H
#define HW2_STDOUTLOGGER_H

#include "Logger.h"

namespace log {

    class StdoutLogger : public BaseLogger {
    public:
        using log::BaseLogger::BaseLogger;

        void flush() override;

    private:
        void log(const std::string &, const log::Level &) override;
    };

}
#endif //HW2_STDOUTLOGGER_H
