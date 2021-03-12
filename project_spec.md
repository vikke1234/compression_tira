# Specification

## File compression to compare
Due to the fact that I didn't implement LZ77, I'll be comparing how different
files compress using the Huffman algorithm.

- Random data
- Text files

For the executable the `yes` executable was used.


|              | Text Files | Executable |
|--------------|------------|------------|
| Uncompressed | 68KB       | 39KB       |
| Compressed   | 42KB       | 31KB       |

# Design
## Data structures
- Trees (Huffman)
- Lists
- Bit string

## What could be made better
- The bitstring encoding could probably be made into O(n) though I'm not sure how
- Threading could be added for the compression

## Time complexities
[Huffman encoding](https://en.wikipedia.org/wiki/Huffman_coding) is roughly 
 _O(n log n)_ or best case _O(n)_. Currently though the encoding of the paths
is too slow though, currently it resembles roughly `O(n*m)` where `n` is the
size of the file and `m` is how much the bit strings need to be resized.

## I/O
The following should go both ways, to allow compression and decompression.
### Input 
Some data (e.g. a picture, text or just some other data)
### Output
Compressed data

# File formats
## Huffman
This essentially how the file structure looks like.
Though the following snippet of code isn't completely valid c++ but it gives
a picture of how it looks like.
```cpp
struct {
    uint8_t tree_size;
      struct path_t {
          uint8_t byte;
          uint8_t len;
          uint16_t path;
      }[tree_size];
    uint64_t total_length;
    uint8_t data[total_length];
};
```
