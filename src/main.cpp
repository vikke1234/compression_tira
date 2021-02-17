#include <iostream>
#include <unistd.h>

#include "../headers/heap.h"
#include "../headers/huffman.h"
int main(int argc, char *argv[]) {
  std::string help = std::string("Usage: ") + argv[0] +
                     "\n-d filename \tdecompression\n-c filename \tcompression\n";
  int opt = 0;
  if(argc < 2) {
    std::cerr << help;
  }
  while ((opt = getopt(argc, argv, "c:d:")) != -1) {
    switch (opt) {
    case 'c':
      huffman_compression(optarg);
      break;
    case 'd':
      huffman_decompress(optarg);
      break;
    default:
      std::cerr << help;
    }
  }
  return 0;
}
