#ifndef HASHMAP_HASH_MAP_H_
#define HASHMAP_HASH_MAP_H_

#include <vector>  

namespace dicts {

template<typename K, typename V, typename H = std::hash<K> >
class HashMap {
  public:
    HashMap();
    
    HashMap(const HashMap& hm);
    
    ~HashMap();
    
    V& get(const K& key) const;
    
    void set(const K& key, const V& val);
    
    void erase(const K& key);
    
    void reorder();
    
    V& operator[](const K& key);
    
    V& operator[](const K& key) const;
    
    HashMap<K, V, H>& operator=(const HashMap& hm);
    
  private:
    int effective_hash(const K& key, int offset) const;
    
    int find_index(const K& key) const;
    double loadFactor() const;
  
    void set(K* key_ptr, V* val_ptr);

    struct Slot {
        enum class State{USED, UNUSED, DELETED};
        K* key;
        V* val;
        State state;
        
        Slot(): key(nullptr),
                val(nullptr)
        {
          state = State::UNUSED;
        }
        
        // the pointers are deleted manually through the vector associated by it,
        // see the code in reorder 
        ~Slot(){}
        
    };
    
    static const int kInitDataSize = 100;
    
    const double kLoadFactorBound = 0.4;
    
    const H hash_fn_;
    
    int used_slots_;
    
    std::vector<Slot> data_; 
    
};  



template<typename K, typename V, typename H >
HashMap<K,V,H>::HashMap(): 
                hash_fn_(H()) 
{
                used_slots_ = 0;
                data_.resize(kInitDataSize);
}

template<typename K, typename V, typename H >
HashMap<K, V, H>::HashMap(const HashMap& hm) {
  *this = hm;
}

template<typename K, typename V, typename H >
HashMap<K, V, H>& HashMap<K, V, H>::operator= (const HashMap& hm) {
  for(auto& slot_iter: data_) {
    if (slot_iter.state != Slot::State::UNUSED) {
      delete slot_iter.key;
      slot_iter.key = nullptr;
    }
    
    if (slot_iter.state == Slot::State::USED) {
      delete slot_iter.val;
      slot_iter.val = nullptr;
    }
  }
  
  for(auto& slot_iter : hm.data_){
    if (slot_iter.state == Slot::State::USED){
      this->set(*slot_iter.key, *slot_iter.val);
    }
  }
  
  return *this;  
}

template<typename K, typename V, typename H >
int HashMap<K,V,H>::effective_hash(const K& key, int offset) const {
  // effective_hash uses Quadratic probing
  return (hash_fn_(key) % data_.size() + offset + offset * offset) % data_.size();
}

template<typename K, typename V, typename H >
int HashMap<K, V, H>::find_index(const K& key) const {
  
  int offset = 0;
  int idx = effective_hash(key, offset);
  
  while(data_[idx].state != Slot::State::UNUSED and *data_[idx].key != key ) {
    offset++;
    idx = effective_hash(key, offset);
  }
  
  return idx;
}


template<typename K, typename V, typename H >
V& HashMap<K,V,H>::get(const K& key) const {
  int idx = find_index(key);
  return *data_[idx].val;
}

template<typename K, typename V, typename H>
void HashMap<K, V, H>::set(K* key_ptr, V* val_ptr) {
  int idx = find_index(*key_ptr);
  
  if (data_[idx].val != nullptr){
    delete data_[idx].val;
    delete data_[idx].key;
  }
  
  data_[idx].key = key_ptr;
  data_[idx].val = val_ptr;
  
  if (data_[idx].state == Slot::State::UNUSED) {
    used_slots_++;
    data_[idx].state = Slot::State::USED;
    
    if(loadFactor() > kLoadFactorBound) {
      reorder();
    } 
    
  }
  
}


template<typename K, typename V, typename H>
void HashMap<K, V, H>::set(const K& key, const V& val) {
  set(new K(key), new V(val));
}


template<typename K, typename V, typename H>
void HashMap<K, V, H>::erase(const K& key) {
  int idx = find_index(key);
  data_[idx].state = Slot::State::DELETED;
  
  delete data_[idx].val;
}

template<typename K, typename V, typename H>
void HashMap<K, V, H>::reorder(){
  std::vector<Slot> data_copy = data_;
  
  for (auto& slot_old : data_) {
    slot_old.state = Slot::State::UNUSED;
    slot_old.key = nullptr;
    slot_old.val = nullptr;
  }
  
  used_slots_ = 0;
  data_.resize(data_.size() * 2);
  
  for(auto& slot_iter : data_copy) {

    if (slot_iter.state == Slot::State::USED) {
      set(slot_iter.key, slot_iter.val);
    }
    else if (slot_iter.state == Slot::State::DELETED) {
      delete slot_iter.key;
      slot_iter.key = nullptr;
    }
  }
  
}

template<typename K, typename V, typename H>
double HashMap<K, V, H>::loadFactor() const {
  return static_cast<double>(used_slots_) / data_.size();
}

template<typename K, typename V, typename H>
HashMap<K, V, H>::~HashMap(){
  for (auto& slot_iter : data_){
    if (slot_iter.state != Slot::State::UNUSED){
      delete slot_iter.key;
      slot_iter.key = nullptr;
      
      if (slot_iter.state == Slot::State::USED){
        delete slot_iter.val;
        slot_iter.val = nullptr;
      }
    }
  }
}

template<typename K, typename V, typename H>
V& HashMap<K, V, H>::operator[](const K& key) {
  int idx = find_index(key);
  
  if (data_[idx].state != Slot::State::USED){
    // we have to initialize the pointer with something
    // could be a deleted key or just unused
    
    V default_val = V();
    set(key, default_val);
  }
  
  return get(key);
}

// the operator[] can be called with const requiring that the key is inserted
template<typename K, typename V, typename H>
V& HashMap<K, V, H>::operator[](const K& key) const{
  int idx = find_index(key);
  
  return get(key);
}


}

#endif
