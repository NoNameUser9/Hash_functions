#pragma once
#include "pair.hpp"
template<class T>
struct node
{
    pair<T> data;
    node* prev;
    node* next;
};
