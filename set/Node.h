#pragma once
#include <iostream>
using namespace std;

template <typename T, typename Comparator> class Set;

template <typename T>
class Node{
    T info;
    Node<T>* left;
    Node<T>* right;
    Node<T>* parent;
    char color;

public:
    Node(const T&, Node<T>* = NULL, Node<T>* = NULL, Node<T>* = NULL, char c = 'r');
    friend class Set<T,Comparator<T>>;
};

template<typename T>
Node<T>::Node(const T& t, Node<T>* l, Node<T>* r, Node<T>* p, char ch) : info(t), left(l), right(r), parent(p), color(ch){}