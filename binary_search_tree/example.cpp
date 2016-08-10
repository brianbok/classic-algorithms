#include <iostream>
#include "binary_search_tree.hpp"
#include <string>

int main(){
  dicts::BST<int, std::string> bst;
  
  bst.set(1,"aaa");
  bst.set(2,"baa");
  bst.set(-1,"baa");
  bst.set(0,"baa");
  bst.set(-2,"baa");
  bst.set(-3,"baa");
  bst.set(5,"baa");
  //bst.erase(2);
  
  dicts::BST<int, std::string>::ConstIterator it = bst.begin();
  
  for(it = bst.begin(); it != bst.end(); it++){
  
    std::cout<< (*it).first <<" "<<(*it).second<<std::endl;
      
  }
  
  std::cout<<bst.get(1)<<std::endl;
  
  
}
