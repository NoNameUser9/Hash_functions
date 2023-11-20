#pragma once
#include <string>

size_t HashFunctionHorner(const std::string& s, size_t table_size, size_t key)
{
    size_t hash_result = 0;
    for (const char i : s)
        hash_result = (key * hash_result + i) % table_size;
    
    hash_result = (hash_result * 2 + 1) % table_size;
    return hash_result;
}

struct HashFunction1 
{
    auto operator()(const std::string& s, const size_t table_size) const
    {
        return HashFunctionHorner(s, table_size, table_size - 1);
    }
};

struct HashFunction2 
{
    auto operator()(const std::string& s, const size_t table_size) const
    {
        return HashFunctionHorner(s, table_size, table_size + 1);
    }
};

template <class T = std::string, class THash1 = HashFunction1, class THash2 = HashFunction2>
class HashTable
{
    static constexpr size_t default_size{8};
    constexpr static double rehash_size = 0.75;
    struct Node
    {
        T value;
        bool state;
        explicit Node(const T& value) : value(value), state(true) {}
    };
    Node** arr_;
    size_t size_;
    size_t buffer_size_;
    size_t size_all_non_nullptr_;
    void resize()
    {
        const size_t past_buffer_size = buffer_size_;
        buffer_size_ *= 2;
        size_all_non_nullptr_ = 0;
        size_ = 0;
        Node** arr2 = new Node * [buffer_size_];
        for (size_t i = 0; i < buffer_size_; ++i)
            arr2[i] = nullptr;
        std::swap(arr_, arr2);
        for (size_t i = 0; i < past_buffer_size; ++i)
            if (arr2[i] && arr2[i]->state)
                add(arr2[i]->value);
        
        for (size_t i = 0; i < past_buffer_size; ++i)
            if (arr2[i])
                delete arr2[i];
        
        delete[] arr2;
    }

    void rehash()
    {
        size_all_non_nullptr_ = 0;
        size_ = 0;
        Node** arr2 = new Node * [buffer_size_];
        for (size_t i = 0; i < buffer_size_; ++i)
            arr2[i] = nullptr;
        std::swap(arr_, arr2);
        for (size_t i = 0; i < buffer_size_; ++i)
            if (arr2[i] && arr2[i]->state)
                add(arr2[i]->value);

        for (size_t i = 0; i < buffer_size_; ++i)
            if (arr2[i])
                delete arr2[i];

        delete[] arr2;
    }

public:
    HashTable()
    {
        buffer_size_ = default_size;
        size_ = 0;
        size_all_non_nullptr_ = 0;
        arr_ = new Node*[buffer_size_];
        for (size_t i = 0; i < buffer_size_; ++i)
            arr_[i] = nullptr;
    }
    ~HashTable()
    {
        for (size_t i = 0; i < buffer_size_; ++i)
            if (arr_[i])
                delete arr_[i];
        
        delete[] arr_;
    }
    bool add(const T& value, const THash1& hash1 = THash1(), const THash2& hash2 = THash2())
    {
        if (size_ + 1 > static_cast<size_t>(rehash_size * buffer_size_))
            resize();
        else if (size_all_non_nullptr_ > 2 * size_)
            rehash();
        
        size_t h1 = hash1(value, buffer_size_);
        const size_t h2 = hash2(value, buffer_size_);
        size_t i = 0;
        int first_deleted = -1;
        while (arr_[h1] != nullptr && i < buffer_size_)
        {
            if (arr_[h1]->value == value && arr_[h1]->state)
                return false;
            if (!arr_[h1]->state && first_deleted == -1)
                first_deleted = h1;
            h1 = (h1 + h2) % buffer_size_;
            ++i;
        }
        if (first_deleted == -1)
        {
            arr_[h1] = new Node(value);
            ++size_all_non_nullptr_;
        }
        else
        {
            arr_[first_deleted]->value = value;
            arr_[first_deleted]->state = true;
        }
        ++size_;
        return true;
    }
    bool remove(const T& value, const THash1& hash1 = THash1(), const THash2& hash2 = THash2())
    {
        size_t h1 = hash1(value, buffer_size_);
        const size_t h2 = hash2(value, buffer_size_);
        size_t i = 0;
        while (arr_[h1] != nullptr && i < buffer_size_)
        {
            if (arr_[h1]->value == value && arr_[h1]->state)
            {
                arr_[h1]->state = false;
                --size_;
                return true;
            }
            h1 = (h1 + h2) % buffer_size_;
            ++i;
        }
        return false;
    }
    bool find(const T& value, const THash1& hash1 = THash1(), const THash2& hash2 = THash2())
    {
        size_t h1 = hash1(value, buffer_size_);
        const size_t h2 = hash2(value, buffer_size_);
        size_t i = 0;
        while (arr_[h1] != nullptr && i < buffer_size_)
        {
            if (arr_[h1]->value == value && arr_[h1]->state)
                return true;
            h1 = (h1 + h2) % buffer_size_;
            ++i;
        }
        return false;
    }
};
