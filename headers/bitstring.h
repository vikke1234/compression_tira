#ifndef BITSTRING_H
#define BITSTRING_H

#include "vec.h"
#include <climits>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>

constexpr std::uint8_t BITS_PER_ELEMENT = sizeof(std::uint64_t) * CHAR_BIT;

class bitstring {
  std::int8_t bits_left = 64;
  vec<std::uint64_t> bits = vec<std::uint64_t>(4);

public:
  /**
   @brief length in bits
   @details max length of the tree is 255 but this is a 64 bit since it also
   stores the length of the entire string
  */
  std::uint64_t len = 0;

  bitstring();

  /* TODO: add a list initializer for uint8_t? */

  bitstring(std::initializer_list<std::uint64_t> list);

  bitstring(const bitstring &bs);
  /**
   * @brief make a bitstring out of a number
   */
  bitstring(const std::uint64_t n);

  bitstring(const std::uint64_t n, const std::uint32_t len);

  /**
   @brief gets the bits at index i
   @returns the bit at index i or 0 if out of bounds
  */
  std::uint8_t get_bit(std::size_t i);

  /**
   * @brief tries to set the bit i
   */
  void set_bit(std::size_t i);

  /**
   * @brief tries to unset the bit i
   */
  void unset_bit(std::size_t i);

  /**
   * @brief encodes a bitstring to the end of the current bitstring
   */
  void encode(bitstring &to_encode);

  /**
   * @brief reverses the entire bitstring using compiler specific stuff
   */
  bitstring &reverse();
  /**
   @brief reserves `n` bytes for the byte string
  */
  void reserve(std::size_t size);

  /**
   * @brief this is basically just to make it "easier" to read the tree
   * correctly, should NOT be used for anything else
   */
  void write_tree_path(std::ofstream &stream) const;

  /**
   * @brief does bitwise or on a bitstring
   */
  bitstring operator|(const bitstring &mask) const;
  /**
   * @brief does bitwise and on a bitstring
   */
  bitstring operator&(const bitstring &mask) const;

  /**
   * @brief shifts a bitstring right
   */
  bitstring operator>>(const int i) const;

  /**
   * @brief shifts a bitstring left
   */
  bitstring operator<<(const int i) const;

  /**
   * @brief shifts a bitstring right
   */
  bitstring &operator>>=(const int i);
  /**
   * @brief shifts a bitstring left
   */
  bitstring &operator<<=(const int i);
  /**
   * @brief does bitwise or on a bitstring
   */
  bitstring &operator|=(const bitstring &mask);
  /**
   * @brief does bitwise and on a bitstring
   */
  bitstring &operator&=(const bitstring &mask);
  /**
   * @brief compares bitstrings if they're equal
   */
  bool operator==(const bitstring &bs) const;
  /**
   * @brief compares bitstrings if they're not equal
   */
  bool operator!=(const bitstring &bs) const;
  bitstring &operator=(const bitstring &other);

  friend std::ostream &operator<<(std::ostream &os, const bitstring &value);
  friend std::ofstream &operator<<(std::ofstream &stream, const bitstring &bs);

private:
  /**
    this doesn't return a bitstring due to only
    being used while encoding to save the bits from 1 element
  */
  std::uint64_t build_mask(std::size_t len) { return (1 << len) - 1; }
};

#endif /* BITSTRING_H */
