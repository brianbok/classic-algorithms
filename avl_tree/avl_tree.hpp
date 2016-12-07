#ifndef AVL_TREE_H_
#define AVL_TREE_H_

#include <utility>  
#include <iostream>  
#include <stack>
#include <cassert>  

namespace dicts {

template<typename K, typename V> 
class AVLDict {
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
		
		
		AVLDict();
		
		AVLDict(const AVLDict& bst);
		
		~AVLDict();
		
		bool exists(const K& key) const;
		
		V& get(const K& key);
		
		void set(const K& key, const V& val);
		
		void erase(const K& key);

		AVLDict& operator= (const AVLDict& bst);
		
		ConstIterator begin();
		
		ConstIterator end();
		

	private: 
		struct Node {
			int height;
			
			K key;
			V val;
			
			Node* parent;
			Node* left;
			Node* right;
			
			Node(Node* p, const K& k, const V& v) {
				height = 1;

				key = k; 
				val = v;
				
				parent  = p;
				left    = nullptr;
				right   = nullptr;
			}
			
			void update_height() {
				int rh = 0, lh = 0;
		
				if (right != nullptr) {
					rh = right->height;
				} 
				if (left != nullptr) {
					lh = left->height;
				}
				
				height = std::max(lh, rh) + 1;
			}
			
			int balance_factor() {
				int bf = 0;
		
				if (right != nullptr) {
					bf += right->height;
				} 
				if (left != nullptr) {
					bf -= left->height;
				}
				
				return bf;
			}
			
		};
		
		static void printNode(std::ostream& os, Node* node, std::string prefix, bool isTail);
		
		Node* findNode(Node* subtree, const K& key) const; 
		
		void rebalance(Node* curr_node);
		
		void recalculate_height(Node* curr_node);
		
		bool hasGrandParent(Node* subtree) const;
		
		void rightRotate(Node* n);
		
		void leftRotate(Node* n);
		
		bool sideOfChild(Node* subtree, bool zag1, bool zag2) const;
				
		Node* set(Node*& subtree, Node* parent, const K& key, const V& val);
		
		static Node* subtreeMin(Node* subtree);
		
		static Node* subtreeMax(Node* subtree);
		
		static Node* subtreeMaxKey(Node* subtree, Node* parent);
		
		static Node* subtreeMinKey(Node* subtree, Node* parent);
		
		Node* erase_node(Node* node);
		
		Node* root;
		
	friend class ConstIterator;
	
	friend std::ostream& operator<< (std::ostream& os, const AVLDict<K,V>& spDict){
		printNode(os, spDict.root, "", true);
		return os;
	}

};


template<typename K, typename V> 
AVLDict<K, V>::AVLDict() {
		root = nullptr;
}

template<typename K, typename V> 
AVLDict<K, V>::~AVLDict() {
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
AVLDict<K, V>::AVLDict(const AVLDict& bst) {
	for (ConstIterator it = bst.begin(); it != bst.end(); it++) {
		set( (*it).first, (*it).second);
	}
}

template<typename K, typename V> 
bool AVLDict<K, V>::exists(const K& key) const {
	return findNode(root, key) != nullptr;
}

template<typename K, typename V> 
V& AVLDict<K, V>::get(const K& key){
	Node* node = findNode(root, key);
	return node->val;
}

template<typename K, typename V> 
void AVLDict<K, V>::rebalance(Node* curr_node) {
	int bf = curr_node->balance_factor();
	if (abs(bf) >= 3 ) {
		// something went wrong
		assert(false);
	} else if (bf == 2) {
		Node* q = curr_node->right; // it can't be null because bf is 2
		if (q->balance_factor() == 1 or q->balance_factor() == 0){
			leftRotate(curr_node);
			if (curr_node == root) {
				root = q;
			}
		} else {
			Node* r = q->left;
			rightRotate(q);
			leftRotate(curr_node);
			if (curr_node == root) {
				root = r;
			}
		}
	} else if (bf == -2) {
		Node* q = curr_node->left; // it can't be null because bf is -2
		if (q->balance_factor() == -1 or q->balance_factor() == 0){
			rightRotate(curr_node);
			if (curr_node == root) {
				root = q;
			}
		} else {
			Node* r = q->right;
			leftRotate(q);
			rightRotate(curr_node);
			if (curr_node == root) {
				root = r;
			}
		}
	}
	
}


template<typename K, typename V> 
void AVLDict<K, V>::set(const K& key, const V& val) {
	Node* current_node = findNode(root, key);
	if (current_node != nullptr) {
		current_node->val = val;
	} else {		
		current_node = set(root, nullptr, key, val);
		while(current_node != nullptr) {
			current_node->update_height();
			if (abs(current_node->balance_factor()) >= 2) {
				rebalance(current_node);
			}
			
			current_node = current_node->parent;
		}
	}
}

template<typename K, typename V> 
void AVLDict<K, V>::erase(const K& key){
	Node* node = findNode(root, key);
	node = erase_node(node);
	
	while(node != nullptr) {
		node->update_height();
		if (abs(node->balance_factor()) >= 2) {
			rebalance(node);
		}
		
		node = node->parent;
	}
}

template<typename K, typename V> 
typename AVLDict<K,V>::ConstIterator AVLDict<K, V>::begin(){
	ConstIterator it = ConstIterator(root);
	return it;
}

template<typename K, typename V> 
typename AVLDict<K,V>::ConstIterator AVLDict<K, V>::end(){
	ConstIterator it = ConstIterator(nullptr, root);
	return it;
}


template<typename K, typename V> 
typename AVLDict<K, V>::Node* AVLDict<K, V>::findNode(Node* subtree, const K& key) const{
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
typename AVLDict<K, V>::Node* AVLDict<K, V>::set(Node*& subtree, Node* parent, const K& key, const V& val){
	if (subtree == nullptr) {
		// by using the reference to pointer, we update parent's pointer 
		subtree = new Node(parent, key, val);
		return subtree;
	}
	else if (subtree->key == key) {
		assert(false);
		// Should not happen
	}
	else if (key < subtree->key) {
		return set(subtree->left, subtree, key, val);
	}
	else {
		return set(subtree->right, subtree, key, val);
	}
}

template<typename K, typename V> 
typename AVLDict<K,V>::Node* AVLDict<K, V>::subtreeMaxKey(Node* subtree, Node* parent){
	if (subtree == nullptr){
		return parent;
	}
	else {
		return subtreeMaxKey(subtree->right, subtree);
	}
}


template<typename K, typename V> 
typename AVLDict<K,V>::Node* AVLDict<K, V>::subtreeMin(Node* subtree){
	return subtreeMinKey(subtree->left, subtree);
}

template<typename K, typename V> 
typename AVLDict<K,V>::Node* AVLDict<K, V>::subtreeMax(Node* subtree){
	return subtreeMaxKey(subtree->right, subtree);
}

template<typename K, typename V> 
typename AVLDict<K,V>::Node* AVLDict<K, V>::subtreeMinKey(Node* subtree, Node* parent){
	if (subtree == nullptr){
		return parent;
	}
	else {
		return subtreeMinKey(subtree->left, subtree);
	}
}

template<typename K, typename V> 
typename AVLDict<K,V>::Node* AVLDict<K,V>::erase_node(Node* node){

  if (node->left == nullptr and node->right == nullptr){
    if (node != root){
      if (node->parent->left == node){
        node->parent->left = nullptr;
      }
      else{
        node->parent->right = nullptr;
      }
    }
    
    Node* res = node->parent;
    res->update_height();
    
    delete node;
    
    return res;
  }
  else if (node->left != nullptr and node->right != nullptr){ 
    // Going always right in the left subtree is the predecessor
    Node* predecessor = subtreeMaxKey(node->left, node);
    
    std::swap(node->key, predecessor->key);
    std::swap(node->val, predecessor->val);
    
    return erase_node(predecessor); 
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
    
    child->update_height();
    if (child->parent != nullptr) {
		child->parent->update_height();
	}
    
    delete node;
    return child;
    
  }
  
}

template<typename K, typename V> 
const std::pair<K&, V&> AVLDict<K, V>::ConstIterator::operator*() const {
	std::pair<K&, V&> res {current->key, current->val};
	return res;
}


template<typename K, typename V> 
typename AVLDict<K,V>::ConstIterator& AVLDict<K, V>::ConstIterator::operator=(const ConstIterator& it) {
	current = it.current;
	prev = it.prev;
	
	return *this;
}

template<typename K, typename V> 
bool AVLDict<K, V>::ConstIterator::operator==(const ConstIterator& it) const {
	return (current == it.current and prev == it.prev);
}

template<typename K, typename V> 
bool AVLDict<K, V>::ConstIterator::operator!=(const ConstIterator& it) const {
	return !(*this == it);
}


template<typename K, typename V> 
typename AVLDict<K,V>::ConstIterator& AVLDict<K, V>::ConstIterator::operator++(int) {
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
void AVLDict<K, V>::rightRotate(Node* n) {
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
	} else if (new_root->parent != nullptr) {
		new_root->parent->right = new_root;
	}
	
	new_root->update_height();
	n->update_height();
}

template<typename K, typename V>
void AVLDict<K, V>::leftRotate(Node* n) {
	assert(n != nullptr);
	assert(n->right != nullptr);
	
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
	} else if (new_root->parent != nullptr) {
		new_root->parent->right = new_root;
	}
	
	new_root->update_height();
	n->update_height();
}


template<typename K, typename V>
void AVLDict<K,V>::printNode(std::ostream& os, Node* node, std::string prefix, bool isTail){
		if (node == nullptr){
			return;
		}
		os << (prefix + (isTail ? "└── " : "├── ")) << node->key << std::endl;
		if (node->left != nullptr){
			printNode(os, node->right, (prefix + (isTail ? "    " : "│   ")), false);
			printNode(os, node->left, (prefix + (isTail ? "    " : "│   ")), true);
		}
		else {
			printNode(os, node->right, (prefix + (isTail ? "    " : "│   ")), true);
		}
} 

}

#endif
