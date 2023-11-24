#pragma once
#include "pair.hxx"
template<class T>
struct node
{
    pair<T> data;
    node* prev;
    node* next;
};
