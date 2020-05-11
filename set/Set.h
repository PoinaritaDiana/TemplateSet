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

    void cleanup(Node<T>*);  

    //Metode folosite pentru insert
    void fixInsert(Node<T>*);   
    void leftRotate(Node<T>*);   
    void rightRotate(Node<T>*); 

    //Metode folosite pentru remove
    void removeNode(Node<T>*);  
    Node<T>* minimum(Node<T>*) const;
    Node<T>* search(const T&); 
    void fixDelete(Node<T>*);

public:
    Set(); 
    Set(const Set<T>& s);  
    ~Set();

    Set<T>& operator =(const Set<T>&); 
    void insert(const T&); 
    void remove(const T&); 
    int sizeSet() const;      
    bool find(const T&) const;       

    template <typename U>
    friend ostream& operator <<(ostream&, const Set<U>&);   
};


//Constructor fara parametrii care initializeaza un set gol
template<typename T, typename F>
Set<T, F>::Set() :size(0), root(NULL) {}


//Constructor de copiere
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


//Functie recursiva pentru a sterge elementele SDR/postordine
template <typename T, typename F>
void Set<T, F>::cleanup(Node<T>* n) {
    if (n) {
        cleanup(n->left);
        cleanup(n->right);
        delete n;
    }
}


//Destructor
template<typename T, typename F>
Set<T, F>::~Set() {
    this->cleanup(root);
    root = NULL;
    size = 0;
}


//Supraincarcare operator de atribuire "="
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


//Roratie spre stanga
template<typename T, typename F>
void Set<T, F>::leftRotate(Node<T>* n) {
    //Nodul n "coboara", iar locul lui este luat de fiul drept
    //Parintele nodului n devine parintele fiului drept
    //Fiul drept devine parinte pentru nodul n 
    //Fiul stang al nRight devine fiu drept pentru nodul n
    
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


//Rotire spre dreapta
template<typename T, typename F>
void Set<T, F>::rightRotate(Node<T>* n) {
   //Nodul n "coboara", iar locul lui este luat de fiul stang
   //Parintele nodului n devine parintele fiului stang
   //Fiul stang devine parinte pentru nodul n 
   //Fiul drept al nLeft devine fiu stang pentru nodul n

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


//Functie pentru garantarea pastrarii proprietatilor RBT dupa inserare 
template<typename T, typename F>
void Set<T, F>::fixInsert(Node<T>* newNode) {
    //Daca nodul si parintele sau sunt rosii, se incalca o regula RBT
    while (newNode != root && newNode->color == 'r' && newNode->parent->color == 'r') {
        Node<T>* parentNode = newNode->parent;
        Node<T>* grandParentNode = parentNode->parent;
        Node<T>* uncleNode = NULL;

        //Left case
        if (parentNode == grandParentNode->left) {
            uncleNode = grandParentNode->right;
            //Daca uncle e NULL sau are culoarea rosie, atunci switchColors
            //parent=black, uncle=black, grandParent=red
            //Verific daca prin schimbarea culorii, grandParent incalca vreo regula
            if (uncleNode != NULL && uncleNode->color == 'r') {
                grandParentNode->color = 'r';
                parentNode->color = 'b';
                uncleNode->color = 'b';
                newNode = grandParentNode;
            }
            else {
                //Daca uncle e NULL(e tot negru) sau are culoarea neagra 

                //Left-Right case(triunghi): rotatie-stanga(parinte),
                //iar apoi rotatie-dreapta(grandParent)
                if (newNode == parentNode->right) {
                    newNode = parentNode;
                    leftRotate(newNode);
                }

                //Left-Left case(linie): rotatie-dreapta(grandParent)
                //Parent primeste culoarea neagra,
                //iar fostul grandParent culoarea rosie
                rightRotate(grandParentNode);
                newNode->parent->color = 'b';
                grandParentNode->color = 'r';
            }
        }
        //Right case (se construieste in oglinda cu Left case)
        else {
            uncleNode = grandParentNode->left;
            //Daca uncle e NULL sau are culoarea rosie, atunci switchColors
            //parent=black, uncle=black, grandParent=red
            //Verific daca prin schimbarea culorii, grandParent incalca vreo regula
            if (uncleNode != NULL && uncleNode->color == 'r') {
                grandParentNode->color = 'r';
                parentNode->color = 'b';
                uncleNode->color = 'b';
                newNode = grandParentNode;
            }
            else {
                //Daca uncle e NULL(e tot negru) sau are culoarea neagra 
                //Right-Left case(triunghi): rotatie-dreapta(parinte),
                //iar apoi rotatie-stanga(grandParent)
                if (newNode == parentNode->left) {
                    newNode = parentNode;
                    rightRotate(newNode);
                }

                //Right-Right case(linie): rotatie-stanga(grandParent)
                //Parent primeste culoarea neagra,
                //iar fostul grandParent culoarea rosie
                leftRotate(grandParentNode);
                newNode->parent->color = 'b';
                grandParentNode->color = 'r';
            }
        }
    }
    //Radacina are intotdeauna culoarea neagra
    root->color = 'b';
}


//Metoda pentru adaugare element in set
template<typename T, typename F>
void Set<T, F>::insert(const T& t){
    //Daca valoarea se mai gaseste in set, ok devine 0
    int ok = 1;

    Node<T>* newNode = new Node<T>(t);  //Nodul de inserat
    Node<T>* previous = NULL;           //Nodul anterior(viitor parinte al nodului de inserat)
    Node<T>* n = root;

    //Daca setul este gol
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
        //Daca valoarea poate fi inserata, verific ordinea fata de parinte
        if (ok) {
            this->size++;
            if (cmp(t,previous->info))
                previous->left = newNode;
            else
                previous->right = newNode;
            newNode->parent = previous;
        }
    }
    //Dupa ce inserez nodul, verific daca proprietatile RBT sunt pastrate
    if (ok)
        fixInsert(newNode);
}


//Returneaza cel mai din stanga nod din subarborele drept al unui nod
template<typename T, typename F>
Node<T>* Set<T, F>::minimum(Node<T>* n) const {
    Node<T>* minNode = n;
    while (minNode->left != NULL)
        minNode = minNode->left;
    return minNode;
}


//Metoda de cautare, doar ca returneaza nodul cu valoarea data
template<typename T, typename F>
Node<T>* Set<T, F>::search(const T& t) {
    Node<T>* n = this->root;
    while (n) {
        if (!cmp(n->info, t) && !cmp(t, n->info))
            return n;
        else {
            if (!cmp(n->info, t) && cmp(t, n->info))
                n = n->left;
            else
                n = n->right;
        }
    }
    return NULL;
}


template<typename T, typename F>
void Set<T, F>::fixDelete(Node<T>* n) {
    if (n == root)
        return;

    Node<T>* parent = n->parent;
    Node<T>* sibling;
    if (parent == NULL)
        sibling = NULL;
    if (n == n->parent->left)
        sibling = parent->right;
    else
        sibling = parent->left;

    if (sibling == NULL)
        fixDelete(parent);
    else {
        //Daca sibling rosu : switchColor(parent,sibling) = {parent = red, sibling = black}
        //Rotate(parent), transformandu-se in unul din cazurile cand sibling negru: fixDelete(n)
        if (sibling->color == 'r') {
            parent->color = 'r';
            sibling->color = 'b';
            //Daca sibling = left child of parent -> rightRotate(parent)
            if (sibling == sibling->parent->left)
                rightRotate(parent);
            //Daca sibling = right child of parent -> leftRotate(parent)
            else
                leftRotate(parent);
            fixDelete(n);
        }
        else {
            //Daca sibling negru si ambii copii negru(sau NULL):
            //Renuntam la "un negru" de la n(double black) si sibling(black)
            // => n are culoarea neagra si sibling rosie
            //Adaug o culoare neagra extra la parent (parent devine black sau double-black)
            //Apelez fixDelete(parent) daca parent este double-black
            if (sibling->color == 'b' && (sibling->left == NULL || sibling->left->color == 'b') && 
                (sibling->right == NULL || sibling->right->color == 'b')){
                sibling->color = 'r';
                if (parent->color == 'b')
                    fixDelete(parent);
                else
                    parent->color = 'b';
            }
            //Daca sibling negru si are cel putin un fiu de culoare rosie
            else {
                //Daca sibling negru si fiul stang este rosu (deci, fiul drept are culoarea neagra):
                //sibling = red, leftChild = black (swapColor(leftChild,sibling))
                //rightRotate(sibling) => se transforma in alt caz
                if (sibling->left != NULL and sibling->left->color == 'r') {
                    sibling->color = 'r';
                    sibling->left->color = 'b';
                    rightRotate(sibling);
                    fixDelete(n);
                }
                //Daca sibling negru si fiul drept este rosu (deci, fiul stang are culoarea neagra):
                //rightChild = black si swapColor(sibling, parent)
                //leftRotate(parent)
                if (sibling->right != NULL and sibling->right->color == 'r') {
                    sibling->right->color = 'b';
                    sibling->color = parent->color;
                    parent->color = 'b';
                    leftRotate(parent);
                }
            }
        }
    }
}


//Metoda pentru stergere element din set(dupa nod)
template<typename T, typename F>
void Set<T, F>::removeNode(Node<T>* nodeDelete) {
    //Aflu nodul cu care va fi inlocuit nodeDelete
    Node<T>* replace = NULL;
    if (nodeDelete->right == NULL && nodeDelete->left == NULL)
        replace = NULL;
    if (nodeDelete->right == NULL && nodeDelete->left != NULL)
        replace = nodeDelete->left;
    if (nodeDelete->right != NULL && nodeDelete->left == NULL)
        replace = nodeDelete->right;
    if (nodeDelete->right != NULL && nodeDelete->left != NULL)
        replace = minimum(nodeDelete->right);


    //Daca nodeDelete este frunza(adica repalce=NULL)
    if (replace == NULL) {
        //Daca este radacina, inseamna ca set-ul devine gol
        if (nodeDelete == root)
            root = NULL;
        else {
            //Daca nodeDelete are culoarea neagra -> double black
            //Trebuie sa verific daca se pastreaza proprietatile RBT
            if (nodeDelete->color == 'b')
                fixDelete(nodeDelete);
            //Daca este rosu, doar il sterg pentru ca nu influenteaza proprietatile
            //Sau dupa ce se rezolva fixDelete, in cazul in care este de culoare neagra
            if (nodeDelete == nodeDelete->parent->left)
                nodeDelete->parent->left = NULL;
            else
                nodeDelete->parent->right = NULL;
            delete nodeDelete;
        }
    }
    else {
        //Daca nodeDelete are 2 copii
        if (nodeDelete->left != NULL && nodeDelete->right != NULL) {
            //Interschimb valoarea celor doua noduri(replace si nodeDelete)
            //Apelez removeNode pentru replace (care va avea maxim un copil)
            T val = nodeDelete->info;
            nodeDelete->info = replace->info;
            replace->info = val;
            removeNode(replace);
        }
        //Daca nodeDelete are un copil
        else {
            //Daca nodul de sters este radacina
            //Replace devine noua radacina (avand neaparat culoarea neagra)
            if (nodeDelete == root) {
                replace->parent = NULL;
                replace->color = 'b';
                root = replace;
                delete nodeDelete;
            }
            else {
                //Refac legaturile
                if (nodeDelete == nodeDelete->parent->left)
                    nodeDelete->parent->left = replace;
                else
                    nodeDelete->parent->right = replace;
                if (replace != NULL)
                    replace->parent = nodeDelete->parent;

                //Daca replace negru si nodeDelete negru -> double black => trebuie sa verific prop
                if ((replace->color == 'b' || replace == NULL) && nodeDelete->color == 'b') {
                    nodeDelete->info = replace->info;
                    if (replace == nodeDelete->left) {
                        replace->right = nodeDelete->right;
                        if (nodeDelete->right != NULL)
                            nodeDelete->right->parent = replace;
                    }
                    else {
                        replace->left = nodeDelete->left;
                        if (nodeDelete->left != NULL)
                            nodeDelete->left->parent = replace;
                    }
                    delete replace;
                    fixDelete(nodeDelete);
                }
                else {
                    //Daca au culori diferite, atunci mut replace si il colorez cu negru
                    replace->color = 'b';
                    if (replace == nodeDelete->left) {
                        replace->right = nodeDelete->right;
                        if (nodeDelete->right != NULL)
                            nodeDelete->right->parent = replace;
                    }
                    else {
                        replace->left = nodeDelete->left;
                        if (nodeDelete->left != NULL)
                            nodeDelete->left->parent = replace;
                    }
                    delete nodeDelete;
                }
            }
        }
    }
}


//Metoda pentru stergere element din set(dupa o anumita valoare)
template<typename T, typename F>
void Set<T, F>::remove(const T& t){
    if (root == NULL)
        return;

    Node<T>* nodeDelete = search(t);
    if (nodeDelete != NULL) {
        size--;
        removeNode(nodeDelete);
    }
}


//Metoda care intoarce numarul de elemente din set
template<typename T, typename F>
int Set<T, F>::sizeSet() const{
    return this->size;
}


//Metoda care verifica daca un element se afla in set
template<typename T, typename F>
bool Set<T, F>::find(const T& t) const{
    Node<T>* rd = this->root;
    if (rd == NULL) 
        return false;
    while (rd) {
        if (!cmp(rd->info, t) && !cmp(t, rd->info)) 
            return true;
        else {
            if (!cmp(rd->info, t) && cmp(t, rd->info))
                rd = rd->left;
            else 
                rd = rd->right;
        }
    }
    return false;
}


//Supraincarcarea operatorului de afisare "<<"
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

