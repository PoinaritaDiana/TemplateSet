#pragma once
#include "Comparator.h"
#include "Node.h"
#include <list>
using namespace std;

template <typename T, typename F= Comparator<T>>
class Set {
    Node <T>* root;
    int size;

    void cleanup(Node<T>* &);  //Done

public:
    Set(); //Done
    Set(const Set<T>& s);
    ~Set(); //Done

    Set<T>& operator =(const Set<T>&);
    void insert(const T&);
    void remove(const T&);
    int sizeSet() const;      //Done
    bool find(const T&, T* const) const;       //Done

    template <typename U>
    friend ostream& operator <<(ostream&, const Set<U>&); //Done
};


template<typename T, typename F>
Set<T, F>::Set() :size(0), root(NULL) {}


template<typename T, typename F>
Set<T, F>::Set(const Set<T>& s){
    if (s.size == 0)
        return;
    this->size = s.size;
    Node<T>* n = root;
    this->root = new Node<T>(n->info);

}


template<typename T, typename F>
Set<T>& Set<T, F>::operator=(const Set<T>& s) {
    if (&s == this) {
        return *this;
    }
    this->~Set();

    if (s.size == 0) {
        return *this;
    }
    this->size = s.size;
    Node<T>* n = root;
    this->root = new Node<T>(n->info);

    return *this;
}


template <typename T, typename F>
void Set<T,F>::cleanup(Node<T>*& n){
    if (n) {
        cleanup(n->left);
        cleanup(n->right);
        delete n;
    }
}


template<typename T, typename F>
Set<T, F>::~Set(){
    this->cleanup(root);
    root = NULL;
    size = 0;
}



template<typename T, typename F>
void Set<T, F>::insert(const T& t)
{
}

template<typename T, typename F>
void Set<T, F>::remove(const T& t){
   F comp;
}

template<typename T, typename F>
int Set<T, F>::sizeSet() const{
    return this->size;
}

template<typename T, typename F>
bool Set<T, F>::find(const T& t, T* const rd)const{
    if (rd == NULL) return false;
    else {
        F comp;
        if (!comp(rd->info,t) && !comp(t, rd->info)) return true;
        if (!comp(rd->info, t) && comp(t, rd->info)) return find(t, rd->left);
        if (comp(rd->info, t) && !comp(t, rd->info)) return find(t, rd->left);
    }
}

template<typename T>
ostream& operator<<(ostream& out, const Set<T>& s){
    if (s.root) {
        list <Node<T>*> queue;
        queue.push_back(s.root);
        while (queue.size()) {
            out << queue.front()->info << " ";
            if (queue.front()->left)
                queue.push_back(queue.front()->left);
            if (queue.front()->right)
                queue.push_back(queue.front()->right);
            queue.pop_front();
        }
    }
}

