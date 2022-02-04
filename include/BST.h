#ifndef _BST_h
#define _BST_h

#include <iostream>
#include <utility>
#include <memory>
#include <iterator>
#include <algorithm>
#include <vector>
#include <cassert>

#include "iterator.h"
#include "node.h"

/**
 * @brief Binary Search Tree class templated on KT (key type), VT (value type), and F (type of comparison operator).
 * 
 * @tparam KT Key type of nodes of BST.
 * @tparam VT Value type of nodes of BST.
 * @tparam F Type of comparison operator to induce ordering in BST. Default: std::less<const KT>.
 */
template<typename KT, typename VT, typename F = std::less<const KT>>
class BST{
    using PairType = std::pair<const KT, VT>; // Pair Type

    using node = _node<KT, VT>; 
    using iterator = _iterator<node, PairType>;
    using const_iterator = _iterator<node, const PairType>;

    using IteratorBoolPair = std::pair<iterator, bool>; // Iterator-Bool Pair type

    F f;
    std::size_t _size;
    std::unique_ptr<node> head;

    /**
     * @brief Helper function to insert a node inside a BST.
     * 
     * @tparam OT
     * @param pair Pair to be inserted.
     * @return IteratorBoolPair Returns a pair of an iterator (pointing to node) and an bool. 
     * The bool is true if a new node has been allocated, false otherwise
     * (i.e., the key was already present in the tree). 
     */
    template <typename OT> IteratorBoolPair _insert(OT&& pair){ 
        // if BST is empty:
        if(!head.get()){ 
            auto _node = new node{std::forward<OT>(pair)}; 
            head.reset(_node);
            ++_size;
            return IteratorBoolPair{iterator{_node}, true}; 
        }

        // if BST not empty:
        auto tmp = head.get();
        while(true){
            if ( f(pair.first, tmp->pair.first) ){
                if(tmp->left.get()){
                    tmp = tmp -> left.get();
                }
                else{
                    auto _node = new node{std::forward<OT>(pair)};
                    _node->parent = tmp;
                    tmp->left.reset(_node);
                    ++_size;
                    return IteratorBoolPair{iterator{_node}, true};
                }
            }
            else if( f(tmp->pair.first, pair.first) ){
                if(tmp->right.get()){
                    tmp = tmp -> right.get();
                }
                else{
                    auto _node = new node{std::forward<OT>(pair)};
                    _node->parent = tmp;
                    tmp->right.reset(_node);
                    ++_size;
                    return IteratorBoolPair{iterator{_node}, true};
                }
            }
            else {
                return IteratorBoolPair{iterator{tmp}, false};
            }
        }
    }
    /**
     * @brief Helper function to implement overload of operator[]. Returns a reference to 
     * the value that is mapped to a key equivalent to x, performing an insertion if 
     * such key does not already exist. In this case, a default value of value type is 
     * when inserting the pair. 
     * 
     * @tparam OT
     * @param key The key we want to find BST. 
     * @return VT& Reference to value type. 
     */
    template <typename OT> VT& _sub(OT&& key){ 
        auto iteratorboolpair = insert(PairType{std::forward<OT>(key),{}});
        return iteratorboolpair.first->second;
    }
    /**
     * @brief Helper function to implement find() and cfind().
     * Find a given key. If the key is present, returns a pointer to the proper node, nullptr otherwise.
     * 
     * @tparam OT
     * @param key Key we want to find in the tree.
     * @return node* Pointer to node.
     */
    template <typename OT> node* _find(OT&& key) const noexcept {
        if(!head.get()){
            std::cout<<"BST is empty."<<std::endl;
            return nullptr;
        }
        auto tmp = head.get();
        while(true){
            if ( f(std::forward<OT>(key), tmp->pair.first)){
                if(tmp->left.get())
                    tmp = tmp -> left.get();
                else{
                    return nullptr;
                }
            }
            else if ( f(tmp->pair.first, std::forward<OT>(key)) ) {
                if(tmp->right.get())
                    tmp = tmp -> right.get();
                else{
                    return nullptr;
                }
            }
            else{
                return tmp;
            }
        }
    }
    /**
     * @brief Helper function to delete a leaf node. Used only for the purpose of erasing
     * a node.
     * 
     * @param leaf Pointer to leaf node.
     */
    void delete_leaf(node *const leaf) noexcept {
        auto parent = leaf->parent;
        leaf->parent = nullptr;
        if(parent->left.get() == leaf)
            parent->left.reset(nullptr);   
        else
            parent->right.reset(nullptr);
        --_size;
        return;
    }
    /**
     * @brief Helper function for swapping a node (with two children) and its successor inside a BST. Used solely for the purpose of erasing
     * a node. We take advantage of the fact that the node has to children to avoid checking some conditions.
     * 
     * @param node1 Pointer to node1.
     * @param node2 Pointer to node2.
     */
    void swap_with_successor_of_node_with_two_children(node* const node1, node* const node2) noexcept{
        // The following conditions are always satisfied:
        // node1 has both left and right child.
        // node2 must have a parent. 
        // node 2 doesn't have a left child (i.e it is nullptr).

        // checking that the node1 is predecessor of node2.
        assert( f(node1->pair.first, node2->pair.first) );
        assert( node1->left.get() && node1->right.get());
        assert( node2->parent );

        auto parent_of_node1 = node1->parent;
        auto parent_of_node2 = node2->parent;
        
        auto left_node_of_node1 = node1->left.release();
        auto right_node_of_node1 = node1->right.release();

        // We know that successor of node with two children won't have a left child by construction.
        assert( node2->left.get() == nullptr);
        auto right_node_of_node2 = node2->right.release(); 
        
        node1->left.reset(nullptr);
        node1->right.reset(right_node_of_node2);
        if(right_node_of_node2)
            right_node_of_node2->parent = node1;

        // In the case where the successor is the adjacent (right) node, then
        // the parent of the predecessor becomes the successor and the right node of the successor 
        // becomes the predecessor.
        if(right_node_of_node1 == node2){
            node1->parent = node2;
            node2->right.reset(node1);
        }
        else{
            node1->parent = parent_of_node2;
            if(parent_of_node2->left.get()==node2){
            parent_of_node2->left.release();
            parent_of_node2->left.reset(node1);
            }
            else{
                parent_of_node2->right.release();
                parent_of_node2->right.reset(node1);
            }

            node2->right.reset(right_node_of_node1);
            right_node_of_node1->parent = node2;
        }

        node2->left.reset(left_node_of_node1);
        left_node_of_node1->parent = node2;

        node2->parent = parent_of_node1;
        // if parent of node 1 is nullptr then node1 is the root node. Therefore we must 
        // reset the head from node1 to node2.
        if(parent_of_node1){
            if(parent_of_node1->left.get()==node1){
                parent_of_node1->left.release();
                parent_of_node1->left.reset(node2);
            }
            else{
                parent_of_node1->right.release();
                parent_of_node1->right.reset(node2);
            }
        }
        else{
            head.release();
            head.reset(node2);
        }

    }
    /**
     * @brief Helper function to delete a node with one child. Used solely for the purpose of erasing
     * a node.
     * 
     * @param node1 Pointer to node.
     */
    void delete_node_with_one_child(node* const node1) noexcept{
        auto parent_of_node1 = node1->parent;
        node1->parent = nullptr;
        node* child_of_node1;
        if(node1->left.get()){
            child_of_node1 = node1->left.release();
            child_of_node1->parent = parent_of_node1;
        }
        else{
            child_of_node1 = node1->right.release();
            child_of_node1->parent = parent_of_node1;
        }
        if(parent_of_node1->left.get() == node1)
            parent_of_node1->left.reset(child_of_node1);   
        else
            parent_of_node1->right.reset(child_of_node1);
        --_size;
        return;
        
    }
    /**
     * @brief Helper function for erasing a node from the BST.
     * 
     * @tparam O
     * @param key Key to be erased.
     */
    template <typename O> void _erase(O&&key) noexcept {
        auto _node = _find(std::forward<O>(key));
        if (!_node){ 
            std::cout<<"Erase failed. This key does not exist."<<std::endl;
            return;
        }
        if(!_node->left && !_node->right){
            delete_leaf(_node);
            return;
        }
        else if(_node->left && _node->right){
            auto _node_pred = _node;
            _node = next(_node); //successor can have either no children (leaf) or only right child.

            if(!_node->right.get()){
                //successor has no children and is leaf node
                swap_with_successor_of_node_with_two_children(_node_pred, _node);
                delete_leaf(_node_pred);
            }
            else{
                // successor has a right child
                swap_with_successor_of_node_with_two_children(_node_pred,_node);
                delete_node_with_one_child(_node_pred);
            }
        }
        else{
            if(_node->right){
                delete_node_with_one_child(_node);
            }
            else{
                delete_node_with_one_child(_node);
            }
            return;
        }       
    }
        
    /**
     * @brief Helper function which takes a sorted vector of pairs, and recursively inserts the median located
     * between @param start and @param end of the vector into the BST. 
     * 
     * @param vector_of_pairs sorted vector of pairs to be inserted into the BST
     * @param start start of vector from where median should be calculated.
     * @param end end of vector from where median should be calculated. 
     */
    void _insert_medians(const std::vector<PairType>& vector_of_pairs, const int start, const int end){
        if(start > end){
            return;
        }
        auto mid = (end+start)/2;

        insert(vector_of_pairs[mid]);

        _insert_medians(vector_of_pairs, start, mid-1);
        _insert_medians(vector_of_pairs, mid+1, end);
    }

    public:

    /**
     * @brief Emplace values inside BST. 
     * 
     * @tparam Types 
     * @param args arguments to be packed.
     * @return IteratorBoolPair a pair of an iterator (pointing to node) and an bool. 
     * The bool is true if a new node has been allocated, false otherwise
     * (i.e., the key was already present in the tree). 
     */
    template <typename ... Types> // packing the types
    IteratorBoolPair emplace(Types&& ... args) noexcept { //packing the arguments
        return insert(PairType{std::forward<Types>(args)...}); //unpack
    }

    /**
     * @brief Clears the tree.
     * 
     */
    void clear() noexcept{
        _size = 0;
        head.reset();
    }

    /**
     * @brief Finds a given key. If the key is present, returns an iterator to the proper node,
     *  end() otherwise.
     * 
     * @param key
     * @return iterator
     */
    auto find(const KT& key) noexcept {return iterator{_find(key)}; }
    //iterator find(KT&& x) noexcept{ return iterator{_find(std::move(x))}; }

    // dont need to implement both versions for r value and l value since an r value is coherent with a const
    // reference (a const reference cannot be in the left side of an assignment)
    /**
     * @brief Const version of find().
     * 
     * @param key
     * @return const_iterator 
     */
    auto find(const KT& key) const noexcept{ return const_iterator{_find(key)}; } 
    //const_iterator find(KT&& x) const noexcept{return const_iterator{_find(std::move(x))}; }

    /**
     * @brief Subscripting operator. Returns a reference to the value type of the node if the
     * key exists in the BST and inserts the key if it doesn't.
     * 
     * @param key L-value reference to key to be subscripted
     * @return VT& reference to value type.
     */
    VT& operator[](const KT& key) { return _sub(key); }
    /**
     * @brief Subscripting operator. Returns a reference to the value type of the node if the
     * key exists in the tree and inserts the key if it doesn't.
     * 
     * @param key R-value reference to key to be subscripted.
     * @return VT& reference to value type.
     */
    VT& operator[](KT&& key) { return _sub(std::move(key)); }
    
    /**
     * @brief Returns iterator to the beginning of the BST. 
     * 
     * @return iterator
     */
    auto begin() noexcept{ 
        auto tmp = head.get();
        while (tmp->left)
            tmp = tmp->left.get();
        return iterator{tmp};
    }
    /**
     * @brief Returns constant iterator to the beginning of the BST.  
     * 
     * @return const_iterator
     */
    auto begin() const noexcept { 
        auto tmp = head.get();
        while(tmp-> left)
            tmp = tmp->left.get();
        return const_iterator{tmp};
    }
    /**
     * @brief Returns constant iterator to the beginning of the BST.  
     * 
     * @return const_iterator
     */
    auto cbegin() const noexcept{
        auto tmp = head.get();
        while(tmp-> left)
            tmp = tmp->left.get();
        return const_iterator{tmp};
    }
    /**
     * @brief Returns iterator to end of BST. 
     * 
     * @return iterator
     */
    auto end() noexcept { 
        return iterator{nullptr};
    }
    /**
     * @brief Returns constant iterator to end of BST. 
     * 
     * @return const_iterator
     */
    auto end() const noexcept{
        return const_iterator{nullptr};
    }
    /**
     * @brief Returns constant iterator to end of BST. 
     * 
     * @return const_iterator
     */
    auto cend() const noexcept{
        return const_iterator{nullptr};
    }

    /**
     * @brief Construct a new BST object. Default constructor.
     * 
     */
    BST() = default;
    /**
     * @brief Construct a new BST object.
     * 
     * @param f Comparison operator.
     */
    BST(F f) noexcept: f{std::move(f)}{}; 
    /**
     * @brief Destroy the BST object.
     * 
     */
    ~BST() noexcept= default; 

    // copy semantics 

    /**
     * @brief Copy constructor of BST.
     * 
     * @param bst2 Reference to BST object.
     */
    BST(const BST &bst2): _size{bst2._size} {
        if(bst2.head.get()){
            head.reset(new node{bst2.head, nullptr});
        }      
    }
    /**
     * @brief Copy assignment of BST.
     * 
     * @param bst2 Reference to BST object.
     * @return BST& Rerference to BST object.
     */
    BST& operator=(BST& bst2 ) {
        head.reset(); // reset the head
        auto tmp = bst2; //copy ctor 
        *this = std::move(tmp); //move assignment
        return *this;
    }


    // Move semantics. No raw ptrs so default move is fine.
    /**
     * @brief Default move constructor of BST.
     * 
     */
    BST(BST &&) = default;

    /**
     * @brief Default move assignment of BST.
     * 
     * @return Reference to BST object.
     */
    BST& operator=(BST &&) = default;

    /**
     * @brief Insert a <key,value> pair in the BST.
     * 
     * @param pair L-value reference to pair of type std::less<Key Type, Value Type> to be inserted.
     * @return IteratorBoolPair Returns a pair of an iterator (pointing to node) and an bool. 
     * The bool is true if a new node has been allocated, false otherwise
     * (i.e., the key was already present in the tree). 
     * type std::pair<iterator,bool>
     */
    IteratorBoolPair insert(const PairType& pair) { return _insert(pair); }
    /**
     * @brief Insert a l-value of <key,value> pair in the BST.
     * 
     * @param pair R-value reference to pair of type std::less<Key Type, Value Type> to be inserted.
     * @return IteratorBoolPair Returns a pair of an iterator (pointing to node) and an bool. 
     * The bool is true if a new node has been allocated, false otherwise
     * (i.e., the key was already present in the tree). 
     * type std::pair<iterator,bool>
     */
    IteratorBoolPair insert(PairType&& pair) { return _insert(std::move(pair));}
    /**
     * @brief Erase a key from the BST.
     * 
     * @param key L-value reference to key to be erased.
     */
    void erase(const KT&key) noexcept { return _erase(key); }
    // /**
    // * @brief Erase a key from the BST.
    // * 
    // * @param key R-value reference to key to be erased.
    // */
    // void erase(KT&& key)noexcept{ return _erase(std::move(key)); }
    // not needed since r value is coherent with const l value reference.
    
    /**
     * @brief Balance the tree by copying the contents of the tree into a std::vector<> and then recursively
     * inserting the medians of the vector into the BST.
     * 
     */
    void balance() {
        std::vector<PairType> v;
        v.reserve(_size);
        for(const auto& x: *this){
            v.emplace_back(x);
        }
        /* v.reserve(_size);
        std::uninitiliazed_copy(begin(), end(), v.begin()); */
        clear();
        _insert_medians(v, 0, v.size()-1);
        return;
    }
    /**
     * @brief Overload of operator put-to.
     * 
     * @param os Reference to std::ostream.
     * @param bst Const reference to BST 
     * @return std::ostream& 
     */
    friend std::ostream &operator<<(std::ostream &os, const BST &bst){
    if(!bst.head){
        os << "BST is empty => size: [" <<bst._size << "] ";
        os << std::endl;
        return os;
    }
    
    os << "size: [" << bst._size << "] ";

    for (const auto& el : bst)
        os << el.first << " ";
    os << std::endl;
    return os;
  }

};

#endif