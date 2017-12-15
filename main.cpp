#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include "Factorizer.h"

bool ex = false;
bool paused = false;

std::mutex lockcalc;
std::mutex lockinput;
std::condition_variable cv;

void factorize (std::ifstream& ifs, std::ofstream& ofs, char* output) {

    while (ifs.good()) {

        if (!ofs.is_open()) {
            ofs.open(output, std::fstream::app);
        }

        std::unique_lock<std::mutex> locker(lockcalc);

        uint64_t number;
        ifs >> number;

        Factorizer f(number);

        std::string factors = f.toString();

        ofs << number << ": " << factors << std::endl;

        if (ex) {
            break;
        }

        while (paused) {
            cv.wait(locker);
            ofs.close();
        }

    }

    ifs.close();
    ofs.close();

}

void input() {


    std::unique_lock<std::mutex> locker(lockinput);

    std::string in;
    while (!ex) {

        std::cin >> in;

        if (in == "exit") {
            ex = true;
        }

        if (in == "pause") {
            paused = true;
        }

        if (in == "resume") {
            paused = false;
            cv.notify_one();
        }

    }

}

int main(int argc, char* argv[]) {

    if (argc < 2) {
        std::cout << "Please, pass input and output file names" << std::endl;
        exit(1);
    }

    std::ifstream ifs;
    std::ofstream ofs;

    ifs.open(argv[1], std::fstream::in);
    ofs.open(argv[2], std::fstream::out);

    if (!ifs.good()) {
        std::cout << "Can't open input file" << std::endl;
        exit(1);
    }

    std::thread t1(factorize, std::ref(ifs), std::ref(ofs), argv[2]);
    std::thread t2(input);

    t1.join();
    t2.join();

    return 0;
}