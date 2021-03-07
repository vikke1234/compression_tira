#include "../headers/bitstring.h"

#ifdef WIN32
#define __builtin_bswap64 _byteswap_uint64
#else
#include <endian.h>
#endif
#include <algorithm>
#include <cassert>

void bitstring::encode(bitstring &to_encode) {
  assert(to_encode.len > 0);
  auto encoded_len = to_encode.len;
  if (encoded_len > bits_left) {
    bits.push_back(0);
    bits_left = BITS_PER_ELEMENT;
  } else {
      bits_left -= encoded_len;
  }

  auto encoded = to_encode << this->len;
  *this |= encoded;
  this->len += to_encode.len;
}

bitstring &bitstring::operator|=(const bitstring &mask) {
  if (bits.size() < mask.bits.capacity()) {
    bits.resize(mask.bits.capacity() + 1);
  }
  for (std::size_t table = 0;
       table < bits.capacity() && table < mask.bits.capacity(); table++) {
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
  int actual_shift = i % BITS_PER_ELEMENT;
  int offset_table = i / BITS_PER_ELEMENT;
  if (bits.size() + offset_table >= bits.capacity()) {
    bits.reserve((bits.size() + offset_table + 1) * 3 / 2);
  }
  /* 0 is a special case which we have to handle, you would get a shift underflow
   * if you didn't do this, i.e. shifting by 64 */
  if (actual_shift == 0) {
    for (int i = bits.size(); i >= 0; i--) {
      bits[i + offset_table] = bits[i];
    }
  } else {
    for (int table = bits.size(); table >= 0; table--) {
      if (table > 0) {
        std::uint64_t spared_bits = (bits[table - 1] >> (BITS_PER_ELEMENT - actual_shift));
        std::uint64_t shifted_bits = (bits[table] << actual_shift);
        std::uint64_t result = shifted_bits | spared_bits;
        bits[table + offset_table] = result;
      } else {
        std::uint64_t result = bits[table] << actual_shift;
        bits[table + offset_table] = result;
      }
    }
  }

  if (offset_table > 0) {
    for (int i = 0; i < offset_table; i++) {
      bits[i] = 0;
    }
  }
  return *this;
}

bitstring &bitstring::operator>>=(const int i) {
  throw std::logic_error("not implemented");
}

void bitstring::unset_bit(std::size_t i) {
  if (i >= std::max((std::size_t)BITS_PER_ELEMENT,
                    BITS_PER_ELEMENT * bits.capacity())) {
    bits.resize(i / BITS_PER_ELEMENT + 1);
  }
  int table = i / BITS_PER_ELEMENT;
  int index = i % BITS_PER_ELEMENT;
  bits[table] &= ~(1 << index);
}

void bitstring::set_bit(std::size_t i) {
  if (i >= std::max((std::size_t)BITS_PER_ELEMENT,
                    BITS_PER_ELEMENT * bits.capacity())) {
    bits.resize(i / BITS_PER_ELEMENT + 1);
  }
  int table = i / BITS_PER_ELEMENT;
  int index = i % BITS_PER_ELEMENT;
  bits[table] |= 1 << index;
}

std::uint8_t bitstring::get_bit(std::size_t i) {
  if (i >= BITS_PER_ELEMENT * bits.capacity()) {
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

bitstring::bitstring() {
  bits.push_back(0);
}

bitstring::bitstring(std::initializer_list<std::uint64_t> list) : bits(4) {
  for (auto n : list) {
    bits.push_back(n);
  }
}

bitstring::bitstring(const std::uint64_t n) : bits(4) { bits.push_back(n); }

bitstring::bitstring(const std::uint64_t n, const std::uint32_t len) : bits(4) {
  bits.push_back(n);
  this->len = len;
}

bitstring::bitstring(const bitstring &bs)
    : len(bs.len), bits(bs.bits), bits_left(bs.bits_left) {}

std::ofstream &operator<<(std::ofstream &stream, const bitstring &bs) {
  std::cout << "table size: " << bs.bits.size() << "\n";
  for (std::size_t table = 0; table < bs.bits.size(); table++) {
    /* we want the written  */
    std::uint64_t big_endian = bs.bits[table];
    const unsigned char *bytes = (const unsigned char *)&big_endian;
    stream.write((const char*)bytes, sizeof(big_endian));
  }
  return stream;
}

bitstring &bitstring::reverse() {
  vec<std::uint64_t> reversed(bits.size());
  for (unsigned int i = 0; i < bits.size(); i++) {
    reversed[bits.size() - i] = __bswap_64(bits[i]);
  }
  return *this;
}

void bitstring::write_tree_path(std::ofstream &stream) const {
  std::cout << "bitstring length: " << len << "\n";
  for (unsigned int i = 0; i < bits.capacity(); i++) {
    auto n = bits[i];
    stream.write((const char *)&n, sizeof(n));
  }
}

std::ostream &operator<<(std::ostream &os, const bitstring &value) {
  for (int table = value.len / BITS_PER_ELEMENT; table >= 0; table--) {
    for (int j =
             std::min(value.len - 1, (std::uint64_t)(BITS_PER_ELEMENT - 1llu));
         j >= 0; j--) {
      os << ((value.bits[table] >> j) & 1);
    }
  }
  os << "\n";
  return os;
}
