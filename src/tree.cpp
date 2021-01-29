#include "../headers/tree.h"

using namespace HuffmanTree;

/**
 @brief Creates a node for the tree
 @param the queue to get the values for the tree
 @return std::shared_ptr<Node> a pointer to a new node
*/
static std::shared_ptr<Tree>
create_node(std::priority_queue<std::pair<std::uint8_t, std::uint64_t>> &queue);

/**
 * @brief Sums up all of the frequencies so for in the tree
 * @param tree the tree from which to sum up
 * @return uint64_t sum of frequencies
 */
static std::uint64_t sum_frequencies(std::shared_ptr<Tree> tree);

/**
 * @brief      This function correctly places new nodes into the tree
 * @param      tree the existing tree
 * @param      node the node to add
 *
 * @return     a pointer to the new tree
 */
static std::shared_ptr<Tree> stabilize_tree(std::shared_ptr<Tree> tree,
                                            std::shared_ptr<Tree> node);
static std::shared_ptr<Tree> create_node(queue_t &queue) {
  if (queue.empty()) {
    return nullptr;
  }
  std::shared_ptr<Tree> root(new Tree);
  auto first = queue.top();
  queue.pop();
  root->byte = first.first;
  root->freq = first.second;
  return root;
}

static std::shared_ptr<Tree> stabilize_tree(std::shared_ptr<Tree> tree,
                                            std::shared_ptr<Tree> node) {
  std::shared_ptr<Tree> stabilized(new Tree);

  if (tree < node) {
    stabilized->left = tree;
    stabilized->right = node;
  } else {
    stabilized->left = node;
    stabilized->right = tree;
  }
  stabilized->freq = sum_frequencies(tree);
  return stabilized;
}

static std::uint64_t sum_frequencies(std::shared_ptr<Tree> tree) {
  if (tree == nullptr) {
    return 0;
  }
  return sum_frequencies(tree->left) + sum_frequencies(tree->right);
}

extern std::shared_ptr<Tree> HuffmanTree::create_tree(queue_t queue) {
  std::shared_ptr<Tree> tree = create_node(queue);
  while (!queue.empty()) {
    auto node = create_node(queue);
    stabilize_tree(tree, node);
  }
  return tree;
}
