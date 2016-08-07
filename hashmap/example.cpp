#include "hash_map.hpp"
#include <string>
#include <iostream>

int main(){
  dicts::HashMap<int,std::string> hm;
  
  hm[-1] = "aaa";
  hm[2] = "bbb";
  
  for(int i = 0; i < 100000; i++) {
    hm[i] = "cccd";
  }
  
  std::cout<<hm[-1]<<std::endl;

  std::cout<<hm[2]<<std::endl;
}
