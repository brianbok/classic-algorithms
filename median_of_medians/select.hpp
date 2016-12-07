#ifndef aaaa
#define aaaa

#include <vector>
#include <math.h>
#include <iostream>
#include <algorithm>

namespace order {

template<typename T>
T select(std::vector<T> a, int r);

template<typename T>
T  median_of_medians(std::vector<T> a) {
	std::vector<std::vector<T> > lists;
	
	for (int i = 0; i < a.size(); i+= 5) {
		std::vector<T> v;
		lists.push_back(v);
		for (int j = i; j < std::min((int) a.size(), i+5); j++) {
			lists[i/5].push_back(a[j]);
		} 
	}
	
	std::vector<T> medians;
	
	for(int i = 0; i < lists.size(); i++) {
		std::sort(lists[i].begin(), lists[i].end());
		int sz = lists[i].size();
		medians.push_back(lists[i][(sz - 1) / 2]);
	}
	
	return select<T>(medians, ((int)medians.size() - 1) / 2);
}

template<typename T>
T select(std::vector<T> a, int r) {
	if (a.size() == 1) {
		return a[0];
	}
	
	T x = median_of_medians<T>(a);

	std::vector<T> small, big;
	for(const auto& elem : a) {
		if (elem < x) {
			small.push_back(elem);
		} else if (elem > x){
			big.push_back(elem);
		}
	}
	
	//std::cout << r << " "<< small.size() << " " << big.size() << std::endl;
	
	if (r == small.size()) {
		return x;
	} else if (r < small.size() ) {
		return select<T>(small, r);
	} else {
		return select<T>(big, r - small.size() - 1);
	}
			
}


} 
#endif
