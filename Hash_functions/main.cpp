#include <iostream>

// #include "hash_table.h"
// Implementing hash table in C++

#include <iostream>
#include <list>
using namespace std;

class hash_table
{
    int capacity;
    list<int> *table;

public:
    hash_table(int V);
    void insertItem(int key, int data);
    void deleteItem(int key);

    int checkPrime(int n)
    {
        int i;
        if (n == 1 || n == 0)
        {
            return 0;
        }
        for (i = 2; i < n / 2; i++)
        {
            if (n % i == 0)
            {
                return 0;
            }
        }
        return 1;
    }
    int getPrime(int n)
    {
        if (n % 2 == 0)
        {
            n++;
        }
        while (!checkPrime(n))
        {
            n += 2;
        }
        return n;
    }

    int hashFunction(int key)
    {
        return (key % capacity);
    }
    void displayHash();
};

hash_table::hash_table(int c)
{
    int size = getPrime(c);
    this->capacity = size;
    table = new list<int>[capacity];
}
void hash_table::insertItem(int key, int data)
{
    int index = hashFunction(key);
    table[index].push_back(data);
}

void hash_table::deleteItem(int key)
{
    int index = hashFunction(key);

    list<int>::iterator i;
    for (i = table[index].begin(); i != table[index].end(); ++i)
        if (*i == key)
            break;
    // --i;
    if (i != table[index].end())
        table[index].erase(i);
}

void hash_table::displayHash()
{
    for (int i = 0; i < capacity; i++)
    {
        cout << "table[" << i << "]";
        for (auto x : table[i])
            cout << " --> " << x;
        cout << endl;
    }
}

int main()
{
    int key[] = {231, 321, 212, 321, 433, 262};
    int data[] = {123, 432, 523, 43, 423, 111};
    int size = sizeof(key) / sizeof(key[0]);

    hash_table h(size);

    for (int i = 0; i < size; i++)
        h.insertItem(key[i], data[i]);

    h.deleteItem(321);
    h.displayHash();
    
}
