#pragma once
// #include <initializer_list>
#include <iostream>

#include "node.hpp"

template<class T>
class list
{
public:
    list(): size_(0), head_(nullptr), tail_(nullptr){}
    explicit list(node<T>* in): size_(1), head_(in), tail_(head_){}
    list(std::initializer_list<T> list): size_(1), head_(list), tail_(head_){}
    ~list(){clear();}
    
    void push_back(node<T>* in)
    {
        if(size_ == 0)
        {
            tail_ = in;
            head_ = tail_;
            ++size_;
            return;
        }
        
        tail_->next = in;
        tail_ = in;
        ++size_;
    }

    node<T>* back() { return tail_; }

    void print()
    {
        auto temp = head_;
        std::cout << temp->data.value;
        temp = temp->next;
        while (temp != nullptr)
        {
            std::cout << "-->" << temp->data.value;
            temp = temp->next;
        }
        std::cout << "\n";
    }

    void remove(const size_t& idx)
    {
        auto temp = head_;
        if (idx > 0)
            for (size_t i = 0; i < idx - 1; ++i)
                temp = temp->next;
        if (temp->next->next != nullptr)
        {
            temp->next = temp->next->next;
            delete temp->next;
        }
        else
            delete temp->next;
    }

    void clear()
    {
        while (head_ != nullptr)
        {
            const auto temp = head_;
            head_ = head_->next;
            delete temp;
            --size_;
        }
    }

    size_t size()
    {
        // auto s = size_;
        return size_;
    }

    pair<T> operator[](const size_t& idx)
    {
        auto it = head_;
        if(idx == 0)
            return it->data;
        
        for (size_t i = 0; i < idx; ++i)
            it = it->next;
        
        return it->data;
    }
private:
    size_t size_;
    node<T>* head_;
    node<T>* tail_;
};
