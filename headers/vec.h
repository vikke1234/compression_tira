#ifndef VEC_H
#define VEC_H

#include <algorithm>
#include <cstdint>
#include <iterator>
#include <stdexcept>

/* simple implementation of the stl vector, implemented due to course
 * restrictions
 * TODO: add initializer list
 */
template <typename T> class vec {
  std::size_t current_index = 0;
  std::size_t array_capacity = 10;
  T *array = new T[array_capacity](); /* smart pointer? */

public:
  /**
   * @brief defaults to array_size = 10
   */
  vec() = default;

  vec(const std::size_t size) {
    array = new T[size]();
    array_capacity = size;
  }

  vec(const vec &v) {
    array = new T[v.array_capacity];
    current_index = v.current_index;
    array_capacity = v.array_capacity;
    std::copy(v.array, v.array + v.array_capacity, array);
  }

  vec(vec &&temp) {
    current_index = std::exchange(temp.current_index, 0);
    array_capacity = std::exchange(temp.array_capacity, 0);
    array = std::exchange(temp.array, new T[temp.array_capacity]());
  }

  ~vec() { delete[] array; }

  /**
   * @brief pushes to the back of the vector a value
   */
  void push_back(const T &val) {
    if (current_index < array_capacity) {
      array[current_index++] = val;
    } else {
      realloc();
      array[current_index++] = val;
    }
  }

  T &back() const { return array[current_index - 1]; }

  T &front() const { return array[0]; }

  /**
   * @returns the amount of elements, not necessarely equal to capacity
   */
  std::size_t size() const { return current_index; }
  std::size_t capacity() const { return array_capacity; }

  /**
   * @brief reserves `n` elemts to the array
   */
  void reserve(const std::size_t new_capacity) {
    T *new_array = new T[new_capacity]();
    std::copy(array, array + array_capacity, new_array);
    delete[] array;
    array = new_array;
    array_capacity = new_capacity;
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
        delete[] array;
        array = new T[other.array_capacity];
        array_capacity = other.array_capacity;
      }
      current_index = other.current_index;
      std::copy(other.array, other.array + other.array_capacity, array);
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
