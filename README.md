# TemplateSet
 
Set <class T, class F = Comparator> 
Implement a template class for SET data structure:
The class provides quick access to the items it stores, without imposing any restrictions on the order of the items. 
The Comparator class will be used as the default comparator to determine if a value already exists in the set.

The Set class must provide: 
* constructor without parameters that initializes an empty set
* copy constructor and = operator
* method for adding and deleting an element from the set
* method that checks if an element is in the set
* method that returns the number of elements in the set
* overloading << operator
* specialization of the class Comparator for int, which considers two integers equal if they have the same parity
