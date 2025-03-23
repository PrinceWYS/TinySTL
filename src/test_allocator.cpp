#include <iostream>
#include <vector>   // TODO: 将 vector 替换为自己的 vector
#include "allocator.h"
#include "type_traits.h"

void test1() {
    std::cout << "allocator test: vector\n";
    std::vector<int, tinystl::allocator<int>> vec(5, 5);
    for (auto &each : vec) {
        std::cout << each << " ";
    }
    std::cout << std::endl;
    std::cout << "allocator test 01 success\n";
}

int main() {
    std::cout << "test_allocator start\n";
    test1();
    std::cout << "test_allocator end\n";
    return 0;
}