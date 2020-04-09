#include "Process.h"

int main() {

    std::string data;

    try {
        const std::string bin_path("/bin/cat");
        Process p(bin_path);

        std::getline(std::cin, data);
        p.write(data.c_str(), data.length());
        std::string data1(data.length(), '\0');
        p.read(data1.data(), data.length());
        std::cout << data1 << std::endl;

    } catch(std::exception &e) {
        std::cout << e.what();
        return -1;
    }
    return 0;
}