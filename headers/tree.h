#ifndef TREE_H
#define TREE_H

#include <cstdint>
#include <memory>
#include <queue>
#include <utility>

namespace HuffmanTree {

/* this is here only to make the priority queue be in the correct order */
struct CompareFreq {
  constexpr bool operator()(std::pair<std::uint8_t, std::uint64_t> &a,
                            std::pair<std::uint8_t, std::uint64_t> &b) {
    return a.second < b.second;
  }
};

using queue_t =
    std::priority_queue<std::pair<std::uint8_t, std::uint64_t>,
                        std::vector<std::pair<std::uint8_t, std::uint64_t>>,
                        HuffmanTree::CompareFreq>;

class Tree;

/**
 * @brief      creates the huffman coding tree
 * 
 * @param      queue the priority queue with <byte, frequency>
 *
 * @return     Tree * the root of the tree which is made
 */
extern std::shared_ptr<Tree> create_tree(queue_t queue);

/**
 * Tree structure for containing frequencies for the huffman coding
 */
class Tree {
public:
  std::uint8_t byte = 0;
  std::uint64_t freq = 0; /* frequency of the byte,
                             NOTE it's uint64 to allow for very large filesizes
                             though it probably wouldn't need to be quite this large,
                             but due to the fact that there's a max of 255 different
                             values for the byte it doesn't take up that much more space
                          */

  std::shared_ptr<Tree> left = nullptr;
  std::shared_ptr<Tree> right = nullptr;

  /**
   * @brief      checkcs if the node is a leaf or not
   *
   * @return     bool
   */
  inline bool isleaf() const { return left == nullptr && right == nullptr; }

  inline bool operator<(const Tree &other) const {
    return this->freq < other.freq;
  }
  inline bool operator>(const Tree &other) const {
    return this->freq > other.freq;
  }
  inline bool operator==(const Tree &other) const {
    return this->freq == other.freq;
  }
};
} // namespace HuffmanTree

#endif
