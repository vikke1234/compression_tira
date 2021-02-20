#ifndef VEC_H
#define VEC_H

#include <algorithm>
#include <cstdint>
#include <iterator>
#include <stdexcept>
#include <memory>
#include <cassert>

/* simple implementation of the stl vector, implemented due to course
 * restrictions
 * TODO: add initializer list
 */
template <typename T> class vec {
  std::size_t current_index = 0;
  std::size_t array_capacity = 10;
  std::unique_ptr<T[]> array = std::make_unique<T[]>(array_capacity); /* smart pointer? */

public:
  /**
   * @brief defaults to array_size = 10
   */
  vec() = default;

  vec(const std::size_t size) {
    array.reset(new T[size]());
    array_capacity = size;
  }

  vec(const vec &v) {
    array.reset(new T[v.array_capacity]);
    current_index = v.current_index;
    array_capacity = v.array_capacity;
    std::copy(v.array.get(), v.array.get() + v.array_capacity, array.get());
  }

  vec(vec &&temp) {
    current_index = std::exchange(temp.current_index, 0);
    array_capacity = std::exchange(temp.array_capacity, 0);
    array = std::exchange(temp.array, new T[temp.array_capacity]());
  }

  /**
   * @brief pushes to the back of the vector a value
   */
  void push_back(const T &val) {
    if (current_index < array_capacity - 1) {
      array[current_index++] = val;
    } else {
      realloc();
      array[current_index++] = val;
    }
  }

  T &back() const {
    assert(current_index > 0);
    return array[current_index-1];
  }

  T &front() const { return array[0]; }

  /**
   * @returns the amount of elements, not necessarely equal to capacity
   */
  inline std::size_t size() const { return current_index; }
  inline std::size_t capacity() const { return array_capacity; }

  /**
   * @brief reserves `n` elemts to the array
   */
  void reserve(const std::size_t new_capacity) {
    T *new_array = new T[new_capacity]();
    std::copy(array.get(), array.get() + array_capacity, new_array);
    array.reset(new_array);
    array_capacity = new_capacity;
  }

  void resize(const std::size_t new_size) {
    if(new_size > this->capacity()) {
      reserve(new_size * 3 / 2);
    }
    current_index = new_size;
  }

  /**
   * @brief fetches element `i` from the array, if out of bounds returns the
   * default constructor
   */
  T &get(const std::size_t index) const {
    if (index < array_capacity) {
      return array[index];
    } else {
      throw std::range_error("index");
    }
  }

  /**
     @brief @see{get}
   */
  T &operator[](const std::size_t index) const { return get(index); }

  vec<T> &operator=(const vec &other) {
    if (this != &other) {
      if (array_capacity != other.array_capacity) {
        array.reset(new T[other.array_capacity]);
        array_capacity = other.array_capacity;
      }
      current_index = other.current_index;
      std::copy(other.array.get(), other.array.get() + other.array_capacity, array.get());
    }
    return *this;
  }

private:
  /**
   * @brief reallocates the array
   */
  void realloc() { reserve(array_capacity * 3 / 2); }
};
#endif /* VEC_H */
