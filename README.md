# Binary Search Tree Repository

This repository consists of the implementation of a **template** binary search tree (BST). 
A BST is a hierarchical (ordered) data structure where each **node** can have at most two children, namely, **left** and **right** child. Each node stores a **pair** of a **key** and the associated **value**. The binary tree is ordered according to the keys. 
If we assume that we sort the keys in ascending order (i.e., we use the less than `<` operator), then given a node `N`, all the nodes having keys **smaller** than the key of the node `N` are on the **left**. All the nodes with a key **greater** than the key of the node `N` are on the **right**.

### How to compile

The repository contains a Makefile, therefore the program can be compiled with the `make` command. However, the specific command line arguments used are as follows:

To compile:
`g++ -I include -g -std=c++17 -DNDEBUG -Wall -Wextra -c -o main.o main.cpp`

To create excutable:
`g++ main.o -o main.x`

To run:
`./main.x`

### Implementation Specifics

From the implementation point of view, the BST is templated on the `KT` the key type, `VT` the value type, and `F` the type of the comparison operator which by default is set to `std::less<Key Type>`.
The BST relies on the node class found in `node.h`. A node has has two `std::unique_ptr`: `left` and `right` pointing to the left and right child, respectively. The pointers point to `nullptr` if they have no children. Furthermore, a node also has a raw pointer pointing to the parent of the node. Keys and values are stored using `std::pair<const KT,VT>`.
Lastly, the iterator for the BST was implemented in `iterator.h`. 

### Supported functions:
##### Insert

```c++
std::pair<iterator, bool> insert(const pair_type& x);
std::pair<iterator, bool> insert(pair_type&& x);
```
They are used to insert a new node. The function returns a pair of an iterator (pointing to the node) and a bool. The bool is true if a new node has been allocated, false otherwise (i.e., the key was already present in the tree). As mentioned above, `pair_type` is `std::pair<const KT, VT>`. 

##### Emplace

```c++
template< class... Types >
std::pair<iterator,bool> emplace(Types&&... args);
```
Inserts a new element into the container constructed in-place with the given args if there is no element with the key in the container.

##### Clear

```c++
void clear();
```
Clear the content of the tree.

##### Begin

```c++
iterator begin();
const_iterator begin() const;
const_iterator cbegin() const;
```

Return an iterator to the left-most node (which, likely, is not the root node).

##### End

```c++
iterator end();
const_iterator end() const;
const_iterator cend() const;
```

Return an iterator to one-past the last element.

##### Find

```c++
iterator find(const key_type& x);
const_iterator find(const key_type& x) const;
```
Find a given key. If the key is present, returns an iterator to the proper node, `end()` otherwise.

##### Balance

```c++
void balance();
```
Balance the tree by copying the ordered pairs of the BST into a vector, clearing the tree, and then recurisvely inserting the medians of the vector.

##### Subscripting operator
```c++
value_type& operator[](const key_type& x);
value_type& operator[](key_type&& x);
```

Returns a reference to the value that is mapped to a key equivalent to `x`, performing an insertion if such key does not already exist.

##### Put-to operator
```c++
friend
std::ostream& operator<<(std::ostream& os, const bst& x);
```

Implement the friend function **inside** the class, such that you do not have to specify the templates for `bst`.

##### Copy and move
The copy semantics perform a deep-copy. Move semantics are as usual.

##### Erase
```c++
void erase(const key_type& x);
```
Removes the element (if one exists) with the key equivalent to key.
