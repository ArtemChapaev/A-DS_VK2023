// 3_2. Реализовать дек с динамическим зацикленным буфером.
// Требования: Дек должен быть реализован в виде класса.

// Формат входных данных.
// В первой строке количество команд n. n ≤ 1000000.
// Каждая команда задаётся как 2 целых числа: a b.
// a = 1 - push front
// a = 2 - pop front
// a = 3 - push back
// a = 4 - pop back
// Команды добавления элемента 1 и 3 заданы с неотрицательным параметром b.
// Для очереди используются команды 2 и 3. Для дека используются все четыре команды.
// Если дана команда pop*, то число b - ожидаемое значение. Если команда pop вызвана для пустой структуры данных, то ожидается “-1”.

// Формат выходных данных.
// Требуется напечатать YES - если все ожидаемые значения совпали. Иначе, если хотя бы одно ожидание не оправдалось, то напечатать NO.


#include <iostream>
#include <cassert>

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
    bool IsEmpty() const;
private:
    T *array;
    int front;
    int back;
    unsigned capacity;
    unsigned size;
    void grow();
};

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

template<class T>
Deque<T>::~Deque<T>() {
    if (array != nullptr) {
        delete[]array;
    }
}

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
bool Deque<T>::IsEmpty() const {
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
    assert(!IsEmpty());

    T data = array[front];
    --size;
    front = front == capacity - 1 ? 0 : front + 1;
    return data;
}

template<class T>
T Deque<T>::PopBack() {
    assert(!IsEmpty());

    back = back == 0 ? capacity - 1 : back - 1;
    T data = array[back];
    --size;
    return data;
}
