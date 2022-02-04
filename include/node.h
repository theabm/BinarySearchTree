#ifndef node_h
#define node_h

#include <utility>
#include <memory>
#include <iostream>
/**
 * @brief A templated struct of node which contains a <key,value> pair, a parent, and
 * left and right children. 
 * @tparam KT The key type of the pair contained by the node.
 * @tparam VT The value type of the pair contained by the node.
 */
template<typename KT, typename VT> // template on Key Type of BST and Value Type.
struct _node{
using PT = std::pair<const KT, VT>; //PT - Pair Type.
        /**
         * @brief Pair contained by node. Of type std::pair<const Key Type, Value Type>
         * 
         */
        PT pair;
        /**
         * @brief Raw pointer to the parent of the node.
         * 
         */
        _node* parent;
        /**
         * @brief Unique pointer to left child.
         * 
         */
        std::unique_ptr<_node> left;
        /**
         * @brief Unique pointer to right child.
         * 
         */
        std::unique_ptr<_node> right;
        /**
         * @brief Construct a new node object from a reference to a pair type. Sets parent to nullptr.
         * 
         * @param elem A reference to a pair type.
         */
        explicit _node(const PT& elem) noexcept:pair{elem}, parent{nullptr}{std::cout<<"l-value node ctor"<< std::endl;}
        /**
         * @brief Construct a new node object from an r-value of pair type. Sets parent to nullptr.
         * 
         * @param elem An r-value of pair type.
         */
        explicit _node(PT&& elem) noexcept: pair{std::move(elem)}, parent{nullptr}{std::cout<<"r-value node ctor"<< std::endl;}

        /**
         * @brief Construct a new node object from a unique pointer to node and a raw pointer to
         * a parent. Used for the sole purpose of copying a BST like structure. 
         * 
         * @param x Unique pointer to node. 
         * @param p Raw pointer to node.
         */
        _node(const std::unique_ptr<_node>& x, _node* const p): pair{x->pair},parent{p}{
            if(x->left){
                left.reset(new _node{x->left, this});
            }
            if(x->right){
                right.reset( new _node{x->right, this} );
            }
        }
        /**
         * @brief Destroy the node object.
         * 
         */
        ~_node() noexcept = default;
    };

#endif