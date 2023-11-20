#pragma once
#define INIT_SIZE 100
#include <string>

struct Ht_item
{
    size_t key;
    std::string value;
};
// template<class T>
class HashTable
{
public:
    HashTable(): size_(INIT_SIZE), size_all_non_nullptr_(size_)
    {
        // Ht_item** temp = items_;
        items_ = new Ht_item*[size_];
        for(size_t i = 0; i < size_; ++i)
            items_[i] = nullptr;
        // {
            // delete[] temp[i];
        // }
        // resize(INIT_SIZE);
    }
    static Ht_item* create_item(const size_t& key, const std::string& value)
    {
        auto* item = new Ht_item;
        item->key = key;
        item->value = value;

        return item;
    }
    void insert(const size_t& key, const std::string& value)
    {
        items_[0] = create_item(key, value);
        ++size_;
        ++size_all_non_nullptr_;
    }

    [[nodiscard]] auto find(const size_t& key) const
    {
        
        for (size_t i = 0; key != items_[i]->key || i < size_;++i)
            if(key == items_[i]->key)
                return items_[i]->value;

        return "0";
    }

    [[nodiscard]] auto remove(const size_t& key)
    {
        if(const auto temp = find(key); temp != "0")
        {
            for (size_t i = 0; key != items_[i]->key || i < size_;++i)
                if(key == items_[i]->key)
                    items_[i] = nullptr;
                    // return items_[i]->value;
            --size_all_non_nullptr_;
            
            return true;
        }

        return false;
    }

    void resize(const size_t& size)
    {
        Ht_item** temp = items_;
        items_ = new Ht_item*[size];
        for(size_t i = 0; i < size_; ++i)
        {
            items_[i] = temp[i];
            delete[] temp[i];
        }
        delete[] temp;
    }

    void rehash()
    {
        auto temp = new Ht_item*[size_all_non_nullptr_];
        size_t j = 0;
        for (size_t i = 0; i < size_; ++i)
        {
            if(items_[i] == nullptr)
                continue;

            temp[j++] = items_[i];
        }
        
    }
private:
    Ht_item** items_ = nullptr;
    size_t size_{0};
    size_t size_all_non_nullptr_{0};
};
