#include <iostream>
#include "splay_tree.hpp"
#include <string>

int main(){
  dicts::SplayDict<int, std::string > spDict;
  spDict.set(1,"aaa");
  spDict.set(2,"baa");
  spDict.set(-1,"baa");
  spDict.set(0,"baa");
  spDict.set(-2,"baa");
  
  spDict.set(-3,"baa");
  
  spDict.set(5,"baa");
  std::cout << spDict << std::endl;

  spDict.erase(2);
  std::cout << spDict << std::endl;

  
  dicts::SplayDict<int, std::string>::ConstIterator it = spDict.begin();
  
  for(it = spDict.begin(); it != spDict.end(); it++){
    std::cout<< (*it).first <<" "<<(*it).second<<std::endl;
      
  }
  
  std::cout<<spDict.get(-2)<<std::endl;
  
  std::cout << spDict << std::endl;
  
  
}
