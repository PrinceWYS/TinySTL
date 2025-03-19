#include <iostream>
#include <vector>   // TODO: 将 vector 替换为自己的 vector
#include "allocator.h"
#include "type_traits.h"

void test1() {
    std::cout << "allocator test 01\n";
    std::vector<int, tinystl::allocator<int>> vec(5, 5);
    for (auto &each : vec) {
        std::cout << each << " ";
    }
    std::cout << std::endl;
    std::cout << "allocator test 01 success\n";
}

int main() {
    test1();

    return 0;
}