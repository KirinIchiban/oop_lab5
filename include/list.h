#pragma once
#include <memory>
#include "iterator.h"
#include <memory_resource>


template <typename T>
class D_linked_list {
public:
    D_linked_list() : polymorph_allocator(std::pmr::get_default_resource()) {}
    D_linked_list(std::pmr::memory_resource* mem_pointer) : polymorph_allocator(mem_pointer) {}

    My_iterator<T> begin();
    My_iterator<T> end();
    bool is_empty();
    int lenght();
	void push_back(const T& value);
	void push_front(const T& value);
	bool pop_back();
	bool pop_front();
    T get_value(Node<T>* node);
    Node<T>* get_head() const;
    ~D_linked_list() {
        Node<T>* curr = head;
        while (curr) {
            Node<T>* next = curr->next;
            destroy_elem(curr);        
            polymorph_allocator.deallocate(curr, 1); 
            curr = next;
        }
    }
    
private:
    Node<T>* head = nullptr;
    std::pmr::polymorphic_allocator<Node<T>> polymorph_allocator;
    void destroy_elem(Node<T>* elem) {
        std::allocator_traits<decltype(polymorph_allocator)>::destroy(polymorph_allocator, &(elem->value));
        //polymorph_allocator.deallocate(elem, 1);
    }
};

template <typename T>
My_iterator<T> D_linked_list<T>::begin() {
    return My_iterator(head);
}

template <typename T>
My_iterator<T> D_linked_list<T>::end() {
    return My_iterator<T>(nullptr);
}

template <typename T>
T D_linked_list<T>::get_value(Node<T>* node) {
    if (!node) throw std::runtime_error("Node is null");
    return node->value;
}

template <typename T>
void D_linked_list<T>::push_front(const T& value) {
    Node<T>* new_node = polymorph_allocator.allocate(1);
    polymorph_allocator.construct(new_node, value, head, nullptr);
    if (head != nullptr) {
        head->prev = new_node;
    }
    head = new_node;
}

template <typename T> 
void D_linked_list<T>::push_back(const T& value) {
    Node<T>* new_node = polymorph_allocator.allocate(1);
    polymorph_allocator.construct(new_node, value, nullptr, nullptr);
    if (!head) {
        head = new_node;
        return;
    }
    
    Node<T>* curr = head;
    while (curr->next) {  
        curr = curr->next;  
    }
    curr->next = new_node;  
    new_node->prev = curr;
}

template <typename T>
bool D_linked_list<T>::pop_back() {
    if (!head) {
        return false;
    }
    if (!head->next) {
        pop_front();
        return true;
    }
    Node<T>* cur = head;
    while (cur->next && cur->next->next) {
        cur = cur->next;
    }
    Node<T>* old = cur->next;
    cur->next = nullptr;
    destroy_elem(old);
    polymorph_allocator.deallocate(old, 1);
    return true;
}

template <typename T>
bool D_linked_list<T>::pop_front() {
    if (!head) {
        return false;
    }
    Node<T>* old = head;
    head = head->next;
    polymorph_allocator.deallocate(old, 1);
    return true;
}

template <typename T>
bool D_linked_list<T>::is_empty() {
    if (head == nullptr) return true;
    return false;
}

template <typename T>
int D_linked_list<T>::lenght() {
    int count = 0;
    for (auto it = this->begin(); it != this->end(); ++it) {
        count++;
    }
    return count;
}

template <typename T>
Node<T>* D_linked_list<T>::get_head() const {
            return head;
}