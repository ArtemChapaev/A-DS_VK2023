// 4.1.Солдаты.
// В одной военной части решили построить в одну шеренгу по росту. Т.к. часть была далеко не
// образцовая, то солдаты часто приходили не вовремя, а то их и вовсе приходилось выгонять из
// шеренги за плохо начищенные сапоги. Однако солдаты в процессе прихода и ухода должны были
// всегда быть выстроены по росту – сначала самые высокие, а в конце – самые низкие. За
// расстановку солдат отвечал прапорщик, который заметил интересную особенность – все
// солдаты в части разного роста. Ваша задача состоит в том, чтобы помочь прапорщику
// правильно расставлять солдат, а именно для каждого приходящего солдата указывать,
// перед каким солдатом в строе он должен становится.

// Требования:
// Cкорость выполнения команды - O(log n).
// Решение должно поддерживать передачу функции сравнения снаружи.

// Формат входных данных:
// Первая строка содержит число N – количество команд (1 ≤ N ≤ 30 000). В каждой следующей
// строке содержится описание команды: число 1 и X если солдат приходит в строй (X – рост
// солдата, натуральное число до 100 000 включительно) и число 2 и Y если солдата, стоящим
// в строе на месте Y надо удалить из строя. Солдаты в строе нумеруются с нуля.

// Формат выходных данных:
// На каждую команду 1 (добавление в строй) вы должны выводить число K – номер позиции, на
// которую должен встать этот солдат (все стоящие за ним двигаются назад).

#include <iostream>
#include <stack>

// l > r, because our ordered tree is decreasing
struct IntCmp {
    bool operator()(int l, int r) { return l > r; }
};

template<class T, class Compare>
class AvlTree {
public:
    AvlTree(const Compare &_cmp) : root(nullptr), cmp(_cmp) {}

    ~AvlTree();
    AvlTree(AvlTree &) = delete;
    AvlTree &operator=(AvlTree &) = delete;

    // return newNode's ordinal statistics
    unsigned Add(const T &key);

    void Del(unsigned ordStat);
private:

    struct Node {
        Node *Left;
        Node *Right;
        T Key;
        unsigned Height;
        unsigned Count;

        Node(const T &key) : Left(nullptr), Right(nullptr), Key(key), Height(1), Count(1) {}
    };

    void FixHeight(Node *node);
    void FixCount(Node *node);

    int GetBalance(Node *node) const;
    void Balance(Node *&node);

    void RotateRight(Node *&node);
    void RotateLeft(Node *&node);

    void Remove(Node *&node);

    // если определять функцию вне класса, то придется перенести определение Node в public
    Node *FindMinNode(Node *&node, Node **minNodePtr) {
        if (node->Left == nullptr) {
            *minNodePtr = node;
            return node->Right;
        }
        node->Left = FindMinNode(node->Left, minNodePtr);
        Balance(node);
        return node;
    }

    unsigned add(Node *&node, const T &key);
    void del(Node *&node, unsigned ordStat);

    Node *root;
    Compare cmp;
};

template<class T, class Compare>
AvlTree<T, Compare>::~AvlTree() {
    if (root == nullptr) {
        return;
    }

    // if tree is not empty we delete all nodes using post-order DFS
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

    root = nullptr;
}

template<class T, class Compare>
void AvlTree<T, Compare>::FixHeight(Node *node) {
    unsigned rightHeight = node->Right ? node->Right->Height : 0;
    unsigned leftHeight = node->Left ? node->Left->Height : 0;

    unsigned maxHeight = std::max(rightHeight, leftHeight);
    node->Height = maxHeight + 1;
}

template<class T, class Compare>
void AvlTree<T, Compare>::FixCount(Node *node) {
    unsigned rightCount = node->Right ? node->Right->Count : 0;
    unsigned leftCount = node->Left ? node->Left->Count : 0;

    node->Count = rightCount + leftCount + 1;
}

template<class T, class Compare>
int AvlTree<T, Compare>::GetBalance(Node *node) const {
    long rightHeight = node->Right ? node->Right->Height : 0;
    long leftHeight = node->Left ? node->Left->Height : 0;
    return static_cast<int>(rightHeight - leftHeight);
}

template<class T, class Compare>
void AvlTree<T, Compare>::RotateRight(Node *&node) {
    Node *child = node->Left;
    node->Left = child->Right;
    child->Right = node;

    FixHeight(node);
    FixCount(node);

    FixHeight(child);
    FixCount(child);

    node = child;
}

template<class T, class Compare>
void AvlTree<T, Compare>::RotateLeft(Node *&node) {
    Node *child = node->Right;
    node->Right = child->Left;
    child->Left = node;

    FixHeight(node);
    FixCount(node);

    FixHeight(child);
    FixCount(child);

    node = child;
}

template<class T, class Compare>
void AvlTree<T, Compare>::Balance(Node *&node) {
    FixHeight(node);
    FixCount(node);

    if (GetBalance(node) == 2) {
        if (GetBalance(node->Right) == -1) {
            RotateRight(node->Right);
        }
        RotateLeft(node);
    }

    if (GetBalance(node) == -2) {
        if (GetBalance(node->Left) == 1) {
            RotateLeft(node->Left);
        }
        RotateRight(node);
    }
}

template<class T, class Compare>
unsigned AvlTree<T, Compare>::add(Node *&node, const T &key) {
    if (node == nullptr) {
        node = new Node(key);
        return 0;
    }
    // stat shows what ordinal statistics the element has in current subtree
    unsigned stat = 0;
    if (cmp(key, node->Key)) {
        stat = add(node->Left, key);
    } else {
        // if key insert into the right subtree, its stat is nodeStat + 1 + add(node->Right, key)
        unsigned nodeStat = node->Left ? node->Left->Count : 0;
        stat = nodeStat + 1 + add(node->Right, key);
    }
    Balance(node);
    return stat;
}

template<class T, class Compare>
unsigned AvlTree<T, Compare>::Add(const T &key) {
    return add(root, key);
}

template<class T, class Compare>
void AvlTree<T, Compare>::Remove(Node *&node) {
    if (node->Left == nullptr && node->Right == nullptr) {
        // node is leaf
        delete node;
        node = nullptr;
        return;
    }

    Node *minNode = nullptr;
    if (node->Left != nullptr && node->Right != nullptr) {
        // find min of right subtree
        node->Right = FindMinNode(node->Right, &minNode);
    } else if (node->Left != nullptr) {
        // if node->Right doesn't exist, node->Left is leaf
        minNode = node->Left;
        node->Left = nullptr;
    } else if (node->Right != nullptr) {
        // if node->Left doesn't exist, node->Right is leaf
        minNode = node->Right;
        node->Right = nullptr;
    }

    Node *oldNode = node;

    node = minNode;
    node->Left = oldNode->Left;
    node->Right = oldNode->Right;
    delete oldNode;
}

template<class T, class Compare>
void AvlTree<T, Compare>::del(Node *&node, unsigned ordStat) {
    unsigned nodeStat = node->Left ? node->Left->Count : 0;

    if (node == nullptr) {
        return;
    }

    if (nodeStat == ordStat) {
        Remove(node);
    } else if (nodeStat < ordStat) {
        del(node->Right, ordStat - nodeStat - 1);
    } else {
        del(node->Left, ordStat);
    }

    if (node != nullptr) {
        Balance(node);
    }
}

template<class T, class Compare>
void AvlTree<T, Compare>::Del(unsigned ordStat) {
    del(root, ordStat);
}

int main() {
    IntCmp cmp;
    AvlTree<unsigned, IntCmp> avlTree(cmp);

    unsigned n = 0;
    std::cin >> n;

    unsigned instruction;
    unsigned key = 0;
    for (int i = 0; i < n; ++i) {
        std::cin >> instruction >> key;
        if (instruction == 1) {
            unsigned ordStat = avlTree.Add(key);
            std::cout << ordStat << std::endl;
        } else {
            // here key is ordinal statistics
            avlTree.Del(key);
        }
    }

    return 0;
}
