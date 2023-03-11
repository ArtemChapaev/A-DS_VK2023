#include <iostream>

const int INITIAL_SIZE = 4;

template<class T>
class Deque {
public:
    Deque() : array(nullptr), capacity(0), size(0) {}

    ~Deque();

    Deque(Deque &) = delete;
    Deque &operator=(const Deque &) = delete;

    void PushFront(const T &);
    void PushBack(const T &);
    T PopFront();
    T PopBack();
    bool IsEmpty();
private:
    T *array;
    T front;
    T back;
    unsigned capacity;
    unsigned size;
    void grow();
};

template<class T>
void Deque<T>::grow() {
    T *old_array = array;
    capacity = capacity ? capacity * 2 : INITIAL_SIZE;

    array = new int[capacity];
    for (int i = 0; i < size; ++i) {
        array[i] = old_array[front];
        front = (front + 1) % size;
    }
    front = 0;
    back = size;

    if (old_array != nullptr) {
        delete[] old_array;
    }
}

template<class T>
Deque<T>::~Deque<T>() {
    while (!IsEmpty()) {
        PopBack();
    }
    delete[]array;
}

template<class T>
bool Deque<T>::IsEmpty() {
    return !size; // if size != 0 then return false
}

template<class T>
void Deque<T>::PushFront(const T &data) {
    if (capacity == size) {
        grow();
    }
    front = front == 0 ? capacity - 1 : front - 1;
    array[front] = data;
    ++size;
}

template<class T>
void Deque<T>::PushBack(const T &data) {
    if (capacity == size) {
        grow();
    }
    array[back] = data;
    back = back == capacity - 1 ? 0 : back + 1;
    ++size;
}

template<class T>
T Deque<T>::PopFront() {
    if (IsEmpty()) {
        return -1;
    }
    T data = array[front];
    --size;
    front = front == capacity - 1 ? 0 : front + 1;
    return data;
}

template<class T>
T Deque<T>::PopBack() {
    if (IsEmpty()) {
        return -1;
    }
    back = back == 0 ? capacity - 1 : back - 1;
    T data = array[back];
    --size;
    return data;
}

int main() {
    Deque<int> deque;
    int n = 0;
    std::cin >> n;
    bool result = true;
    for (int i = 0; i < n; ++i) {
        int command = 0;
        int data = 0;
        std::cin >> command >> data;
        switch (command) {
            case 1:
                deque.PushFront(data);
                break;
            case 2:
                if (deque.IsEmpty()) {
                    result = result && data == -1;
                } else {
                    result = result && deque.PopFront() == data;
                }
                break;
            case 3:
                deque.PushBack(data);
                break;
            case 4:
                if (deque.IsEmpty()) {
                    result = result && data == -1;
                } else {
                    result = result && deque.PopBack() == data;
                }
                break;
        }
    }
    std::cout << (result ? "YES\n" : "NO\n");

    return 0;
}
