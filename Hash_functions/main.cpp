#include <iostream>
#include "HashTable/HashTable.hpp"
using namespace std;

int main()
{
    srand(static_cast<unsigned>(time(nullptr)));

    typedef double my_type;
    HashTable<my_type> h;
    // HashTable<my_type> h2;
    // constexpr size_t size = static_cast<size_t>(10 * 1 << 12) * 7 / 10 - 409;
    constexpr size_t size = 10 * 1 << 12;
    size_t keys[size];
    // size_t test[size];
    my_type values[size];
    for (size_t i = 0; i < size; ++i)
    {
        // test[i] = HashTable<my_type>::hash(i, size);
        keys[i] = i;
        // keys[i] = rand() % size;
        values[i] = rand();
        h.insert(keys[i], values[i]);
    }

    // for (size_t i = size; i > 0;)
    // {
    //     // test[i] = HashTable<my_type>::hash(i, size);
    //     test[--i] = rand() % size;
    //     values[i] = rand();
    //     h2.insert(test[i], values[i]);
    // }

    // size_t it = 0;
    // for (const auto key : keys)
    // {
    //     cout << it++ << ": ";
    //     h.find_list(key)->print_by_key(key);
    // }
    // h.print();

    // h.insert(1, 21);
    // h.insert(1, 21);
    // h.insert(1, 21);
    cout << "collision: " << h.num_of_collision() << " from " << h.size() << " elements\nbuckets_size: " << h.buckets_size() <<
        "\nfull_size: " << h.full_size() << "\ncollision %: " << static_cast<double>(h.num_of_collision()) / static_cast<double>(h.size()) * 100;

    // cout << "\n\ncollision: " << h2.num_of_collision() << " from " << h2.size() << " elements\nbuckets_size: " << h2.buckets_size() <<
    //     "\nfull_size: " << h.full_size() << "\ncollision %: " << static_cast<double>(h2.num_of_collision()) / static_cast<double>(h2.size()) * 100;

    return 0;
}
