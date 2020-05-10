#include <iostream>
#include "Set.h"

using namespace std;

int main(){

    /*Set<char> s;
    s.insert('a');
    s.insert('b');
    s.insert('c');
    s.insert('a');
    s.insert('d');
    s.insert('f');
    s.insert('e');*/
    Set<int>s;
    s.insert(2);
    cout << s << endl;
    cout << s.sizeSet() << endl;
    Set<int>cs;
    cs.insert(1);
    cs= s;
    cs.insert(3);
    cout << cs << endl;
    cout << cs.sizeSet() << endl;

    return 0;
}
