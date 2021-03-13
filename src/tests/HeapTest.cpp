#include "../../headers/heap.h"
#include "../../headers/bitstring.h"
#include "../../headers/vec.h"
#include <climits>

#include <catch2/catch_all.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Heap inserting", "[heap]") {
  Heap heap;
  SECTION("inserting one and popping it") {
    heap.insert(new Node(0, 123));
    Node *node = heap.pop();
    REQUIRE(node->freq == 123);
    delete node;
    REQUIRE(heap.get_size() == 0);
  }

  SECTION("insert two") {
    Heap heap;
    heap.insert(new Node(0, 1));
    heap.insert(new Node(1, 2));
    Node *n1 = heap.pop();
    REQUIRE(n1->freq == 1);
    Node *n2 = heap.pop();
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
    REQUIRE(node->freq == 3);
    delete node;
    node = heap.pop();
    REQUIRE(node->freq == 42);
    delete node;
  }
  SECTION("insert many") {
    int n = UCHAR_MAX;
    for (int i = 0; i < n; i++) {
      heap.insert(new Node(0, 'a' + i));
    }
    for (int i = 0; i < n; i++) {
      Node *node = heap.pop();
      REQUIRE(node->freq == 'a' + i);
      delete node;
    }
  }
}

TEST_CASE("Vectors", "[vector]") {
  SECTION("initializing") {
    vec<int> v1;
    for(int i = 0; i < v1.capacity(); i++) {
      REQUIRE(v1[i] == 0);
    }
  }

  SECTION("inserting") {
    vec<int> vector;
    vector[0] = 123;
    REQUIRE(vector[0] == 123);
  }

  SECTION("push back") {
    vec<int> vector;
    vector.push_back(123);
    vector.push_back(321);
    REQUIRE(vector[1] == 321);
  }

  SECTION("back") {
    vec<int> vector;
    vector.push_back(123);
    vector.push_back(321);
    REQUIRE(vector.back() == 321);
    vector.back() = 123;
    REQUIRE(vector.back() == 123);
  }

  SECTION("front") {
    vec<int> vector;
    vector.push_back(123);
    vector.push_back(321);
    REQUIRE(vector.front() == 123);
    vector.front() = 0xff;
    REQUIRE(vector.front() == 0xff);
  }
}

TEST_CASE("Bitstrings", "[bitstring]") {
  SECTION("initializing") {
    bitstring bs;
    bitstring bs2(0x1);
    bitstring bs3(0xa);
    INFO("bs: " << bs);
    for (int i = 0; i < 255; ++i) {
      CHECK(bs.get_bit(i) == 0);
      REQUIRE(bs.get_bit(i) == 0);
    }

    REQUIRE(bs2.get_bit(0) == 1);
    REQUIRE(bs3.get_bit(0) == 0);
    REQUIRE(bs3.get_bit(1) == 1);
  }

  SECTION("setting bits") {
    bitstring bs;
    bs.set_bit(0);
    REQUIRE(bs.get_bit(0) == 1);
    bs.set_bit(65);
    REQUIRE(bs.get_bit(65) == 1);
  }

  SECTION("unsetting bits") {
    bitstring bs{0xff};
    bs.unset_bit(0);
    bs.unset_bit(4);
    REQUIRE(bs.get_bit(0) == 0);
    REQUIRE(bs.get_bit(4) == 0);
  }
  SECTION("copying") {
    bitstring start{0xaa}, assign_to;
    assign_to = start;
    REQUIRE(start == assign_to);
  }
  SECTION("comparing") {
    std::uint8_t ptr[] = {0x0, 0xf};
    bitstring bs(ptr, 2);
    bitstring equal(ptr, 2);
    bitstring not_equal(0xde);

    REQUIRE(bs == equal);
    REQUIRE(bs != not_equal);
  }

  SECTION("shifting left") {
    bitstring start_value{0xaa};
    std::uint8_t initial_val[] = {0x0, 0xaa};
    bitstring end_result(initial_val, 2);
    bitstring new_value;
    new_value = start_value << 8;
    REQUIRE(end_result == new_value);
  }


  SECTION("large shift left") {
    std::uint8_t endresult[] = {0llu, 0xff};
    bitstring bs{0xff}, compare_to(endresult, 2);
    bitstring result = bs << 8;
    REQUIRE(result == compare_to);
  }

  SECTION("logical OR") {
    bitstring start_value, compare_to(1llu), new_value, mask(1llu);
    new_value = start_value | mask;
    REQUIRE(new_value == compare_to);
  }

  SECTION("logical AND") {
    bitstring start_value(1), mask(0), compare_to(0);
    REQUIRE((start_value & mask) == compare_to);
  }

  SECTION("simple encoding") {
    bitstring start(0), append(0xff), end_value(0xff);
    append.len = 8;
    start.encode(append);
    REQUIRE(start == end_value);
  }

  SECTION("complex encoding") {
    std::uint8_t end_resultptr[] = {0xff, 0xff, 0xa};
    bitstring start(0);
    bitstring append(0xff);
    bitstring last(0xa);
    bitstring end_result{end_resultptr, 3};

    start.len = 0;
    append.len = 8;
    last.len = 4;

    start.encode(append);
    start.encode(append);
    start.encode(last);
    end_result.len = start.len;
    REQUIRE(start == end_result);
  }
}
