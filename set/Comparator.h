#pragma once

template <typename T>
class Comparator{
public:
	bool operator () (const T& val1, const T& val2){
		if(val1<val2)
			return true;
		else
			return false;
	}
};

template <>
class Comparator<int> {
public:
	int operator() (const int& i1, const int& i2) {
		if (i1 % 2== i2 % 2)
			return false;
		else
			return true;
	}
};