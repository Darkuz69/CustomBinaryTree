#include <iostream>
#include <exception>

class BinaryTreeDuplicateValueException : public std::exception {
  const char* what() const noexcept override {
    return "Duplicated values are not supported!!";
  }
};


template <typename Type>
struct Node {
  Type value;
  Node<Type> *left_node;
  Node<Type> *right_node;
  Node<Type>(Type data) : value(data), left_node(nullptr), right_node(nullptr) {}  
};

template <typename Type>
class BinaryTree {
private:
  Node<Type> *root;

public:
  BinaryTree() {
    this->root = nullptr;
  }

private:
  void _insertNode(Node<Type> *&curr_node, Type data) {
    if(curr_node == nullptr) {
      curr_node = new Node<Type>(data);
      return;
    }

    if(data < curr_node->value) {
      this->_insertNode(curr_node->left_node, data);
    } else if (data > curr_node->value) {
      this->_insertNode(curr_node->right_node, data);
    } else {
      throw BinaryTreeDuplicateValueException();
    }

  }

  void _displayInOrder(Node<Type> *curr_node, bool &first) {
    if(curr_node == nullptr) {
      return;
    }

    this->_displayInOrder(curr_node->left_node, first);

    if(!first) {
      std::cout << ", ";
    }
    std::cout << curr_node->value;
    first = false;

    this->_displayInOrder(curr_node->right_node, first);
  }

  void deleteTree(Node<Type> *curr_node) {
    if(curr_node == nullptr) {
      return;
    }

    this->deleteTree(curr_node->left_node);
    this->deleteTree(curr_node->right_node);

    delete curr_node;
    curr_node = nullptr;
  }

public:
  void insertNode(Type data) {
    this->_insertNode(this->root, data);
  }

  void displayInOrder() {
    std::cout << "[";
    bool first = true;
    this->_displayInOrder(this->root, first);
    std::cout << "]";
  }

  ~BinaryTree() {
    this->deleteTree(this->root);
  }
};

int main() {

  BinaryTree<int> myTree;
  myTree.insertNode(4);
  myTree.insertNode(2);
  myTree.insertNode(5);
  myTree.insertNode(1);
  myTree.insertNode(3);

  myTree.displayInOrder();
}
