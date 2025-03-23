#include <iostream>
#include <list>
#include "alloc.h"
#include "allocator.h"
#include "type_traits.h"

enum { NUMBERS = 10000000 };

void test1() {
    std::cout << "alloc test: time of insert NUMBERS nums into lists\n";
    
    std::list<int> list1;
    std::list<int, tinystl::allocator<int>> list2;
    std::list<int, tinystl::alloc<int>> list3;

    clock_t start = clock();
    for (int i = 0; i < NUMBERS; ++i) {
        list1.push_back(i);
    }
    clock_t end = clock();
    std::cout << "Time of insert " << NUMBERS << " numbers with STL allocator " << (end - start) << "\n";

    start = clock();
    for (int i = 0; i < NUMBERS; ++i) {
        list2.push_back(i);
    }
    end = clock();
    std::cout << "Time of insert " << NUMBERS << " numbers with tinystl allocator " << (end - start) << "\n";

    start = clock();
    for (int i = 0; i < NUMBERS; ++i) {
        list3.push_back(i);
    }
    end = clock();
    std::cout << "Time of insert " << NUMBERS << " numbers with tinystl alloc " << (end - start) << "\n";

    std::cout << "alloc test 01 success\n";
}

int main() {
    std::cout << "test_alloc start\n";
    test1();
    std::cout << "test_alloc end\n";
    return 0;
}