// 3.1
// Постройте B-дерево минимального порядка t и выведите его по слоям.
// В качестве ключа используются числа, лежащие в диапазоне 0..232 -1

// Требования:
// B-дерево должно быть реализовано в виде шаблонного класса.
// Решение должно поддерживать передачу функции сравнения снаружи.

// Формат входных данных:
// Сначала вводится минимальный порядок дерева t.
// Затем вводятся элементы дерева.

// Формат выходных данных:
// Программа должна вывести B-дерево по слоям. Каждый слой на новой строке,
// элементы должны выводится в том порядке, в котором они лежат в узлах.

#include <iostream>
#include <sstream>
#include <vector>
#include <queue>

template<class T>
class BTree {
public:
    BTree(size_t _t) : root(nullptr), t(_t) {}

    ~BTree() { delete root; }
    BTree(BTree &) = delete;
    BTree &operator=(BTree &) = delete;

    void Insert(const T &key);

    void BFS(std::ostream &stream) const;

private:
    struct Node {
        bool IsLeaf;
        std::vector<T> Keys;
        std::vector<Node *> Children;

        Node(bool isLeaf) : IsLeaf(isLeaf) {}

        ~Node() { for (Node *child: Children) { delete child; }}
    };

    Node *root;
    size_t t;

    bool isNodeFull(Node *node) const { return node->Keys.size() == 2 * t - 1; }

    void insertNonFull(Node *node, const T &key);
    void splitChild(Node *node, int pos);
};

template<class T>
void BTree<T>::Insert(const T &key) {
    if (root == nullptr) {
        root = new Node(true);
    }

    if (isNodeFull(root)) {
        Node *newRoot = new Node(false);
        newRoot->Children.push_back(root);
        root = newRoot;
        splitChild(root, 0);
    }

    insertNonFull(root, key);
}

template<class T>
void BTree<T>::insertNonFull(Node *node, const T &key) {
    if (node->IsLeaf) {
        node->Keys.resize(node->Keys.size() + 1);
        int pos = node->Keys.size() - 2;
        while (pos >= 0 && key < node->Keys[pos]) {
            node->Keys[pos + 1] = node->Keys[pos];
            --pos;
        }
        node->Keys[pos + 1] = key;
    } else {
        int pos = node->Keys.size() - 1;
        while (pos >= 0 && key < node->Keys[pos]) {
            --pos;
        }
        if (isNodeFull(node->Children[pos + 1])) {
            splitChild(node, pos + 1);
            if (key > node->Keys[pos + 1]) {
                ++pos;
            }
        }
        insertNonFull(node->Children[pos + 1], key);
    }
}

template<class T>
void BTree<T>::splitChild(Node *node, int pos) {
    // save this node because we need split its children into 2 nodes
    Node *childForSplit = node->Children[pos];

    // find place for insert newKey
    node->Keys.resize(node->Keys.size() + 1);
    if (node->Keys.size() > 1) {
        int p = node->Keys.size() - 2;
        while (p >= pos) {
            node->Keys[p + 1] = node->Keys[p];
            --p;
        }
    }

    // insert newKey in node->Keys
    T middleChildKey = childForSplit->Keys[t - 1];
    node->Keys[pos] = middleChildKey;

    // find place for insert newChild
    node->Children.resize(node->Children.size() + 1);
    if (node->Children.size() > 2) {
        int p = node->Children.size() - 2;
        while (p > pos) {
            node->Children[p + 1] = node->Children[p];
            --p;
        }
    }

    // create new Node instead of childForSplit and insert first part of childForSplit's Keys
    // no new nodes have Keys[t-1]
    node->Children[pos] = new Node(childForSplit->IsLeaf);
    node->Children[pos]->Keys.resize(t - 1);
    for (int i = 0; i < t - 1; ++i) {
        node->Children[pos]->Keys[i] = childForSplit->Keys[i];
    }

    // insert into new Node first part of childForSplit's Children
    if (!childForSplit->IsLeaf) {
        node->Children[pos]->Children.resize(t);
        for (int i = 0; i < t; ++i) {
            node->Children[pos]->Children[i] = childForSplit->Children[i];
        }
    }

    // create new Node and insert second part of childForSplit's Keys
    node->Children[pos + 1] = new Node(childForSplit->IsLeaf);
    node->Children[pos + 1]->Keys.resize(t - 1);
    for (int i = t; i < 2 * t - 1; ++i) {
        node->Children[pos + 1]->Keys[i - t] = childForSplit->Keys[i];
    }

    // insert into new Node second part of childForSplit's Children
    if (!childForSplit->IsLeaf) {
        node->Children[pos + 1]->Children.resize(t);
        for (int i = t; i < 2 * t; ++i) {
            node->Children[pos + 1]->Children[i - t] = childForSplit->Children[i];
        }
    }
    if (!childForSplit->IsLeaf) {

    }
    childForSplit->Children.resize(0);
    delete childForSplit;
}

template<class T>
void BTree<T>::BFS(std::ostream &stream) const {
    std::queue<Node *> queue;
    queue.push(root);
    queue.push(nullptr);

    while (!queue.empty()) {
        Node *node = queue.front();
        queue.pop();

        if (node == nullptr) {
            stream << std::endl;
            if (!queue.empty()) {
                queue.push(nullptr);
            }
            continue;
        }
        for (T key: node->Keys) {
            stream << key << " ";
        }
        for (Node *child: node->Children) {
            queue.push(child);
        }
    }
}

int main() {
    unsigned t;
    std::cin >> t;
    BTree<unsigned> bTree(t);

    unsigned key;
    while (std::cin >> key) {
        bTree.Insert(key);
    }

    bTree.BFS(std::cout);
}
