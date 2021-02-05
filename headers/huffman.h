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
extern void huffman_compression(const std::string &filename);


#endif // HUFFMAN_H
