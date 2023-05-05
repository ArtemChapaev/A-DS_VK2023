// 2.3.
// Дано число N < 106 и последовательность целых чисел из [-231..231] длиной N.

// Требуется построить бинарное дерево, заданное наивным порядком вставки.
// Т.е., при добавлении очередного числа K в дерево с корнем root, если root→Key ≤ K,
// то узел K добавляется в правое поддерево root; иначе в левое поддерево root.

// Выведите элементы в порядке post-order (снизу вверх).

// Требования:
// Рекурсия запрещена.
// Решение должно поддерживать передачу функции сравнения снаружи.

#include <iostream>
#include <stack>

struct IntCmp {
    bool operator()(int l, int r) { return l < r; }
};

template<class T, class Compare>
class BinaryTree {
public:
    BinaryTree(const Compare &_cmp) : root(nullptr), cmp(_cmp) {}

    ~BinaryTree();
    BinaryTree(BinaryTree &) = delete;
    BinaryTree &operator=(BinaryTree &) = delete;

    void Add(const T &key);

    // post-order (снизу вверх)
    void DfsInOrder(void (*visit)(const T &key)) const;

private:
    struct Node {
        Node *Left;
        Node *Right;
        T Key;

        Node(const T &key) : Left(nullptr), Right(nullptr), Key(key) {}
    };

    Node *root;
    Compare cmp;
};

template<class T, class Compare>
BinaryTree<T, Compare>::~BinaryTree() {
    if (root == nullptr) {
        return;
    }

    // if tree is not empty
    std::stack<Node *> stackForDelete;
    std::stack<Node *> auxiliaryStack;
    auxiliaryStack.push(root);

    while (!auxiliaryStack.empty()) {
        Node *node = auxiliaryStack.top();
        auxiliaryStack.pop();
        stackForDelete.push(node);

        if (node->Left != nullptr) {
            auxiliaryStack.push(node->Left);
        }
        if (node->Right != nullptr) {
            auxiliaryStack.push(node->Right);
        }
    }

    while (!stackForDelete.empty()) {
        Node *node = stackForDelete.top();
        stackForDelete.pop();
        delete node;
    }
};

template<class T, class Compare>
void BinaryTree<T, Compare>::Add(const T &key) {
    Node *&node = root;
    while (node != nullptr) {
        if (cmp(key, node->Key)) {
            node = node->Left;
        } else {
            node = node->Right;
        }
    }

    node = new Node(key);
}

// post-order (снизу вверх)
template<class T, class Compare>
void BinaryTree<T, Compare>::DfsInOrder(void (*visit)(const T &key)) const {
    if (root == nullptr) {
        return;
    }

    // if tree is not empty
    std::stack<T> dfsStack;
    std::stack<Node *> auxiliaryStack;
    auxiliaryStack.push(root);

    while (!auxiliaryStack.empty()) {
        Node *node = auxiliaryStack.top();
        auxiliaryStack.pop();
        dfsStack.push(node->Key);

        if (node->Left != nullptr) {
            auxiliaryStack.push(node->Left);
        }
        if (node->Right != nullptr) {
            auxiliaryStack.push(node->Right);
        }
    }

    while (!dfsStack.empty()) {
        T key = dfsStack.top();
        dfsStack.pop();
        visit(key);
    }
}

int main() {
    IntCmp cmp;
    BinaryTree<int, IntCmp> tree(cmp);

    unsigned n = 0;
    std::cin >> n;
    for (int i = 0; i < n; ++i) {
        int key = 0;
        std::cin >> key;
        tree.Add(key);
    }

    tree.DfsInOrder(
            [](const int &key) {
                std::cout << key << " ";
            });

    return 0;
}
