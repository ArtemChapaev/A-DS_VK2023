// 1.1.
// Реализуйте структуру данных типа “множество строк” на основе динамической хеш-таблицы с открытой адресацией.
// Хранимые строки непустые и состоят из строчных латинских букв.

// Хеш-функция строки должна быть реализована с помощью вычисления значения многочлена методом Горнера.

// Начальный размер таблицы должен быть равным 8-ми. Перехеширование выполняйте при добавлении элементов в случае,
// когда коэффициент заполнения таблицы достигает 3/4.

// Структура данных должна поддерживать операции добавления строки в множество, удаления строки из множества
// и проверки принадлежности данной строки множеству.

// Для разрешения коллизий используйте квадратичное пробирование.
// i-ая проба : g(k, i) = g(k, i-1) + i (mod m). m - степень двойки.

#include <iostream>
#include <vector>
#include <string>
#include <iterator>

const unsigned INITIAL_SIZE = 8;
const float ALPHA = 0.75;
const unsigned SIMPLE_NUM = 151;

struct StringHasher {
    unsigned operator()(const std::string &key) const {
        unsigned hash = 0;
        for (char i: key) {
            hash = hash * SIMPLE_NUM + i;
        }
        return hash;
    }
};

template<class T, class H>
class HashTable {
public:
    HashTable(int initialSize, const H &hasher);

    bool Has(const T &key) const;
    bool Add(const T &key);
    bool Delete(const T &key);

private:
    H hasher;

    struct HashTableNode {
        T Key;
        unsigned Hash;

        enum State {
            Empty, Filled, Deleted
        };
        State State;

        HashTableNode() : Hash(0), State(State::Empty) {}

        HashTableNode(const T &key, unsigned hash) : Key(key), Hash(hash), State(State::Filled) {}
    };

    std::vector<HashTableNode> table;
    unsigned FilledNodeCount;
    unsigned DeletedNodeCount;
    void growTable(unsigned size);
};

template<class T, class H>
HashTable<T, H>::HashTable(int initialSize, const H &_hasher) :hasher(_hasher), table(initialSize), FilledNodeCount(0),
                                                               DeletedNodeCount(0) {}

template<class T, class H>
bool HashTable<T, H>::Has(const T &key) const {
    unsigned hash = hasher(key) % table.size();

    for (unsigned stepCount = 0; stepCount < table.size(); ++stepCount) {
        if (table[hash].Key == key) {
            return true;
        }
        if (table[hash].State == HashTableNode::State::Empty) {
            return false;
        }
        hash = (hash + stepCount + 1) % table.size();
    }
    // if we check all elements return false
    return false;
}

template<class T, class H>
bool HashTable<T, H>::Add(const T &key) {
    if (FilledNodeCount + DeletedNodeCount > ALPHA * table.size()) {
        if (FilledNodeCount > DeletedNodeCount) {
            growTable(table.size() * 2);
        } else {
            growTable(table.size());
        }
    }

    unsigned absHash = hasher(key);
    unsigned hash = absHash % table.size();

    unsigned firstDeletedNode = 0;
    for (unsigned stepCount = 0; stepCount < table.size(); ++stepCount) {
        if (table[hash].Key == key) {
            return false;
        }
        if (table[hash].State == HashTableNode::State::Empty) {
            // if we have visited deleted node before, use it for insertion
            if (firstDeletedNode) {
                hash = firstDeletedNode;
                --DeletedNodeCount;
            }
            table[hash].Key = key;
            table[hash].Hash = absHash;
            table[hash].State = HashTableNode::State::Filled;
            ++FilledNodeCount;
            return true;
        }
        // check first visit of deleted node
        if (table[hash].State == HashTableNode::State::Deleted && !firstDeletedNode) {
            firstDeletedNode = hash;
        }
        hash = (hash + stepCount + 1) % table.size();
    }

    // non-reachable code, because always there are empty nodes in table
    return false;
}

template<class T, class H>
bool HashTable<T, H>::Delete(const T &key) {
    unsigned hash = hasher(key) % table.size();

    for (unsigned stepCount = 0; stepCount < table.size(); ++stepCount) {
        if (table[hash].State == HashTableNode::State::Empty) {
            return false;
        }
        if (table[hash].Key == key) {
            table[hash].State = HashTableNode::State::Deleted;
            table[hash].Key = T();
            --FilledNodeCount;
            ++DeletedNodeCount;
            return true;
        }
        hash = (hash + stepCount + 1) % table.size();
    }

    // non-reachable code, because always there are empty nodes in table
    return false;
}

template<class T, class H>
void HashTable<T, H>::growTable(unsigned newSize) {
    std::vector<HashTableNode> newTable(newSize);
    for (int i = 0; i < table.size(); ++i) {
        // move in newTable only relevant keys
        if (table[i].State != HashTableNode::State::Filled) {
            continue;
        }

        unsigned hash = table[i].Hash % newSize;
        // fill only empty node
        for (unsigned stepCount = 0; stepCount < table.size(); ++stepCount) {
            if (newTable[hash].State == HashTableNode::State::Empty) {
                newTable[hash].Key = table[i].Key;
                newTable[hash].Hash = table[i].Hash;
                newTable[hash].State = HashTableNode::State::Filled;
                break;
            }
            hash = (hash + stepCount + 1) % newSize;
        }
    }
    table = std::move(newTable);
}

int main() {
    StringHasher h;
    HashTable<std::string, StringHasher> table(INITIAL_SIZE, h);
    char operation = 0;
    std::string word;
    while (std::cin >> operation >> word) {
        switch (operation) {
            case '+':
                std::cout << (table.Add(word) ? "OK" : "FAIL") << std::endl;
                break;
            case '-':
                std::cout << (table.Delete(word) ? "OK" : "FAIL") << std::endl;
                break;
            case '?':
                std::cout << (table.Has(word) ? "OK" : "FAIL") << std::endl;
                break;
        }
    }
    return 0;
}
