#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <time.h>
#include <cstring>
#include <unordered_map>

// #include "hash_table.h"

using namespace std;
//
// size_t HashFunctionHorner(const std::string& s, const size_t table_size, const size_t key)
// {
//     size_t hash_result = 0;
//     for (size_t i = 0; s[i] != s.size(); ++i)
//     {
//         hash_result = (key * hash_result + s[i]) % table_size;
//     }
//     hash_result = (hash_result * 2 + 1) % table_size;
//     return hash_result;
// }
//
// struct HashFunction1 
// {
//     int operator()(const std::string& s, const size_t table_size) const
//     {
//         return HashFunctionHorner(s, table_size, table_size - 1);
//     }
// };
//
// struct HashFunction2 
// {
//     int operator()(const std::string& s, const size_t table_size) const
//     {
//         return HashFunctionHorner(s, table_size, table_size + 1);
//     }
// };

size_t generateRandLong()
{ //генератор большого(12 цифрового) ключа для хеш-функции

    constexpr size_t num = 12;
    size_t a = 1;
    size_t r = 0;
    for (size_t i = 0; i < num; i++)
    {
        if (i == num - 1)
        {
            r += a * (static_cast<size_t>(rand() % 9) + 1);
            break;
        }
        r += a * (rand() % 10);
        a *= 10;
    }
    
    return r;
}

struct Data
{ // структура данных 
    std::string name;
    static inline size_t amount_of_students{0};
    bool turnstiles;

    Data(): turnstiles(false) {}
    
    Data(const std::string& a, const size_t b, const bool c)
    { // конструктор Data
        // strncpy(name, a, 10);
        amount_of_students = b;
        turnstiles = c;
    }
};

struct HashNode
{ //узел, необходимый для реализации односвязного списка
    size_t key;
    Data data;
    HashNode* next;
    HashNode(const Data& data, const size_t key, HashNode* next)
    {
        this->data = data;
        this->key = key;
        this->next = next;
    }
};

struct LinkedList
{ // односвязный список для каждой ячейки в массиве
    
    HashNode* head = nullptr;
    HashNode* tail = nullptr;

    void push_back(const Data& data, const size_t key) // метод, необходимый для "insert" в таблицу
    {
        auto* new_hash_node = new HashNode(data, key, nullptr);

        if (head == nullptr)
        {
            head = new_hash_node;
            tail = new_hash_node;
        }
        else
        {
            tail->next = new_hash_node;
            tail = new_hash_node;
        }
    }
};

struct HashTable2
{ //сама хеш-таблица
public:
    void insert(const size_t key, const Data& value) // добавление элемента в таблицу
    {
        
        if (load_factor_ < static_cast<float>(real_size_) / static_cast<float>(m_))
        {
            m_ *= alpha_;
            auto* NewbucketsArray = new LinkedList[m_];
            for (int i = 0; i < m_ / alpha_; i++)
                NewbucketsArray[i] = buckets_array_[i];
            
            delete[] buckets_array_;
            buckets_array_ = NewbucketsArray;
        }
        const auto index = hash(key);
        HashNode* currentNode = buckets_array_[index].head;
        while (currentNode != nullptr)
        {
            if (currentNode->key == key)
            {
                currentNode->data = value;
                return;
            }
            currentNode = currentNode->next;
        }
        
        buckets_array_[index].push_back(value, key);
        
        real_size_++;
    }

    [[nodiscard]] Data* find(const size_t key) const
    //поиск элемента по таблице
    {
        const auto index = hash(key);
        HashNode* current_node = buckets_array_[index].head;
        if (current_node == nullptr)
            //cout << "There is no such an element with such a key" << endl;
            return nullptr;
        if (current_node->key == key)
            return &current_node->data;
        
        while (current_node != nullptr)
        {
            if (current_node->key == key)
                return &current_node->data;
            
            current_node = current_node->next;
        }
        return nullptr;
    }
    Data erase(const size_t key) // удаление элемента из таблицы
    {
        if(const auto index = hash(key); buckets_array_[index].head == nullptr)
        {
            //cout << "There is no such an element in the list" << endl;
            const Data a("\0", 0, false);
            return a;
        }
        else
        {
            if (const HashNode* temp_node = buckets_array_[index].head; temp_node->key == key)
            {
                if (temp_node->next == nullptr)
                {
                    buckets_array_[index].head = nullptr;
                    const Data a = temp_node->data;
                    delete temp_node;
                    real_size_--;
                    return a;
                }
                
                buckets_array_[index].head = buckets_array_[index].head->next;
                const Data a = temp_node->data;
                delete temp_node;
                real_size_--;
                return a;
            }
            
            HashNode* prev_node = buckets_array_[index].head;
            HashNode* current_node = buckets_array_[index].head->next;
            while (current_node != nullptr)
            {
                if (current_node->key == key)
                    break;
                
                prev_node = current_node;
                current_node = current_node->next;
            }
            if (current_node == nullptr)
            {
                //cout << "There is no such an element with such a key" << endl;
                const Data a("\0", 0, 0);
                return a;
            }
            prev_node->next = current_node->next;
            real_size_--;
            const Data a = current_node->data;
            delete current_node;
            return a;
        }
    }

    [[nodiscard]] auto size() const
    // размер таблицы
    {
        return real_size_;
    }
private:
    
    [[nodiscard]] auto hash(const size_t key) const
    { //хеш-функция
        //cout << key % m << endl;
        return key % m_;
    }
    size_t m_ = 8;
    LinkedList* buckets_array_ = new LinkedList[m_];
    
    int alpha_ = 2;
    size_t real_size_ = 0;
    float load_factor_ = 3;
};



bool testHashTable() // сравнение моей таблицы с unordered_map
{
    constexpr size_t iters = 500000;
    constexpr size_t keys_amount = iters * 1;
    // generate random keys:
    const auto keys = new size_t[keys_amount];
    auto* keys_to_insert = new size_t[iters];
    auto* keys_to_erase = new size_t[iters];
    auto* keys_to_find = new size_t[iters];
    for (size_t i = 0; i < keys_amount; i++)
        keys[i] = generateRandLong();
    
    for (size_t i = 0; i < iters; i++)
    {
        keys_to_insert[i] = keys[generateRandLong() % keys_amount];
        keys_to_erase[i] = keys[generateRandLong() % keys_amount];
        keys_to_find[i] = keys[generateRandLong() % keys_amount];
    }
    // test my HashTable:
    HashTable2 hash_table;
    const clock_t my_start = clock();
    for (size_t i = 0; i < iters; i++)
        hash_table.insert(keys_to_insert[i], Data());

    const auto my_insert_size = hash_table.size();
    for (size_t i = 0; i < iters; i++)
        hash_table.erase(keys_to_erase[i]);

    const auto my_erase_size = hash_table.size();
    size_t my_found_amount = 0;
    for (size_t i = 0; i < iters; i++)
        if (hash_table.find(keys_to_find[i]) != nullptr)
            my_found_amount++;

    const clock_t my_end = clock();
    const float my_time = static_cast<float>(my_end - my_start) / CLOCKS_PER_SEC;
    // test STL hash table:
    unordered_map<size_t, Data> unordered_map;
    const clock_t stl_start = clock();
    for (size_t i = 0; i < iters; i++)
        unordered_map.insert({ keys_to_insert[i], Data() });

    const auto stl_insert_size = unordered_map.size();
    for (size_t i = 0; i < iters; i++)
        unordered_map.erase(keys_to_erase[i]);

    const auto stl_erase_size = unordered_map.size();
    size_t stl_found_amount = 0;
    for (size_t i = 0; i < iters; i++)
        if (unordered_map.contains(keys_to_find[i]))
            stl_found_amount++;

    const clock_t stl_end = clock();
    const float stl_time = static_cast<float>(stl_end - stl_start) / CLOCKS_PER_SEC;
    cout << "My HashTable:" << endl;
    cout << "Time: " << my_time << ", size: " << my_insert_size << " - " << my_erase_size << ", found amount: " << my_found_amount << endl;
    cout << "STL unordered_map:" << endl;
    cout << "Time: " << stl_time << ", size: " << stl_insert_size << " - " << stl_erase_size << ", found amount: " << stl_found_amount << endl << endl;
    delete [] keys;
    delete [] keys_to_insert;
    delete [] keys_to_erase;
    delete [] keys_to_find;
    
    if (my_insert_size == stl_insert_size && my_erase_size == stl_erase_size && my_found_amount == stl_found_amount)
    {
        cout << "Сompleted" << endl;
        return true;
    }
    cerr << ":(" << endl;
    return false;
}



int main()
{
    srand(static_cast<unsigned>(time(nullptr)));
    
    testHashTable();
    // HashTable<std::string> h;
    // Data d;
    // h.Add("123");
    // h.Remove("123");
    // d.
    // h.insert(1, 256);
    // size_t size = 7;
    // size_t key[] = {0,1,2,3,4,5,6};
    // std::string data_init[] = {"0,1,2,3,4,5,6", "0,1,2,3,4,6", "4,5,6", "0,1,2", "0,1,2,5,6", "0,1,2,3,4", ",2,3,4,5,6"};
    // Data data[7];
    // for(size_t i = 0; i < size; ++i)
        // data[i].name = data_init[i];
    
    // HashTable2 h;
    // for(size_t i = 0; i < size; ++i)
        // h.insert(key[i], data[i]);
    // Data* dd;
    // dd = h.find(5);
    // cout << dd->name;
    return 0;
}
