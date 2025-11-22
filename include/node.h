#pragma once

template <typename T>
struct Node {
    T value;
    Node<T>* next = nullptr;
    Node<T>* prev = nullptr;
    Node(const T& val, Node<T>* nxt, Node<T>* prv) 
    : value(val), next(nxt), prev(prv) {}
    Node() : value(), next(nullptr), prev(nullptr) {}
};
