#include "Logger.h"
#include "FileLogger.h"
#include "StderrLogger.h"
#include "StdoutLogger.h"

int main(){
    log::Level l1(log::WARNING);
    log::Level l2(log::INFO);

    log::FileLogger a("./test", l1);
    log::StderrLogger b(l2);

    a.error("one");
    a.warning("two");
    a.info("three");

    b.error("one!");
    b.warning("two!");
    b.info("three!");
    b.debug("hop");

    return 0;
}

