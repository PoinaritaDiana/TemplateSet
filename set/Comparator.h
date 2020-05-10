#pragma once

template <typename T>
class Comparator{
public:
	bool operator () (const T& val1, const T& val2) const{
		if(val1<val2)
			return true;
		else
			return false;
	}
};


//Specializarea clasei Comparator pentru int, 
//care sa considere doi intregi egali daca au aceeasi paritate
template <>
class Comparator<int> {
public:
	int operator() (const int& i1, const int& i2) const {
		if (i1 % 2== i2 % 2)
			return false;
		else
			return true;
	}
};