#include "NetErrors.h"

namespace mp {
    NetErrors::NetErrors(const std::string &str) {
        what_str_ = str;
    }
    const char *NetErrors::what() const noexcept {
        return what_str_.data();
    }
}

