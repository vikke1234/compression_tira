#include <filesystem>
#include <fstream>
#include <iterator>
#include <limits>
#include <map>
#include <queue>
#include <string>
#include <utility>
#include <vector>
#include <cstring>

#include "../headers/huffman.h"
#include <stdlib.h>

namespace fs = std::filesystem;

void huffman_compression2(std::string filename) {
  std::ifstream fp(filename, std::ios::binary);
  std::vector<std::uint8_t> data;
  std::uint8_t frequencies[255] = {0};
  std::uint8_t translation_table[255] = {0};
  for (int i = 0; i < 255; i++) {
    translation_table[i] = i;
  }

  data.reserve(fs::file_size(filename));

  /* has to be char or it will not work */
  std::copy(std::istreambuf_iterator<char>(fp),
            std::istreambuf_iterator<char>(), std::back_inserter(data));
  for (std::uint8_t byte : data) {
    frequencies[byte]++;
  }
  /*sort frequencies along with the translation table accordingly*/
}

/* NOTE:
   counting sort?
 */
void Huffman::create_tree(std::string filename) {
  std::ifstream fp(filename, std::ios::binary);
  std::vector<std::uint8_t> uncompressed;
  std::uint8_t frequencies[UINT8_MAX] = {0};
  std::priority_queue<std::pair<std::uint8_t, std::uint64_t>,
                      std::vector<std::pair<std::uint8_t, std::uint64_t>>,
                      HuffmanTree::CompareFreq>
      queue;

  uncompressed.reserve(fs::file_size(filename));
  /* has to be char or it will not work */
  std::copy(std::istreambuf_iterator<char>(fp),
            std::istreambuf_iterator<char>(), std::back_inserter(uncompressed));
  for (std::uint8_t byte : uncompressed) {
    frequencies[byte]++;
  }
  for (std::uint8_t i = 0; i < UINT8_MAX; i++) {
    queue.push(std::pair<uint8_t, uint64_t>(i, frequencies[i]));
  }
  this->tree = HuffmanTree::create_tree(queue);
}

void Huffman::create_codetable(std::shared_ptr<HuffmanTree::Tree> tree,
                               std::uint32_t code, std::uint8_t index) {
  if (tree == nullptr) {
    return;
  }
  if (tree->isleaf()) {
    this->codes[tree->byte] = code;
  }
  create_codetable(tree->left, code, index + 1);
  create_codetable(tree->right, (code | (1 << index)), index + 1);
}

void Huffman::compress_internal(HuffmanTree::Tree *tree, compressed_t& data) {
  /* the frequencies can be discarded since they're
     useless after initially creating the table */
}

compressed_t Huffman::compress() {
  /* I will try to make this better next week,
     this is pretty badly made and uses a bit of unnecessary space */

}



std::shared_ptr<std::vector<std::uint8_t>> Huffman::decompress() {
  return nullptr;
}
