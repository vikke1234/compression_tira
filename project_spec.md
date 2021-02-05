# Specification

## Algorithms to compare
- Huffman
- Lempel Ziv Welch

# Design
## Data structures
- Trees (Hoffman)
- Lists

## Time complexities
[Huffman encoding](https://en.wikipedia.org/wiki/Huffman_coding) is roughtly 
 _O(n log n)_ or best case _O(n)_ while [Lempel
 Ziv](https://en.wikipedia.org/wiki/Lempel-Ziv_complexity) is _O (n)_ 
 
## I/O
The following should go both ways, to allow compression and decompression.
### Input 
Some data (e.g. a picture, text or just some other data)
### Output
Compressed data

# File formats
## Huffman
```cpp
struct {
    uint8_t tree_size;
      struct Node {
          uint8_t byte;
          uint8_t len;
          uint16_t path;
      }[tree_size];
    uint64_t total_length;
    uint8_t data[total_length];
};
```

## Lempel ziv
