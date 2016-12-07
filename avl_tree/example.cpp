#include <iostream>
#include "avl_tree.hpp"
#include <string>

int main(){
  dicts::AVLDict<int, std::string > avl_dict;
  
  avl_dict.set(1,"aaa");
  avl_dict.set(2,"baa");
  avl_dict.set(-1,"baa");
  avl_dict.set(0,"baa");
  avl_dict.set(-2,"baa");
  std::cout << avl_dict << std::endl;
  
  avl_dict.set(-3,"baa");
  std::cout << avl_dict << std::endl;

  avl_dict.set(5,"baa");
  std::cout << avl_dict << std::endl;

  avl_dict.erase(2);
  std::cout << avl_dict << std::endl;

  
  dicts::AVLDict<int, std::string>::ConstIterator it = avl_dict.begin();
  
  for(it = avl_dict.begin(); it != avl_dict.end(); it++){
    std::cout<< (*it).first <<" "<<(*it).second<<std::endl;
      
  }
  
  std::cout<<avl_dict.get(-2)<<std::endl;
  
  std::cout << avl_dict << std::endl;
  
  
}
