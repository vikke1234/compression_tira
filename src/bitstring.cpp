#include "../headers/bitstring.h"

#ifdef WIN32
#define __builtin_bswap64 _byteswap_uint64
#else
#include <endian.h>
#endif
#include <cassert>
#include <algorithm>

void bitstring::encode(bitstring &to_encode) {
  assert(to_encode.len > 0);
  auto encoded_len = to_encode.len;
  this->len += encoded_len;
  int table = 0;
  do {
    /* warning scary bit stuff... tl;dr it will put the paths into as few
     * bytes as possible */
    if (bits_left > encoded_len) {
      /* this should be the last time the loop runs */
      bits.back() |= to_encode.bits[table] << (BITS_PER_ELEMENT - bits_left);
      bits_left -= encoded_len;
    } else {
      bits.back() |= to_encode.bits[table] << (BITS_PER_ELEMENT - bits_left);
      auto bits_written = bits_left;
      bits_left = std::abs((long long)bits_left - (long long)encoded_len);
      std::uint64_t the_rest = to_encode.bits[table] >> bits_left;
      encoded_len -= bits_left + bits_written;
      bits_left = BITS_PER_ELEMENT - bits_left;
      bits.push_back(the_rest);
    }
    table++;
  } while (table < to_encode.len / sizeof(bits[0]) && encoded_len > 0);
}

bitstring &bitstring::operator|=(const bitstring &mask) {
  for (std::size_t table = 0; table < bits.capacity() && table < mask.bits.capacity();
       table++) {
    bits[table] |= mask.bits[table];
  }
  return *this;
}

bitstring bitstring::operator|(const bitstring &mask) const {
  bitstring bs = *this;
  bs |= mask;
  return bs;
}

bitstring &bitstring::operator&=(const bitstring &mask) {
  for (std::uint8_t table = 0;
       table < bits.capacity() && table < mask.bits.capacity(); table++) {
    bits[table] &= mask.bits[table];
  }
  return *this;
}

bitstring bitstring::operator&(const bitstring &mask) const {
  bitstring bs = *this;
  bs &= mask;
  return bs;
}

bitstring bitstring::operator>>(const int i) const {
  bitstring bs = *this;
  bs >>= i;
  return bs;
}

bitstring bitstring::operator<<(const int i) const {
  bitstring bs = *this;
  bs <<= i;
  return bs;
}

bitstring &bitstring::operator<<=(const int i) {
  std::uint8_t carry = 0;
  const std::uint64_t carry_mask = 1llu << (BITS_PER_ELEMENT - 1);
  for (std::uint8_t table = 0; table < bits.size(); table++) {
    carry = !!(bits[table] & carry_mask);
    bits[table] <<= i;
    if (table != 0) {
      bits[table] |= carry;
    }
  }
  return *this;
}

bitstring &bitstring::operator>>=(const int i) {
  std::uint8_t carry = 0;
  const std::uint64_t carry_mask = 1;
  for (std::uint8_t table = 0; table < bits.size(); table++) {
    carry = bits[table] & carry_mask;
    bits[table] >>= i;
    if (table != 0) {
      bits[table] |= carry;
    }
  }
  return *this;
}

void bitstring::unset_bit(std::size_t i) {
  if (i >= std::max((std::size_t)BITS_PER_ELEMENT, BITS_PER_ELEMENT * bits.capacity())) {
    bits.resize(i % BITS_PER_ELEMENT + 1);
  }
  int table = i / BITS_PER_ELEMENT;
  int index = i % BITS_PER_ELEMENT;
  bits[table] &= ~(1 << index);
}

void bitstring::set_bit(std::size_t i) {
  if (i >= std::max((std::size_t)BITS_PER_ELEMENT, BITS_PER_ELEMENT * bits.capacity())) {
    bits.resize(i % BITS_PER_ELEMENT + 1);
  }
  int table = i / BITS_PER_ELEMENT;
  int index = i % BITS_PER_ELEMENT;
  bits[table] |= 1 << index;
}

std::uint8_t bitstring::get_bit(std::size_t i) {
  if (i > BITS_PER_ELEMENT * bits.capacity()) {
    return 0;
  }
  std::uint8_t table = i / BITS_PER_ELEMENT;
  std::uint16_t index = i % BITS_PER_ELEMENT;
  return (bits[table] >> index) & 1;
}

void bitstring::reserve(const std::size_t size) { bits.reserve(size); }

bitstring &bitstring::operator=(const bitstring &other) {
  if (this != &other) {
    bits = other.bits;
    len = other.len;
    bits_left = other.bits_left;
  }
  return *this;
}

bool bitstring::operator==(const bitstring &bs) const {
  for (std::size_t i = 0; i < bits.capacity() && i < bs.bits.capacity(); i++) {
    if (bits[i] != bs.bits[i]) {
      return false;
    }
  }

  return true;
}

bool bitstring::operator!=(const bitstring &bs) const {
  return !(operator==(bs));
}

bitstring::bitstring()
  :bits(4) {
}
bitstring::bitstring(std::initializer_list<std::uint64_t> list) : bits(4) {
  for (auto n : list) {
    bits.push_back(n);
  }
  bits.resize(list.size());
}

bitstring::bitstring(const std::uint64_t n) : bits(4) { bits.push_back(n); }

bitstring::bitstring(const std::uint64_t n, const std::uint32_t len) {
  bits[0] = n;
  this->len = len;
}

bitstring::bitstring(const bitstring &bs)
    : len(bs.len), bits(bs.bits), bits_left(bs.bits_left) {}

std::ofstream &operator<<(std::ofstream &stream, const bitstring &bs) {
  auto len = bs.len;
  std::cout << "table size: " << bs.bits.size() << "\n";
  for (std::size_t table = 0; table < bs.bits.size(); table++) {
    /* we want the written  */
    std::uint64_t big_endian = bs.bits[table];
    stream.write((const char *)&big_endian, sizeof(big_endian));
  }
  return stream;
}

bitstring &bitstring::reverse() {
  vec<std::uint64_t> reversed(bits.size());
  for (int i = 0; i < bits.size(); i++) {
    reversed[bits.size() - i] = __bswap_64(bits[i]);
  }
  return *this;
}

void bitstring::write_tree_path(std::ofstream &stream) const {
  std::cout << "bitstring length: " << len << "\n";
  for (int i = 0; i < bits.capacity(); i++) {
    stream.write((const char *)&bits[i], sizeof(bits[i]));
  }
}

std::ostream &operator<<(std::ostream &os, const bitstring &value) {
  for (int table = value.len / BITS_PER_ELEMENT; table >= 0; table--) {
    for (int j = std::min(value.len, (std::uint64_t)(BITS_PER_ELEMENT - 1llu)); j >= 0; j--) {
      os << ((value.bits[table] >> j) & 1);
    }
  }
  os << "\n";
  return os;
}
