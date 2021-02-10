#include "../headers/huffman.h"
#include "../headers/heap.h"
#include <bitset>
#include <climits>
#include <cstdio>
#include <filesystem>
#include <iostream>
#include <limits>
#include <math.h>
#include <stdlib.h>
#include <string>

/*
  needed to keep track of how long it actually is
  e.g. if the path would be 1 then you'd not know if there's
  4 0's before it
*/
union path_t {
  struct {
    std::uint8_t character;
    std::uint8_t len;
    std::uint16_t path;
  };
  std::uint32_t writeable;
};

namespace fs = std::filesystem;
static void write_to_file(std::uint8_t *data, std::uint8_t uniques,
                          const std::size_t file_size, path_t *paths,
                          const std::string &filename);

/**
 * @brief builds a mask of `n` length, shouldn't be used for >8 bit things
 * @return a mask of length `n`
 */
static inline std::uint8_t build_mask(std::uint8_t len) {
  return (1u << len) - 1u;
}
/**
 * @brief this reverses the bits of a number
 * https://graphics.stanford.edu/~seander/bithacks.html#ReverseByteWith64BitsDiv
 */

inline std::uint8_t reverse_byte(std::uint8_t byte) {
#ifdef __x86_64
  return ((byte * 0x802lu & 0x22110lu) | (byte * 0x8020lu & 0x88440lu)) *
             0x10101lu >>
         16;
#else
  return static_cast<std::uint8_t>(
      ((byte * 0x80200802ull) & 0x0884422110ull) * 0x0101010101ull >> 32);
#endif
}

inline std::uint16_t reverse_bits(std::uint16_t s) {
  std::uint8_t high = s >> CHAR_BIT, low = s & 0xff;
  return ((std::uint16_t)reverse_byte(low) << CHAR_BIT) | reverse_byte(high);
}

/**
 * @brief builds the paths for each byte in the tree
 * @details left in the tree will be a 0, right will be a 1
 * NOTE: the paths will be in reverse order, should probably be changed
 */
static void build_codes(const Node *node, path_t (&paths)[UCHAR_MAX],
                        const std::uint16_t path, const std::uint8_t index) {
  if (node == nullptr) {
    return;
  }
  if (node->type == node_type_t::DATA) {
    paths[node->byte] = {node->byte, index, path};
    return;
  }

  build_codes(node->left, paths, path, index + 1);
  build_codes(node->right, paths, path | (1 << index), index + 1);
}

/**
 * @brief compresses file to filename.huff
 */
extern void huffman_compression(const std::string &filename) {
  if (!fs::exists(filename)) {
    printf("Error: file not found %s\n", filename.c_str());
    return;
  }
  std::uint64_t frequencies[UCHAR_MAX] = {0};
  /*
   * This *SHOULD* be enough since a tree is at most UCHAR_MAX
   * high (more like floor(lg(255)) = 7) though since it's not completely
   * balanced it might be more
   */
  path_t paths[UCHAR_MAX] = {0};
  const std::size_t file_size = fs::file_size(filename);
  std::uint8_t *data = new std::uint8_t[file_size];
  /* this is to know how many nodes will exist when writing to file */
  std::uint8_t uniques = 0;
  Heap heap;

  /* got tired of trying to implement the c++ way...
     this was a lot simpler smh */
  FILE *fp = fopen(filename.c_str(), "rb");
  fread(data, sizeof(std::uint8_t), file_size, fp);
  fclose(fp);

  for (std::size_t i = 0; i < file_size; i++) {
    if (!frequencies[data[i]]) {
      uniques++;
    }
    frequencies[data[i]]++;
  }

  for (std::uint8_t byte = 0; byte < UCHAR_MAX; byte++) {
    if (frequencies[byte] != 0) {
      Node *node = new Node(byte, frequencies[byte]);
      heap.insert(node);
    }
  }

  Node *root = nullptr, *left = nullptr, *right = nullptr;
  std::uint8_t height = 0;
  while (heap.get_size() > 1) {
    left = heap.pop();
    right = heap.pop();
    root = new Node(0, left->freq + right->freq, node_type_t::FILLER);
    root->left = left;
    root->right = right;
    heap.insert(root);
    height++;
  }
  root = heap.pop();
  build_codes(root, paths, 0, 0);
  write_to_file(data, uniques, file_size, paths, filename);
  delete root;
  delete[] data;
}

/**
 * @brief writes the data to file in compressed form
 *
 * @details it will be encoded from the most significant bit first
 * e.g. 0x2 will be encoded as 0x80, if a bit string won't fit it will be
 * encoded into two bytes e.g. if we've already encoded into a byte 0xa8 (1010
 * 1000) and there's 3 bits left and we want to encode 0xb(1011) we'd shift it
 * by 1 so we get 0x5 (0101) and get the mask 0x1 then we do 0xa8 |= 0x5 = 0xad
 * (1010 1101) and then we move on to the next byte. Where we then use the mask
 * to get the left over bits and shift it to the most significant bit and OR it
 * in.
 */
static void write_to_file(std::uint8_t *data, std::uint8_t uniques,
                          const std::size_t file_size, path_t *paths,
                          const std::string &filename) {
  FILE *output = fopen(std::string(filename + ".huff").c_str(), "wb");
  fwrite(&uniques, sizeof(uniques), 1, output);
  for (std::uint8_t i = 0; i < UCHAR_MAX; i++) {
    if (paths[i].len != 0) {
      std::cout << "path: " << std::bitset<8>(paths[i].path)
                << ", character: " << paths[i].character
                << ", len: " << +paths[i].len << "\n";
      fwrite(&paths[i], sizeof(path_t), 1, output);
    }
  }
  std::size_t filesize_spot = ftell(output);
  /*
   * keep track of the total amount of bits existing
   * (there may be some way of doing this better)
   * but if there's any "extra" bits we don't know
   * when to stop otherwise
   */
  std::uint64_t total_bits = 0;
  std::uint8_t bit_iterator = 0;
  /* the entire array won't be written, only total_bits/8*/
  std::uint8_t *compressed_data = new std::uint8_t[file_size];
  /* the reversed path  */
  std::uint8_t to_write = 0;
  std::size_t compressed_iterator = 0;
  const std::uint8_t INIT_BITS = CHAR_BIT;
  /* keeps track of when a byte is filled */
  std::uint8_t bits_left = INIT_BITS;

  for (std::size_t i = 0; i < file_size; i++) {
    path_t path = paths[data[i]];
    total_bits += path.len;
    std::uint8_t len = path.len;
    /* since the bits are in reverse order from when building the path we
     * reverse them */
    std::uint16_t reversed = reverse_bits(path.path) >> (16 - path.len);
    /* warning scary bit stuff... tl;dr it will put the paths into as few bytes
     * as possible */
    if (bits_left - len >= 0) {
      to_write |= reversed << (bits_left - len);
      bits_left -= len;
    } else {
      std::uint8_t diff = std::abs((long)(bits_left - len));
      std::uint16_t mask = build_mask(diff);
      bits_left = INIT_BITS;
      to_write |= reversed >> (len - diff);
      compressed_data[compressed_iterator++] = to_write;
      len -= diff;
      /* this is because it might have to be written into 3 bytes if it doesn't
       * fit */
      if (len > CHAR_BIT) {
        /*
         * what happens here is that the high byte gets shifted so as much
         * of the low byte fits into it, then it's written to the compressed
         * data after which the rest of the low byte that didn't fit gets
         * shifted up into the 3rd byte
         * this is necessary only when the path won't fit into 2 bytes
         */
        std::uint8_t high = reversed >> CHAR_BIT, low = reversed & 0xff;
        std::uint8_t leftover = len - INIT_BITS;
        high <<= INIT_BITS - leftover;
        high |= low >> leftover;
        compressed_data[compressed_iterator++] = high;
        to_write = low << (INIT_BITS - leftover);
        bits_left = INIT_BITS - leftover;
      } else {
        to_write = (reversed & mask) << (INIT_BITS - len);
        bits_left = INIT_BITS - len;
      }
    }

    if (bits_left == 0 || i == file_size - 1) {
      bits_left = INIT_BITS;
      compressed_data[compressed_iterator++] = to_write;
      to_write = 0;
    }
  }
  std::cout << "total bits: " << total_bits << "\n";
  fwrite(&total_bits, sizeof(total_bits), 1, output);
  fwrite(compressed_data, sizeof(*compressed_data), compressed_iterator,
         output);
  fclose(output);

  for (int i = 0; i < compressed_iterator; i++) {
    std::bitset<8> bits(compressed_data[i]);
    std::cout << bits << " ";
  }
  std::cout << std::endl;
  delete[] compressed_data;
}

void huffman_decompress(std::string &filename) {
  FILE *fp = fopen(filename.c_str(), "rb");
  std::uint8_t tree_size = 0;
  fread(&tree_size, sizeof(tree_size), 1, fp);
  path_t *paths = new path_t[tree_size];
  fread(paths, sizeof(path_t), tree_size, fp);
  std::uint64_t total_bits = 0;
  fread(&total_bits, sizeof(std::uint64_t), 1, fp);
  std::size_t data_start = ftell(fp);
  fseek(fp, 0l, SEEK_END);
  std::size_t data_size = ftell(fp) - data_start;
  std::uint8_t *data = new std::uint8_t[data_size];
  fseek(fp, data_start, SEEK_SET);
  fread(data, sizeof(std::uint8_t), data_size, fp);

  /*
    build the tree, slow as shit to make it though...
    luckily it's not that large...
    TODO: storing it better might allow it to be faster?
   */
  Node *root = new Node(0, 0, node_type_t::FILLER);
  for (int i = 0; i < tree_size; i++) {
    path_t path = paths[i];
    Node *node = root;
    for (int i = 0; i < path.len; i++) {
      if (path.path & 1 << (path.len - 1)) {
        if (node->left == nullptr) {
          node->left = new Node(0, 0, node_type_t::FILLER);
        }
        node = node->left;
      } else {
        if (node->right == nullptr) {
          node->right = new Node(0, 0, node_type_t::FILLER);
        }
        node = node->right;
      }
    }
    node->byte = path.character;
    node->type = node_type_t::DATA;
  }

  std::uint8_t bit_iterator = CHAR_BIT;
  while (total_bits > 0) {
  }
}
