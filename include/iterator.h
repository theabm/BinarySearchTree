#ifndef iterator_h
#define iterator_h

#include <iterator>
#include <memory>
#include <utility>
/**
 * @brief Iterator class templated on Type T and Value Type O.
 * 
 * @tparam T Type of iterator.
 * @tparam O Value type that the iterator points to. 
 */
template<typename T, typename O>
class _iterator{
  /**
   * @brief Pointer to type T.
   * 
   */
  T* current;

 public:
  using value_type = O;
  using reference = value_type &;
  using pointer = value_type *;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::forward_iterator_tag;

  /**
   * @brief Construct a new iterator object from a pointer to type T.
   * 
   * @param ptr Pointer to type T from which we want to create an iterator. 
   */
  explicit _iterator(T* ptr) noexcept: current{ptr} {}

  /**
   * @brief Destroy the iterator object
   * 
   */
  ~_iterator() noexcept = default;

  /**
   * @brief Overload of operator*().
   * 
   * @return reference
   */
  reference operator*() const noexcept { return current->pair;}
  /**
   * @brief Overload of operator->().
   * 
   * @return pointer 
   */
  pointer operator->() const noexcept { return &**this; }

  // pre-increment
  /**
   * @brief Overload pre increment operator++().
   * 
   * @return _iterator& 
   */
  _iterator &operator++() noexcept {
     current = next(current);
     return *this;
    }

  // post-increment
  /**
   * @brief Overload post increment operator++().
   * 
   * @return _iterator& 
   */
  _iterator operator++(int) noexcept {
     auto tmp = *this;
     ++(*this);
     return tmp;
    }

  /**
   * @brief Overload of operator bool(). Returns TRUE if iterator
   * points to something that isn't nullptr. FALSE otherwise.  
   * 
   * @return true
   * @return false 
   */
  operator bool() const noexcept {return !(current==nullptr); }
  /**
   * @brief Oveerload of operator==(). Returns TRUE if both iterators point to same object.
   * FALSE otherwise.
   * 
   * @param lhs left hand side reference to _iterator.
   * @param rhs right hand side reference to _iterator.
   * @return true 
   * @return false 
   */
  friend bool operator==(const _iterator &lhs, const _iterator &rhs) noexcept {
    return lhs.current == rhs.current;
  }
  /**
   * @brief Overload of operator!=(). Returns TRUE if iterators point to different objects. FALSE otherwise.
   * 
   * @param lhs left hand side reference to _iterator.
   * @param rhs right hand side reference to _iterator.
   * @return true 
   * @return false 
   */
  friend bool operator!=(const _iterator &lhs, const _iterator &rhs) noexcept { 
      return !(lhs == rhs); 
      }
  /**
   * @brief Overload of put-to operator. Prints the value of what the iterator is point to. 
   * 
   * @param os 
   * @param x _iterator&
   * @return std::ostream& 
   */
  friend std::ostream &operator<<(std::ostream &os, const _iterator &x) {
    os << "[" << x.current << "]"<< std::endl;
    return os;
  }

  /**
  * @brief Helper function to find the successor of a node. It is called in the implementation
  * of operator++().
  * 
  * @param _node Pointer to node.
  * @return node* Pointer to the successor node.
  */
  static T* next(T* _node) noexcept{
    // if current is nullptr return again nullptr.
    if(!_node){
      return nullptr;
    }
    // if current has a right child, the successor is found
    // by going right once and then all the way left while possible.
    if(_node->right.get()){
        _node = _node->right.get();
        while(_node->left){
            _node = _node -> left.get();
        }
    }
    // if current has no right child, then the successor can only be found by looking at the 
    // parents of current. 
    // If current has no parent, then we simply return nullptr.
    // If current has a parent, then as long as current is a right child of parent 
    // (i.e parent < current) we must keep looking further up until we eventually
    // find a node for which current is in its left subtree (i.e node > current).
    // Once we exit from the while condition, current->parent is the node we are 
    // looking for. Therefore we need to to set current to the parent and return *this.
    // In the case that the BST is a right deep tree, we simply cycle upward towards
    // toward the root and then return a nullptr. 
    else{ 
        while(_node->parent && _node->parent->right.get() == _node){
            _node = _node->parent;
            }
        _node = _node->parent;
        }
    return _node;
  }

};

#endif