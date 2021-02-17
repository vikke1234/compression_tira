#include "../headers/bitstring.h"

void bitstring::encode(bitstring &to_encode) {
  auto len = to_encode.len;
  auto remeinder = len % BITS_PER_ELEMENT;
  for (std::uint8_t table = 0; table < to_encode.bits.size(); table++) {
    /* warning scary bit stuff... tl;dr it will put the paths into as few
     * bytes as possible */
    if ((std::int8_t)bits_left > len) {
      bits.back() |= to_encode.bits[table] << (BITS_PER_ELEMENT - bits_left);
      bits_left -= len;
    } else {
      bits.back() |= to_encode.bits[table] << (BITS_PER_ELEMENT - bits_left);
      bits_left = std::abs((long long)bits_left - len);
      std::uint64_t the_rest = to_encode.bits[table] >> bits_left;
      bits_left = BITS_PER_ELEMENT - bits_left;
      bits.push_back(the_rest);
    }
  }
}

bitstring &bitstring::bitstring::operator|=(const bitstring &mask) {
  for (int table = 0; table < bits.capacity() && table < mask.bits.capacity();
       table++) {
    bits[table] |= mask.bits[table];
  }
  return *this;
}

bitstring bitstring::bitstring::operator|(const bitstring &mask) const {
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
  if (i > BITS_PER_ELEMENT * bits.capacity()) {
    return;
  }
  std::uint8_t table = i / BITS_PER_ELEMENT;
  std::uint16_t index = i % BITS_PER_ELEMENT;
  bits[table] &= ~(1 << index);
}

void bitstring::set_bit(std::size_t i) {
  if (i > BITS_PER_ELEMENT * bits.capacity() - 1) {
    return;
  }
  std::uint8_t table = i / BITS_PER_ELEMENT;
  std::uint16_t index = i % BITS_PER_ELEMENT;
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
  for (int i = 0; i < bits.capacity() && i < bs.bits.capacity(); i++) {
    if (bits[i] != bs.bits[i]) {
      return false;
    }
  }

  return true;
}

bool bitstring::operator!=(const bitstring &bs) const {
  return !(operator==(bs));
}

bitstring::bitstring(std::initializer_list<std::uint64_t> list) : bits() {
  for (auto n : list) {
    bits.push_back(n);
  }
}

bitstring::bitstring(const std::uint64_t n) : bits() { bits[0] = n; }

bitstring::bitstring(const std::uint64_t n, const std::uint32_t len) {
  bits[0] = n;
  this->len = len;
}

bitstring::bitstring(const bitstring &bs)
    : len(bs.len), bits(bs.bits), bits_left(bs.bits_left) {}
