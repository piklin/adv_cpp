#include "NetError.h"

namespace net {

    NetError::NetError(const std::string &str) : what_str_(str) {
    }

    const char *NetError::what() const noexcept {
        return what_str_.data();
    }

}