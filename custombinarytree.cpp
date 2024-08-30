#include <iostream>
#include <exception>

class BinaryTreeDuplicateValueException : public std::exception {
  const char* what() const noexcept override {
    return "Duplicated values are not supported!!";
  }
};


class BinaryTreeUnknownValueException : public std::exception {
  const char* what() const noexcept override {
    return "Given value was not found!!";
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
  void _insertNode(Type data) {
    if(this->root == nullptr) {
      this->root = new Node<Type>(data);
      return;
    }

    Node<Type> *itr_node = this->root;
    Node<Type> *before_node = nullptr;

    while(itr_node != nullptr) {
      before_node = itr_node;
      if(data < itr_node->value) {
        itr_node = itr_node->left_node;
      } else if(data > itr_node->value) {
        itr_node = itr_node->right_node;
      } else {
        throw BinaryTreeDuplicateValueException();
      }
    }

    if(data < before_node->value) {
      before_node->left_node = new Node<Type>(data);
    } else {
      before_node->right_node = new Node<Type>(data);
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

  void _deleteTree(Node<Type> *curr_node) {
    if(curr_node == nullptr) {
      return;
    }

    this->_deleteTree(curr_node->left_node);
    this->_deleteTree(curr_node->right_node);

    delete curr_node;
    curr_node = nullptr;
  }

  bool _hasNoChildNode(Node<Type> *curr_node) {
    return ((curr_node->left_node == nullptr) && (curr_node->right_node == nullptr));
  }

  Node<Type> *_getNodeBefore(Node<Type> *curr_node) {
    Node<Type> *itr_node = this->root;
    Node<Type> *before_node = nullptr;

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

  Node<Type> *_getNode(Type data) {
    if(this->root == nullptr) {
      return nullptr;
    }

    Node<Type> *curr_node = this->root;
    while(curr_node != nullptr) {
      if(data < curr_node->value) {
        curr_node = curr_node->left_node;
      } else if(data > curr_node->value) {
        curr_node = curr_node->right_node;
      } else {
        return curr_node;
      }
    }

    return nullptr;
  }

  void _removeNodeNoChild(Node<Type> *curr_node) {
    if(curr_node == this->root) {
      delete this->root;
      this->root = nullptr;
      return;
    }

    Node<Type> *before_node = this->_getNodeBefore(curr_node);
    if(before_node->left_node == curr_node) {
      before_node->left_node = nullptr;
    } else if(before_node->right_node == curr_node) {
      before_node->right_node = nullptr;
    }

    delete curr_node;
    curr_node = nullptr;
  }

  void _removeNodeTwoChild(Node<Type> *curr_node) {
    Node<Type> *successor_node = curr_node->left_node;
    while(successor_node->right_node != nullptr) {
      successor_node = successor_node->right_node;
    }

    Node<Type> *before_node = this->_getNodeBefore(successor_node);

    if(this->_hasNoChildNode(successor_node)) {
      if(before_node->left_node == successor_node) {
        before_node->left_node = nullptr;
      } else if(before_node->right_node == successor_node) {
        before_node->right_node = nullptr;
      }

      curr_node->value = successor_node->value;

      delete successor_node;
      successor_node = nullptr;
    } else {
      curr_node->value = successor_node->value;
      before_node->right_node = successor_node->left_node;

      delete successor_node;
      successor_node = nullptr;
    }
  }

  void _removeNodeOneChild(Node<Type> *curr_node) {
    if(curr_node == this->root) {
      Node<Type> *temp_node = this->root;

      this->root = (this->root->left_node != nullptr) ? this->root->left_node : this->root->right_node;

      delete temp_node;
      temp_node = nullptr;
      return;
    }

    Node<Type> *before_node = this->_getNodeBefore(curr_node);
    Node<Type> *temp_node = curr_node;

    curr_node = (curr_node->left_node != nullptr) ? curr_node->left_node : curr_node->right_node;
    if(before_node->left_node == temp_node) {
      before_node->left_node = curr_node;
    } else if(before_node->right_node == temp_node) {
      before_node->right_node = curr_node;
    }

    delete temp_node;
    temp_node = nullptr;
  }

  void _removeNode(Type data) {
    Node<Type> *toBeRemoved_node = this->_getNode(data);

    if(toBeRemoved_node == nullptr) {
      return;
    }

    if(this->_hasNoChildNode(toBeRemoved_node)) {
      this->_removeNodeNoChild(toBeRemoved_node);
    } else if((toBeRemoved_node->left_node != nullptr) && (toBeRemoved_node->right_node != nullptr)) {
      this->_removeNodeTwoChild(toBeRemoved_node);
    } else {
      this->_removeNodeOneChild(toBeRemoved_node);
    }
  }

public:
  void insertNode(Type data) {
    this->_insertNode(data);
  }

  void displayInOrder() {
    std::cout << "[";
    bool first = true;
    this->_displayInOrder(this->root, first);
    std::cout << "]";
  }

  void removeNode(Type data) {
    this->_removeNode(data);
  }

  ~BinaryTree() {
    this->_deleteTree(this->root);
  }
};

int main() {

  BinaryTree<int> myTree;

}
