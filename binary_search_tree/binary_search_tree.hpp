#ifndef BINARY_SEARCH_TREE_H_
#define BINARY_SEARCH_TREE_H_

#include <utility>  
#include <iostream>  
#include <stack>  

namespace dicts {

template<typename K, typename V> 
class BST {
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
    
    
    BST();
    
    BST(const BST& bst);
    
    ~BST();
    
    bool exists(const K& key) const;
    
    V& get(const K& key) const;
    
    void set(const K& key, const V& val);
    
    void erase(const K& key);

    BST& operator= (const BST& bst);
    
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
    
    Node* findNode(Node* subtree, const K& key) const; 
    
    void set(Node*& subtree, Node* parent, const K& key, const V& val);
    
    static Node* subtreeMin(Node* subtree);
    
    static Node* subtreeMaxKey(Node* subtree, Node* parent);
    
    static Node* subtreeMinKey(Node* subtree, Node* parent);
    
    void erase_node(Node* node);
    
    Node* root;
    
  friend class ConstIterator;
    
};

template<typename K, typename V> 
BST<K, V>::BST() {
    root = nullptr;
}

template<typename K, typename V> 
BST<K, V>::~BST() {
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
BST<K, V>::BST(const BST& bst) {
  for (ConstIterator it = bst.begin(); it != bst.end(); it++) {
    set( (*it).first, (*it).second);
  }
}

template<typename K, typename V> 
bool BST<K, V>::exists(const K& key) const {
  return findNode(root, key) != nullptr;
}

template<typename K, typename V> 
V& BST<K, V>::get(const K& key) const {
  return findNode(root, key)->val;
}

template<typename K, typename V> 
void BST<K, V>::set(const K& key, const V& val) {
  set(root, nullptr, key, val);
}

template<typename K, typename V> 
void BST<K, V>::erase(const K& key){
  Node* node = findNode(root, key);
  
  erase_node(node);
}

template<typename K, typename V> 
typename BST<K,V>::ConstIterator BST<K, V>::begin(){
  ConstIterator it = ConstIterator(root);
  return it;
}

template<typename K, typename V> 
typename BST<K,V>::ConstIterator BST<K, V>::end(){
  ConstIterator it = ConstIterator(nullptr, root);
  return it;
}


template<typename K, typename V> 
typename BST<K, V>::Node* BST<K, V>::findNode(Node* subtree, const K& key) const{
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
void BST<K, V>::set(Node*& subtree, Node* parent, const K& key, const V& val){
  if (subtree == nullptr) {
    // by using the reference to pointer, we update parent's pointer 
    subtree = new Node(parent, key, val);
  }
  else if (subtree->key == key) {
    subtree->val = val;
  }
  else if (key < subtree->key) {
    set(subtree->left, subtree, key, val);
  }
  else {
    set(subtree->right, subtree, key, val);
  }
}

template<typename K, typename V> 
typename BST<K,V>::Node* BST<K, V>::subtreeMaxKey(Node* subtree, Node* parent){
  if (subtree == nullptr){
    return parent;
  }
  else {
    return subtreeMaxKey(subtree->right, subtree);
  }
}


template<typename K, typename V> 
typename BST<K,V>::Node* BST<K, V>::subtreeMin(Node* subtree){
  return subtreeMinKey(subtree->left, subtree);
}

template<typename K, typename V> 
typename BST<K,V>::Node* BST<K, V>::subtreeMinKey(Node* subtree, Node* parent){
  if (subtree == nullptr){
    return parent;
  }
  else {
    return subtreeMinKey(subtree->left, subtree);
  }
}

template<typename K, typename V> 
void BST<K, V>::erase_node(Node* node){

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
const std::pair<K&, V&> BST<K, V>::ConstIterator::operator*() const {
  std::pair<K&, V&> res {current->key, current->val};
  return res;
}


template<typename K, typename V> 
typename BST<K,V>::ConstIterator& BST<K, V>::ConstIterator::operator=(const ConstIterator& it) {
  current = it.current;
  prev = it.prev;
  
  return *this;
}

template<typename K, typename V> 
bool BST<K, V>::ConstIterator::operator==(const ConstIterator& it) const {
  return (current == it.current and prev == it.prev);
}

template<typename K, typename V> 
bool BST<K, V>::ConstIterator::operator!=(const ConstIterator& it) const {
  return !(*this == it);
}


template<typename K, typename V> 
typename BST<K,V>::ConstIterator& BST<K, V>::ConstIterator::operator++(int) {
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


}

#endif
