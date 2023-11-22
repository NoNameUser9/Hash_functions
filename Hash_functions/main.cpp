// #define _CRT_SECURE_NO_WARNINGS
// #include <iostream>
#include <time.h>
// #include <cstring>
// #include <unordered_map>

#include <iostream>
#include <unordered_map>

// #include "hash_table.h"
#include "HashTable\HashTable.hpp"
//
using namespace std;
//

int main()
{
    srand(static_cast<unsigned>(time(nullptr)));
    
    // cout << dd->name;
    typedef int my_type;
    HashTable<my_type> h;
    // HashTable<>
    constexpr  size_t size = 50;
    const auto keys = new my_type[size];
    const auto values = new my_type[size];
    for (size_t i = 0; i < size ; ++i)
    {
        keys[i] = rand();
        values[i] = rand();
        h.insert(keys[i], values[i]);
    }
    
    // std::cout << "hash: " << h.find_debug(1).key << "  key: " << h.find_debug(1).value << "\n";
    // std::cout << "hash: " << h.find_debug(2).key << "  key: " << h.find_debug(2).value << "\n";
    // std::cout << "hash: " << h.find_debug(221).key << "  key: " << h.find_debug(221).value << "\n";
    // std::cout << h.find(221);
    for (size_t i = 0; i < size; ++i)
        h.find_list(keys[i])->print();

    cout << "collision: " << h.num_of_collision() << " from " << h.size() << " elements\n";
    return 0;
}
