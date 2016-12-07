#include "select.hpp"
#include <iostream>


int main() {
	std::vector<int> v;
	for(int i = 0; i < 10000; i++) {
		v.push_back(i);
	}
	
	
	int x = order::select(v, 1000);
	
	std::cout << x << std::endl;

}
