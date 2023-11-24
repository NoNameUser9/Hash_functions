#include <iostream>
#include "HashTable\HashTable.hxx"
using namespace std;
#define PROGRAM "HashTable"
#define VER " ver 1.0.0\n"

int main()
{
    srand(static_cast<unsigned>(time(nullptr)));

    typedef double my_type;
    HashTable<my_type> h;
    constexpr size_t size = 15 * 1 << 12;
    size_t keys[size];
    my_type values[size];
    for (size_t i = 0; i < size; ++i)
    {
        keys[i] = rand() % size;
        values[i] = rand();
        h.insert(keys[i], values[i]);
    }
    
    size_t ms = 0;
    while (true)
    {
        system("cls");
        cout << PROGRAM VER
                "\n0.show all elements\n"
                "1.show all elements by key\n"
                "2.insert by key\n"
                "3.find by key\n"
                "4.remove by key\n"
                "5.count the number of collisions\n"
                "6.show size\n"
                "7.exit\n";
        cin >> ms;
        switch (ms)
        {
        case 0:
            {
                system("cls");
                h.print();
                system("pause");
                break;
            }
        case 1:
            {
                system("cls");
                size_t it = 0;
                for (const auto key : keys)
                {
                    cout << it++ << ": ";
                    if(h.find_bucket(key) != nullptr)
                        h.find_bucket(key)->print_by_key(key);
                }
                system("pause");
                break;
            }
        case 2:
            {
                system("cls");
                size_t key;
                my_type val;
                cout << " enter the key: ";
                cin >> key;
                cout << " enter the value: ";
                cin >> val;
                h.insert(key, val);
                system("pause");
                break;
            }
        case 3:
            {
                system("cls");
                size_t key;
                cout << "find by key: ";
                cin >> key;
                try {cout << "founded element: " << h.find(key) << "\n";}
                catch (std::exception& ex) {cerr << ex.what();}
                system("pause");
                break;
            }
        case 4:
            {
                system("cls");
                cout << "enter the key for delete the element: ";
                size_t in;
                cin >> in;
                cout << h.remove(in) << "\n";
                system("pause");
                break;
            }
        case 5:
            {
                system("cls");
                cout << "collision: " << h.num_of_collision() << " from " << h.size() << " elements\nbuckets_size: " << h.buckets_size() <<
                    "\nfull_size: " << h.full_size() << "\ncollision %: " << static_cast<double>(h.num_of_collision()) /
                        static_cast<double>(h.size()) * 100 << "\n";
                system("pause");
                break;
            }
        case 6:
            {
                system("cls");
                cout << "size: " << h.size() << "\n";
                system("pause");
                break;
            }
        case 7:
            {
                exit(EXIT_SUCCESS);
            }
        default:
            {
                break;
            }
        }
    }
}
