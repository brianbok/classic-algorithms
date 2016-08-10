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
      
      void insert(T el);
      
      MaxHeap(T a[], int n);
      MaxHeap();
      
   private:
      void heapify();
      void heapDown(int);
      void heapUp(int);
      
      int leftChild(int);
      int rightChild(int);
      int parent(int);
      
      bool validNode(int);

      std::vector<T> elems;
};

template <class T>
MaxHeap<T>::MaxHeap(T a[], int n){
   elems = std::vector<T>(a,a+n);
   heapify();
}

template <class T>
MaxHeap<T>::MaxHeap(){
   elems = std::vector<T>();
}


template <class T>
void MaxHeap<T>::insert(T e){
  elems.push_back(e);
  heapUp((int) elems.size() - 1);
}

template <class T>
void MaxHeap<T>::heapUp(int x){
  while(x != 0 and elems[x] > elems[parent(x)]){
    std::swap(elems[x], elems[parent(x)]);
    x = parent(x);
  }
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

