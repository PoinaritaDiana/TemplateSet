#include <iostream>
#include "Set.h"
#include <string>
#include <cassert>

using namespace std;

void setTests() {
    //Teste pentru tipul char
    Set <char> setChar;

    //Test constructor
    assert(setChar.sizeSet() == 0);

    setChar.insert('a');
    setChar.insert('b');
    setChar.insert('c');
    setChar.insert('a');
    setChar.insert('d');
    setChar.insert('f');
    setChar.insert('e');

    //Test obtinere nr. elemente (6 pentru ca 'a' nu se introduce de 2 ori)
    assert(setChar.sizeSet() == 6); 

    //Test pentru a verifica daca un element se afla in set
    assert(setChar.find('a') == 1);
    assert(setChar.find('m') == 0);

    //Test pentru verificare eliminare
    setChar.remove('b');
    assert(setChar.sizeSet() == 5);
    assert(setChar.find('b') == 0);

    //Teste pentru tipul int
    Set <int> sInt;
    sInt.insert(2);
    sInt.insert(4);

    //Test pentru specializarea Comparator
    //2 si 4 au aceeasi paritate, deci nu se mai insereaza 4
    assert(sInt.sizeSet()==1);
  
    Set <int> csInt;
    csInt.insert(1);
    //Test operator de atribuire
    //Acum csInt contine doar elementul 2, deci poate fi inserat 3
    //pentru ca au paritati diferite (deci size=2)
    csInt = sInt;
    csInt.insert(3);
    assert(csInt.sizeSet() == 2); 

    //Teste pentru tipul string
    Set <string> setString;
    setString.insert("informatica");
    setString.insert("carte");
    setString.insert("bine");
    setString.insert("programare");

    //Test pentru eliminare
    //Nu exista "tema" in set, deci size ramane 4
    setString.remove("tema");
    assert(setString.sizeSet() == 4);

    //Test pentru constructorul de copiere
    Set <string> cSetString = setString;
    assert(cSetString.sizeSet() == 4);
    assert(cSetString.find("programare") == 1);

    //Test destructor
    cSetString.~Set();
    assert(cSetString.sizeSet() == 0);
}


int main(){

    setTests();
    cout << "Testele au trecut";
    return 0;
}
