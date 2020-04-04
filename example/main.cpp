#include <iostream>
#include <vector>

#include "Process.h"

#define BIN_PATH "/bin/cat"

int main() {

    std::string data = nullptr;

    try {
        Process p(BIN_PATH);

        std::getline(std::cin, data));
        p.write(data.c_str(), data.length());
        char readed_data[data.length()];
        p.read(readed_data, data.length());
        std::cout << std::string(readed_data) << std::endl;

    } catch(...) {
        return -1;
    }
    return 0;
}