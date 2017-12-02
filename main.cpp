#include <iostream>
#include <fstream>
#include <vector>
#include "Factorizer.h"

int main(int argc, char* argv[]) {

    if (argc < 2) {
        std::cout << "Please, pass input and output file names" << std::endl;
        exit(1);
    }

    std::ifstream ifs;
    std::ofstream ofs;

    ifs.open(argv[1], std::ifstream::in);
    ofs.open(argv[2], std::ifstream::out);

    if (!ifs.good()) {
        std::cout << "Can't open input file" << std::endl;
        exit(1);
    }

    while (ifs.good()) {

        uint64_t number;
        ifs >> number;

        Factorizer f(number);

        std::string factors = f.toString();

        ofs << number << ": " << factors << std::endl;

    }

    ifs.close();
    ofs.close();

    return 0;
}