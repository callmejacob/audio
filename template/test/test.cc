#include <iostream>

#include "lib.h"

using namespace audio_al;

int main(int argc, char** argv) {
    std::cout << "fuck" << std::endl;

    // std::function<int(int, char**)> func;

    auto func = [](int argc, char** argv) ->int {
        return argc;
    };

    auto ret = func(argc, argv);

    std::cout << "func ret:" << ret << std::endl;

    std::cout << "lib result:" << feature::get_result(argc, argv) << std::endl;

    return 0;
}