#include <cxxtest/TestSuite.h>
#include <climits>

#include "../heap.h"

using namespace CxxTest;

class HeapTest : public TestSuite {
public:
  void test_one_insert(void) {
    Heap heap;
    heap.insert(new Node(0, 123));
    Node *node = heap.pop();

    TS_ASSERT(node->freq == 123);
    delete node;
  }

  void test_two_insert(void) {
    Heap heap;
    heap.insert(new Node(0, 1));
    heap.insert(new Node(1, 2));
    Node *n1 = heap.pop();
    Node *n2 = heap.pop();
    TS_ASSERT(n1->freq == 1);
    TS_ASSERT(n2->freq == 2);
  }

  void test_insert_and_remove(void) {
    Heap heap;
    heap.insert(new Node(0, 1));
    heap.insert(new Node(1, 2));
    Node *node = heap.pop();
    TS_ASSERT(node->freq == 1);
    heap.insert(new Node(2, 42));
    delete node;
    node = heap.pop();
    TS_ASSERT(node->freq == 2);
  }
};
