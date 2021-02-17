#ifndef BITSTRING_H
#define BITSTRING_H

#include "vec.h"
#include <climits>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <iterator>

constexpr std::uint8_t BITS_PER_ELEMENT = sizeof(std::uint64_t) * CHAR_BIT;

class bitstring {
  std::int8_t bits_left = 64;
  vec<std::uint64_t> bits = vec<std::uint64_t>(4);

public:
  std::uint32_t len = 0; /* max length of the tree is 255 */

  bitstring() = default;

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
   @brief reserves `n` bytes for the byte string
  */
  void reserve(std::size_t size);
  bitstring operator|(const bitstring &mask) const;
  bitstring operator&(const bitstring &mask) const;
  bitstring operator>>(const int i) const;
  bitstring operator<<(const int i) const;

  bitstring &operator>>=(const int i);
  bitstring &operator<<=(const int i);
  bitstring &operator|=(const bitstring &mask);
  bitstring &operator&=(const bitstring &mask);
  bool operator==(const bitstring &bs) const;
  bool operator!=(const bitstring &bs) const;
  bitstring &operator=(const bitstring &other);

  friend std::ostream &operator<<(std::ostream &os, const bitstring &value) {
    for (int table = value.bits.size(); table >= 0; table--) {
      for (int j = BITS_PER_ELEMENT; j >= 0; j--) {
        os << ((value.bits[table] >> j) & 1);
      }
    }
    return os;
  }

private:
  /**
    this doesn't return a bitstring due to only
    being used while encoding to save the bits from 1 element
  */
  std::uint64_t build_mask(std::size_t len) { return (1 << len) - 1; }
};

#endif /* BITSTRING_H */
