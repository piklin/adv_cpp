#include <iostream>

#include "Logger.h"

int main(){
    Level l1("warning");
    Level l2("info");

    FileLogger a("./test", l1);
    StderrLogger b(l2);

    a.error("one");
    a.warning("two");
    a.info("three");

    b.error("one!");
    b.warning("two!");
    b.info("three!");
    b.debug("hop");

    return 0;
}

