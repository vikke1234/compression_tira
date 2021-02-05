#include "../headers/heap.h"

Node *Heap::pop(void) {
  Node *node = this->nodes[0];
  this->nodes[0] = this->nodes[this->index - 1];
  this->nodes[this->index - 1] = nullptr;
  this->index--;
  this->heapify(0);
  return node;
}

void Heap::insert(Node *node) {
  std::uint8_t i = this->index++;
  while (i && node < this->nodes[(i - 1) / 2]) {
    this->nodes[i] = this->nodes[(i - 1) / 2];
    i = (i - 1) / 2;
  }
  this->nodes[i] = node;
}

void Heap::heapify(const std::uint8_t index) {
  std::uint8_t smallest = index;
  std::uint8_t left = index * 2 + 1;
  std::uint8_t right = index * 2 + 2;

  if (left < this->index && this->nodes[left] < this->nodes[smallest]) {
    smallest = left;
  }

  if (right < this->index && this->nodes[right] < this->nodes[smallest]) {
    smallest = right;
  }

  if (smallest != index) {
    this->swap(smallest, index);
    this->heapify(smallest);
  }
}
