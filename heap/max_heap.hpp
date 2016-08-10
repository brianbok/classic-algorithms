#ifndef MAX_HEAP_HPP_
#define MAX_HEAP_HPP_

#include <iostream>
#include <cassert>
#include <algorithm>    // std::swap
#include <vector> 

template<class T>
class MaxHeap{
   public:
      T getMax();
      void popMax();
      MaxHeap(T a[], int n);
   private:
      std::vector<T> elems;
      
      void heapify();
      void heapDown(int);
      
      int leftChild(int);
      int rightChild(int);
      int parent(int);
      
      bool validNode(int);
};

template <class T>
MaxHeap<T>::MaxHeap(T a[], int n){
   elems = std::vector<T>(a,a+n);
   heapify();
}


template <class T>
void MaxHeap<T>::heapDown(int x){
   int maxLeftRightMe = elems[x];
   int maxLeftRightMeIdx = x;
      
   if (validNode(leftChild(x))){
      
      if (elems[leftChild(x)] > maxLeftRightMe){
         maxLeftRightMe = elems[leftChild(x)];
         maxLeftRightMeIdx = leftChild(x);
      }
      
   }
   
   if (validNode(rightChild(x))){
      
      if (elems[rightChild(x)] > maxLeftRightMe){
         maxLeftRightMe = elems[rightChild(x)];
         maxLeftRightMeIdx = rightChild(x);
      }
      
   }
   
   if (maxLeftRightMeIdx != x){
      std::swap(elems[x], elems[maxLeftRightMeIdx]);
      heapDown(maxLeftRightMeIdx);
   }
 
}

template <class T>
void MaxHeap<T>::heapify(){
   for (int i=elems.size()/2; i>=0;i--){ // TODO:chequear div
      heapDown(i);
   }
}

template <class T>
int MaxHeap<T>::leftChild(int x){
   return 2*x+1;
}

template <class T>
int MaxHeap<T>::rightChild(int x){
   return 2*x+2;
}


template <class T>
int MaxHeap<T>::parent(int x){
   return (x-1)/2;
}

template <class T>
bool MaxHeap<T>::validNode(int x){
   return x <= (int) (elems.size())-1;
}

template <class T>
T MaxHeap<T>::getMax(){
   return elems[0];
}


template <class T>
void MaxHeap<T>::popMax(){
   std::swap(elems[0], elems[(int)elems.size()-1]);
   elems.pop_back();
   heapDown(0);
}

#endif //MAX_HEAP_HPP_

