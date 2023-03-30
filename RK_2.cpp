#include <cassert>
#include <iostream>
#include <string>

const int INITIAL_SIZE = 4;

template<class T>
class Stack {
public:
    Stack() : array(nullptr), capacity(0), size(0) {}

    ~Stack();

    Stack(Stack &) = delete;
    Stack &operator=(const Stack &) = delete;

    void Push(const T &);
    T Pop();
    bool IsEmpty() const;
private:
    T *array;
    unsigned capacity;
    unsigned size;
    void grow();
};

int main() {
    Stack<char> Stack;

    char bracket;
    while (!std::cin.eof()) {
        std::cin >> bracket;
        Stack.Push(bracket);
    }

    bool result = true;

    // variable shows how many brackets are closed at this moment
    int nClosedBrackets = 0;
    while (!Stack.IsEmpty()) {
        char bracket = Stack.Pop();
        if (bracket == ')') {
            ++nClosedBrackets;
        } else {
            --nClosedBrackets;
            if (nClosedBrackets < 0) {
                result = false;
                break;
            }
        }
    }

    std::cout << (result ? "YES\n" : "NO\n");

    return 0;
}

template<class T>
Stack<T>::~Stack<T>() {
    if (array != nullptr) {
        delete[]array;
    }
}

template<class T>
void Stack<T>::grow() {
    T *oldArray = array;
    capacity = capacity ? capacity * 2 : INITIAL_SIZE;

    array = new T[capacity];
    for (int i = 0; i < size; ++i) {
        array[i] = oldArray[i];
    }

    if (oldArray != nullptr) {
        delete[] oldArray;
    }
}

template<class T>
bool Stack<T>::IsEmpty() const {
    return !size; // if size != 0 then return false
}

template<class T>
void Stack<T>::Push(const T &data) {
    if (capacity == size) {
        grow();
    }

    array[size++] = data;
}

template<class T>
T Stack<T>::Pop() {
    assert(!IsEmpty());

    T data = array[--size];
    return data;
}
