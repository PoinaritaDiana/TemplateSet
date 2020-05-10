#pragma once
#include "Comparator.h"
#include "Node.h"
#include <list>
using namespace std;

template <typename T, typename F= Comparator<T>>
class Set {
    Node <T>* root;
    int size;
    Comparator<T> cmp;


    void cleanup(Node<T>*);  //Done

    void fixInsert(Node<T>*); //Done
    void leftRotate(Node<T>*); //Done
    void rightRotate(Node<T>*); //Done

public:
    Set(); //Done
    Set(const Set<T>& s);  //Done
    ~Set(); //Done

    Set<T>& operator =(const Set<T>&); //Done
    void insert(const T&); //Done
    void remove(const T&);
    int sizeSet() const;      //Done 
    bool find(const T&) const;       //Done

    template <typename U>
    friend ostream& operator <<(ostream&, const Set<U>&);   //Done
};


template<typename T, typename F>
Set<T, F>::Set() :size(0), root(NULL) {}


template<typename T, typename F>
Set<T, F>::Set(const Set<T>& s){
    if (s.size == 0)
        return;
    list <Node<T>*> queue;
    queue.push_back(s.root);
    while (queue.size()) {
        this->insert(queue.front()->info);
        if (queue.front()->left)
            queue.push_back(queue.front()->left);
        if (queue.front()->right)
            queue.push_back(queue.front()->right);
        queue.pop_front();
    }
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
    
    list <Node<T>*> queue;
    queue.push_back(s.root);
    while (queue.size()) {
        this->insert(queue.front()->info);
        if (queue.front()->left)
            queue.push_back(queue.front()->left);
        if (queue.front()->right)
            queue.push_back(queue.front()->right);
        queue.pop_front();
    }
    return *this;
}


template <typename T, typename F>
void Set<T,F>::cleanup(Node<T>* n){
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
void Set<T, F>::leftRotate(Node<T>* n) {
    Node<T>* nRight = n->right;
    nRight->parent = n->parent;
    if (n->parent == NULL)
        root = nRight;
    else {
        if (n == n->parent->left)
            n->parent->left = nRight;
        else
            n->parent->right = nRight;
    }
    n->parent = nRight;
    n->right = nRight->left;
    if (n->right != NULL)
        n->right->parent = n;
    nRight->left = n;
}


template<typename T, typename F>
void Set<T, F>::rightRotate(Node<T>* n) {
    Node<T>* nLeft = n->left;
    nLeft->parent = n->parent;
    if (nLeft->parent == NULL)
        root = nLeft;
    else {
        if (n == n->parent->left)
            n->parent->left = nLeft;
        else
            n->parent->right = nLeft;
    }
    n->parent = nLeft;
    n->left = nLeft->right;
    if (n->left != NULL)
        n->left->parent = n;
    nLeft->right = n;
}

template<typename T, typename F>
void Set<T, F>::fixInsert(Node<T>* newNode) {
    while (newNode != root && newNode->color == 'r' && newNode->parent->color == 'r') {
        Node<T>* parentNode = newNode->parent;
        Node<T>* grandParentNode = parentNode->parent;
        Node<T>* uncleNode = NULL;

        //Left
        if (parentNode == grandParentNode->left) {
            uncleNode = grandParentNode->right;
            if (uncleNode != NULL && uncleNode->color == 'r') {
                grandParentNode->color = 'r';
                parentNode->color = 'b';
                uncleNode->color = 'b';
                newNode = grandParentNode;
            }
            else {
                //Left-Right 
                if (newNode == parentNode->right) {
                    newNode = parentNode;
                    leftRotate(newNode);
                }
                rightRotate(grandParentNode);
                newNode->parent->color = 'b';
                grandParentNode->color = 'r';
            }
        }

        //Right
        else {
            uncleNode = grandParentNode->left;
            if (uncleNode != NULL && uncleNode->color == 'r') {
                grandParentNode->color = 'r';
                parentNode->color = 'b';
                uncleNode->color = 'b';
                newNode = grandParentNode;
            }
            else {
                //Right-Left
                if (newNode == parentNode->left) {
                    newNode = parentNode;
                    rightRotate(newNode);
                }
                leftRotate(grandParentNode);
                newNode->parent->color = 'b';
                grandParentNode->color = 'r';
            }
        }
    }
    root->color = 'b';
}


template<typename T, typename F>
void Set<T, F>::insert(const T& t){
    int ok = 1;
    Node<T>* newNode = new Node<T>(t);
    Node<T>* n = root;
    Node<T>* previous = NULL;

    if (n == NULL) {
        root = newNode;
        this->size++;
    }
    else {
        while (n != NULL && ok == 1) {
            previous = n;
            if (cmp(t, n->info))
                n = n->left;
            else {
                if (!cmp(t, n->info) && cmp(n->info,t))
                    n = n->right;
                else
                    ok = 0;
            }
        }
        if (ok) {
            this->size++;
            if (cmp(t,previous->info))
                previous->left = newNode;
            else
                previous->right = newNode;
            newNode->parent = previous;
        }
    }
    if (ok)
        fixInsert(newNode);
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
bool Set<T, F>::find(const T& t)const{
    Node<T>* rd = this->root;
    if (rd == NULL) return false;
    while (rd) {
        if (!cmp(rd->info, t) && !cmp(t, rd->info)) 
            return true;
        if (!cmp(rd->info, t) && cmp(t, rd->info)) 
            rd=rd->left;
        if (cmp(rd->info, t) && !cmp(t, rd->info)) 
            rd=rd->right;
    }
    return false;
}

template<typename T>
ostream& operator<<(ostream& out, const Set<T>& s){
    if (s.root) {
        list <Node<T>*> queue;
        queue.push_back(s.root);
        while (queue.size()) {
            out << queue.front()->getInfo() << " ";
            if (queue.front()->getLeft())
                queue.push_back(queue.front()->getLeft());
            if (queue.front()->getRight())
                queue.push_back(queue.front()->getRight());
            queue.pop_front();
        }
    }
    return out;
}

