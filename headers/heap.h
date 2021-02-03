#ifndef HEAP_H
#define HEAP_H
#include <climits>
#include <cstdint>

/**
 * @brief a node in the priority tree
 */
struct Node {
  std::uint8_t byte = 0;
  std::uint64_t freq = 0;
  Node *left = nullptr, *right = nullptr;
  Node(std::uint8_t byte, std::uint64_t freq) {
    this->byte = byte;
    this->freq = freq;
  }

  bool isLeaf(void) { return left == nullptr && right == nullptr; }
  bool operator<(Node *node) { return this->freq < node->freq; }
};

/**
 * @brief the priority queue for the huffman algorithm
 */
class Heap {
  std::uint8_t index = 0; /* doesn't need to be larger than 8 bits since
                             the array is at most UCHAR_MAX */
  Node *nodes[UCHAR_MAX] = {0};
  std::uint32_t paths[UCHAR_MAX] = {0};

public:
  /*
   * @brief removes the top item from the queue and return it
   * @return Node *
   */
  Node *pop(void);
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

private:
  /**
   * @brief orders the nodes correctly
   * @return void
   */
  void heapify(const std::uint8_t index);
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
    this->nodes[index] = this->nodes[smallest];

    std::uint32_t swap_path = this->paths[smallest];
    this->paths[smallest] = this->paths[index];
    this->paths[index] = swap_path;
  }
};

#endif /* HEAP_H */
