#include "max_heap.hpp"
#include <iostream>

int main(){
  MaxHeap<int> heap;
  
  heap.insert(1);
  heap.insert(2);
  
  std::cout<<heap.getMax()<<std::endl;
  heap.popMax();
  std::cout<<heap.getMax()<<std::endl;
  
}
