#include <iostream>
#include <cstring>

#include "memory_allocator.hpp"

int main() {
    // Test case 1
    std::cout << "Test case 1: create and free a new memory block" << std::endl;
    int* ptr1 = static_cast<int*>(_malloc(sizeof(int)));
    *ptr1 = 42;
    std::cout << "Value at ptr1: " << *ptr1 << std::endl << std::endl;
    _free(ptr1);

    // Test case 2
    std::cout << "Test case 2: reuse a free block without splitting" << std::endl;
    int* ptr2 = static_cast<int*>(_malloc(sizeof(int)));
    *ptr2 = 42;
    std::cout << "Value at ptr2: " << *ptr2 << std::endl << std::endl;
    _free(ptr2);

    // Test case 3
    std::cout << "Test case 3: create and free a large memory block" << std::endl;
    char* ptr3 = static_cast<char*>(_malloc(1000 * sizeof(char)));
    strcpy(ptr3, "Hello");
    std::cout << "String at ptr3: " << ptr3 << std::endl << std::endl;
    _free(ptr3);

    // Test case 4
    std::cout << "Test case 4: reuse a large free memory block and create a new split for the remaining part" << std::endl;
    char* ptr4 = static_cast<char*>(_malloc(5 * sizeof(char)));
    strcpy(ptr4, "Hi");
    std::cout << "String at ptr4: " << ptr4 << std::endl << std::endl;
    _free(ptr4);

    return 0;
}