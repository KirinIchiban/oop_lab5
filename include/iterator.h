#pragma once

#include "node.h"

template <typename T>
class My_iterator {
public:
    using difference_type = std::ptrdiff_t; 
    using value_type = T;
    using reference = T& ;
    using pointer = T*;
    using iterator_category = std::forward_iterator_tag;

    My_iterator() noexcept : current(nullptr) {}
    My_iterator(Node<T>* ptr) : current(ptr) {}
    My_iterator(const My_iterator& other) : current(other.current) {}
    
    My_iterator& operator=(const My_iterator& other) {
        if (this != &other) { 
            current = other.current;
        }
        return *this;
    }

    bool operator==(const My_iterator& other) const{
        if(current == other.current) return true;
        return false;
    }

    bool operator!=(const My_iterator& other) const{
        if(current != other.current) return true;
        return false;
    }

    reference operator*() const {return current->value;} 
    pointer operator->() const {return &(current->value);}  

    My_iterator& operator++() {
        if (!current) {
            throw std::runtime_error("False iterator increment");
        }
        current = current->next;
        return *this;
    }

    My_iterator operator++(int) {
        My_iterator temp = *this;
        ++(*this);
        return temp;
    }

    My_iterator& operator--() {
        if (!current) {
            throw std::runtime_error("False iterator decrement");
        }
        current = current->prev;
        return *this;
    }

private:
    Node<T>* current = nullptr;
};