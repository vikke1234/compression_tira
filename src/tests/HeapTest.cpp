#include <climits>
#include "../../headers/heap.h"

#include <catch2/catch_all.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Heap inserting", "[heap]") {
  Heap heap;
  SECTION("inserting one and popping it") {
    heap.insert(new Node(0, 123));
    Node *node = heap.pop();
    REQUIRE(node->freq == 123);
    delete node;
  }

  SECTION("insert two") {
    Heap heap;
    heap.insert(new Node(0, 1));
    heap.insert(new Node(1, 2));
    Node *n1 = heap.pop();
    Node *n2 = heap.pop();
    REQUIRE(n1->freq == 1);
    REQUIRE(n2->freq == 2);
  }
  SECTION("inserting after removal") {
    heap.insert(new Node(0, 3));
    heap.insert(new Node(1, 2));
    Node *node = heap.pop();
    REQUIRE(node->freq == 2);
    heap.insert(new Node(2, 42));
    delete node;
    node = heap.pop();
    REQUIRE(node->freq == 2);
    delete node;
  }
}
