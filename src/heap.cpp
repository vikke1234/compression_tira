#include "../headers/heap.h"

Node *Heap::pop(void) {
  Node *node = this->nodes[0];
  this->nodes[0] = this->nodes[this->size - 1];
  this->nodes[this->size - 1] = nullptr;
  this->size--;
  this->heapify(0);
  return node;
}

void Heap::insert(Node *node) {
  std::uint8_t i = this->size++;
  this->nodes[i] = node;
  while (i && *node < *this->nodes[(i - 1) / 2]) {
    this->swap(i, (i - 1) / 2);
    i = (i - 1) / 2;
  }
}


void Heap::heapify(const std::uint8_t index) {
  std::uint8_t smallest = index;
  std::uint8_t left = index * 2;
  std::uint8_t right = index * 2 + 1;

  if (left < this->size && this->nodes[left] < this->nodes[smallest]) {
    smallest = left;
  }

  if (right < this->size && this->nodes[right] < this->nodes[smallest]) {
    smallest = right;
  }

  if (smallest != index) {
    this->swap(smallest, index);
    this->heapify(smallest);
  }
}
