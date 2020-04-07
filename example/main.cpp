#include "Process.h"

int main() {

    std::string data;

    try {
        const std::string bin_path("/bin/cat");
        Process p(bin_path);

        std::getline(std::cin, data);
        p.write(data.c_str(), data.length());
        auto *readed_data = new char[data.length()];
        p.read(readed_data, data.length());
        std::cout << std::string(readed_data) << std::endl;

    } catch(std::exception &e) {
        std::cout << e.what();
        return -1;
    }
    return 0;
}