#ifndef SPLAY_TREE_H_
#define SPLAY_TREE_H_

#include <utility>  
#include <iostream>  
#include <stack>  

namespace dicts {

template<typename K, typename V> 
class SplayDict {
  private:
    struct Node; //Forward declaration
  
  public:
    class ConstIterator {
      public:
        const std::pair<K&, V&> operator*() const;
        
        ConstIterator& operator++ (int);
        
        ConstIterator& operator= (const ConstIterator& const_it);
        
        bool operator== (const ConstIterator& const_it) const;
        
        bool operator!= (const ConstIterator& const_it) const;
        
        ConstIterator(Node* root){
          if (root != nullptr){
            current = subtreeMin(root);
            prev = current->parent;
          }
          else {
            current = root;
            prev = nullptr;
          }
        }
        
        ConstIterator(Node* c, Node* p){
          current = c;
          prev = p; 
        }
        
        
      private:
        Node* current;
        Node* prev;
    };
    
    
    SplayDict();
    
    SplayDict(const SplayDict& bst);
    
    ~SplayDict();
    
    bool exists(const K& key) const;
    
    V& get(const K& key);
    
    void set(const K& key, const V& val);
    
    void erase(const K& key);

    SplayDict& operator= (const SplayDict& bst);
    
    ConstIterator begin();
    
    ConstIterator end();
    

  private: 
    struct Node {
      K key;
      V val;
      
      Node* parent;
      Node* left;
      Node* right;
      
      Node(Node* p, const K& k, const V& v) {
        key = k; 
        val = v;
        
        parent  = p;
        left    = nullptr;
        right   = nullptr;
      }
      
    };
    
    static void printNode(std::ostream& os, Node* node, std::string prefix, bool isTail);
    
    Node* findNode(Node* subtree, const K& key) const; 
    
    bool hasGrandParent(Node* subtree) const;
    
    void rightRotate(Node* n);
    
    void leftRotate(Node* n);
    
    bool sideOfChild(Node* subtree, bool zag1, bool zag2) const;
    
    void splay(Node* subtree);
    
    void set(Node*& subtree, Node* parent, const K& key, const V& val);
    
    static Node* subtreeMin(Node* subtree);
    
    static Node* subtreeMaxKey(Node* subtree, Node* parent);
    
    static Node* subtreeMinKey(Node* subtree, Node* parent);
    
    void erase_node(Node* node);
    
    Node* root;
    
  friend class ConstIterator;
  
  friend std::ostream& operator<< (std::ostream& os, const SplayDict<K,V>& spDict){
    printNode(os, spDict.root, "", true);
    return os;
  }

};


template<typename K, typename V> 
SplayDict<K, V>::SplayDict() {
    root = nullptr;
}

template<typename K, typename V> 
SplayDict<K, V>::~SplayDict() {
  std::stack<Node*> nodes;
  nodes.push(root);
  
  while(!nodes.empty()){
    Node* current = nodes.top();
    nodes.pop();
    
    if (current != nullptr){
      nodes.push(current->left);
      nodes.push(current->right);

      delete current;
    }
    
  }
}

template<typename K, typename V> 
SplayDict<K, V>::SplayDict(const SplayDict& bst) {
  for (ConstIterator it = bst.begin(); it != bst.end(); it++) {
    set( (*it).first, (*it).second);
  }
}

template<typename K, typename V> 
bool SplayDict<K, V>::exists(const K& key) const {
  return findNode(root, key) != nullptr;
}

template<typename K, typename V> 
V& SplayDict<K, V>::get(const K& key){
  Node* node = findNode(root, key);
  splay(node);
  return node->val;
}

template<typename K, typename V> 
void SplayDict<K, V>::set(const K& key, const V& val) {
  set(root, nullptr, key, val);
}

template<typename K, typename V> 
void SplayDict<K, V>::erase(const K& key){
  Node* node = findNode(root, key);
  
  erase_node(node);
}

template<typename K, typename V> 
typename SplayDict<K,V>::ConstIterator SplayDict<K, V>::begin(){
  ConstIterator it = ConstIterator(root);
  return it;
}

template<typename K, typename V> 
typename SplayDict<K,V>::ConstIterator SplayDict<K, V>::end(){
  ConstIterator it = ConstIterator(nullptr, root);
  return it;
}


template<typename K, typename V> 
typename SplayDict<K, V>::Node* SplayDict<K, V>::findNode(Node* subtree, const K& key) const{
  if (subtree == nullptr){
    return nullptr;
  }
  else if (subtree->key == key) {
    return subtree;
  }
  else if (key < subtree->key) {
    return findNode(subtree->left, key);
  }
  else {
    return findNode(subtree->right, key);
  }  
}


template<typename K, typename V> 
void SplayDict<K, V>::set(Node*& subtree, Node* parent, const K& key, const V& val){
  if (subtree == nullptr) {
    // by using the reference to pointer, we update parent's pointer 
    subtree = new Node(parent, key, val);
    splay(subtree);
  }
  else if (subtree->key == key) {
    subtree->val = val;
    splay(subtree);
  }
  else if (key < subtree->key) {
    set(subtree->left, subtree, key, val);
  }
  else {
    set(subtree->right, subtree, key, val);
  }
}

template<typename K, typename V> 
typename SplayDict<K,V>::Node* SplayDict<K, V>::subtreeMaxKey(Node* subtree, Node* parent){
  if (subtree == nullptr){
    return parent;
  }
  else {
    return subtreeMaxKey(subtree->right, subtree);
  }
}


template<typename K, typename V> 
typename SplayDict<K,V>::Node* SplayDict<K, V>::subtreeMin(Node* subtree){
  return subtreeMinKey(subtree->left, subtree);
}

template<typename K, typename V> 
typename SplayDict<K,V>::Node* SplayDict<K, V>::subtreeMinKey(Node* subtree, Node* parent){
  if (subtree == nullptr){
    return parent;
  }
  else {
    return subtreeMinKey(subtree->left, subtree);
  }
}

template<typename K, typename V> 
void SplayDict<K, V>::erase_node(Node* node){

  if (node->left == nullptr and node->right == nullptr){
    if (node != root){
      if (node->parent->left == node){
        node->parent->left = nullptr;
      }
      else{
        node->parent->right = nullptr;
      }
    }
    
    delete node;
  }
  else if (node->left != nullptr and node->right != nullptr){ 
    // Going always right in the left subtree is the predecessor
    Node* predecessor = subtreeMaxKey(node->left, node);
    
    std::swap(node->key, predecessor->key);
    std::swap(node->val, predecessor->val);
    
    erase_node(predecessor); 
  }
  else {
    Node* child;
    
    if (node->left != nullptr) {
      child = node->left;
    }
    else {
      child = node->right;
    }
    
    // root doesn't have a parent
    if (root != node ){
      if (node->parent->left == node){
        node->parent->left = child;
        child->parent = node->parent;
      }
      else {
        node->parent->right = child;
        child->parent = node->parent;
      }
    }
    else {
      root = child;
      child->parent = nullptr;
    }
    
    delete node;
  }
  
}

template<typename K, typename V> 
const std::pair<K&, V&> SplayDict<K, V>::ConstIterator::operator*() const {
  std::pair<K&, V&> res {current->key, current->val};
  return res;
}


template<typename K, typename V> 
typename SplayDict<K,V>::ConstIterator& SplayDict<K, V>::ConstIterator::operator=(const ConstIterator& it) {
  current = it.current;
  prev = it.prev;
  
  return *this;
}

template<typename K, typename V> 
bool SplayDict<K, V>::ConstIterator::operator==(const ConstIterator& it) const {
  return (current == it.current and prev == it.prev);
}

template<typename K, typename V> 
bool SplayDict<K, V>::ConstIterator::operator!=(const ConstIterator& it) const {
  return !(*this == it);
}


template<typename K, typename V> 
typename SplayDict<K,V>::ConstIterator& SplayDict<K, V>::ConstIterator::operator++(int) {
  if (prev != nullptr and prev->left == current) {
    if (current->right != nullptr){
      current = subtreeMin(current->right);
      prev = current->parent;
    }
    else {
      std::swap(current, prev); 
    }
  }
  else if (prev != nullptr and prev->right == current) {
    if (current->right != nullptr) {
      current = subtreeMin(current->right);
      prev = current->parent;
    }
    else {
      do {
        prev = current;
        current = current->parent;
      } while (current != nullptr and current->right == prev);
    }
  }
  else if (current->left == prev){
    if (current->right != nullptr){
      current = subtreeMin(current->right);
      prev = current->parent;
    }
    else {
      do {
        prev = current;
        current = current->parent;
      } while (current != nullptr and current->right == prev);
    }
    
  }
  
  return *this;
}

template<typename K, typename V>
bool SplayDict<K, V>::sideOfChild(Node* subtree, bool zag1, bool zag2) const {
  Node* grand_parent = subtree->parent->parent;
  Node* possible_parent;
  if (zag1 == false) {
    possible_parent = grand_parent->left;
  }
  else{
    possible_parent = grand_parent->right;
  }
  
  if (possible_parent == nullptr){
    return false;
  }
  
  Node* possible_child;
  if (zag2 == false) {
    possible_child = possible_parent->left;
  }
  else {
    possible_child = possible_parent->right;
  }
  
  return possible_child == subtree; 
}

template<typename K, typename V>
bool SplayDict<K, V>::hasGrandParent(Node* subtree) const {
  if (subtree->parent == nullptr) {
    return false;
  }
  return subtree->parent->parent != nullptr;
}

template<typename K, typename V>
void SplayDict<K, V>::rightRotate(Node* n) {
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

template<typename K, typename V>
void SplayDict<K, V>::leftRotate(Node* n) {
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

template<typename K, typename V>
void SplayDict<K, V>::splay(Node* subtree){
  while(hasGrandParent(subtree)) {
    if (sideOfChild(subtree, false, false)) {
      // Zig Zig
      
      rightRotate(subtree->parent->parent);
      rightRotate(subtree->parent);      
    } else if (sideOfChild(subtree, false, true)) {
      // Zig Zag
      
      leftRotate(subtree->parent);
      rightRotate(subtree->parent);
      
    } else if (sideOfChild(subtree, true, false)) {
      // Zag Zig
      
      rightRotate(subtree->parent);
      leftRotate(subtree->parent);      
    } else if (sideOfChild(subtree, true, true)) {
      // Zag Zag
      
      leftRotate(subtree->parent->parent);
      leftRotate(subtree->parent);
    }
  }
  
  if(subtree->parent != nullptr) {
    if (subtree->parent->left == subtree) {
      rightRotate(subtree->parent);
    }
    else {
      leftRotate(subtree->parent);
    }
  }
  
  
  root = subtree;
}

template<typename K, typename V>
void SplayDict<K,V>::printNode(std::ostream& os, Node* node, std::string prefix, bool isTail){
    if (node == nullptr){
      return;
    }
    os << (prefix + (isTail ? "└── " : "├── ")) << node->key << std::endl;
    if (node->right != nullptr){
      printNode(os, node->left, (prefix + (isTail ? "    " : "│   ")), false);
      printNode(os, node->right, (prefix + (isTail ? "    " : "│   ")), true);
    }
    else {
      printNode(os, node->left, (prefix + (isTail ? "    " : "│   ")), true);
    }
} 

}

#endif
