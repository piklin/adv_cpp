#ifndef HW4_NETERRORS_H
#define HW4_NETERRORS_H

#include <iostream>
namespace mp {

    class NetErrors : std::exception {
    public:
        explicit NetErrors(const std::string &);

        const char *what() const noexcept override;

    private:
        std::string what_str_;
    };
}
#endif //HW4_NETERRORS_H
