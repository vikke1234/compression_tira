#ifndef HEAP_H
#define HEAP_H
#include <climits>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <algorithm>

enum node_type_t { DATA, FILLER };
constexpr const std::size_t NODES_SIZE = UCHAR_MAX * 2 + 1;

/**
 * @brief a node in the priority tree
 */
struct Node {
  std::uint8_t byte = 0;
  std::uint64_t freq = 0;
  node_type_t type = DATA;
  Node *left = nullptr, *right = nullptr;
  Node(std::uint8_t byte, std::uint64_t freq) {
    this->byte = byte;
    this->freq = freq;
  }

  Node(std::uint8_t byte, std::uint64_t freq, node_type_t type) {
    this->byte = byte; /* doesn't really matter what this is since
                       it will be checked from the type when building
                       the code table. */
    this->freq = freq;
    this->type = type;
  }
  ~Node() {
    if (this->left != nullptr) {
      delete this->left;
    }
    if (this->right != nullptr) {
      delete this->right;
    }
  }
  static int height(Node const *node) {
    if (node == nullptr)
      return 0;

    return std::max(height(node->left), height(node->right)) + 1;
  }
  int height() {
    return height(this);
  }
  void __attribute__((noinline)) print_tree(int indent = 0) {
    if (right != nullptr) {
      right->print_tree(indent + 4);
    }
    if (indent) {
      std::cout << std::setw(indent) << " ";
    }
    std::cout << std::hex << +byte << "\n ";
    if (left != nullptr) {
      left->print_tree(indent + 4);
    }
  }
  friend bool operator<(const Node &left, const Node &right) {
    return left.freq < right.freq;
  }
};

/**
 * @brief the priority queue for the huffman algorithm
 */
class Heap {
  int size = 0;
  Node *nodes[NODES_SIZE] = {0};
  std::uint32_t paths[UCHAR_MAX+1] = {0};

public:
  /*
   * @brief removes the top item from the queue and return it
   * @return Node *
   */
  Node *pop(void);

  Node *top(void);
  /*
   * @brief only fetches the top value from the queue
   * @return Node *
   */
  Node *peek(void) const { return this->nodes[0]; }
  /*
   * @brief inserts a node into the queue
   * @return void
   */
  void insert(Node *node);

  Node *operator[](std::uint8_t index) { return this->nodes[index]; }
  int get_size(void) { return this->size; }
  ~Heap() {
    for (std::uint8_t i = 0; i < this->size; i++) {
      if (this->nodes[i] != nullptr) {
        delete this->nodes[i];
      }
    }
  }

private:
  /**
   * @brief orders the nodes correctly
   * @return void
   */
  void heapify(const std::uint16_t index);
  /**
   * @brief      swaps two nodes also swaps the path for the two
   *
   * @param      smallest index
   * @param      index to swap
   *
   * @return     void
   */
  void swap(std::uint8_t smallest, std::uint8_t index) {
    Node *swap_node = this->nodes[smallest];
    this->nodes[smallest] = this->nodes[index];
    this->nodes[index] = swap_node;
  }

  /**
   * @brief checks if a given index is a leaf
   * @return bool
   */
  bool isLeaf(std::uint8_t index) {
    return this->nodes[index * 2] == nullptr && this->nodes[index * 2 + 1];
  }
};

#endif /* HEAP_H */
