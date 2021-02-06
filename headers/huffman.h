#ifndef HUFFMAN_H
#include <cstdint>
#include <memory>
#include <string>
#include <vector>


/**
 * @brief      huffman compression for a file
 *
 * @details    uses huffman coding to compress a file
 *
 * @param      filename file to compress
 *
 * @return     void
 */
extern void huffman_compression(const std::string &filename);
/**
 * @brief decompresses a file huffman compressed filename
 * @param filename of the file
 * @return void
 */
extern void huffman_decompress(const std::string &filename);

#endif // HUFFMAN_H
