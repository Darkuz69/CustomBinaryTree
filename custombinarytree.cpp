#include <iostream>
#include <exception>

// Custom exception for duplicate values in the binary tree
// This is thrown when an attempt is made to insert a value that already exists
class BinaryTreeDuplicateValueException : public std::exception {
  const char* what() const noexcept override {
    return "Duplicated values are not supported!!";
  }
};

// Custom exception for unknown values in the binary tree
// This can be thrown when attempting to access or remove a non-existent value
class BinaryTreeUnknownValueException : public std::exception {
  const char* what() const noexcept override {
    return "Given value was not found!!";
  }
};

// Node structure for the binary tree
// Each node contains a value and pointers to its left and right children
template <typename Type>
struct Node {
  Type value;              // The data stored in the node
  Node<Type> *left_node;   // Pointer to the left child node
  Node<Type> *right_node;  // Pointer to the right child node
  
  // Constructor: initializes a node with a given value and null child pointers
  Node<Type>(Type data) : value(data), left_node(nullptr), right_node(nullptr) {}
};

// Binary Tree class
// Implements a binary search tree with basic operations like insert, remove, and display
template <typename Type>
class BinaryTree {
private:
  Node<Type> *root;  // Pointer to the root node of the tree

public:
  // Constructor: initializes an empty tree
  BinaryTree() : root(nullptr) {}

private:
  // Helper function to insert a new node into the tree
  // This function maintains the binary search tree property
  void _insertNode(Type data) {
    // If the tree is empty, create the root node
    if(this->root == nullptr) {
      this->root = new Node<Type>(data);
      return;
    }

    Node<Type> *itr_node = this->root;
    Node<Type> *before_node = nullptr;

    // Traverse the tree to find the correct position for the new node
    while(itr_node != nullptr) {
      before_node = itr_node;
      if(data < itr_node->value) {
        itr_node = itr_node->left_node;
      } else if(data > itr_node->value) {
        itr_node = itr_node->right_node;
      } else {
        // Throw exception if the value already exists in the tree
        throw BinaryTreeDuplicateValueException();
      }
    }

    // Insert the new node as a leaf at the correct position
    if(data < before_node->value) {
      before_node->left_node = new Node<Type>(data);
    } else {
      before_node->right_node = new Node<Type>(data);
    }
  }

  // Helper function to display the tree in-order
  // This results in displaying the node values in ascending order
  void _displayInOrder(Node<Type> *curr_node, bool &first) {
    if(curr_node == nullptr) {
      return;
    }

    // Recursively traverse the left subtree
    this->_displayInOrder(curr_node->left_node, first);

    // Print the current node's value
    if(!first) {
      std::cout << ", ";
    }
    std::cout << curr_node->value;
    first = false;

    // Recursively traverse the right subtree
    this->_displayInOrder(curr_node->right_node, first);
  }

  // Helper function to delete the entire tree
  // This function performs a post-order traversal to delete all nodes
  void _deleteTree(Node<Type> *curr_node) {
    if(curr_node == nullptr) {
      return;
    }

    // Recursively delete left and right subtrees
    this->_deleteTree(curr_node->left_node);
    this->_deleteTree(curr_node->right_node);

    // Delete the current node after its children have been deleted
    delete curr_node;
    curr_node = nullptr;
  }

  // Check if a node has no children (i.e., it's a leaf node)
  bool _hasNoChildNode(Node<Type> *curr_node) {
    return ((curr_node->left_node == nullptr) && (curr_node->right_node == nullptr));
  }

  // Get the parent node of a given node
  // This is used in the node removal process
  Node<Type> *_getNodeBefore(Node<Type> *curr_node) {
    Node<Type> *itr_node = this->root;
    Node<Type> *before_node = nullptr;

    // Traverse the tree to find the parent of the given node
    while(itr_node != nullptr && itr_node != curr_node) {
      before_node = itr_node;
      if(curr_node->value < itr_node->value) {
        itr_node = itr_node->left_node;
      } else {
        itr_node = itr_node->right_node;
      }
    }

    return before_node;
  }

  // Find a node with a given value
  // Returns nullptr if the value is not found in the tree
  Node<Type> *_getNode(Type data) {
    if(this->root == nullptr) {
      return nullptr;
    }

    Node<Type> *curr_node = this->root;
    // Traverse the tree to find the node with the given value
    while(curr_node != nullptr) {
      if(data < curr_node->value) {
        curr_node = curr_node->left_node;
      } else if(data > curr_node->value) {
        curr_node = curr_node->right_node;
      } else {
        return curr_node;  // Node found
      }
    }

    return nullptr;  // Node not found
  }

  // Remove a node with no children (leaf node)
  void _removeNodeNoChild(Node<Type> *curr_node) {
    // Special case: if the node to be removed is the root
    if(curr_node == this->root) {
      delete this->root;
      this->root = nullptr;
      return;
    }

    Node<Type> *before_node = this->_getNodeBefore(curr_node);
    // Update the parent's pointer to null
    if(before_node->left_node == curr_node) {
      before_node->left_node = nullptr;
    } else if(before_node->right_node == curr_node) {
      before_node->right_node = nullptr;
    }

    // Delete the node
    delete curr_node;
    curr_node = nullptr;
  }

  // Remove a node with two children
  // This is the most complex case of node removal
  void _removeNodeTwoChild(Node<Type> *curr_node) {
    // Find the in-order successor (leftmost node in the right subtree)
    Node<Type> *successor_node = curr_node->left_node;
    while(successor_node->right_node != nullptr) {
      successor_node = successor_node->right_node;
    }

    Node<Type> *before_node = this->_getNodeBefore(successor_node);

    if(this->_hasNoChildNode(successor_node)) {
      // If the successor has no children, simply replace the current node's value
      if(before_node->left_node == successor_node) {
        before_node->left_node = nullptr;
      } else if(before_node->right_node == successor_node) {
        before_node->right_node = nullptr;
      }

      curr_node->value = successor_node->value;

      delete successor_node;
      successor_node = nullptr;
    } else {
      // If the successor has a left child, replace the current node's value and adjust pointers
      curr_node->value = successor_node->value;
      before_node->right_node = successor_node->left_node;

      delete successor_node;
      successor_node = nullptr;
    }
  }

  // Remove a node with one child
  void _removeNodeOneChild(Node<Type> *curr_node) {
    // Special case: if the node to be removed is the root
    if(curr_node == this->root) {
      Node<Type> *temp_node = this->root;
      // Set the root to the non-null child
      this->root = (this->root->left_node != nullptr) ? this->root->left_node : this->root->right_node;

      delete temp_node;
      temp_node = nullptr;
      return;
    }

    Node<Type> *before_node = this->_getNodeBefore(curr_node);
    Node<Type> *temp_node = curr_node;

    // Set curr_node to its non-null child
    curr_node = (curr_node->left_node != nullptr) ? curr_node->left_node : curr_node->right_node;
    // Update the parent's pointer to skip the removed node
    if(before_node->left_node == temp_node) {
      before_node->left_node = curr_node;
    } else if(before_node->right_node == temp_node) {
      before_node->right_node = curr_node;
    }

    // Delete the node
    delete temp_node;
    temp_node = nullptr;
  }

  // Helper function to remove a node
  // This function handles all cases of node removal
  void _removeNode(Type data) {
    Node<Type> *toBeRemoved_node = this->_getNode(data);

    if(toBeRemoved_node == nullptr) {
      return;  // Node not found, nothing to remove
    }

    // Choose the appropriate removal method based on the node's children
    if(this->_hasNoChildNode(toBeRemoved_node)) {
      this->_removeNodeNoChild(toBeRemoved_node);
    } else if((toBeRemoved_node->left_node != nullptr) && (toBeRemoved_node->right_node != nullptr)) {
      this->_removeNodeTwoChild(toBeRemoved_node);
    } else {
      this->_removeNodeOneChild(toBeRemoved_node);
    }
  }

public:
  // Public method to insert a node into the tree
  void insertNode(Type data) {
    this->_insertNode(data);
  }

  // Public method to display the tree in-order
  void displayInOrder() {
    std::cout << "[";
    bool first = true;
    this->_displayInOrder(this->root, first);
    std::cout << "]";
  }

  // Public method to remove a node from the tree
  void removeNode(Type data) {
    this->_removeNode(data);
  }

  // Destructor to clean up the entire tree
  // This ensures that all dynamically allocated memory is properly freed
  ~BinaryTree() {
    this->_deleteTree(this->root);
  }
};
