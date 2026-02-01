#include <istream>
#include "red-black-tree.hpp"

int main() {

    rbtree::RBTree_t<int> tree;

    std::ifstream input("data.dat");
    {
        int value;
        while (input >> value) {
            tree.insert(value);
        }
    }
    if (tree.contains(20)) {
        std::cout << "Tree contains 20\n";
    } else {
        std::cout << "Tree does not contain 20\n";
    }

    tree.dump("rbtree_dump");

    printf("%ld\n", tree.number_in_range(10, 50));

    return 0;
}