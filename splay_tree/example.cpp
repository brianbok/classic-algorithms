#include <iostream>
#include "splay_tree.hpp"
#include <string>

struct Node {
  Node* parent;
  Node* left;
  Node* right;
  int key;
  
  Node(int k): key(k){
    left = right = nullptr;
    parent = nullptr;
  }
};

void rightRotate(Node* n) {
  Node* new_root = n->left;
  
  n->left = new_root->right;
  
  new_root->right = n;
  
  new_root->parent = n->parent;
  
  n->parent = new_root;
  
  if (n->left != nullptr){
    n->left->parent = n;
  }
  
  if (new_root->parent != nullptr and new_root->parent->left == n){
    new_root->parent->left = new_root;
  }
  else if (new_root->parent != nullptr) {
    new_root->parent->right = new_root;
  }
}

void leftRotate(Node* n) {
  Node* new_root = n->right;
  
  n->right = new_root->left;
  
  new_root->left = n;
  
  new_root->parent = n->parent;
  
  n->parent = new_root;
  
  if (n->right != nullptr) {
    n->right->parent = n;
  }
  
  if (new_root->parent != nullptr and new_root->parent->left == n) {
    new_root->parent->left = new_root;
  }
  else if (new_root->parent != nullptr) {
    new_root->parent->right = new_root;
  }
}
void printNode(Node* node, std::string prefix, bool isTail){
    if (node == nullptr){
      return;
    }
    std::cout << (prefix + (isTail ? "└── " : "├── ")) << node->key << std::endl;
    if (node->left != nullptr){
      printNode(node->right, (prefix + (isTail ? "    " : "│   ")), false);
      printNode(node->left, (prefix + (isTail ? "    " : "│   ")), true);
    }
    else {
      printNode(node->right, (prefix + (isTail ? "    " : "│   ")), true);
    }
}

int main(){
  dicts::SplayDict<int, std::string > spDict;
  spDict.set(1,"aaa");
  //std::cout << spDict << std::endl;
  spDict.set(2,"baa");
  //std::cout << spDict << std::endl;
  spDict.set(-1,"baa");
  //std::cout << spDict << std::endl;
  spDict.set(0,"baa");
  //std::cout << spDict << std::endl;
  spDict.set(-2,"baa");
  //std::cout << spDict << std::endl;
  spDict.set(-3,"baa");
  //std::cout << spDict << std::endl;
  spDict.set(5,"baa");
  //std::cout << spDict << std::endl;

  //spDict.erase(2);
  
  
  dicts::SplayDict<int, std::string>::ConstIterator it = spDict.begin();
  
  std::cout << spDict << std::endl;
  for(it = spDict.begin(); it != spDict.end(); it++){
  
    std::cout<< (*it).first <<" "<<(*it).second<<std::endl;
      
  }
  
  std::cout<<spDict.get(1)<<std::endl;
  
  std::cout << spDict << std::endl;
  
  
}
