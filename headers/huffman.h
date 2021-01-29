#ifndef HUFFMAN_H
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "../headers/tree.h"

/**
 * @brief      huffman compression for a file
 *
 * @details    uses huffman coding to compress a file
 *
 * @param      filename file to compress
 *
 * @return     unsigned char *
 */
void huffman_compression(std::string filename);

using compressed_t = std::shared_ptr<std::vector<std::uint8_t>>;

class Huffman {

  std::shared_ptr<HuffmanTree::Tree> tree;
  std::uint32_t codes[255] = {0};

public:
  Huffman(std::string filename) {
    create_tree(filename);
    create_codetable(this->tree, 0, 0);
  }

  /**
   * @brief compresses the file given to the class
   * @return a shared pointer to a vector containing the compressed data
   */
  compressed_t compress();
  compressed_t decompress();

private:
  /**
   * @brief encodes the tree to binary form
   *
   * @details encodes it so that the left tree will be encoded first,
   * doing it by breadth first traversal
   *
   * @return compressed_t a shared pointer of a vector<uint8_t> which contains
   * the binary tree
   */
  void compress_internal(HuffmanTree::Tree *tree, compressed_t& data);
  compressed_t decompress_internal(HuffmanTree::Tree tree);

  /**
   * @brief creates the binary tree required by the huffman coding algorithm
   */
  void create_tree(std::string filename);

  /*
   * @brief creates the code table from the tree
   * NOTE this will probably be optimized later to be done
   * while the tree is created
   */
  void create_codetable(std::shared_ptr<HuffmanTree::Tree> tree,
                        std::uint32_t code, std::uint8_t index);
};

#endif // HUFFMAN_H
