#pragma once
#define INIT_SIZE 10
#include <string>
#include <iostream>
#include <vector>

// #include "node.h"
#include "list.hpp"

template<class T>
class HashTable
{
    typedef pair<T> elem;
    typedef node<T> nugget;
    typedef list<T> bucket;

public:
    HashTable(): size_(INIT_SIZE), size_buckets_(0), size_all_elements_(0)
    {
        items_ = new bucket*[size_];
        for (size_t i = 0; i < size_; ++i)
            items_[i] = nullptr;
    }

    [[nodiscard]] static size_t hash(const size_t& key, const size_t& size)
    {
        // good algorithm
        const double c = (sqrt(5) - 1) / 2;
        const auto hash1 = static_cast<size_t>(static_cast<double>(size) * static_cast<double>(key) * fmod(c, 1.0)); // Простейшая хеш-функция
        return hash1 % size;

        // bad algorithm;
        // size_t hash = 0;
        // for (auto c : std::to_string(key))
        // {
        //     hash = (hash * 131) + c; // Простейшая хеш-функция
        // }
        // return hash % size;
    }
    
    void insert(const size_t& key, const T& value)
    {
        if(load_factor_ * static_cast<double>(size_) < static_cast<double>(size_all_elements_))
        {
            resize(size_ * 2);
            rehash(size_ / 2);//old size
        }
        
        insert_without_test(key, value);
    }

    [[nodiscard]] auto find_debug(const size_t& key) const
    {
        auto h = hash(key, size_);
        
        if(items_[h] != nullptr)
            for (size_t i = 0; i < items_[h]->size(); ++i)
                if (items_[h]->operator[](i).key == key)
                    return pair<size_t>{h, key};
        
        return pair<size_t>{0, 0};
    }
    
    [[nodiscard]] T find(const size_t& key) const
    {
        auto h = hash(key, size_);
        if(items_[h] != nullptr)
        {
            size_t i = 0;
            for (; i < items_[h]->size(); ++i)
                if (items_[h]->operator[](i).key == key)
                    return items_[h]->operator[](i).value;
        }
        
        return static_cast<T>(0);
    }

    [[nodiscard]] bucket* find_list(const size_t& key) const
    {
        auto h = hash(key, size_);
        if(items_[h] != nullptr)
        {
            auto t = items_[h];
            size_t i = 0;
            for (; i < items_[h]->size(); ++i)
                if (items_[h]->operator[](i).key == key)
                    return items_[h];
        }
        
        return nullptr;
    }

    size_t num_of_collision()
    {
        size_t count = 0;
        for (size_t i = 0; i < size_; ++i)
            if(items_[i] != nullptr && items_[i]->size() > 1)
                    count += items_[i]->size() - 1;
        
        return count;
    }

    [[nodiscard]] size_t size() const
    {
        return size_all_elements_;
    }
    
    void print()
    {
        size_t it = 0;
        for (size_t i = 0; i < size_; ++i)
            if (items_[i] != nullptr)
            {
                std::cout << it++ << ": ";
                items_[i]->print();
            }
    }
    
    bool remove(const size_t& key)
    {
        auto h = hash(key);

        if (items_[h] != nullptr)
        {
            if (items_[h]->size_ > 1)
                for (size_t i = 0; i < items_[h]->size(); ++i)
                    if (items_[h]->operator[](i).key == key)
                    {
                        items_[h]->remove(i);
                        --size_all_elements_;
                    }
            else
            {
                delete items_[h];
                items_[h] = nullptr;
            }
            
            return true;
        }

        return false;
    }
    
    void resize(const size_t& size)
    {
        bucket** temp = items_;//old size
        items_ = new bucket*[size];
        for(size_t i = 0; i < size_; ++i)
            items_[i] = temp[i];
        
        for (size_t i = size_; i < size; ++i)
            items_[i] = nullptr;

        size_ = size;
        
        delete[] temp;
    }

    [[nodiscard]] size_t full_size() const
    {
        return size_;
    }

    [[nodiscard]] size_t buckets_size() const
    {
        return size_buckets_;
    }

    void rehash(const size_t& old_size)
    {
        auto temp = new bucket*[old_size];

        for (size_t i = 0; i < old_size; ++i)
            temp[i] = items_[i];

        for (size_t i = 0; i < size_; ++i)
            items_[i] = nullptr;

        size_all_elements_ = 0;
        size_buckets_ = 0;

        for (size_t i = 0; i < old_size; ++i)
            if (temp[i] != nullptr)
                for (size_t ii = 0; ii < temp[i]->size(); ++ii)
                    insert_without_test(temp[i]->operator[](ii).key, temp[i]->operator[](ii).value);
    }
private:
    bucket** items_;
    size_t size_;
    size_t size_buckets_;
    size_t size_all_elements_;
    double load_factor_{0.69};
    static bucket* create_item(const size_t& key, const T& value)
    {
        return new bucket(new nugget{elem{key, value}, nullptr, nullptr});
    }
    
    void insert_without_test(const size_t& key, const T& value)
    {
        auto h = hash(key, size_);
        if(items_[h] != nullptr)
        {
            for (size_t i = 0; i < items_[h]->size(); ++i)
            {
                if(items_[h]->operator[](i).key != key)
                    continue;
                return;
            }

            items_[h]->push_back(new nugget{elem{key, value}, nullptr, nullptr});
            ++size_all_elements_;
            return;
        }

        items_[h] = create_item(key, value);
        
        ++size_buckets_;
        ++size_all_elements_;
    }
};
