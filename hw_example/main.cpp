#include "Logger.h"
#include "FileLogger.h"
#include "StderrLogger.h"
#include "StdoutLogger.h"

int main(){

    log::FileLogger a("./test", log::Level::ERROR);
    log::StderrLogger b(log::Level::WARNING);

    a.error("one");
    a.warning("two");
    a.info("three");

    b.error("one!");
    b.warning("two!");
    b.info("three!");
    b.debug("hop");

    return 0;
}

