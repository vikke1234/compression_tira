#include "../headers/huffman.h"
#include "../headers/bitstring.h"
#include "../headers/heap.h"
#include <climits>
#include <cstdio>
#include <filesystem>
#include <initializer_list>
#include <iostream>
#include <limits>
#include <stdlib.h>
#include <string>

/*
  needed to keep track of how long it actually is
  e.g. if the path would be 1 then you'd not know if there's
  4 0's before it
*/
struct path_t {
  std::uint8_t character = 0;
  std::uint8_t len = 0;
  bitstring path = {0};

  friend std::ofstream &operator<<(std::ofstream &stream, const path_t &path) {
    std::cout << "writing byte: 0x" << std::hex << path.character
              << ", to: " << stream.tellp() << "\n";
    stream.write((const char *)&path.character, sizeof(character));
    std::cout << "length: " << +path.len << ", to: " << stream.tellp() << "\n";
    stream.write((const char *)&path.len, sizeof(len));
    std::cout << "writing to: " << stream.tellp() << ", path: " << path.path;
    path.path.write_tree_path(stream);
    return stream;
  }
};

void decompress(std::uint8_t *data, std::size_t data_size,
                std::uint64_t total_bits, Node *const root);
namespace fs = std::filesystem;
static void write_to_file(std::uint8_t *data, std::uint16_t tree_size,
                          const std::size_t file_size, path_t *paths,
                          const std::string &filename);

/**
 * @brief builds the paths for each byte in the tree
 * @details left in the tree will be a 0, right will be a 1
 * @param node a pointer to the node to traverse
 * @param paths a reference to an array of paths to store how to traverse the
 * tree for each byte
 * @param path the current path taken
 * @param index basically the length of the entire path, but also used to index
 * into path
 *
 * NOTE: the paths will be in reverse order, should probably be changed
 */
static void build_paths(const Node *const node, path_t (&paths)[UCHAR_MAX + 1],
                        bitstring path, const std::uint8_t index = 0u) {
  if (node == nullptr) {
    return;
  }
  if (node->type == node_type_t::DATA) {
    std::uint8_t tmp = index > 0 ? index : 1;
    paths[node->byte] = {node->byte, tmp, path};
    paths[node->byte].path.len = tmp;
    std::cout << "found byte: " << node->byte
              << ", at: " << paths[node->byte].path;
    return;
  }
  path.unset_bit(index);
  build_paths(node->left, paths, path, index + 1);
  path.set_bit(index);
  build_paths(node->right, paths, path, index + 1);
}

extern void huffman_compression(const std::string &filename) {
  if (!fs::exists(filename)) {
    printf("Error: file not found %s\n", filename.c_str());
    return;
  }

  std::uint64_t frequencies[UCHAR_MAX + 1] = {0llu};
  /*
   * This *SHOULD* be enough since a tree is at most UCHAR_MAX
   * high (more like floor(lg(255)) = 7) though since it's not completely
   * balanced it might be more
   */
  path_t paths[UCHAR_MAX + 1];
  const std::size_t file_size = fs::file_size(filename);
  std::uint8_t *data = new std::uint8_t[file_size];
  /* this is to know how many nodes will exist when writing to file */
  std::uint16_t tree_size = 0;
  Heap heap;

  /* got tired of trying to implement the c++ way...
     this was a lot simpler smh */
  FILE *fp = fopen(filename.c_str(), "rb");
  fread(data, sizeof(std::uint8_t), file_size, fp);
  fclose(fp);

  for (std::size_t i = 0; i < file_size; i++) {
    if (!frequencies[data[i]]) {
      tree_size++;
    }
    frequencies[data[i]]++;
  }

  for (int byte = 0; byte < UCHAR_MAX+1; byte++) {
    if (frequencies[byte] != 0) {
      Node *node = new Node(byte, frequencies[byte]);
      heap.insert(node);
    }
  }

  /* here we will build the tree so we will be able to decode the data later */
  Node *root = nullptr, *left = nullptr, *right = nullptr;
  while (heap.get_size() > 1) {
    left = heap.pop();
    right = heap.pop();
    root = new Node(0, left->freq + right->freq, node_type_t::FILLER);
    root->left = left;
    root->right = right;
    heap.insert(root);
  }

  root = heap.peek();

  bitstring initial_path;
  initial_path.len = 0;
  build_paths(root, paths, initial_path);

  root->print_tree();
  std::cout << "height: " << root->height() << "\n";
  write_to_file(data, tree_size, file_size, paths, filename);

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
static void write_to_file(std::uint8_t *data, std::uint16_t tree_size,
                          const std::size_t file_size, path_t *paths,
                          const std::string &filename) {
  std::ofstream output(filename + ".huff", std::ios::binary | std::ios::out);
  std::cout << "writing to: " << output.tellp() << ", tree size: " << +tree_size
            << "\n";
  output.write((const char *)&tree_size, sizeof(tree_size));
  for (int i = 0; i < UCHAR_MAX+1; i++) {
    if (paths[i].len != 0) {
      std::cout << "\n";
      output << paths[i];
    }
  }
  bitstring compressed_data;
  compressed_data.len = 0;
  std::cout << "encoding paths\n";
  for (std::size_t i = 0; i < file_size; i++) {
    const std::uint8_t byte = data[i];
    path_t &path = paths[byte];
    compressed_data.encode(path.path);
  }

  std::cout << "total bits: 0x" << std::hex << compressed_data.len
            << ", writing at: 0x" << output.tellp() << "\n";
  output.write((const char *)&compressed_data.len, sizeof(compressed_data.len));
  std::cout << "writing data to: 0x" << output.tellp() << "\n";
  output << compressed_data;
  output.close();
  std::cout << std::endl;
}

extern void huffman_decompress(const std::string &filename) {
  std::cout << "Opening: " << filename << "\n";
  if (!fs::exists(filename)) {
    std::cout << "Error file not found: " << filename << "\n";
    return;
  }
  /*
    the main point of this function is to read the necessary data
    to decompress it, it also builds the tree which it then passes
    to the actual decompressing function

    there's probably some vulnerabilities here as there's not much
    data validation, to fix this it would probably be worth to place
    a couple of asserts in here
  */
  std::ifstream stream(filename, std::ios::binary);
  /* unique nodes available */
  std::uint16_t tree_size = 0;
  stream.read((char *)&tree_size, sizeof(tree_size));
  std::cout << "Tree size: 0x" << std::hex << tree_size << "\n";
  path_t *paths = new path_t[tree_size];

  for (int i = 0; i < tree_size; i++) {
    std::cout << "at: " << std::hex << stream.tellg() << "\n";
    std::size_t to_read = sizeof(paths[i].character);
    stream.read((char *)&paths[i].character, to_read);

    to_read = sizeof(paths[i].len);
    stream.read((char *)&paths[i].len, sizeof(paths[i].len));

    /* this should be made better */
    std::uint8_t p[32] = {0};
    to_read = paths[i].len / (sizeof(*p) * CHAR_BIT) + 1;
    stream.read((char *)p, to_read);
    bitstring bs(p, to_read); // shit tier code

    paths[i].path = bs;
    std::cout << "Read byte: 0x" << +paths[i].character
              << ", length: " << +paths[i].len << "\n";
  }

  decltype(bitstring::len) total_bits = 0;
  std::cout << "reading total bits at: 0x" << std::hex << stream.tellg()
            << "\n";
  stream.read((char *)&total_bits, sizeof(total_bits));

  auto data_start = stream.tellg();
  stream.seekg(0, stream.end);
  std::size_t data_size = stream.tellg() - data_start;

  std::uint8_t *data = new std::uint8_t[data_size];
  stream.seekg(data_start);

  std::cout << "reading data from: 0x" << data_start << "\n";
  stream.read((char *)data, sizeof(data) * data_size);

  std::cout << "total bits: 0x" << total_bits << "\n";

  /*
    build the tree, slow as shit to make it though...
    luckily it's not that large...
    TODO: storing it better might allow it to be faster?
   */
  Node *root = new Node(0, 0, node_type_t::FILLER);
  for (int i = 0; i < tree_size; i++) {
    path_t path = paths[i];
    Node *node = root;
    std::cout << "Building tree for: 0x" << std::hex << +path.character
              << ", len: " << +path.len << "\n";
    std::string p = "";
    for (std::int16_t len = 0; len < path.len; len++) {
      if (path.path.get_bit(len)) {
        if (node->right == nullptr) {
          node->right = new Node(0, 0, node_type_t::FILLER);
        }
        node = node->right;
        p += "1";
      } else {
        if (node->left == nullptr && node->type != DATA) {
          node->left = new Node(0, 0, node_type_t::FILLER);
        }
        node = node->left;
        p += "0";
      }
    }

    std::cout << p + "\n\n";
    node->byte = path.character;
    node->type = node_type_t::DATA;
  }

  path_t temp_paths[UCHAR_MAX + 1] = {0};
  decompress(data, data_size, total_bits, root);

  delete[] data;
  delete[] paths;
  delete root;
}

/**
 * @brief decompersses the data in data
 * @param data the data to compress
 * @param data_size the size of the data array
 * @param total_bits the amount of bits in data
 * @param root the root of the tree
 */
void decompress(std::uint8_t *data, std::size_t data_size,
                std::uint64_t total_bits, Node *const root) {
  assert(root != nullptr);
  assert(data != nullptr);

  FILE *uncompressed = fopen("output", "wb");
  Node *copy = root;
  copy->print_tree();
  std::size_t data_iterator = 0;
  std::string p = "";

  /* walk the tree and decompress the file */
  while (total_bits > 0 && data_iterator < data_size) {
    std::uint8_t byte = data[data_iterator++];
    for (std::int16_t index = 0; index < CHAR_BIT && total_bits > 0; index++) {
      if (byte & (1 << index)) {
        if (copy != nullptr) {
          copy = copy->right;
        }
      } else {
        if (copy != nullptr) {
          copy = copy->left;
        }
      }
      total_bits--;

      if (copy != nullptr) {
        if (copy->type == node_type_t::DATA) {
          fwrite(&copy->byte, sizeof copy->byte, 1, uncompressed);
          copy = root;
          p = "";
          /*
            it's PROBABLY about as fast to write like this as it would be
            to copy to a buffer and write it all at once, dunno
          */
        }
      } else {
        std::cerr << "Error could not traverse tree " << p << "\n";
        return;
      }
    }
  }
  std::cout << "bits left: " << total_bits << "\n";
}
